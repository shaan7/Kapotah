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

#ifndef TRANSFERXMLPARSER_H
#define TRANSFERXMLPARSER_H

#include "abstractxmlparser.h"
#include <transfer.h>
#include <QList>

class TransferXmlData : public AbstractXmlData
{
    public:
        TransferXmlData() : AbstractXmlData(), totalSize(0), totalNumFiles(0), totalNumDirs(0),
                            isSearchResponse(false) { }
        QList<Kapotah::TransferFile> files;
        QStringList items;
        quint64 totalSize;
        quint64 totalNumFiles;
        quint64 totalNumDirs;
        bool isSearchResponse;
        QString id;
};

class TransferXmlParser : public AbstractXmlParser
{

    public:
        virtual QString composeXml (AbstractXmlData* data);
        virtual AbstractXmlData* parseXml (const QString& xml);
};

#endif // TRANSFERXMLPARSER_H
