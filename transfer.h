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

#ifndef KAPOTAH_TRANSFER_H
#define KAPOTAH_TRANSFER_H

#include <QObject>
#include <QHostAddress>
#include "transferthread.h"

namespace Kapotah
{

    struct TransferFile {
        QString id;
        QString path;
        quint64 size;
    };

    class Transfer : public QObject
    {
            Q_OBJECT

        public:
            enum TransferType { Incoming, Outgoing };
            explicit Transfer (TransferType type, QList<TransferFile> files, QHostAddress peer,
                               QObject* parent = 0);
            virtual ~Transfer();
            void start();

        private:
            QList<TransferFile>::iterator m_filesIterator;

            //Details about the transfer
            QHostAddress m_peerAddress;
            TransferType m_type;
            quint64 m_totalSize;
            quint64 m_numFiles;
            QList<TransferFile> m_files;
            TransferFile m_currentFile;

            TransferThread *m_thread;

        private  slots:
            void startNextFile();
            void currentFileFinished();
            void reportProgress(quint64 done, quint64 size);

        signals:
            void progress(quint64 done, quint64 total);
            void done();
    };

}

#endif // KAPOTAH_TRANSFER_H