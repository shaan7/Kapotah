#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>
#include <QModelIndex>
#include "peermanager.h"
#include <QListWidgetItem>
#include "chatdialog.h"
#include "server.h"

namespace Ui
{
    class DialogClass;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(Server *server,QWidget *parent = 0);
    ~Dialog();

private:
    Ui::DialogClass *ui;
    PeerManager *manager;
    QHash<QString,ChatDialog*> openChatDialogs;
    Server *m_server;

private slots:
    void startPeerManager();
    void addNewPeer(QString peer);
    void removePeer(QString peer);
    ChatDialog* openChatWindow(QListWidgetItem*);

public slots:
    void unregisterChatDialog();
    void messageRecieved(QString message,QString username);
};

#endif // DIALOG_H
