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


#include "xmlparser.h"

XmlParser::XmlParser() : m_reader (0)
{
    m_type = Error;
}

XmlParser::~XmlParser()
{

}

bool XmlParser::parseXml (QString &string)
{
    m_reader = new QXmlStreamReader (string);
    m_reader->readNext();

    while (!m_reader->atEnd()) {
        if (m_reader->isStartElement()) {
            if (m_reader->name() == "announce") {
                parseAnnounceXml();
                break;
            } else if (m_reader->name() == "message") {
                parseMessageXml();
                break;
            } else {
                m_reader->raiseError ("Not recognized");
                return false;
            }
        } else {
            m_reader->readNext();
        }
    }
}

QString XmlParser::composeXml()
{
    switch (m_type) {

    case Error:
        return QString();

    case Announce:
        return composeAnnounceXml();

    case Message:
        return composeMessageXml();
    }
}

XmlParser::Type XmlParser::type()
{
    return m_type;
}

void XmlParser::setType (XmlParser::Type type)
{
    m_type = type;
}

QString XmlParser::senderName()
{
    return m_senderName;
}

void XmlParser::setSenderName (QString name)
{
    m_senderName = name;
}

QString XmlParser::message()
{
    return m_message;
}

void XmlParser::setMessage (QString message)
{
    m_message = message;
}

void XmlParser::parseAnnounceXml()
{
    m_senderName = m_reader->attributes().value ("senderName").toString();
    m_type = Announce;
}

QString XmlParser::composeAnnounceXml()
{
    QString xml;
    QXmlStreamWriter writer (&xml);
    writer.setAutoFormatting (true);
    writer.writeStartDocument();
    writer.writeStartElement ("announce");
    writer.writeAttribute ("senderName", m_senderName);
    writer.writeEndDocument();
    return xml;
}

void XmlParser::parseMessageXml()
{
    m_message = m_reader->readElementText();
    m_type = Message;
}

QString XmlParser::composeMessageXml()
{
    QString xml;
    QXmlStreamWriter writer (&xml);
    writer.setAutoFormatting (true);
    writer.writeStartDocument();
    writer.writeTextElement ("message", m_message);
    writer.writeEndDocument();
    return xml;
}
