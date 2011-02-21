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

#include "transfermanager.h"
#include "transfer.h"
#include "incomingtransfer.h"
#include "outgoingtransfer.h"
#include <xml/transferxmlparser.h>
#include <messagedispatcher.h>
#include <QDateTime>

using namespace Kapotah;

template<> TransferManager *Kapotah::Singleton<TransferManager>::m_instance = 0;

TransferManager::TransferManager()
{
    connect(MessageDispatcher::instance(), SIGNAL(gotNewTransfer(QString,QHostAddress)),
            SLOT(handleIncomingTransfer(QString,QHostAddress)));
}

TransferManager::~TransferManager()
{

}

Transfer* TransferManager::addTransfer (Transfer::TransferType type, QList< TransferFile > fileList,
                                        quint64 totalSize, quint64 numFiles, quint64 numDirs, QHostAddress peer)
{
    Transfer *transfer = 0;

    if (type == Transfer::Incoming) {
        transfer = new IncomingTransfer (fileList, totalSize, numFiles, numDirs, peer, this);
    } else if (type == Transfer::Outgoing) {
        transfer = new OutgoingTransfer (fileList, peer, this);
    }

    m_transfersList.append (transfer);

    connect (transfer, SIGNAL (done()), SLOT (onTransferFinished()));
    emit newTransferAdded (transfer);
    return transfer;
}

QString TransferManager::newId (QString path)
{
    QString id = path + QString::number (QDateTime::currentMSecsSinceEpoch());

    if (m_paths.contains (id)) {
        qFatal ("THIS IS NASTY, I MEAN IT, STOP _NOW_");
    }

    m_paths[id] = path;

    return id;
}

QString TransferManager::pathForId (QString id)
{
    return m_paths[id];
}

void TransferManager::handleIncomingTransfer (QString transfer, QHostAddress peer)
{
    TransferXMLParser parser;
    TransferXMLData *data = static_cast<TransferXMLData*>(parser.parseXML(transfer));
    addTransfer (Transfer::Incoming, data->files, data->totalSize, data->totalNumFiles, data->totalNumDirs, peer);
    delete data;
}

void TransferManager::onTransferFinished()
{
    emit transferFinished(qobject_cast<Transfer*>(sender()));
}

#include "transfermanager.moc"
