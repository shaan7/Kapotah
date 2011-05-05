/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2011 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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

#include "searchdialog.h"

#include "announcer.h"
#include "filesearcher.h"
#include "transfermanager.h"
#include "transfer.h"
#include "peermanager.h"
#include <incomingtransfer.h>
#include <ui/notifications/notifications.h>
#include <QFileDialog>

using namespace Kapotah;

SearchDialog::SearchDialog(QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f)
{
    ui.setupUi(this);
    setWindowTitle("Search");
    ui.searchButton->setToolTip("Search");
    ui.downloadButton->setToolTip("Download");

    connect(ui.searchButton, SIGNAL(clicked()), SLOT(search()));
    connect(ui.settingsButton, SIGNAL(clicked()), SLOT(setSharedDir()));
    connect (TransferManager::instance(), SIGNAL (newTransferAdded (Transfer*)),
             SLOT (addTransfer (Transfer*)));
    connect(ui.downloadButton, SIGNAL(clicked()), SLOT(startTransfer()));
    connect(Kapotah::FileSearcher::instance(), SIGNAL(initDone(QString)), SLOT(notifyIndexingComplete(QString)));
}

void SearchDialog::search()
{
    ui.resultsList->clear();
    m_transfers.clear();
    Kapotah::Announcer::instance()->searchPeersForFile(ui.searchPatternEdit->text());
}

void SearchDialog::setSharedDir()
{
    QString dirname = QFileDialog::getExistingDirectory(this, "Select a directory to share");
    if (dirname.isEmpty())
        return;
    Kapotah::FileSearcher::instance()->setSearchPath(dirname);
}

void SearchDialog::addTransfer (Transfer* transfer)
{
    if ( (!transfer->isSearchResponse()) || (transfer->type() == Transfer::Outgoing) )
        return;
    ui.resultsList->addItem(PeerManager::instance()->peersModel()->peerNameForIp(transfer->peerAddress()));
    m_transfers.append(transfer);
}

void SearchDialog::startTransfer()
{
    if (ui.resultsList->selectedItems().count() == 0)
        return;
    Transfer *transfer = m_transfers.at(ui.resultsList->selectionModel()->currentIndex().row());
    transfer->setIsSearchResponse(false);
    emit TransferManager::instance()->emitNewTransferAdded(transfer);
}

void SearchDialog::notifyIndexingComplete(const QString &path)
{
    NotificationData data;
    data.handler = 0;
    data.icon = ui.settingsButton->icon();
    data.title = "Indexing complete";
    data.message = "For " + path;
    Notifications::instance()->notify(data);
}

#include "searchdialog.moc"
