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
    manager = ptr->manager;
    setWindowTitle("Codename ChatAroma");
    connect(ui->startToolButton,SIGNAL(clicked()),this,SLOT(startPeerManager()));
    connect(ui->filesButton, SIGNAL(clicked()), this, SLOT(showFilesDialog()));
    connect(ui->peerList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(showChatWindowFromItem()));
    connect(ptr->server, SIGNAL(messageRecieved(QString,QHostAddress)), this, SLOT(messageRecieved(QString,QHostAddress)));
    createIcon();
    createActions();
    createTrayIcon();
    createPeerNameMenu();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(notificationClicked()));
    connect(ui->aboutButton, SIGNAL(clicked()), this, SLOT(showAboutDialog()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateTrayIcon()));
    //setAttribute(Qt::WA_TranslucentBackground, true);
    ui->peerList->setContextMenuPolicy(Qt::ActionsContextMenu);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::notificationClicked()
{
    trayIcon->setIcon(QIcon(":/images/chataroma.png"));
    showNormal();
    setFocus();
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
    trayIcon->setToolTip("Chataroma::"+ QString::number(ui->peerList->count())+" user(s) online");
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

    sendMessageAction = new QAction(tr("Send Message"), this);
    connect(sendMessageAction, SIGNAL(triggered()), this, SLOT(showChatWindowFromItem()));

    fileTransferAction = new QAction(tr("File Transfer"), this);
    connect(fileTransferAction, SIGNAL(triggered()), this, SLOT(fileTransferByRightClick()));

    folderTransferAction = new QAction(tr("Folder Transfer"), this);
    connect(folderTransferAction, SIGNAL(triggered()), this, SLOT(folderTransferByRightClick()));
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

void Dialog::createPeerNameMenu()
{
    ui->peerList->addAction(sendMessageAction);
    ui->peerList->addAction(fileTransferAction);
    ui->peerList->addAction(folderTransferAction);
}

void Dialog::fileTransferByRightClick()
{
}

void Dialog::folderTransferByRightClick()
{
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
    ui->peerList->addItem(new QListWidgetItem(QIcon(":/images/chataroma.png"), listEntry(peerIP.toString())));
    updateNumOfPeers();  //updates the number of members online
}

void Dialog::removePeer(QHostAddress peerIP)
{
    //We have to find the row in which the peer is being shown and select the first match
    int rowNumber = ui->peerList->row(ui->peerList->findItems(listEntry(peerIP.toString()), Qt::MatchExactly)[0]);
    ui->peerList->takeItem(rowNumber);
    updateNumOfPeers();
}

void Dialog::updateNumOfPeers()  //function to find the no. of members online
{
    ui->updateNumOfPeers->setText(QString::number(ui->peerList->count()) + " people online");
    trayIcon->setToolTip("Chataroma "+ QString::number(ui->peerList->count()) +" people online");
}

void Dialog::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

ChatDialog* Dialog::createChatWindow(QHostAddress ipAddress)
{
    if (openChatDialogs.contains(ipAddress.toString())) {
        if (!openChatDialogs[ipAddress.toString()]->isVisible())
            openChatDialogs[ipAddress.toString()]->show();
        return openChatDialogs[ipAddress.toString()];
    }

    ChatDialog *chatDlg = new ChatDialog(QHostAddress(ipAddress.toString()), m_ptr, this);
    openChatDialogs[ipAddress.toString()] = chatDlg;    //Save the dialog to the QHash so that we know which chat dialogs are open
    connect(chatDlg, SIGNAL(finished(int)), this, SLOT(unregisterChatDialog()));
    connect(chatDlg, SIGNAL(activated(QHostAddress)), this, SLOT(markAsRead(QHostAddress)));
    return chatDlg;
}

ChatDialog* Dialog::showChatWindow(QHostAddress ipAddress)
{
    ChatDialog *chatDlg = createChatWindow(ipAddress);
    chatDlg->show();
    return chatDlg;
}

ChatDialog* Dialog::showChatWindowFromItem()
{
    return showChatWindow(QHostAddress(IPPart(ui->peerList->selectedItems().at(0)->text())));
}

ChatDialog* Dialog::createChatWindowFromItem(QListWidgetItem *item)
{
    return createChatWindow(QHostAddress(IPPart(item->text())));
}

void Dialog::unregisterChatDialog()
{
    openChatDialogs.remove(openChatDialogs.key(dynamic_cast<ChatDialog*>(sender())));
}

void Dialog::updateTrayIcon()
{
    if (isRed) {
        trayIcon->setIcon(QIcon(":/images/mail-receive.png"));
        isRed = false;
    }
    else {
        trayIcon->setIcon(QIcon(":/images/mail-receive1.png"));
        isRed = true;
    }
}

bool Dialog::event(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate) {
        trayIcon->setIcon(QIcon(":/images/chataroma.png"));
        timer.stop();
    }
    QDialog::event(event);
}

void Dialog::messageRecieved(QString message,QHostAddress ipAddress)
{
    ChatDialog *chatDlg;
    if (openChatDialogs.contains(ipAddress.toString())) {
        chatDlg = openChatDialogs[ipAddress.toString()];
        if (chatDlg->isVisible() && chatDlg->isActiveWindow())
            return;
    }
    else {    //Means the dialog isn't open till now
        //Find the list item where the username is displayed, and open a chatdialog according to it
        int rowNumber = ui->peerList->row(ui->peerList->findItems(listEntry(ipAddress.toString()), Qt::MatchExactly)[0]);
        chatDlg = createChatWindowFromItem(ui->peerList->item(rowNumber));
        chatDlg->messageRecieved(message, ipAddress);
    }

    trayIcon->showMessage("Messsage from " + manager->peerInfo(ipAddress.toString()).name(),"Message recieved");
    trayIcon->setIcon(QIcon(":/images/mail-receive.png"));

    timer.start(500);

    //Set the mail received icon next to the entry in dialog
    QList<QListWidgetItem *> found;
    found = ui->peerList->findItems(listEntry(ipAddress.toString()), Qt::MatchExactly);
    if (found.count()>0) {
        found[0]->setIcon(QIcon(":/images/mail_get.png"));
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

QString Dialog::namePart(QString string) const
{
    return string.split('-')[0];
}

QString Dialog::IPPart(QString string) const
{
    return string.split('-')[1];
}

QString Dialog::listEntry(QString ipAddress) const
{
    return (manager->peerInfo(ipAddress).name() + '-' + ipAddress);
}

void Dialog::markAsRead(QHostAddress ipAddress)
{
    QList<QListWidgetItem *> found;
    found = ui->peerList->findItems(listEntry(ipAddress.toString()), Qt::MatchExactly);
    if (found.count()>0) {
        timer.stop();
        found[0]->setIcon(QIcon(":/images/chataroma.png"));
        trayIcon->setIcon(QIcon(":/images/chataroma.png"));
    }
}

void Dialog::showAboutDialog()
{
    aboutDialog.show();
}
