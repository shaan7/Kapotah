/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <shaan7in@gmail.com>
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

#include "peerenvironment.h"
#include <QStringList>
#include <QProcess>

using namespace Kapotah;

QString PeerEnvironment::getPeerName()
{
    QString username;
    QStringList env = QProcess::systemEnvironment();

    QStringList varPatterns;
    varPatterns << "USER.*" << "USERNAME.*";

    foreach (QString pattern, varPatterns) {
        int i = env.indexOf(QRegExp(pattern));
        if (i > 0) {
            QStringList list = env.at(i).split('=');
            if (list.count() == 2) {
                username = list.at(1).toUtf8();
            }
        }
    }

    return username;
}
