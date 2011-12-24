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

#ifndef MESSAGEXMLPARSER_H
#define MESSAGEXMLPARSER_H

#include "abstractxmlparser.h"
#include <QString>

/**
 * XML data for message
 */
class MessageXmlData : public AbstractXmlData
{
    public:
        QString message;
};

/**
 * XML Parser for message data
 */
class MessageXmlParser : public AbstractXmlParser
{
    public:
        virtual QString composeXml (AbstractXmlData *data);
        virtual AbstractXmlData* parseXml (const QString& xml);
};

#endif // MESSAGEXMLPARSER_H
