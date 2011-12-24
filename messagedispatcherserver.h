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


#ifndef MESSAGEDISPATCHERSERVER_H
#define MESSAGEDISPATCHERSERVER_H

#include <QTcpServer>
#include <QHostAddress>

/**
 * \brief TCP Server that listens for messages on the network
 *
 * This is a TCP server that listens for messages, which include
 * normal text messages, and transfer metadata
 */
class MessageDispatcherServer : public QTcpServer
{
        Q_OBJECT

    protected:
        virtual void incomingConnection (int handle);

    public:
        MessageDispatcherServer (QObject* parent = 0);

    signals:
        /**
         * Emitted when a new message is received
         *
         * @param       message         XML message text
         * @param       peerAddress     peer who sent the message
         */
        void gotMessage (QString message, QHostAddress peerAddress);
};

#endif // MESSAGEDISPATCHERSERVER_H
