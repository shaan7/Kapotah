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

#ifndef FILESERVERTHREAD_H
#define FILESERVERTHREAD_H

#include <QThread>
#include <QMutex>

class FileServerThread : public QThread
{
        Q_OBJECT

    protected:
        virtual void run();

    public:
        enum Status { Waiting, PreparingToSend, ErrorFileNotFound, ErrorIDNotFound, Sending, Finished, Canceled };
        explicit FileServerThread (int socketDescriptor, QObject* parent = 0);
        virtual ~FileServerThread();
        Status status();

    private:
        int m_descriptor;
        bool m_doQuit;
        Status m_status;
        QMutex m_mutex;

        void setStatus(Status status);

    signals:
        void startedTransfer (QString ID);
        void finishedTransfer (QString ID);
        void transferProgress (QString ID, quint64 percentDone);
        void canceledTransfer (QString ID);
        void transferNotFound (QString ID);
};

#endif // FILESERVERTHREAD_H
