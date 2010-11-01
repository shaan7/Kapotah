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


#ifndef KAPOTAH_BROADCASTMANAGER_H
#define KAPOTAH_BROADCASTMANAGER_H

#include "singleton.h"
#include "peer.h"

#include <QObject>
#include <QUdpSocket>

namespace Kapotah
{

    class BroadcastManager : public Singleton<BroadcastManager>
    {
            Q_OBJECT

        public:
            BroadcastManager();
            virtual ~BroadcastManager();

        protected:
            virtual void timerEvent (QTimerEvent*);

        private:
            void updateAddresses();

            QList<QHostAddress> m_broadcastAddresses;
            QList<QHostAddress> m_ipAddresses;
            QUdpSocket m_broadcastSocket;
            int m_timerId;

        private slots:
            void readBroadcast();

        public slots:
            void sendAnnounce (QString username);

        signals:
            void gotAnnounce (Peer peer);
    };

}

#endif // KAPOTAH_BROADCASTMANAGER_H