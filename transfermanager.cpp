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
#include "xmlparser.h"

#include <QDateTime>

using namespace Kapotah;

template<> TransferManager *Kapotah::Singleton<TransferManager>::m_instance = 0;

TransferManager::TransferManager()
{

}

TransferManager::~TransferManager()
{

}

Transfer* TransferManager::addTransfer (Transfer::TransferType type, QList< TransferFile > fileList,
                                        QHostAddress peer)
{
    Transfer *transfer = new Transfer(type, fileList, peer, this);
    m_transfersList.append(transfer);
    connect (transfer, SIGNAL(done()), SIGNAL(transferFinished()));
    emit newTransferAdded(transfer);
}

QString TransferManager::newId (QString path)
{
    QString id = QString::number(QDateTime::currentMSecsSinceEpoch());
    if (m_paths.contains(id)) {
        qFatal("THIS IS NASTY, I MEAN IT, STOP _NOW_");
    }
    m_paths[id] = path;
    return id;
}

void TransferManager::handleIncomingTransfer (QString transfer, QHostAddress peer)
{
    XmlParser parser;
    parser.parseXml(transfer);
    addTransfer(Transfer::Incoming, parser.files(), peer);
}

#include "transfermanager.moc"
