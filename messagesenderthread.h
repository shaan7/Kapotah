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


#ifndef MESSAGESENDERTHREAD_H
#define MESSAGESENDERTHREAD_H

#include <QThread>
#include <QHostAddress>

/**
 * \brief Thread that sends messages to peers
 *
 * This thread is responsible for sending messgaes to peers.
 * A thread is used here so as to not to block the GUI thread
 */
class MessageSenderThread : public QThread
{
    Q_OBJECT

    public:
        explicit MessageSenderThread (QString message, QHostAddress peerAddress,
                                      QObject* parent = 0);
        virtual ~MessageSenderThread();
        virtual void run();

    private:
        bool m_doQuit;
        QString m_message;
        QHostAddress m_peerAddress;
};

#endif // MESSAGESENDERTHREAD_H
