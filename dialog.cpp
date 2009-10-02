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
    connect(ui->peerList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(openChatWindowFromItem(QListWidgetItem*)));
    connect(m_server, SIGNAL(messageRecieved(QString,QHostAddress)), this, SLOT(messageRecieved(QString,QHostAddress)));
    createIcon();
    createActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(notificationClicked()));
    //connect(&newMessageMapper, SIGNAL(mapped(QString)), this, SLOT(openChatWindow(QString)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::notificationClicked()
{
    trayIcon->setIcon(QIcon(":/images/chataroma.png"));
    showNormal();
}

void Dialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    /*case QSystemTrayIcon::Trigger:
        chatDlg->show();
        break;*/
    case QSystemTrayIcon::DoubleClick:
        trayIcon->setIcon(QIcon(":/images/chataroma.png"));
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
    connect(manager,SIGNAL(newPeer(QHostAddress)),this,SLOT(addNewPeer(QHostAddress)));
    connect(manager,SIGNAL(peerGone(QHostAddress)),this,SLOT(removePeer(QHostAddress)));
}

void Dialog::addNewPeer(QHostAddress peerIP)
{
    ui->peerList->addItem(peerIP.toString());
    updateNumOfPeers();  //updates the number of members online
}

void Dialog::removePeer(QHostAddress peerIP)
{
    //We have to find the row in which the peer is being shown and select the first match
    int rowNumber = ui->peerList->row(ui->peerList->findItems(peerIP.toString(), Qt::MatchExactly)[0]);
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

ChatDialog* Dialog::openChatWindow(QHostAddress ipAddress)
{
    QList<QListWidgetItem *> found;
    found = ui->peerList->findItems(ipAddress.toString(), Qt::MatchExactly);
    if (found.count()>0) {
        found[0]->setIcon(QIcon(""));
    }

    if (openChatDialogs.contains(ipAddress.toString())) {
        if (!openChatDialogs[ipAddress.toString()]->isVisible())
            openChatDialogs[ipAddress.toString()]->show();
        return openChatDialogs[ipAddress.toString()];
    }

    chatDlg = new ChatDialog(QHostAddress(ipAddress.toString()), m_ptr, this);
    openChatDialogs[ipAddress.toString()] = chatDlg;    //Save the dialog to the QHash so that we know which chat dialogs are open
    connect(chatDlg, SIGNAL(finished(int)), this, SLOT(unregisterChatDialog()));
    chatDlg->show();
    return chatDlg;
}

ChatDialog* Dialog::openChatWindowFromItem(QListWidgetItem *item)
{
    return openChatWindow(QHostAddress(item->text()));
}

void Dialog::unregisterChatDialog()
{
    openChatDialogs.remove(openChatDialogs.key(dynamic_cast<ChatDialog*>(sender())));
}

void Dialog::messageRecieved(QString message,QHostAddress ipAddress)
{
    if (openChatDialogs.contains(ipAddress.toString())) {
        if (!chatDlg->isVisible())  //Means the dialog has been hidden by the user
        {
            trayIcon->showMessage("Messsage from " + ipAddress.toString(),"Message recieved");
            trayIcon->setIcon(QIcon(":/images/mail-receive.png"));

            QList<QListWidgetItem *> found;
            found = ui->peerList->findItems(ipAddress.toString(), Qt::MatchExactly);
            if (found.count()>0) {
                found[0]->setIcon(QIcon(":/images/mail_get.png"));
            }
        }
      }
    else {    //Means the dialog isn't open till now
        //Find the list item where the username is displayed, and open a chatdialog according to it
        int rowNumber = ui->peerList->row(ui->peerList->findItems(ipAddress.toString(), Qt::MatchExactly)[0]);
        chatDlg = openChatWindowFromItem(ui->peerList->item(rowNumber));
        chatDlg->messageRecieved(message, ipAddress);
    }
}

/*void Dialog::addNewMessageEntry(QString senderIP)
{
    QAction *entry = new QAction(senderIP, this);
    //trayIconMenu->insertSeparator(minimizeAction);
    trayIconMenu->insertAction(minimizeAction, entry);
    newMessageActions[senderIP]=entry;
    newMessageMapper.setMapping(entry, senderIP);
    connect(entry, SIGNAL(triggered()), &newMessageMapper, SLOT(map()));
}*/
