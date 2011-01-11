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
#include <messagesenderthread.h>

#include <QFileInfo>
#include <QDir>

OutgoingTransferThread::OutgoingTransferThread (QHostAddress ip, QList< Kapotah::TransferFile > files,
        QObject* parent) : TransferThread (ip, parent), m_initialList (files), doQuit (false)
{

}

OutgoingTransferThread::~OutgoingTransferThread()
{
    stopTransfer();
    wait();
}

void OutgoingTransferThread::run()
{
    emit startPreparingList();
    foreach (Kapotah::TransferFile file, m_initialList) {
        if (doQuit) {
            break;
        }
        QFileInfo info (file.path);

        if (info.isDir()) {
            QDir parentDir(file.path);
            parentDir.cdUp();
            m_parentDir = parentDir.path();
            addFilesInDir (file.path);
        } else {
            addFileToList(file.path, info.fileName());
        }
    }
    emit donePreparingList();

    emit startSendingList();
    TransferXMLData data;
    TransferXMLParser parser;
    data.type = AbstractXMLData::Transfer;
    data.files = m_files;
    MessageSenderThread *thread = new MessageSenderThread (parser.composeXML(&data), m_ip);
    thread->start();
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

    foreach (QString file, dir.entryList (QStringList ("*"), QDir::Files)) {
        QString fullPath(dir.absoluteFilePath (file));
        addFileToList(fullPath, QDir(m_parentDir).relativeFilePath(fullPath));
    }

    foreach (QString tdir, dir.entryList (QStringList ("*"), QDir::Dirs | QDir::NoDotAndDotDot)) {
        dir.cd (tdir);
        addFilesInDir(dir.absolutePath());
        dir.cdUp();
    }
}

void OutgoingTransferThread::addFileToList (QString fullPath, QString relativePath)
{
    QFileInfo info(fullPath);
    Kapotah::TransferFile relativeFile;         //File without absolute paths
    relativeFile.id = Kapotah::TransferManager::instance()->newId (fullPath);
    relativeFile.path = relativePath;
    relativeFile.size = info.size();
    m_files.append(relativeFile);
    qDebug() << "Adding " << fullPath;
}

#include "outgoingtransferthread.moc"
