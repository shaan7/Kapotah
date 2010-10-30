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


#ifndef KAPOTAH_MESSAGEDISPATCHER_H
#define KAPOTAH_MESSAGEDISPATCHER_H

#include "singleton.h"
#include "messagedispatcherserver.h"

namespace Kapotah
{

    class MessageDispatcher : public Singleton<MessageDispatcher>
    {
            Q_OBJECT

        public:
            MessageDispatcher();
            virtual ~MessageDispatcher();
            MessageDispatcherServer *messageDispatcherServer();

        private:
            MessageDispatcherServer *m_messageDispatcherServer;

        private slots:
            void newMessage (QString message, QHostAddress peerAddress);

        public slots:
            void sendNewMessage (QString message, QHostAddress peerAddress);

        signals:
            void gotNewMessage (QString message, QHostAddress peerAddress);
    };

}

#endif // KAPOTAH_MESSAGEDISPATCHER_H
