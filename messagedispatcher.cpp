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


#include "messagedispatcher.h"
#include "xmlparser.h"
#include "messagesenderthread.h"

using namespace Kapotah;

template<> MessageDispatcher *Kapotah::Singleton<MessageDispatcher>::m_instance = 0;

MessageDispatcher::MessageDispatcher()
{
    m_messageDispatcherServer = new MessageDispatcherServer (this);
    connect (m_messageDispatcherServer, SIGNAL (gotMessage (QString, QHostAddress)),
             SLOT (newMessage (QString, QHostAddress)));
}

MessageDispatcher::~MessageDispatcher()
{
}

MessageDispatcherServer* MessageDispatcher::messageDispatcherServer()
{
    return m_messageDispatcherServer;
}

void MessageDispatcher::newMessage (QString message, QHostAddress peerAddress)
{
    XmlParser parser;
    parser.parseXml (message);
    emit gotNewMessage (message, peerAddress);
    qDebug() << "New Message from " << peerAddress.toString() << " reading " << parser.message();
}

void MessageDispatcher::sendNewMessage (QString message, QHostAddress peerAddress)
{
    XmlParser parser;
    parser.setMessage (message);
    parser.setType (XmlParser::Message);
    MessageSenderThread *thread = new MessageSenderThread (parser.composeXml(), peerAddress, this);
    thread->start();
}

#include "messagedispatcher.moc"
