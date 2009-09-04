/***************************************************************************
 *   Copyright 2009 Shantanu Tushar <jhahoneyk@gmail.com>                  *
 *   Copyright 2009 Sudhendu Roy <sudhendu_roy@yahoo.co.in>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "dialog.h"
#include "ui_dialog.h"
#include "filestatusdialog.h"

Dialog::Dialog(Pointers *ptr,QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogClass)
{
    ui->setupUi(this);
    m_ptr = ptr;
    m_server = ptr->server;
    m_fserver = ptr->fserver;
    manager = ptr->manager;
    setWindowTitle("ChatAroma");
    connect(ui->startToolButton,SIGNAL(clicked()),this,SLOT(startPeerManager()));
    connect(ui->filesButton, SIGNAL(clicked()), this, SLOT(showFilesDialog()));
    connect(ui->peerList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(openChatWindow(QListWidgetItem*)));
    connect(m_server, SIGNAL(messageRecieved(QString,QString)), this, SLOT(messageRecieved(QString,QString)));
    createIcon();
    createActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    //case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    /*case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;*/
    default:
        ;
    }
}

void Dialog::showFilesDialog()
{
    m_ptr->fileStatusDialog->show();
}

void Dialog::createIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/chataroma.png"));
    trayIcon->show();
}

void Dialog::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Dialog::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);
}

void Dialog::startPeerManager()
{
    ui->startToolButton->setEnabled(false);
    manager->startBroadcast();  //Start broadcasting on the network

    connect(manager,SIGNAL(newPeer(QString)),this,SLOT(addNewPeer(QString)));
    connect(manager,SIGNAL(peerGone(QString)),this,SLOT(removePeer(QString)));
}

void Dialog::addNewPeer(QString peer)
{
    ui->peerList->addItem(peer);
    updateNumOfPeers();  //updates the number of members online
}

void Dialog::removePeer(QString peer)
{
    //We have to find the row in which the peer is being shown and select the first match
    int rowNumber = ui->peerList->row(ui->peerList->findItems(peer, Qt::MatchExactly)[0]);
    ui->peerList->takeItem(rowNumber);
    updateNumOfPeers();
}

void Dialog::updateNumOfPeers()  //function to find the no. of members online
{
    ui->updateNumOfPeers->setText(QString::number(ui->peerList->count()) + " user(s) online");
}

void Dialog::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

ChatDialog* Dialog::openChatWindow(QListWidgetItem *item)
{
    if (openChatDialogs.contains(item->text())) {
        if (!chatDlg->isVisible())
            chatDlg->show();
        return 0;
    }

    chatDlg = new ChatDialog(item->text(), m_ptr, this);
    openChatDialogs[item->text()] = chatDlg;    //Save the dialog to the QHash so that we know which chat dialogs are open
    connect(chatDlg, SIGNAL(finished(int)), this, SLOT(unregisterChatDialog()));
    chatDlg->show();
    return chatDlg;
}

void Dialog::unregisterChatDialog()
{
    openChatDialogs.remove(openChatDialogs.key(dynamic_cast<ChatDialog*>(sender())));
}

void Dialog::messageRecieved(QString message,QString username)
{
    if (openChatDialogs.contains(username)) {
        if (!chatDlg->isVisible())
            chatDlg->show();
    }
    else {
        //Find the list item where the username is displayed, and open a chatdialog according to it
        int rowNumber = ui->peerList->row(ui->peerList->findItems(username, Qt::MatchExactly)[0]);
        chatDlg = openChatWindow(ui->peerList->item(rowNumber));
        chatDlg->messageRecieved(message, username);
    }
}
