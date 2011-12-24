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

#ifndef ABSTRACTXMLPARSER_H
#define ABSTRACTXMLPARSER_H

#include <QString>

/**
 * \brief Abstract base class for data storage for XML parsers for parsing Kapotah data
 *
 * This class provides a base for implementing XML data storage classes for various XML
 * parsers which implement AbstractXmlParser
 */
class AbstractXmlData
{
    public:
        enum Type { Error, Announce, Message, Transfer, PeerStatus, TransferStatus, Search };

        /**
         * the type of the XML data
         * \remarks It is important to add more typedefs when new parsers are implemented
         */
        Type type;
};

/**
 * \brief Abstract base class for XML parsers for parsing Kapotah data
 *
 * This class provides a base for implementing XML parsers for various uses
 * inside Kapotah communications
 */
class AbstractXmlParser
{
    public:
        /**
         * Accepts XML data and returns pointer to parsed data
         * \note pointer will actually be to a data storage class
         * deriving from AbstractXmlData
         */
        virtual AbstractXmlData *parseXml (const QString &xml) = 0;

        /**
         * Takes pointer to a data storage class deriving from AbstractXmlData
         * and returns the created XML data
         */
        virtual QString composeXml (AbstractXmlData *data) = 0;
};

#endif // ABSTRACTXMLPARSER_H
