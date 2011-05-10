/*
    This file is part of the Kapotah project.
    Copyright (C) 2009 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2009 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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


#include "peerdialog.h"
#include "peermanager.h"
#include "udpmanager.h"
#include "peersmodel.h"
#include "announcer.h"
#include "notifications/notifications.h"
#include "chatdialog.h"
#include "transferdialog.h"
#include "multicastdialog.h"
#include "searchdialog.h"
#include "aboutdialog.h"

#include "debug.h"
#include "filesearcher.h"
#include <QtGui>

using namespace Kapotah;

PeerDialog::PeerDialog (QDialog* parent) : QDialog (parent), m_transferDialog(new TransferDialog(this)),
    m_multicastDialog(new MulticastDialog(this)), m_searchDialog(new SearchDialog(this)),
    m_aboutDialog(new AboutDialog(this)), m_timer(0), m_isGreyScale(false), m_trayIcon(0)
{
    ui.setupUi(this);
    setWindowTitle(QApplication::applicationName());
    createActions();
    createTrayIcon();
    Notifications::instance();
    ui.peersListView->setModel(PeerManager::instance()->peersModel());
    ui.nameEdit->setText(Kapotah::Announcer::instance()->username());
    ui.tabWidget->setCurrentIndex(0);
    connect (ui.refreshButton, SIGNAL(clicked()), this, SLOT(updatePeer()));
    connect (ui.peersListView, SIGNAL(doubleClicked(QModelIndex)), SLOT(showChatDialogOnUserRequest(QModelIndex)));
    connect (ui.aboutButton, SIGNAL(clicked()), SLOT(showAboutDialog()));
    connect (MessageDispatcher::instance(), SIGNAL(gotNewMessage(QString, QHostAddress)),
             SLOT(showChatDialogOnIncomingMessage(QString,QHostAddress)));
    connect (ui.multicastButton, SIGNAL(clicked()), this, SLOT(showMulticastDialog()));
    connect (ui.transferButton, SIGNAL(clicked()), SLOT(showTransferDialog()));
    connect (ui.searchButton, SIGNAL(clicked()), SLOT(showSearchDialog()));
    connect (ui.nameEdit, SIGNAL(returnPressed()), SLOT(setPeerNameFromUi()));
    connect (ui.notificationsButton, SIGNAL(clicked()),
             Notifications::instance(), SLOT(toggleNotificationsDialog()));
    connect (ui.addSubnetButton, SIGNAL(clicked()), SLOT(askUserForNewAdditionalSubnet()));
    connect (ui.removeSubnetButton, SIGNAL(clicked()), SLOT(removeSelectedAdditionalSubnet()));
    connect (Debug::instance(), SIGNAL(added(QString,QString)), SLOT(appendDebugMessage(QString,QString)));
    //Load settings
    QSettings settings;

    QVariantList list = settings.value("SubnetsList").toList();
    foreach (QVariant item, list) {
        ui.subnetsListWidget->addItem(new QListWidgetItem(QIcon(":/images/peer.png"), item.toString()));
    }
}

ChatDialog* PeerDialog::createChatDialog (QModelIndex index)
{
    PeersModel *model = PeerManager::instance()->peersModel();
    if (m_openChatDialogs.contains(model->data(index, PeersModel::ipAddressRole).toString())) {
        return m_openChatDialogs[model->data(index, PeersModel::ipAddressRole).toString()];
    } else {
        ChatDialog *chatDlg = new ChatDialog(index);
        m_openChatDialogs.insert(model->data(index, PeersModel::ipAddressRole).toString(), chatDlg);
        return chatDlg;
    }
}

void PeerDialog::showChatDialogOnIncomingMessage (QString message, QHostAddress peerAddress)
{
    PeersModel *model = PeerManager::instance()->peersModel();

    if (!m_openChatDialogs.contains(peerAddress.toString())) {
        QModelIndexList list = model->match(model->index(0), PeersModel::ipAddressRole, peerAddress.toString());

        if (list.count() == 0) {
            qDebug() << "NOOOO! someone sent message after being offline, huh?";
        } else {
            createChatDialog(list.at(0))->displayRecievedMessage(message, peerAddress);
        }
    }
}

void PeerDialog::showChatDialogOnUserRequest (QModelIndex index)
{
    createChatDialog(index)->show();
}

void PeerDialog::showMulticastDialog()
{
    m_multicastDialog->show();
}

void PeerDialog::showSearchDialog()
{
    m_searchDialog->show();
}

void PeerDialog::showTransferDialog ()
{
    m_transferDialog->show();
}

void PeerDialog::updatePeer()
{
    UdpManager::instance()->updateAddresses();
}

void PeerDialog::setPeerNameFromUi()
{
    Kapotah::Announcer::instance()->setUserName(ui.nameEdit->text());
}

void PeerDialog::initSubnets()
{
    
}

void PeerDialog::askUserForNewAdditionalSubnet()
{
    QString newSubnet = QInputDialog::getText(this, "Enter new subnet", "Enter new subnet");
    if (newSubnet.isEmpty())
        return;

    ui.subnetsListWidget->addItem(new QListWidgetItem(QIcon(":/images/peer.png"), newSubnet));
    Kapotah::Announcer::instance()->addAdditionalBroadcastAddress(QHostAddress(newSubnet));
}

void PeerDialog::removeSelectedAdditionalSubnet()
{
    if (ui.subnetsListWidget->count() <= 0)
        return;

    QListWidgetItem *item = ui.subnetsListWidget->takeItem(ui.subnetsListWidget->currentRow());
    Kapotah::Announcer::instance()->removeAdditionalBroadcastAddress(QHostAddress(item->text()));
    delete item;
}

void PeerDialog::showAboutDialog()
{
    m_aboutDialog->show();
}

/*--------------------------------------------
 * 
 * code for system tray icon starts here
 * 
----------------------------------------------*/

void PeerDialog::createTrayIcon()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())
        return;
    m_trayIcon = new QSystemTrayIcon(QIcon(":/images/systrayicon.png"));

    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(minimizeAction);
    m_trayIconMenu->addAction(maximizeAction);
    m_trayIconMenu->addAction(restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(quitAction);
    m_trayIcon->setContextMenu(m_trayIconMenu);

    connect (PeerManager::instance()->peersModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), this,
             SLOT(updateSystrayTooltip(QModelIndex, int, int)));
    connect (PeerManager::instance()->peersModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), this,
             SLOT(updateSystrayTooltip(QModelIndex, int, int)));
    connect (MessageDispatcher::instance(), SIGNAL(gotNewMessage(QString,QHostAddress)),
             SLOT(notifySysTray(QString, QHostAddress)));
    connect (m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, 
             SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    m_trayIcon->show();
}

void PeerDialog::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quitApplication()));
}

void PeerDialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (!m_trayIcon)
        return;
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        if (m_timer){
            m_timer->stop();
            m_trayIcon->setIcon(QIcon(":/images/systrayicon.png"));
        }
        if (isVisible())
            hide();
        else
            show();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

void PeerDialog::closeEvent(QCloseEvent* event)
{
    QDialog::closeEvent(event);
    if(m_trayIcon->isVisible()){
    event->ignore();
    }
}

void PeerDialog::updateSystrayTooltip(QModelIndex parent,int start,int end)
{
    if (!m_trayIcon)
        return;
    m_trayIcon->setToolTip("Kapotah (" + QString::number(Kapotah::PeerManager::instance()->peersModel()->rowCount()) + ")");
}

void PeerDialog::quitApplication()
{
    QApplication::quit();
}

PeerDialog::~PeerDialog()
{
    //Save settings
    QSettings settings;

    QVariantList list;
    foreach (QListWidgetItem *item, ui.subnetsListWidget->findItems("", Qt::MatchContains).toSet()) {
        list << item->text();
    }
    settings.setValue("SubnetsList", list);
}

void PeerDialog::appendDebugMessage(const QString& sender, const QString& message)
{
    int rows = ui.debugTable->rowCount();
    QTableWidgetItem *item = new QTableWidgetItem(sender);
    ui.debugTable->insertRow(rows);
    ui.debugTable->setItem(rows, 0, item);
    item = new QTableWidgetItem(message);
    ui.debugTable->setItem(rows, 1, item);
}

#include "peerdialog.moc"
