/***************************************************************************
 *   Copyright 2009 Shantanu Tushar <jhahoneyk@gmail.com>                  *
 *   Copyright 2009 Sudhendu Kumar <sudhendu_roy@yahoo.co.in>                *
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

#ifndef DIRRECIEVERTHREAD_H
#define DIRRECIEVERTHREAD_H

#include <QThread>
#include <QDomNodeList>
#include "peermanager.h"
#include "filereceiverthread.h"
#include "pointers.h"

class DirReceiverThread : public QThread
{
    Q_OBJECT

private:
    Pointers *m_ptr;
    PeerManager *manager;
    QDomNodeList fileList;
    QDomNodeList dirList;
    QHostAddress peerIP;
    QString dirName;
    FileReceiverThread *fileReceiver;
    bool cancel;
    QString m_ID;
public:
    DirReceiverThread(Pointers *ptr, QString ID, QString dir, QDomNodeList files, QDomNodeList dirs,
                      QHostAddress peerIP, QObject *parent);
    ~DirReceiverThread();
    void run();
    void stopTransfer();
signals:
    void done(QString ID);
    void progress(QString ID, int filesDone);
};

#endif // DIRRECIEVERTHREAD_H
