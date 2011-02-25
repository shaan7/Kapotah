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
#include <QtGui>

using namespace Kapotah;

PeerDialog::PeerDialog (QDialog* parent) : QDialog (parent), m_transferDialog(0)
{
    QSystemTrayIcon::isSystemTrayAvailable();
    ui.setupUi(this);    
    setWindowTitle("Kapotah");
    createActions();
    createTrayIcon();
    ui.multicastButton->setToolTip("To multicast, select users and click on \"multicast\" button");
    ui.peersListView->setModel(PeerManager::instance()->peersModel());
    connect (ui.refreshButton, SIGNAL(clicked()), this, SLOT(updatePeer()));
    connect (PeerManager::instance()->peersModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), this,
             SLOT(updateSystrayTooltip(QModelIndex, int, int)));
    connect (PeerManager::instance()->peersModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), this,
             SLOT(updateSystrayTooltip(QModelIndex, int, int)));
    connect (ui.peersListView, SIGNAL(doubleClicked(QModelIndex)), SLOT(createChatDialog(QModelIndex)));
    connect (MessageDispatcher::instance(), SIGNAL(gotNewMessage(QString, QHostAddress)),
             SLOT(createChatDialogOnMessage(QString,QHostAddress)));
    connect (ui.multicastButton, SIGNAL(clicked()), this, SLOT(createMulticastDialog()));
    /*connect (MessageDispatcher::instance(), SIGNAL(gotNewMulticast(QString, QHostAddress)),
             SLOT(createMulticastDialogOnMessage(QString, QHostAddress));*/
    connect (ui.transferButton, SIGNAL(clicked(bool)), SLOT(showTransferDialog(bool)));
    connect (trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, 
             SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    ui.transferButton->animateClick();
    trayIcon->show();
}

ChatDialog* PeerDialog::createChatDialog(QModelIndex index)
{
    PeersModel *model = PeerManager::instance()->peersModel();
    if(!m_openChatDialogs.contains(model->data(index, PeersModel::ipAddressRole).toString()))
    {
        setFocus();
        ChatDialog *chatDlg = new ChatDialog(index);
        chatDlg->show();
        m_openChatDialogs.insert(model->data(index, PeersModel::ipAddressRole).toString(), chatDlg);
        connect (chatDlg, SIGNAL(rejected()), this, SLOT(removeKeyFromHash()));
        return chatDlg;
    }
}

ChatDialog* PeerDialog::createChatDialogOnMessage(QString message, QHostAddress peerAddress)
{
    if (m_openChatDialogs.contains(peerAddress.toString())) //Already created
        return m_openChatDialogs[peerAddress.toString()];

    PeersModel *model = PeerManager::instance()->peersModel();
    QModelIndexList list = model->match(model->index(0), PeersModel::ipAddressRole, peerAddress.toString());

    if (list.count() == 0) {
        qDebug() << "NOOOO! someone sent message after being offline, huh?";
    } else {
        createChatDialog(list.at(0))->displayRecievedMessage(message, peerAddress);
    }
}

MulticastDialog* PeerDialog::createMulticastDialog()
{
    PeersModel *model = PeerManager::instance()->peersModel();
    MulticastDialog *multiDlg = new MulticastDialog(ui.peersListView->selectionModel()->selectedIndexes());
    multiDlg->show();
    return multiDlg;
}

/*ChatDialog* PeerDialog::createMulticastDialogOnMessage(QString message, QHostAddress peerAddress)
{
    if (m_openMulticastDialog.contains(peerAddress.toString())) //Already created
        return m_openChatDialogs[peerAddress.toString()];

    PeersModel *model = PeerManager::instance()->peersModel();
    QModelIndexList list = model->match(model->index(0), PeersModel::ipAddressRole, peerAddress.toString());

    if (list.count() == 0) {
        qDebug() << "NOOOO! someone sent message after being offline, huh?";
    } else {
        createMulticastDialog(list.at(0))->displayRecievedMessage(message, peerAddress);
    }
}*/

void PeerDialog::showTransferDialog (bool checked)
{
    if (!m_transferDialog) {
        m_transferDialog = new TransferDialog(this);
    }
    m_transferDialog->show();
}

void PeerDialog::removeKeyFromHash()
{
    ChatDialog* dlg = qobject_cast<ChatDialog*> (sender());
    m_openChatDialogs.remove(m_openChatDialogs.key(dlg));
}

void PeerDialog::updatePeer()
{
    UdpManager::instance()->updateAddresses();
}

/*--------------------------------------------
 * 
 * code for system tray icon starts here
 * 
----------------------------------------------*/

void PeerDialog::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/images/heart.svg"));
    
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
                this->showNormal();
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

#include "peerdialog.moc"
