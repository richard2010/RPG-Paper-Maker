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

#ifndef DIALOGPROGRESS_H
#define DIALOGPROGRESS_H

#include <QDialog>

// -------------------------------------------------------
//
//  CLASS DialogProgress
//
//  Dialog used for displaying progress.
//
// -------------------------------------------------------

namespace Ui {
class DialogProgress;
}

class DialogProgress : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProgress(QWidget *parent = 0);
    ~DialogProgress();

private:
    Ui::DialogProgress *ui;
    int m_totalCount;
    int m_count;
    int m_beginValue;
    int m_endValue;

public slots:
    void setValueLabel(int v, QString s);
    void setDescription(QString s);
    void setCount(int v);
    void addOne();
};

#endif // DIALOGPROGRESS_H
