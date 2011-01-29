/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2011 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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

#include "peerstatusxmlparser.h"
#include <QXmlStreamReader>
#include <QDebug>

QString PeerStatusXMLParser::composeXML (AbstractXMLData* data)
{
    QString xml;
    QXmlStreamWriter writer (&xml);
    writer.setAutoFormatting (true);
    writer.writeStartDocument();
    writer.writeStartElement ("peerStatus");
    if (static_cast<PeerStatusXMLData*>(data)->isTyping) {
        writer.writeAttribute ("isTyping", "true");
    } else {
        writer.writeAttribute ("isTyping", "false");
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    return xml;
}

AbstractXMLData* PeerStatusXMLParser::parseXML (const QString& xml)
{
    qDebug() << "HERE " << xml;
    QXmlStreamReader reader(xml);
    reader.readNext();

    PeerStatusXMLData *data = new PeerStatusXMLData;
    data->type = AbstractXMLData::Error;

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "peerStatus") {
                data->type = AbstractXMLData::PeerStatus;
                if(reader.attributes().value("isTyping")=="true") {
                    data->isTyping = true;
                } else {
                    data->isTyping = false;
                }
                break;
            }
            data->type = AbstractXMLData::Error;
            break;
        }
        else {
            reader.readNext();
        }
    }
    return data;
}
