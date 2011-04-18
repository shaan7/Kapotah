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

#include "transferxmlparser.h"
#include <QXmlStreamReader>

static const char *s_true = "true";
static const char *s_false = "false";

QString TransferXmlParser::composeXml (AbstractXmlData* data)
{
    TransferXmlData *tdata = static_cast<TransferXmlData*> (data);
    QString xml;
    QXmlStreamWriter writer (&xml);
    writer.setAutoFormatting (true);
    writer.writeStartDocument();
    writer.writeStartElement ("transfer");
    writer.writeAttribute("size", QString::number(tdata->totalSize));
    writer.writeAttribute("files", QString::number(tdata->totalNumFiles));
    writer.writeAttribute("dirs", QString::number(tdata->totalNumDirs));
    writer.writeAttribute("isSearchResponse", tdata->isSearchResponse ? s_true : s_false);
    writer.writeAttribute("id", tdata->id);

    foreach (QString itemName, tdata->items) {
        writer.writeStartElement ("item");
        writer.writeAttribute ("name", itemName);
        writer.writeEndElement();
    }

    foreach (Kapotah::TransferFile file, tdata->files) {
        writer.writeStartElement ("file");
        writer.writeAttribute ("id", file.id);
        writer.writeAttribute ("path", file.path);
        writer.writeAttribute ("size", QString::number (file.size));
        writer.writeEndElement();
    }

    writer.writeEndElement();
    writer.writeEndDocument();
    return xml;
}

AbstractXmlData* TransferXmlParser::parseXml (const QString& xml)
{
    QXmlStreamReader reader (xml);
    reader.readNext();

    TransferXmlData *data = new TransferXmlData;
    data->type = AbstractXmlData::Error;

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "transfer") {
                data->type = AbstractXmlData::Transfer;
                data->files.clear();
                data->totalSize = reader.attributes().value ("size").toString().toULongLong();
                data->totalNumFiles = reader.attributes().value ("files").toString().toULongLong();
                data->totalNumDirs = reader.attributes().value ("dirs").toString().toULongLong();
                data->isSearchResponse =
                    reader.attributes().value("isSearchResponse").toString() == s_true ? true : false;
                data->id = reader.attributes().value("id").toString();
                reader.readNext();

                while (!reader.atEnd()) {
                    if (reader.isStartElement()) {
                        if (reader.name() == "file") {
                            Kapotah::TransferFile file = { reader.attributes().value ("id").toString(),
                                                           reader.attributes().value ("path").toString(),
                                                           reader.attributes().value ("size").toString().toULongLong()
                                                         };
                            data->files.append (file);
                        } else if (reader.name() == "item") {
                            QString itemName = reader.attributes().value("name").toString();
                            data->items.append(itemName);
                        }
                    } else {
                        if (reader.name() == "transfer") {
                            return data;
                        }
                    }
                    reader.readNext();
                }
                break;
            }
        } else {
            reader.readNext();
        }
    }

    return data;
}

