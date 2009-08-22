#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include <QObject>
#include "peermanager.h"

class MessageSender : public QObject
{
    Q_OBJECT

private:
    PeerManager *manager;

public:
    MessageSender(PeerManager *peerManager, QObject *parent=0);

public slots:
    void sendMessage(QString message, PeerInfo destinationPeer);
};

#endif // MESSAGESENDER_H
