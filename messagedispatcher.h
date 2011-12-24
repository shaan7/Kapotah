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


#ifndef KAPOTAH_MESSAGEDISPATCHER_H
#define KAPOTAH_MESSAGEDISPATCHER_H

#include "singleton.h"
#include "messagedispatcherserver.h"

namespace Kapotah
{

    /**
     * \brief Handles sending and notifies reception of messages
     *
     * This classes uses MessageDispatcherServer to send messages,
     * and notifies when new messages arrive
     */
    class MessageDispatcher : public Singleton<MessageDispatcher>
    {
            Q_OBJECT

        public:
            MessageDispatcher();

            /**
             * Returns the associated MessageDispatcherServer
             *
             * @return  pointer to the associated MessageDispatcherServer
             */
            MessageDispatcherServer *messageDispatcherServer();

        private:
            MessageDispatcherServer *m_messageDispatcherServer;

        private slots:
            void newMessage (QString message, QHostAddress peerAddress);

        public slots:
            /**
             * Send a message to a peer on the network
             *
             * @param   message         the message to send
             * @param   peerAddress     the peer to whom the message should be sent
             */
            void sendNewMessage (QString message, QHostAddress peerAddress);

        signals:
            /**
             * Emitted when a new message has been received
             *
             * @param   message         the new message
             * @param   peerAddress     the peer who send the message
             */
            void gotNewMessage (QString message, QHostAddress peerAddress);

            /**
             * Emitted when a new transfer message has been received
             *
             * @param   transfer        the transfer XML data
             * @param   peerAddress     the peer who send the transfer
             */
            void gotNewTransfer (QString transfer, QHostAddress peerAddress);
    };

}

#endif // KAPOTAH_MESSAGEDISPATCHER_H
