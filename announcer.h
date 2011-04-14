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


#ifndef KAPOTAH_ANNOUNCER_H
#define KAPOTAH_ANNOUNCER_H

#include "singleton.h"
#include "peer.h"

#include <QHostAddress>

namespace Kapotah
{

    class Announcer : public Singleton<Announcer>
    {
            Q_OBJECT

        public:
            Announcer();
            virtual ~Announcer();
            QString username();
            void setUserName (const QString &username);

        protected:
            virtual void timerEvent (QTimerEvent* t);

        private:
            int m_timerId;
            QString m_username;

        public slots:
            void processDatagram (const QByteArray &datagram, const QHostAddress &host, quint16 port);
            void peerStatus(QHostAddress address);
            void searchPeersForFile(const QString &pattern);

        signals:
            void gotAnnounce (const Peer &peer);
            void peerTyping (const QHostAddress &peer);
            void gotProgress (const QHostAddress &peer, QString id, quint64 bytesDone, quint64 total, quint64 speed);
            void gotSearchRequest (const QString &pattern, const QHostAddress &host);
    };

}

#endif // KAPOTAH_ANNOUNCER_H
