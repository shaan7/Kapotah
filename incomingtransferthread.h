/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <shaan7in@gmail.com>
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

#ifndef INCOMINGTRANSFERTHREAD_H
#define INCOMINGTRANSFERTHREAD_H

#include "transferthread.h"
#include <QHostAddress>
#include <QMutex>

class IncomingTransferThread : public TransferThread
{
        Q_OBJECT

    public:
        enum Status { Connecting, Requesting, Canceled, PreparingToReceive, ErrorCreatingFile, 
                        ErrorTransferNotFound, Receiving };
        explicit IncomingTransferThread (QHostAddress ip, QString id, QString filename,
                                         quint64 size, QObject* parent = 0);
        virtual ~IncomingTransferThread();
        Status status();

    protected:
        virtual void run();

    public slots:
        virtual void stopTransfer();

    private:
        bool doQuit;
        QString m_id;
        QString m_filename;
        quint64 m_size;
        QMutex m_mutex;
        Status m_status;

        void setStatus(Status status);
};

#endif // INCOMINGTRANSFERTHREAD_H
