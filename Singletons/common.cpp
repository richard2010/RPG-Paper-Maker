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

#include "common.h"
#include <QDirIterator>

Common::Common()
{

}

// -------------------------------------------------------
//
//  INTERMEDIARY FUNCTIONS
//
// -------------------------------------------------------

QString Common::pathCombine(const QString & p1, const QString & p2){
    return QDir::cleanPath(p1 + QDir::separator() + p2);
}

// -------------------------------------------------------

void Common::writeOtherJSON(QString path, const QJsonObject &obj,
                           QJsonDocument::JsonFormat format)
{
    QFile saveFile(path);
    if (!saveFile.open(QIODevice::WriteOnly)) { return; }
    QJsonDocument saveDoc(obj);
    saveFile.write(saveDoc.toJson(format));
}

// -------------------------------------------------------

void Common::readOtherJSON(QString path, QJsonDocument& loadDoc){
    QFile loadFile(path);
    loadFile.open(QIODevice::ReadOnly);
    QByteArray saveData = loadFile.readAll();
    loadDoc = QJsonDocument::fromJson(saveData);
}

// -------------------------------------------------------

void Common::writeArrayJSON(QString path, const QJsonArray &tab){
    QFile saveFile(path);
    if (!saveFile.open(QIODevice::WriteOnly)) { return; }
    QJsonDocument saveDoc(tab);
    saveFile.write(saveDoc.toJson(QJsonDocument::Compact));
}

// -------------------------------------------------------

void Common::readArrayJSON(QString path, QJsonDocument& loadDoc){
    QFile loadFile(path);
    loadFile.open(QIODevice::ReadOnly);
    QByteArray saveData = loadFile.readAll();
    loadDoc = QJsonDocument::fromJson(saveData);
}

// -------------------------------------------------------

bool Common::copyPath(QString src, QString dst)
{
    QDir dir(src);
    if (!dir.exists())
        return false;

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = pathCombine(dst, d);
        if (!dir.mkpath(dst_path)) return false;
        if (!copyPath(pathCombine(src, d), dst_path)) return false;
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        if (!QFile::copy(pathCombine(src, f), pathCombine(dst, f)))
            return false;
    }

    return true;
}

// -------------------------------------------------------

QString Common::getDirectoryPath(QString& file){
    return QFileInfo(file).dir().absolutePath();
}

// -------------------------------------------------------

bool Common::isDirEmpty(QString path){
    return QDir(path).entryInfoList(QDir::NoDotAndDotDot |
                                    QDir::AllEntries).count() == 0;
}

// -------------------------------------------------------

void Common::copyAllFiles(QString pathSource, QString pathTarget){
    QDirIterator files(pathSource, QDir::Files);
    QString path;

    while (files.hasNext()){
        files.next();
        path = Common::pathCombine(pathTarget, files.fileName());
        QFile::remove(path);
        QFile::copy(files.filePath(), path);
    }
}

// -------------------------------------------------------

void Common::deleteAllFiles(QString pathSource){
    QDirIterator files(pathSource, QDir::Files);

    while (files.hasNext()){
        files.next();
        QFile(files.filePath()).remove();
    }
}

// -------------------------------------------------------

QString Common::getFormatNumber(int number, int format, int type){
    return QString("%1").arg(number, format, type, QChar('0'));
}

// -------------------------------------------------------

bool Common::getSubVersions(QString& version, int& m, int& f, int& b) {
    QStringList list = version.split(".");
    if (list.size() != 3)
        return false;
    bool ok;
    int integer;
    integer = list.at(0).toInt(&ok);
    if (!ok)
        return false;
    m = integer;
    integer = list.at(1).toInt(&ok);
    if (!ok)
        return false;
    f = integer;
    integer = list.at(2).toInt(&ok);
    if (!ok)
        return false;
    b = integer;

    return true;
}

// -------------------------------------------------------

int Common::versionDifferent(QString projectVersion, QString otherVersion)
{
    int mProject, fProject, bProject, mEngine, fEngine, bEngine;
    bool ok = getSubVersions(projectVersion, mProject, fProject, bProject);
    bool ok2 = getSubVersions(otherVersion, mEngine, fEngine, bEngine);

    // Error while trying to convert one of the versions version
    if (!ok || !ok2)
        return -2;

    // If project <, return -1, if =, return 0, if > return 1
    if (mProject < mEngine)
        return -1;
    else if (mProject > mEngine)
        return 1;
    else {
        if (fProject < fEngine)
            return -1;
        else if (fProject > fEngine)
            return 1;
        else {
            if (bProject < bEngine)
                return -1;
            else if (bProject > bEngine)
                return 1;
            else
                return 0;
        }
    }
}
