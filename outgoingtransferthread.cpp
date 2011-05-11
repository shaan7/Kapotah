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

#include "outgoingtransferthread.h"

#include "transfermanager.h"
#include "messagedispatcher.h"
#include <xml/transferxmlparser.h>
#include "messagesenderthread.h"
#include "outgoingtransfer.h"
#include "debug.h"

#include <QFileInfo>
#include <QDir>

OutgoingTransferThread::OutgoingTransferThread (QHostAddress ip, QList< Kapotah::TransferFile > files,
                                                QString parentId, bool isSearchResponse, QObject* parent)
                : TransferThread (ip, parent), m_initialList (files), doQuit (false), m_totalSize(0),
                    m_totalFileCount(0), m_totalDirCount(0), m_parentId(parentId), m_isSearchResponse(isSearchResponse)
{

}

OutgoingTransferThread::~OutgoingTransferThread()
{
    m_mutex.lock();
    doQuit = true;
    m_mutex.unlock();
    wait();
}

void OutgoingTransferThread::run()
{
    setStatus(PreparingList);
    emit startPreparingList();

    TransferXmlData data;
    foreach (Kapotah::TransferFile file, m_initialList) {
        m_mutex.lock();
        if (doQuit) {
            m_mutex.unlock();
            break;
        }
        m_mutex.unlock();

        QFileInfo info (file.path);
        data.items.append(info.fileName());

        if (info.isDir()) {
            QDir parentDir(file.path);
            parentDir.cdUp();
            m_parentDir = parentDir.path();
            addFilesInDir (file.path);
        } else {
            addFileToList(file.path, info.fileName());
        }
    }

    m_mutex.lock();
    if (doQuit) {
        deleteLater();
    }
    m_mutex.unlock();

    emit donePreparingList();
    setStatus(SendingList);
    emit startSendingList();
    TransferXmlParser parser;
    data.type = AbstractXmlData::Transfer;
    data.files = m_files;
    data.totalNumDirs = m_totalDirCount;
    data.totalNumFiles = m_totalFileCount;
    data.totalSize = m_totalSize;
    data.isSearchResponse = m_isSearchResponse;
    data.id = m_parentId;
    MessageSenderThread *thread = new MessageSenderThread (parser.composeXml(&data), m_ip);
    thread->start();
    setStatus(SentList);
    emit doneSendingList();
}

void OutgoingTransferThread::stopTransfer()
{
    doQuit = true;
}

void OutgoingTransferThread::addFilesInDir (QString path)
{
    if (doQuit) {
        return;
    }
    QDir dir (path);

    if (dir.count() == 0)
        return;         //FIXME: This ain't great, empty dirs will be skipped

    foreach (QString file, dir.entryList (QDir::Files | QDir::Hidden)) {
        if (!QFileInfo(dir.absoluteFilePath(file)).isDir()) {
            QString fullPath(dir.absoluteFilePath (file));
            addFileToList(fullPath, QDir(m_parentDir).relativeFilePath(fullPath));
        }
    }

    foreach (QString tdir, dir.entryList (QDir::Hidden | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks)) {
        if (QFileInfo(dir.absoluteFilePath(tdir)).isDir()) {  //QDir::Hidden includes even hidden non-dirs, so have to check
            dir.cd (tdir);
            m_totalDirCount++;
            addFilesInDir(dir.absolutePath());
            dir.cdUp();
        }
    }
}

void OutgoingTransferThread::addFileToList (QString fullPath, QString relativePath)
{
    QFileInfo info(fullPath);
    Kapotah::TransferFile relativeFile;         //File without absolute paths
    relativeFile.id = Kapotah::TransferManager::instance()->newId (fullPath);
    relativeFile.path = relativePath;
    relativeFile.size = info.size();
    m_totalSize += info.size();
    m_totalFileCount++;
    m_files.append(relativeFile);
    kaDebug("Adding " + fullPath + QString("Total %1 bytes %2 MiB from %3 files and %4 directories")
                    .arg(m_totalSize).arg(m_totalSize/1024/1024).arg(m_totalFileCount).arg(m_totalDirCount));
}

void OutgoingTransferThread::setStatus(OutgoingTransferThread::Status status)
{
    m_mutex.lock();
    m_status = status;
    m_mutex.unlock();
}

OutgoingTransferThread::Status OutgoingTransferThread::status()
{
    Status status;
    m_mutex.lock();
    status = m_status;
    m_mutex.unlock();

    return status;
}

#include "outgoingtransferthread.moc"
