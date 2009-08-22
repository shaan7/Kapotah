#ifndef PEERMANAGER_H
#define PEERMANAGER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include "peerinfo.h"

class PeerManager : public QObject
{
    Q_OBJECT

private:
    QList<QHostAddress> broadcastAddresses;
    QList<QHostAddress> ipAddresses;
    QString m_username;
    QHash<QString, PeerInfo> peers;
    QUdpSocket broadcastSocket;
    QTimer broadcastTimer;

    void updateAddresses();
    int serverPort;

public:
    PeerManager();
    void startBroadcast();
    void setUsername(QString name)  {   m_username = name;    }
    QString username()  {   return m_username;  }
    PeerInfo peerInfo(QString name);

signals:
    void newPeer(QString name);
    void peerGone(QString name);

public slots:
    void sendBroadcast();
    void checkPeers();
    void readBroadcast();
};

#endif // PEERMANAGER_H
