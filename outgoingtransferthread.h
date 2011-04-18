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

#ifndef OUTGOINGTRANSFERTHREAD_H
#define OUTGOINGTRANSFERTHREAD_H

#include "transferthread.h"
#include "transfer.h"

class OutgoingTransferThread : public TransferThread
{
        Q_OBJECT

    public:
        explicit OutgoingTransferThread (QHostAddress ip, QList<Kapotah::TransferFile> files, QString parentId,
                                         bool isSearchResponse, QObject* parent = 0);
        virtual ~OutgoingTransferThread();

    protected:
        virtual void run();

    public slots:
        virtual void stopTransfer();

    private:
        QList<Kapotah::TransferFile> m_initialList;
        QList<Kapotah::TransferFile> m_files;
        bool doQuit;
        QString m_parentDir;    //Stores the current parent dir while recursing
        quint64 m_totalSize;
        quint64 m_totalFileCount;
        quint64 m_totalDirCount;
        QString m_parentId;
        bool m_isSearchResponse;

        void addFilesInDir (QString path);
        void addFileToList (QString fullPath, QString relativePath);

    signals:
        void startPreparingList();
        void donePreparingList();
        void startSendingList();
        void doneSendingList();
};

#endif // OUTGOINGTRANSFERTHREAD_H
