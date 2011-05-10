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

#include "debug.h"

using namespace Kapotah;

template<> Debug *Kapotah::Singleton<Debug>::m_instance = 0;

DebugStream::DebugStream(const QString& sender) : m_sender(sender)
{
}

// void blah(DebugStream &debug)
// {
//     debug.m_message;
// }

// DebugStream &operator<<(DebugStream &debug, const QString &message)
// {
//     debug.m_message.append(message);
//     return debug;
// }

void DebugStream::done()
{
    Debug::instance()->add(m_sender, m_message);
}

void Debug::add(const QString &sender, const QString& message)
{
    emit added(sender, message);
}

DebugStream& Debug::debug(const QString& sender)
{
    
}

#include "debug.moc"
