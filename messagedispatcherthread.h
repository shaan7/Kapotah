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


#ifndef MESSAGEDISPATCHERTHREAD_H
#define MESSAGEDISPATCHERTHREAD_H

#include <QThread>
#include <QHostAddress>

/**
 * \brief Thread that processes each message the MessageDispatcher receives
 *
 * This is a thread which is run by the MessageDispatcher whenever a new message
 * is about to arrive
 */
class MessageDispatcherThread : public QThread
{
    Q_OBJECT

    public:
        explicit MessageDispatcherThread (int socketDescriptor, QObject* parent = 0);
        virtual ~MessageDispatcherThread();
        virtual void run();

    private:
        int m_descriptor;
        bool m_doQuit;

    signals:
        /**
         * Emitted when the thread is done processing a message
         *
         * @param       message         message raw text
         * @param       peerAddress     address of the peer
         */
        void gotMessage (QString message, QHostAddress peerAddress);
};

#endif // MESSAGEDISPATCHERTHREAD_H
