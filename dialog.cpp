#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(Server *server,QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogClass)
{
    ui->setupUi(this);

    m_server = server;

    connect(ui->refreshButton,SIGNAL(clicked()),this,SLOT(startPeerManager()));
    connect(ui->peerList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(openChatWindow(QListWidgetItem*)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::startPeerManager()
{
    ui->refreshButton->setEnabled(false);
    manager = new PeerManager();    //Create the peer manager
    manager->startBroadcast();  //Start broadcasting on the network

    connect(manager,SIGNAL(newPeer(QString)),this,SLOT(addNewPeer(QString)));
    connect(manager,SIGNAL(peerGone(QString)),this,SLOT(removePeer(QString)));
}

void Dialog::addNewPeer(QString peer)
{
    ui->peerList->addItem(peer);
}

void Dialog::removePeer(QString peer)
{
    //We have to find the row in which the peer is being shown and select the first match
    int rowNumber = ui->peerList->row(ui->peerList->findItems(peer, Qt::MatchExactly)[0]);
    ui->peerList->takeItem(rowNumber);
}

void Dialog::openChatWindow(QListWidgetItem *item)
{
    ChatDialog *dlg = new ChatDialog(item->text(), manager, m_server, this);
    openChatDialogs[item->text()] = dlg;    //Save the dialog to the QHash so that we know which chat dialogs are open
    dlg->show();
}

void Dialog::unregisterChatDialog(QString peerName)
{
    openChatDialogs.remove(peerName);
}
