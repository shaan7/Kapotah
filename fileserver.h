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


#ifndef FILESERVER_H
#define FILESERVER_H

#include <QtNetwork/QTcpServer>

class FileServer : public QTcpServer
{
        Q_OBJECT

    public:
        explicit FileServer (QObject* parent = 0);
        virtual ~FileServer();

    protected:
        virtual void incomingConnection (int handle);

    signals:
        void fileRequested (QString id);
};

#endif // FILESERVER_H
