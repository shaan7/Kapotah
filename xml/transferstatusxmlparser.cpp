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

#include "transferstatusxmlparser.h"
#include <QXmlStreamReader>

QString TransferStatusXmlParser::composeXml(AbstractXmlData* data)
{
    QString xml;
    QXmlStreamWriter writer (&xml);
    writer.setAutoFormatting (true);
    writer.writeStartDocument();
    writer.writeStartElement ("transferStatus");
    writer.writeAttribute ("id", static_cast<TransferStatusXmlData*>(data)->id);
    writer.writeAttribute ("bytesDone", QString::number(static_cast<TransferStatusXmlData*>(data)->bytesDone));
    writer.writeAttribute ("total", QString::number(static_cast<TransferStatusXmlData*>(data)->total));
    writer.writeAttribute ("speed", QString::number(static_cast<TransferStatusXmlData*>(data)->speed));
    writer.writeEndElement();
    writer.writeEndDocument();
    return xml;
}

AbstractXmlData* TransferStatusXmlParser::parseXml(const QString& xml)
{
    QXmlStreamReader reader(xml);
    reader.readNext();

    TransferStatusXmlData *data = new TransferStatusXmlData;
    data->type = AbstractXmlData::Error;

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "transferStatus") {
                data->type = AbstractXmlData::TransferStatus;
                data->id = reader.attributes().value ("id").toString();
                data->bytesDone = reader.attributes().value ("bytesDone").toString().toULongLong();
                data->total = reader.attributes().value ("total").toString().toULongLong();
                data->speed = reader.attributes().value ("speed").toString().toULongLong();
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

TransferStatusXmlParser::TransferStatusXmlParser()
{

}

TransferStatusXmlParser::~TransferStatusXmlParser()
{

}

