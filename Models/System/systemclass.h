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

#ifndef SYSTEMCLASS_H
#define SYSTEMCLASS_H

#include <QStandardItemModel>
#include <QMetaType>
#include "superlistitem.h"

// -------------------------------------------------------
//
//  CLASS SystemClass
//
//  A particulary class (datas).
//
// -------------------------------------------------------

class SystemClass : public SuperListItem
{
public:
    static const QString jsonInitialLevel;
    static const QString jsonMaxLevel;
    static const QString jsonExpBase;
    static const QString jsonExpInflation;
    static const QString jsonExpTable;
    static const QString jsonStats;
    static const QString jsonSkills;

    SystemClass();
    SystemClass(int i, QString n, int initialLevel, int maxLevel, int expBase,
                int expInflation);
    SystemClass(int i, QString n, int initialLevel, int maxLevel, int expBase,
                int expInflation,
                QStandardItemModel *stat, QStandardItemModel *s);
    virtual ~SystemClass();
    int initialLevel() const;
    void setInitialLevel(int i, SystemClass *originalClass = nullptr);
    int maxLevel() const;
    void setMaxLevel(int i, SystemClass *originalClass = nullptr);
    int expBase() const;
    void setExpBase(int i, SystemClass *originalClass = nullptr);
    int expInflation() const;
    void setExpInflation(int i, SystemClass *originalClass = nullptr);
    QHash<int, int> * expTable();
    QStandardItemModel* statisticsProgression() const;
    QStandardItemModel* skills() const;

    static SystemClass * createInheritanceClass();

    void reset();
    void setClassValues();

    virtual SuperListItem* createCopy() const;
    virtual void read(const QJsonObject &json);
    virtual void write(QJsonObject &json) const;

protected:
    int m_initialLevel;
    int m_maxLevel;
    int m_expBase;
    int m_expInflation;
    QHash<int, int> m_expTable;
    QStandardItemModel* m_statisticsProgression;
    QStandardItemModel* m_skills;
};

Q_DECLARE_METATYPE(SystemClass)

#endif // SYSTEMCLASS_H
