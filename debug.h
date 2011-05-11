/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2011 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KAPOTAH_DEBUG_H
#define KAPOTAH_DEBUG_H

#include "singleton.h"

#define kaDebug(x) Kapotah::Debug::instance()->add(Q_FUNC_INFO, x)

#include <QDebug>

namespace Kapotah
{

/**
 * \brief Kapotah's GUI-assisted debug messaging system
 *
 * Debug output printed through can be shown at any place
 * in the GUI, making debugging easier.
 */
class Debug : public Singleton<Debug>
{
    Q_OBJECT

public slots:
    /**
     * Use to add a debug message
     * \note Its generally easier to use the kaDebug(message) macro
     * 
     * @param   sender  name of the sender function, use the macro to guess
     * @param   message debug message to display
     */
    void add(const QString &sender, const QString &message);

signals:
    /**
     * Emitted when a debug message is added, GUI debugging tools
     * should listen to this signal and take proper action
     *
     * @param   sender  the sender function's signature
     * @param   message the debug message to be displayed
     */
    void added(const QString &sender, const QString &message);
};

}

#endif // KAPOTAH_DEBUG_H
