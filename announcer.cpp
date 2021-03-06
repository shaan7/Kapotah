/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <shaan7in@gmail.com>
    Copyright (C) 2010 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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
#include <udpmanager.h>
#include <xml/announcexmlparser.h>
#include <xml/peerstatusxmlparser.h>
#include <xml/transferstatusxmlparser.h>
#include <xml/searchxmlparser.h>

#include <QTimerEvent>

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
    if (t->timerId() == m_timerId) {
        if (!m_username.isEmpty()) {
            AnnounceXmlParser parser;
            AnnounceXmlData data;
            data.senderName = m_username;
            data.type = AnnounceXmlData::Announce;
            UdpManager::instance()->sendBroadcast(parser.composeXml(&data).toUtf8());
        }
    }

    QObject::timerEvent (t);
}

void Announcer::peerStatus(QHostAddress address)
{
    QString addr = address.toString();
    PeerStatusXmlParser parser;
    PeerStatusXmlData data;
    data.isTyping = true;
    data.type = PeerStatusXmlData::PeerStatus;
    UdpManager::instance()->sendDatagram(parser.composeXml(&data).toUtf8(), QHostAddress(addr));
}

void Announcer::processDatagram (const QByteArray& datagram, const QHostAddress& host, quint16 port)
{
    AnnounceXmlParser announceParser;
    PeerStatusXmlParser statusParser;
    TransferStatusXmlParser transferStatusParser;
    SearchXmlParser searchParser;
    QString xml (datagram);
    AnnounceXmlData *announceData = static_cast<AnnounceXmlData*>(announceParser.parseXml(xml));

    if (announceData->type == AnnounceXmlData::Announce) {
        Peer peer (announceData->senderName, host);
        emit gotAnnounce (peer);
        delete announceData;
    } else {
        PeerStatusXmlData *statusData = static_cast<PeerStatusXmlData*>(statusParser.parseXml(xml));
        if (statusData->type == PeerStatusXmlData::PeerStatus) {
            if (statusData->isTyping) {
                emit peerTyping (host);
            }
        delete statusData;
        } else {
            TransferStatusXmlData *transferStatusData = static_cast<TransferStatusXmlData*>(transferStatusParser.parseXml(xml));
            if (transferStatusData->type == TransferStatusXmlData::TransferStatus) {
                emit gotProgress(host, transferStatusData->id, transferStatusData->bytesDone,
                                 transferStatusData->total, transferStatusData->speed);
                delete transferStatusData;
            } else {
                SearchXmlData *searchXmlData = static_cast<SearchXmlData*>(searchParser.parseXml(xml));
                if (searchXmlData->type == SearchXmlData::Search) {
                    if (!UdpManager::instance()->isLocalHostIp(host)) {
                        emit gotSearchRequest(searchXmlData->pattern, host);
                    }
                }
            }
        }
    }
}

void Announcer::searchPeersForFile (const QString& pattern)
{
    SearchXmlData data;
    data.pattern = pattern;
    data.type = AbstractXmlData::Search;
    SearchXmlParser parser;
    Kapotah::UdpManager::instance()->sendBroadcast(parser.composeXml(&data).toUtf8());
}

void Announcer::addAdditionalBroadcastAddress(const QHostAddress& broadcastAddress)
{
    Kapotah::UdpManager::instance()->addBroadcastAddress(broadcastAddress);
}

void Announcer::removeAdditionalBroadcastAddress(const QHostAddress& broadcastAddress)
{
    Kapotah::UdpManager::instance()->addBroadcastAddress(broadcastAddress);
}

#include "announcer.moc"
