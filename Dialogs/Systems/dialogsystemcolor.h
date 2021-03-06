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

#ifndef DIALOGSYSTEMCOLOR_H
#define DIALOGSYSTEMCOLOR_H

#include <QDialog>
#include "systemcolor.h"

// -------------------------------------------------------
//
//  CLASS DialogSystemColor
//
//  A dialog used for editing the model of a system color.
//
// -------------------------------------------------------

namespace Ui {
class DialogSystemColor;
}

class DialogSystemColor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSystemColor(SystemColor& color, QWidget *parent = nullptr);
    ~DialogSystemColor();

    SystemColor& color() const;

    void setBackgroundColor(QColor& color);

private:
    Ui::DialogSystemColor *ui;
    SystemColor& m_color;
    void initialize();

private slots:
    void on_lineEditColor_textChanged(const QString &text);
    void on_pushButtonColor_clicked(bool = false);
};

#endif // DIALOGSYSTEMCOLOR_H
