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

#ifndef KAPOTAH_GLOBAL_H
#define KAPOTAH_GLOBAL_H

#include "announcer.h"

#include <QStringList>

namespace Kapotah{
    /**
     * Caclulates a human readable notation for a byte size
     *
     * @param   num     number in bytes
     */
    inline QString humanReadableNumber(quint64 num) {
        QStringList suffixes;
        suffixes << "" << "Ki" << "Mi" << "Gi" << "Ti";
        int current=0;

        while (num > 1024) {
            num /= 1024;
            current++;
        }

        if (current >= suffixes.count()) {
            qDebug() << "WARNING: Too Much Speed";
            return "X";
        }

        return (QString::number(num) + " " + suffixes.at(current));
    }
}

#endif  //KAPOTAH_GLOBAL_H