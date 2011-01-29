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

#include "announcexmlparser.h"
#include <QXmlStreamReader>

QString AnnounceXMLParser::composeXML (AbstractXMLData *data)
{
    QString xml;
    QXmlStreamWriter writer (&xml);
    writer.setAutoFormatting (true);
    writer.writeStartDocument();
    writer.writeStartElement ("announce");
    writer.writeAttribute ("senderName", static_cast<AnnounceXMLData*>(data)->senderName);
    writer.writeEndElement();
    writer.writeEndDocument();
    return xml;
}

AbstractXMLData* AnnounceXMLParser::parseXML (const QString& xml)
{
    QXmlStreamReader reader(xml);
    reader.readNext();

    AnnounceXMLData *data = new AnnounceXMLData;
    data->type = AbstractXMLData::Error;

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "announce") {
                data->type = AbstractXMLData::Announce;
                data->senderName = reader.attributes().value ("senderName").toString();
                break;
            }
            data->type = AbstractXMLData::Error;
            break;
        } else {
            reader.readNext();
        }
    }
    return data;
}
