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

#include "dirreceiverthread.h"
#include <QDir>

DirReceiverThread::DirReceiverThread(Pointers *ptr, QString ID, QString dir, QDomNodeList files, QDomNodeList dirs, QHostAddress peerip, QObject *parent)
    : QThread(parent), manager(ptr->manager), fileList(files), dirList(dirs), peerIP(peerip), m_ptr(ptr), dirName(dir), m_ID(ID)
{
    cancel = false;
}

void DirReceiverThread::run()
{
    QDir dir(dirName);
    for (int i=0;i<dirList.count();i++) {
        dir.mkdir(dirList.at(i).toElement().attribute("path"));
    }

    for (int i=0;i<fileList.count();i++) {
        if (cancel)
            break;
        fileReceiver = new FileReceiverThread(m_ptr, fileList.at(i).toElement().attribute("ID"),
                                          fileList.at(i).toElement().attribute("size").toULongLong(), peerIP,
                                          dir.absoluteFilePath(fileList.at(i).toElement().attribute("path")), 0);
        //qDebug() << "START " << dir.absoluteFilePath(fileList.at(i).toElement().attribute("path"));
        //fileReceiver->setPId(m_ID);
        //emit receiverChanged(m_ID, fileReceiver, i+1);
        fileReceiver->run();
        delete fileReceiver;

        emit progress(m_ID, i+1);
    }
    emit done(m_ID);    //This will be emitted even if canceled !!
    deleteLater();
}

void DirReceiverThread::stopTransfer()
{
    cancel = true;
}

DirReceiverThread::~DirReceiverThread()
{
    cancel = true;
    wait();
    qDebug() << "DirReceiverThread END";
}
