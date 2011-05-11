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

#include "transferwidget.h"
#include <transfer.h>
#include <incomingtransfer.h>
#include <outgoingtransfer.h>
#include <peermanager.h>
#include "notifications/notifications.h"
#include <kapotah_global.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QToolButton>
#include <QFileDialog>


using namespace Kapotah;

TransferWidget::TransferWidget (Transfer* transfer, QWidget* parent, Qt::WindowFlags f)
    : QWidget (parent, f), m_peerLabel(new QLabel(this)), m_progress(new QProgressBar(this)), m_startStop(new QToolButton(this)),
    m_progressLabel(new QLabel("Waiting for transfer to begin")), m_verticalLayout(new QVBoxLayout()),
    m_horizontalLayout(new QHBoxLayout()), m_transfer(transfer), m_incomingTransfer(0), m_outgoingTransfer(0),
    m_transferStarted(false)
{
    if (transfer->type() == Transfer::Incoming) {
        m_incomingTransfer = qobject_cast<IncomingTransfer*>(transfer);
        connect(m_incomingTransfer, SIGNAL(needDestinationDir()), SLOT(transferNeedsDestinationDir()));
        m_peerLabel->setText("Receiving from ");
        m_startStop->setIcon(QIcon(":/images/download.png"));
        QString peerName = PeerManager::instance()->nameFromIp(transfer->peerAddress());
        QString message = transfer->itemNames().join(", ");
        NotificationData data = { peerName + " sent" , message, QIcon(":/images/download.png"), parent };
        Notifications::instance()->notify(data);
    } else if (transfer->type() == Transfer::Outgoing) {
        m_outgoingTransfer = qobject_cast<OutgoingTransfer*>(transfer);
        m_peerLabel->setText("Sending to ");
        m_startStop->hide();
    }

    m_peerLabel->setText(m_peerLabel->text().append(
        PeerManager::instance()->peersModel()->peerNameForIp(transfer->peerAddress())));

    m_horizontalLayout->addWidget(m_peerLabel);
    m_horizontalLayout->addWidget(m_startStop);

    m_verticalLayout->addItem(m_horizontalLayout);
    m_verticalLayout->addWidget(m_progress);
    m_verticalLayout->addWidget(m_progressLabel);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    setLayout(m_verticalLayout);
    m_progress->setMaximum(100);

    QString itemsList;
    foreach (QString itemName, transfer->itemNames()) {
        itemsList.append(itemName);
        itemsList.append("\n");
    }
    setToolTip(itemsList);

    connect(m_startStop, SIGNAL(clicked(bool)), SLOT(startTransfer()));
    connect(m_transfer, SIGNAL(done()), SLOT(transferFinished()));
    connect(m_transfer, SIGNAL(progress(quint64,quint64,quint32)), SLOT(updateProgress(quint64,quint64,quint32)));
}

void TransferWidget::startTransfer()
{
    if (m_transferStarted) {
        m_transfer->stop();
        deleteLater();
    } else {
        m_transfer->start();
    }
}

void TransferWidget::transferFinished()
{
    m_peerLabel->setText("FINISHED");
    m_progress->setValue(m_progress->maximum());
}

void TransferWidget::transferNeedsDestinationDir()
{
    QString dirname = QFileDialog::getExistingDirectory(this, "Select a dir");
    if (dirname.isEmpty())
        return;
    m_transferStarted = true;
    m_startStop->setIcon(QIcon(":/images/dialog-close.png"));
    m_incomingTransfer->setDestinationDir(dirname);
    m_transfer->start();
}

void TransferWidget::updateProgress (quint64 done, quint64 total, quint32 speed)
{
    m_progress->setValue(done*100/total);
    m_progressLabel->setText(QString("%1B of %2B done at %3B/s").arg(Kapotah::humanReadableNumber(done))
                                .arg(Kapotah::humanReadableNumber(total)).arg(Kapotah::humanReadableNumber(speed)));
}

#include "transferwidget.moc"
