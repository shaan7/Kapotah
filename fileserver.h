/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2010 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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

/**
 *\brief TCP Server which listens for transfer requests
 * This class is a TCP server that listens for file transfer
 * requests
 */
class FileServer : public QTcpServer
{
        Q_OBJECT

    public:
        explicit FileServer (QObject* parent = 0);

    protected:
        virtual void incomingConnection (int handle);

    signals:
        /**
         * Emitted when a file is requested
         *
         * @param       id      id of the requested file requested
         */
        void fileRequested (QString id);
};

#endif // FILESERVER_H
