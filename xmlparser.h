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


#ifndef BROADCASTPARSER_H
#define BROADCASTPARSER_H

#include <QXmlStreamReader>

class XmlParser
{

    public:
        enum Type { Error, Announce, Message };
        XmlParser();
        virtual ~XmlParser();
        bool parseXml(QString &string);
        QString composeXml();

        Type type();
        void setType(Type type);

        QString senderName();
        void setSenderName(QString name);

        QString message();
        void setMessage(QString message);

    private:
        void parseAnnounceXml();
        QString composeAnnounceXml();

        void parseMessageXml();
        QString composeMessageXml();

        QXmlStreamReader *m_reader;
        Type m_type;
        QString m_senderName;
        QString m_message;
};

#endif // BROADCASTPARSER_H
