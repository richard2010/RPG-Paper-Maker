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

#include "paneltextures.h"
#include "ui_paneltextures.h"
#include "systemspritewall.h"
#include "rpm.h"

// -------------------------------------------------------
//
//  CONSTRUCTOR / DESTRUCTOR / GET / SET
//
// -------------------------------------------------------

PanelTextures::PanelTextures(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelTextures),
    m_kind(PictureKind::None),
    m_currentAutotilesID(-1),
    m_currentWallsID(-1)
{
    ui->setupUi(this);
}

PanelTextures::~PanelTextures()
{
    delete ui;
}

// -------------------------------------------------------
//
//  INTERMEDIARY FUNCTIONS
//
// -------------------------------------------------------

void PanelTextures::getTilesetTexture(QRect &rect) const{
    switch (m_kind) {
    case PictureKind::Autotiles:
        ui->widgetAutotilesSelector->currentTexture(rect); break;
    default:
        ui->widgetTilesetSelector->currentTexture(rect);
    }
}

// -------------------------------------------------------

void PanelTextures::setTilesetImage(QString path) {
    ui->widgetTilesetSelector->setImage(path);
    updateTilesetImage();
}

// -------------------------------------------------------

void PanelTextures::setTilesetImageNone() {
    ui->widgetTilesetSelector->setImageNone();
    updateTilesetImage();
}

// -------------------------------------------------------

void PanelTextures::updateTilesetImage() {
    this->setGeometry(0, 0, ui->widgetTilesetSelector->width(), ui
        ->widgetTilesetSelector->height());
    setFixedSize(ui->widgetTilesetSelector->width(), ui->widgetTilesetSelector
        ->height());
}

// -------------------------------------------------------

void PanelTextures::hideAll() {
    ui->widgetTilesetSelector->hide();
    ui->widgetAutotilesSelector->hide();
    ui->widgetWallPreview->hide();
    ui->comboBox->hide();
    ui->labelInformation->hide();
}

// -------------------------------------------------------

void PanelTextures::showTileset() {
    m_kind = PictureKind::None;

    hideAll();
    ui->widgetTilesetSelector->show();
    updateTilesetImage();
}

// -------------------------------------------------------

int PanelTextures::getID() const {
    if (m_kind != PictureKind::None) {
        SystemTileset *tileset = RPM::get()->project()->currentMap()
            ->mapProperties()->tileset();
        int index = ui->comboBox->currentIndex();

        return SuperListItem::getIdByIndex(tileset->model(m_kind), index);
    }

    return -1;
}

// -------------------------------------------------------

QWidget * PanelTextures::getSpecialWidget() const {
    switch (m_kind) {
    case PictureKind::Autotiles:
        return ui->widgetAutotilesSelector;
    case PictureKind::Walls:
        return ui->widgetWallPreview;
    default:
        return ui->widgetTilesetSelector;
    }
}

// -------------------------------------------------------

void PanelTextures::showComboBox() {
    updateComboBoxSize();

    // Setting label text
    if (ui->comboBox->count() == 0) {
        ui->labelInformation->show();
        ui->labelInformation->setText(createlabelText());
        updateLabelSize();
        ui->comboBox->hide();
    } else {
        ui->labelInformation->hide();
        ui->comboBox->show();
        showWidgetSpecial();
    }
}

// -------------------------------------------------------

void PanelTextures::showWidgetSpecial() {
    switch (m_kind) {
    case PictureKind::Autotiles:
        ui->widgetAutotilesSelector->show();
        break;
    case PictureKind::Walls:
        ui->widgetWallPreview->show();
        break;
    default:
        break;
    }
}

// -------------------------------------------------------

void PanelTextures::updateComboBoxSize() {
    QWidget *currentSpecial = getSpecialWidget();
    ui->comboBox->setGeometry(0, 0, this->parentWidget()->width(), ui->comboBox
        ->height());
    ui->comboBox->setFixedSize(this->parentWidget()->width(), ui->comboBox
        ->height());

    int width = qMax(currentSpecial->width(), this->parentWidget()->width());
    int height = ui->comboBox->height() + 6 + currentSpecial->height();
    this->setGeometry(0, 0, width, height);
    setFixedSize(width, height);
}

// -------------------------------------------------------

void PanelTextures::updateLabelSize() {
    ui->labelInformation->setGeometry(0, 0, this->parentWidget()->width(), this
        ->parentWidget()->height());
    ui->labelInformation->setFixedSize(this->parentWidget()->width(), this
        ->parentWidget()->height());
    this->setGeometry(0, 0, this->parentWidget()->width(), this->parentWidget()
        ->height());
    setFixedSize(this->parentWidget()->width(), this->parentWidget()->height());
}

// -------------------------------------------------------

QString PanelTextures::createlabelText() {
    QString kindText = "";
    switch (m_kind) {
    case PictureKind::Autotiles:
        kindText = "autotiles";
        break;
    case PictureKind::Walls:
        kindText = "walls";
        break;
    default:
        break;
    }

    return "You don't have any " + kindText + " in this tileset. You can add "
        "it in the tileset tab in the datas manager.";
}

// -------------------------------------------------------

void PanelTextures::showAutotiles(SystemTileset *tileset) {
    tileset->updateModelAutotiles();
    fillComboBox(tileset, PictureKind::Autotiles);
}

// -------------------------------------------------------

void PanelTextures::showSpriteWalls(SystemTileset *tileset) {
    tileset->updateModelSpriteWalls();
    fillComboBox(tileset, PictureKind::Walls);
}

// -------------------------------------------------------

void PanelTextures::fillComboBox(SystemTileset *tileset, PictureKind kind) {
    m_kind = kind;

    QStandardItemModel *model = tileset->model(kind);
    QStandardItemModel *modelComplete = RPM::get()->project()
        ->specialElementsDatas()->model(kind);

    // ComboBox filling
    ui->comboBox->clear();
    SuperListItem::fillComboBox(ui->comboBox, model);
    for (int i = 0; i < ui->comboBox->count(); i++) {
        SuperListItem *super = reinterpret_cast<SuperListItem *>(model->item(i)
            ->data().value<quintptr>());
        SystemSpecialElement *special = reinterpret_cast<SystemSpecialElement *>
            (SuperListItem::getById(modelComplete->invisibleRootItem(), super
            ->id()));
        ui->comboBox->setItemIcon(i, QIcon(special->picture()->getPath(m_kind)));
    }

    // Select current ID
    int index = SuperListItem::getIndexById(modelComplete->invisibleRootItem(),
        getCurrentID());
    if (index > 0)
        ui->comboBox->setCurrentIndex(index);

    // UI display
    hideAll();
    showComboBox();
}

// -------------------------------------------------------

int PanelTextures::getCurrentID() const {
    switch (m_kind) {
    case PictureKind::Autotiles:
        return m_currentAutotilesID;
    case PictureKind::Walls:
        return m_currentWallsID;
    default:
        return -1;
    }
}

// -------------------------------------------------------

void PanelTextures::updateCurrentID(int id) {
    switch (m_kind) {
    case PictureKind::Autotiles:
        m_currentAutotilesID = id;
        break;
    case PictureKind::Walls:
        m_currentWallsID = id;
        break;
    default:
        break;
    }
}

// -------------------------------------------------------

void PanelTextures::updateImageSpecial(SystemSpecialElement *special)
{
    switch (m_kind) {
    case PictureKind::Autotiles:
        ui->widgetAutotilesSelector->setImage(reinterpret_cast<SystemAutotile *>
            (special));
        break;
    case PictureKind::Walls:
        ui->widgetWallPreview->updatePicture(special->picture(), m_kind);
        break;
    default:
        break;
    }
}

// -------------------------------------------------------
//
//  SLOTS
//
// -------------------------------------------------------

void PanelTextures::onSplitterMoved(int, int) {
    if (m_kind == PictureKind::None)
        updateTilesetImage();
    else {
        if (ui->comboBox->count() == 0)
            updateLabelSize();
        else
            updateComboBoxSize();
    }
}

// -------------------------------------------------------

void PanelTextures::on_comboBox_currentIndexChanged(int) {
    int id = getID();

    // Update index selection
    if (ui->comboBox->count() > 1)
        updateCurrentID(id);

    // Update picture preview
    SystemSpecialElement* special = reinterpret_cast<SystemSpecialElement *>(
        SuperListItem::getById(RPM::get()->project()->specialElementsDatas()
        ->model(m_kind)->invisibleRootItem(), id));

    // Updating image on preview
    updateImageSpecial(special);
}
