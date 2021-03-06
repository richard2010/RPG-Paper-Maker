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

#include "paneldatasmonster.h"
#include "ui_paneldatasmonster.h"
#include "paneldatascharacter.h"

// -------------------------------------------------------
//
//  CONSTRUCTOR / DESTRUCTOR / GET / SET
//
// -------------------------------------------------------

PanelDatasMonster::PanelDatasMonster(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelDatasMonster)
{
    ui->setupUi(this);
}

PanelDatasMonster::~PanelDatasMonster()
{
    delete ui;
}

void PanelDatasMonster::setPanelSuperList(PanelSuperList *list) {
    m_panelSuperList = list;
    ui->panelDatasCharacter->setPanelSuperList(list);
}

// -------------------------------------------------------
//
//  INTERMEDIARY FUNCTIONS
//
// -------------------------------------------------------

void PanelDatasMonster::initialize() {
    connect(ui->panelDatasCharacter->panelDatasClass(), SIGNAL(maxLevelUpdated(
        int)), this, SLOT(on_maxLevelChanged(int)));
}

// -------------------------------------------------------

void PanelDatasMonster::update(SystemMonster *monster, int classIndex) {
    ui->panelDatasCharacter->update(monster, classIndex);
    ui->panelProgressionTableRewardExp->setProgression(monster->experience());
    ui->panelProgressionTableRewardExp->setMaxLevel(monster->maxLevel());
    ui->panelProgressionTableRewardExp->updateProgress();
}

// -------------------------------------------------------

SystemHero * PanelDatasMonster::currentHero() {
    return ui->panelDatasCharacter->currentHero();
}

// -------------------------------------------------------

void PanelDatasMonster::updateClasses() {
    ui->panelDatasCharacter->updateClasses();
}

// -------------------------------------------------------

void PanelDatasMonster::updateClass() {
    ui->panelDatasCharacter->updateClass();
}

// -------------------------------------------------------
//
//  CONSTRUCTOR / DESTRUCTOR / GET / SET
//
// -------------------------------------------------------

void PanelDatasMonster::on_maxLevelChanged(int lvl) {
    ui->panelProgressionTableRewardExp->setMaxLevel(lvl);
    ui->panelProgressionTableRewardExp->updateProgress();
}
