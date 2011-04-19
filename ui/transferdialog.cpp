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

#include "transferdialog.h"
#include <transfermanager.h>
#include "transferwidget.h"
#include "transfer.h"

using namespace Kapotah;

TransferDialog::TransferDialog (QWidget* parent, Qt::WindowFlags f)
    : QDialog (parent, f)
{
    ui.setupUi(this);
    connect (TransferManager::instance(), SIGNAL (newTransferAdded (Transfer*)),
             SLOT (addTransfer (Transfer*)));
}

TransferDialog::~TransferDialog()
{

}

void TransferDialog::addTransfer (Transfer* transfer)
{
    if (transfer->isSearchResponse() && transfer->type() == Transfer::Incoming) {
        qDebug() << "IGNORING SEARCH RESPONSE";
        return;
    }
    //Remove the spacer
    ui.scrollAreaWidgetContents->layout()->takeAt(ui.scrollAreaWidgetContents->layout()->count()-1);
    //Add the transfer widget
    TransferWidget *widget = new TransferWidget (transfer, this);
    ui.scrollAreaWidgetContents->layout()->addWidget(widget);
    //Add a new spacer
    dynamic_cast<QVBoxLayout*>(ui.scrollAreaWidgetContents->layout())->addStretch();
}

#include "transferdialog.moc"
