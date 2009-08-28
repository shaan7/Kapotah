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

Dialog::Dialog(Server *server,FileServer *fserver,QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogClass)
{
    ui->setupUi(this);
    m_server = server;
    m_fserver = fserver;
    setWindowTitle("ChatAroma");
    connect(ui->refreshButton,SIGNAL(clicked()),this,SLOT(startPeerManager()));
    connect(ui->peerList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(openChatWindow(QListWidgetItem*)));
    connect(server, SIGNAL(messageRecieved(QString,QString)), this, SLOT(messageRecieved(QString,QString)));
}
Dialog::~Dialog()
{
    delete ui;
}
void Dialog::startPeerManager()
{
    ui->refreshButton->setEnabled(false);
    manager = new PeerManager(m_server);    //Create the peer manager
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
    ui->updateNumOfPeers->setText(QString::number(ui->peerList->count()) + " member(s) online");
}
ChatDialog* Dialog::openChatWindow(QListWidgetItem *item)
{
    if (openChatDialogs.contains(item->text()))
        return 0;
    ChatDialog *dlg = new ChatDialog(item->text(), manager, m_server, m_fserver, this);
    openChatDialogs[item->text()] = dlg;    //Save the dialog to the QHash so that we know which chat dialogs are open
    connect(dlg, SIGNAL(finished(int)), this, SLOT(unregisterChatDialog()));
    dlg->show();
    return dlg;
}
void Dialog::unregisterChatDialog()
{
    openChatDialogs.remove(openChatDialogs.key(dynamic_cast<ChatDialog*>(sender())));
}
void Dialog::messageRecieved(QString message,QString username)
{
    if (openChatDialogs.contains(username))
        return;
    //Find the list item where the username is displayed, and open a chatdialog according to it
    int rowNumber = ui->peerList->row(ui->peerList->findItems(username, Qt::MatchExactly)[0]);
    ChatDialog *dlg = openChatWindow(ui->peerList->item(rowNumber));
    dlg->messageRecieved(message, username);
}
