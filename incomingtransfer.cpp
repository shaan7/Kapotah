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

#include "incomingtransfer.h"

#include "incomingtransferthread.h"
#include "udpmanager.h"
#include <xml/transferstatusxmlparser.h>
#include <QDir>
#include <QDateTime>
#include <QTimerEvent>

using namespace Kapotah;

IncomingTransfer::IncomingTransfer (QList< TransferFile > files, quint64 totalSize, quint64 numFiles,
                                    quint64 numDirs, QHostAddress peer, QString id, bool isSearchResponse,
                                    QObject* parent) : Transfer (files, totalSize, numFiles, numDirs, peer,
                                                                 isSearchResponse, parent)
{
    m_doneSinceLastSpeedEstimate = 0;
    m_prevTime = QDateTime::currentMSecsSinceEpoch()/1000;  //secs
    m_speed = 0;
    startTimer(1000);
    m_id = id;
}

void IncomingTransfer::start()
{
    if (m_destinationDir.isEmpty()) {
        emit needDestinationDir();
    } else {
        m_filesIterator = m_files.begin();
        startNextFile();
    }
}

Transfer::TransferType IncomingTransfer::type()
{
    return Incoming;
}

void IncomingTransfer::startNextFile()
{
    QDir destinationDir(m_destinationDir);
    m_thread = new IncomingTransferThread (m_peerAddress, m_filesIterator->id,
                                           destinationDir.absoluteFilePath(m_filesIterator->path),
                                           m_filesIterator->size, this);
    connect (m_thread, SIGNAL (done ()), SLOT (currentFileFinished()));
    connect (m_thread, SIGNAL (progress (quint64, quint64)), SLOT (reportProgress (quint64, quint64)));
    m_thread->start();
}

void IncomingTransfer::currentFileFinished()
{
    m_thread->wait();
    ++m_filesDone;
    m_sizeDone += m_filesIterator->size;
    ++m_filesIterator;

    if (m_filesIterator == m_files.end()) {
        emit done();
    } else {
        startNextFile();
    }
}

void IncomingTransfer::setDestinationDir (QString path)
{
    m_destinationDir = path;
}

void IncomingTransfer::reportProgress (quint64 done, quint64 size)
{
    m_doneTillLastProgressReport = m_sizeDone + done;
    emit progress (m_sizeDone + done, m_totalSize, m_speed);

    //Send progress to sender
    TransferStatusXmlData data;
    data.id = m_id;
    data.bytesDone = m_sizeDone + done;
    data.total = m_totalSize;
    data.speed = m_speed;
    data.type = AbstractXmlData::TransferStatus;
    TransferStatusXmlParser parser;
    Kapotah::UdpManager::instance()->sendDatagram(parser.composeXml(&data).toUtf8(), m_peerAddress);
}

void IncomingTransfer::timerEvent (QTimerEvent* event)
{
    m_speed = (m_doneTillLastProgressReport - m_doneSinceLastSpeedEstimate);
    m_doneSinceLastSpeedEstimate = m_doneTillLastProgressReport;
}

void IncomingTransfer::stop()
{
    reportProgress(-m_sizeDone-1, m_totalSize);         //Send negative progress
    delete m_thread;
    emit canceled();
    deleteLater();
}

#include "incomingtransfer.moc"
