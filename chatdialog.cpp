#include "chatdialog.h"
#include "messagesender.h"

ChatDialog::ChatDialog(QString peer, PeerManager *peerManager, Server *server, QWidget *parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    peerName = peer;
    manager = peerManager;
    setWindowTitle(peer + "@" + manager->peerInfo(peer).ipAddress().toString());    //Set the window title to peer@ipaddress
    connect(ui.sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
    connect(server, SIGNAL(messageRecieved(QString,QString)), this, SLOT(messageRecieved(QString,QString)));
}

void ChatDialog::sendMessage()
{
    MessageSender *sender = new MessageSender(manager, this);
    sender->sendMessage(ui.messageEdit->text(),manager->peerInfo(peerName));
    ui.chatEdit->append(peerName + "::" + ui.messageEdit->text());
    ui.messageEdit->clear();
}

void ChatDialog::messageRecieved(QString message, QString username)
{
    if (username == manager->peerInfo(peerName).name()) {
        ui.chatEdit->append(username + "::" + message);
    }
}

ChatDialog::~ChatDialog()
{
}
