/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <jhahoneyk@gmail.com>
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


#ifndef KAPOTAH_UDPMANAGER_H
#define KAPOTAH_UDPMANAGER_H

#include "singleton.h"
#include "peer.h"

#include <QObject>
#include <QUdpSocket>

namespace Kapotah
{

    class UdpManager : public Singleton<UdpManager>
    {
            Q_OBJECT

        public:
            UdpManager();
            virtual ~UdpManager();
            void updateAddresses();

        protected:
            virtual void timerEvent (QTimerEvent*);

        private:
            QList<QHostAddress> m_broadcastAddresses;
            QList<QHostAddress> m_ipAddresses;
            QUdpSocket m_broadcastSocket;
            int m_timerId;

        private slots:
            void readBroadcast();

        public slots:
            void sendBroadcast (const QByteArray &datagram);
            void sendDatagram (const QByteArray &datagram, const QHostAddress &host);

        signals:
            void gotDatagram (const QByteArray &datagram, const QHostAddress &host, quint16 port);
    };

}

#endif // KAPOTAH_UDPMANAGER_H
