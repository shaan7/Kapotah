/***************************************************************************
 *   Copyright 2009 Shantanu Tushar <jhahoneyk@gmail.com>                  *
 *   Copyright 2009 Sudhendu Roy <sudhendu_roy@yahoo.co.in>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef FILERECIEVERTHREAD_H
#define FILERECIEVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include "peermanager.h"
#include "pointers.h"

class FileRecieverThread : public QThread
{
    Q_OBJECT

private:
    bool doQuit;
    QString ID;
    QString peer;
    PeerManager *manager;
    bool readyToRecieve;
    bool startedRecieving;
    QString filename;
    QFile file;
    qint64 size;
    qint64 bytesCopied;

public:
    FileRecieverThread(Pointers *ptr, QString fileID, qint64 fileSize, QString sendingPeer, QString outputFilename, QObject *parent);
    ~FileRecieverThread();

    void run();
    void stopTransfer();

signals:
    void progress(QString ID, QString peer, QString fileName, qint64 size, qint64 bytesDone);
    void done();
};

#endif // FILERECIEVERTHREAD_H
