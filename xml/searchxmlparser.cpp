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

#include "searchxmlparser.h"
#include <QXmlStreamReader>

QString SearchXmlParser::composeXml(AbstractXmlData* data)
{
    QString xml;
    QXmlStreamWriter writer (&xml);
    writer.setAutoFormatting (true);
    writer.writeStartDocument();
    writer.writeStartElement ("search");
    writer.writeAttribute ("pattern", static_cast<SearchXmlData*>(data)->pattern);
    writer.writeEndElement();
    writer.writeEndDocument();
    return xml;
}

AbstractXmlData* SearchXmlParser::parseXml(const QString& xml)
{
    QXmlStreamReader reader(xml);
    reader.readNext();

    SearchXmlData *data = new SearchXmlData;
    data->type = AbstractXmlData::Error;

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "search") {
                data->type = AbstractXmlData::Search;
                data->pattern = reader.attributes().value ("pattern").toString();
                break;
            }
            data->type = AbstractXmlData::Error;
            break;
        } else {
            reader.readNext();
        }
    }
    return data;
}
