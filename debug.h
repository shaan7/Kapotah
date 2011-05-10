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

class DebugStream
{
public:
    DebugStream(const QString &sender);
    //friend DebugStream &operator<<(DebugStream &debug, const QString &message);
    friend void blah(DebugStream &debug);
    void done();

private:
    QString m_sender;
    QString m_message;
};

class Debug : public Singleton<Debug>
{
    Q_OBJECT

public:
    

public slots:
    DebugStream &debug(const QString& sender);
    void add(const QString &sender, const QString &message);

signals:
    void added(const QString &sender, const QString &message);
};

}

#endif // KAPOTAH_DEBUG_H
