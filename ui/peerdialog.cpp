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

PeerDialog::PeerDialog (QDialog* parent) : QDialog (parent), m_transferDialog(new TransferDialog(this))
    , m_openMulticastDialog(0), m_openSearchDialog(0), m_openAboutDialog(0), m_timer(0), m_isGreyScale(false)
{
    QSystemTrayIcon::isSystemTrayAvailable();
    ui.setupUi(this);
    setWindowTitle("Kapotah");
    createActions();
    createTrayIcon();
    Notifications::instance();
    ui.multicastButton->setToolTip("To multicast, select users and click on \"multicast\" button");
    ui.peersListView->setModel(PeerManager::instance()->peersModel());
    ui.nameEdit->setText(Kapotah::Announcer::instance()->username());
    ui.refreshButton->setToolTip("Refresh Peers");
    ui.transferButton->setToolTip("Transfer Log");
    ui.searchButton->setToolTip("Search");
    ui.tabWidget->setCurrentIndex(0);
    connect (ui.refreshButton, SIGNAL(clicked()), this, SLOT(updatePeer()));
    connect (PeerManager::instance()->peersModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), this,
             SLOT(updateSystrayTooltip(QModelIndex, int, int)));
    connect (PeerManager::instance()->peersModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), this,
             SLOT(updateSystrayTooltip(QModelIndex, int, int)));
    connect (ui.peersListView, SIGNAL(doubleClicked(QModelIndex)), SLOT(showChatDialogOnUserRequest(QModelIndex)));
    connect (ui.aboutButton, SIGNAL(clicked()), SLOT(showAboutDialog()));
    connect (MessageDispatcher::instance(), SIGNAL(gotNewMessage(QString,QHostAddress)),
             SLOT(notifySysTray(QString, QHostAddress)));
    connect (MessageDispatcher::instance(), SIGNAL(gotNewMessage(QString, QHostAddress)),
             SLOT(showChatDialogOnIncomingMessage(QString,QHostAddress)));
    connect (ui.multicastButton, SIGNAL(clicked()), this, SLOT(createMulticastDialog()));
    connect (ui.transferButton, SIGNAL(clicked(bool)), SLOT(showTransferDialog(bool)));
    connect (ui.searchButton, SIGNAL(clicked()), SLOT(showSearchDialog()));
    connect (trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, 
             SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect (ui.nameEdit, SIGNAL(returnPressed()), SLOT(setPeerNameFromUi()));
    connect (ui.notificationsButton, SIGNAL(clicked()),
             Notifications::instance(), SLOT(toggleNotificationsDialog()));
    connect (ui.addSubnetButton, SIGNAL(clicked(bool)), SLOT(askUserForNewAdditionalSubnet()));
    connect (ui.removeSubnetButton, SIGNAL(clicked(bool)), SLOT(removeSelectedAdditionalSubnet()));
    connect (Debug::instance(), SIGNAL(added(QString,QString)), SLOT(appendDebugMessage(QString,QString)));
    trayIcon->show();
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

MulticastDialog* PeerDialog::createMulticastDialog()
{
    PeersModel *model = PeerManager::instance()->peersModel();
    MulticastDialog *multiDlg = new MulticastDialog(this);
    multiDlg->show();
    return multiDlg;
}

SearchDialog* PeerDialog::showSearchDialog()
{
    SearchDialog *srchDlg = new SearchDialog(this);
    srchDlg->show();
    return srchDlg;
}

void PeerDialog::showTransferDialog (bool checked)
{
    if (m_transferDialog) {
        m_transferDialog->show();
    }
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

    ui.subnetsListWidget->addItem(newSubnet);
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

AboutDialog* PeerDialog::showAboutDialog()
{
    AboutDialog *aboutDlg = new AboutDialog(this);
    aboutDlg->show();
    return aboutDlg;
}

/*--------------------------------------------
 * 
 * code for system tray icon starts here
 * 
----------------------------------------------*/

void PeerDialog::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/images/systrayicon.png"));
    
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
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
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        if (m_timer){
            m_timer->stop();
            trayIcon->setIcon(QIcon(":/images/systrayicon.png"));
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

void PeerDialog::notifySysTray(QString str, QHostAddress peerAddress)
{
    if(!m_timer){
        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(changeSysTrayIcon()));
        m_timer->start(500);
    }
}

void PeerDialog::changeSysTrayIcon()
{
    if (!m_isGreyScale) {
        trayIcon->setIcon(QIcon(":/images/systrayicon-greyscale.png"));
        m_isGreyScale = true;
    }
    else {
        trayIcon->setIcon(QIcon(":/images/systrayicon.png"));
        m_isGreyScale = false;
    }
}

void PeerDialog::closeEvent(QCloseEvent* event)
{
    QDialog::closeEvent(event);
    if(trayIcon->isVisible()){
    event->ignore();
    }
}

void PeerDialog::updateSystrayTooltip(QModelIndex parent,int start,int end)
{
    trayIcon->setToolTip("Kapotah (" + QString::number(Kapotah::PeerManager::instance()->peersModel()->rowCount()) + ")");
}

void PeerDialog::quitApplication()
{
    QApplication::quit();
}

PeerDialog::~PeerDialog()
{

}

void PeerDialog::appendDebugMessage(const QString& sender, const QString& message)
{
    ui.debugText->appendHtml("<b>"+sender+"</b><br />"+message+"<br />");
}

#include "peerdialog.moc"
