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

class TransferXmlData;

namespace Kapotah
{

    /**
     * \brief Singleton class which handles current transfers
     *
     * This class maintains pointers to ongoing transfers,
     * both incoming and outgoing.
     * The class should be used when a new transfer is to be
     * added to the system.
     * Provides convenience functions to generate IDs and resolve
     * the IDs to files
     */
    class TransferManager : public Singleton<TransferManager>
    {
            Q_OBJECT

        public:
            TransferManager();

            /**
             * Adds a new transfer from the XML data for the transfer
             *
             * @param   type    the TransferType
             * @param   data    XML data for the transfer
             * @param   peer    address of the remote peer
             * @param   isSearchResponse        if the transfer is result of a search
             */
            Transfer *addTransfer (Transfer::TransferType type, TransferXmlData data, QHostAddress peer,
                bool isSearchResponse);

            /**
             * Adds a new transfer from the list of files
             *
             * @param   type    the TransferType
             * @param   files   the list of TransferFile
             * @param   peer    address of the remote peer
             * @param   isSearchResponse        if the transfer is result of a search
             */
            Transfer *addTransfer (Transfer::TransferType type, QList<TransferFile> files, QHostAddress peer,
                bool isSearchResponse);

            /**
             * Generate a random ID for the given path
             *
             * @param   path    path to be used when generating ID
             */
            QString newId (QString path);

            /**
             * Return the path associcated with the ID
             *
             * @param   id      the ID to search
             */
            QString pathForId (QString id);

            /**
             * Use whenever its required to emulate a "re-add" of a Transfer in case
             * it was ignored for some reason
             * \note Use only when absolutely necessary
             *
             * @param   transfer        the Transfer to re-add
             */
            void emitNewTransferAdded(Transfer *transfer);

        private:
            QList<Transfer*> m_transfersList;
            QHash<QString, QString> m_paths;
            quint64 m_seqNo;

        private slots:
            void handleIncomingTransfer (QString transfer, QHostAddress peer);
            void onTransferFinished();

        signals:
            /**
             * Emitted when a new transfer is added
             *
             * @param   transfer        The Transfer object which has been added
             */
            void newTransferAdded (Transfer *transfer);

            /**
             * Emitted when a transfer finishes
             *
             * @param   transfer        The Transfer object which has finished
             */
            void transferFinished (Transfer *transfer);
    };

}

#endif // KAPOTAH_TRANSFERMANAGER_H
