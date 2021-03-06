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

#include "systemweapon.h"

// -------------------------------------------------------
//
//  CONSTRUCTOR / DESTRUCTOR / GET / SET
//
// -------------------------------------------------------

SystemWeapon::SystemWeapon() : SystemArmor()
{

}

SystemWeapon::SystemWeapon(int i, LangsTranslation* names, int k) :
    SystemArmor(i,names, k)
{

}

SystemWeapon::~SystemWeapon(){

}

// -------------------------------------------------------
//
//  READ / WRITE
//
// -------------------------------------------------------

void SystemWeapon::read(const QJsonObject &json){
    SystemArmor::read(json);
}

void SystemWeapon::write(QJsonObject &json) const{
    SystemArmor::write(json);
}
