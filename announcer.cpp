/*
    This file is part of the Kapotah project.
    Copyright (C) 2009 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2009 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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


#include "announcer.h"
#include "udpmanager.h"

#include <QTimerEvent>
#include "xmlparser.h"

using namespace Kapotah;

static const int s_broadcastInterval = 1000;

template<> Announcer *Kapotah::Singleton<Announcer>::m_instance = 0;

Announcer::Announcer()
{
    m_timerId = startTimer (s_broadcastInterval);
    connect (UdpManager::instance(), SIGNAL(gotDatagram(QByteArray,QHostAddress,quint16)),
             SLOT(processDatagram(QByteArray,QHostAddress,quint16)));
}

Announcer::~Announcer()
{

}

QString Announcer::username()
{
    return m_username;
}

void Announcer::setUserName (const QString& username)
{
    m_username = username;
}

void Announcer::timerEvent (QTimerEvent* t)
{
    XmlParser parser;
    parser.setSenderName (m_username);
    parser.setType (XmlParser::Announce);

    if (t->timerId() == m_timerId) {
        if (!m_username.isEmpty()) {
            UdpManager::instance()->sendBroadcast (parser.composeXml().toUtf8());
        }
    }

    QObject::timerEvent (t);
}

void Announcer::processDatagram (const QByteArray& datagram, const QHostAddress& host, quint16 port)
{
    XmlParser parser;
    QString xml (datagram);
    parser.parseXml (xml);

    if (parser.type() == XmlParser::Announce) {
        Peer peer (parser.senderName(), host);
        emit gotAnnounce (peer);
    }
}

#include "announcer.moc"
