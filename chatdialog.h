#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "ui_chatdialog.h"
#include "peermanager.h"
#include "server.h"

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    ChatDialog(QString name, PeerManager *peerManager, Server *server, QWidget *parent = 0);
    ~ChatDialog();

private:
    Ui::ChatDialog ui;
    QString peerName;
    PeerManager *manager;

private slots:
    void sendMessage();
    void messageRecieved(QString message, QString username);
};

#endif // CHATDIALOG_H
