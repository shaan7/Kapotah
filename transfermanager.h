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

#ifndef KAPOTAH_TRANSFERMANAGER_H
#define KAPOTAH_TRANSFERMANAGER_H

#include "singleton.h"
#include "transfer.h"

#include <QList>
#include <QHash>
#include <QHostAddress>

namespace Kapotah
{

    class TransferManager : public Singleton<TransferManager>
    {
            Q_OBJECT

        public:
            TransferManager();
            virtual ~TransferManager();
            Transfer *addTransfer (Transfer::TransferType type, QList<TransferFile> fileList, QHostAddress peer);
            QString newId (QString path);

        private:
            QList<Transfer*> m_transfersList;
            QHash<QString, QString> m_paths;

        private slots:
            void handleIncomingTransfer (QString transfer, QHostAddress peer);

        signals:
            void newTransferAdded (Transfer *transfer);
            void transferFinished (Transfer *transfer);
    };

}

#endif // KAPOTAH_TRANSFERMANAGER_H
