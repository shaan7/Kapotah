/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <shaan7in@gmail.com>
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

#include "messagexmlparser.h"
#include <QXmlStreamReader>
#include <QDebug>

QString MessageXmlParser::composeXml (AbstractXmlData* data)
{
    QString xml;
    QXmlStreamWriter writer (&xml);
    writer.setAutoFormatting (true);
    writer.writeStartDocument();
    writer.writeTextElement ("message", static_cast<MessageXmlData*>(data)->message);
    writer.writeEndDocument();
    return xml;
}

AbstractXmlData* MessageXmlParser::parseXml (const QString& xml)
{
    QXmlStreamReader reader(xml);
    reader.readNext();

    MessageXmlData *data = new MessageXmlData;
    data->type = AbstractXmlData::Error;

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "message") {
                data->type = AbstractXmlData::Message;
                data->message = reader.readElementText();
                break;
            } else {
                data->type = AbstractXmlData::Error;
                break;
            }
        }
        else {
            reader.readNext();
        }
    }
    return data;
}
