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
#include <QHBoxLayout>

using namespace Kapotah;

TransferDialog::TransferDialog (QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f)
{
    setWindowTitle ("Transfers");
    m_layout = new QVBoxLayout();
    setLayout (m_layout);
    connect (TransferManager::instance(), SIGNAL (newTransferAdded (Transfer*)),
             SLOT (addTransfer (Transfer*)));
    resize(200, 100);
}

TransferDialog::~TransferDialog()
{

}

void TransferDialog::addTransfer (Transfer* transfer)
{
    TransferWidget *widget = new TransferWidget (transfer, this);
    m_layout->addWidget (widget);
}

#include "transferdialog.moc"
