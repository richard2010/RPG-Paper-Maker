/*
    RPG Paper Maker Copyright (C) 2017-2019 Marie Laporte

    This file is part of RPG Paper Maker.

    RPG Paper Maker is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RPG Paper Maker is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "floors.h"
#include "rpm.h"

// -------------------------------------------------------
//
//  CONSTRUCTOR / DESTRUCTOR / GET / SET
//
// -------------------------------------------------------

Floors::Floors() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_programStatic(nullptr)
{

}

Floors::~Floors()
{
    QHash<Position, FloorDatas*>::iterator i;
    for (i = m_all.begin(); i != m_all.end(); i++)
        delete i.value();
}

// -------------------------------------------------------
//
//  INTERMEDIARY FUNCTIONS
//
// -------------------------------------------------------

bool Floors::isEmpty() const{
    return m_all.size() == 0;
}

// -------------------------------------------------------

FloorDatas *Floors::getFloor(Position& p) const{
    return m_all.value(p);
}

// -------------------------------------------------------

void Floors::setFloor(Position& p, FloorDatas *floor){
    m_all.insert(p, floor);
}

// -------------------------------------------------------

FloorDatas *Floors::removeFloor(Position& p){
    FloorDatas* floor = m_all.value(p);

    if (floor != nullptr)
        m_all.remove(p);

    return floor;
}

// -------------------------------------------------------

void Floors::removeFloorOut(MapProperties& properties) {
    QList<Position> list;
    QHash<Position, FloorDatas*>::iterator i;
    for (i = m_all.begin(); i != m_all.end(); i++) {
        Position position = i.key();

        if (position.x() >= properties.length() ||
            position.z() >= properties.width())
        {
            delete *i;
            list.push_back(position);
        }
    }

    for (int j = 0; j < list.size(); j++)
        m_all.remove(list.at(j));
}

// -------------------------------------------------------

MapElement* Floors::updateRaycasting(int squareSize, float& finalDistance,
                                     Position &finalPosition, QRay3D &ray)
{
    MapElement* element = nullptr;

    for (QHash<Position, FloorDatas*>::iterator i = m_all.begin();
         i != m_all.end(); i++)
    {
        Position position = i.key();
        FloorDatas* floor = i.value();
        if (updateRaycastingAt(position, floor, squareSize, finalDistance,
                               finalPosition, ray))
        {
            element = floor;
        }
    }

    return element;
}

// -------------------------------------------------------

bool Floors::updateRaycastingAt(Position &position, FloorDatas* floor,
                                int squareSize, float &finalDistance,
                                Position &finalPosition, QRay3D& ray)
{
    float newDistance = floor->intersection(squareSize, ray, position);
    if (RPM::getMinDistance(finalDistance, newDistance)) {
        finalPosition = position;
        return true;
    }

    return false;
}

// -------------------------------------------------------
//
//  GL
//
// -------------------------------------------------------

void Floors::initializeVertices(QHash<Position, MapElement *> &previewSquares,
                                int squareSize, int width, int height)
{
    m_vertices.clear();
    m_indexes.clear();
    int count = 0;

    // Create temp hash for preview
    QHash<Position, FloorDatas*> floorsWithPreview(m_all);
    QHash<Position, MapElement*>::iterator it;
    for (it = previewSquares.begin(); it != previewSquares.end(); it++) {
        MapElement* element = it.value();
        if (element->getSubKind() == MapEditorSubSelectionKind::Floors)
            floorsWithPreview[it.key()] = (FloorDatas*) element;
        else if (element->getSubKind() == MapEditorSubSelectionKind::Autotiles)
            floorsWithPreview.remove(it.key());
    }

    // Initialize vertices
    QHash<Position, FloorDatas*>::iterator i;
    for (i = floorsWithPreview.begin(); i != floorsWithPreview.end(); i++) {
        FloorDatas* floor = i.value();
        Position p = i.key();
        floor->initializeVertices(squareSize, width, height, m_vertices,
                                  m_indexes, p, count);
    }
}

// -------------------------------------------------------

void Floors::initializeGL(QOpenGLShaderProgram *programStatic){
    if (m_programStatic == nullptr){
        initializeOpenGLFunctions();

        m_programStatic = programStatic;
    }
}

// -------------------------------------------------------

void Floors::updateGL(){
    Map::updateGLStatic(m_vertexBuffer, m_indexBuffer, m_vertices, m_indexes,
                        m_vao, m_programStatic);
}

// -------------------------------------------------------

void Floors::paintGL(){
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, 0);
    m_vao.release();
}

// -------------------------------------------------------
//
//  READ / WRITE
//
// -------------------------------------------------------

void Floors::read(const QJsonObject & json){
    QJsonArray tabFloors = json["floors"].toArray();

    for (int i = 0; i < tabFloors.size(); i++){
        QJsonObject obj = tabFloors.at(i).toObject();
        Position p;
        p.read(obj["k"].toArray());
        QJsonObject objLand = obj["v"].toObject();
        FloorDatas* floor = new FloorDatas;
        floor->read(objLand);
        m_all[p] = floor;
    }
}

// -------------------------------------------------------

void Floors::write(QJsonObject & json) const{
    QJsonArray tabFloors;

    QHash<Position, FloorDatas*>::const_iterator i;
    for (i = m_all.begin(); i != m_all.end(); i++){
        QJsonObject objHash;
        QJsonArray tabKey;
        Position position = i.key();
        position.write(tabKey);
        FloorDatas* floor = i.value();
        QJsonObject objFloor;
        floor->write(objFloor);
        objHash["k"] = tabKey;
        objHash["v"] = objFloor;
        tabFloors.append(objHash);
    }
    json["floors"] = tabFloors;
}
