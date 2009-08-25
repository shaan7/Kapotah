#ifndef FILESENDERTHREAD_H
#define FILESENDERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "peermanager.h"
#include "fileserver.h"

class FileSenderThread : public QThread
{
    Q_OBJECT
public:
    FileSenderThread(PeerManager *peermanager, FileServer* fileServer, QString fileToSend, PeerInfo destinationPeer);

    void run();

private:
    PeerManager *manager;
    FileServer *fserver;
    QString filename;
    PeerInfo destination;
    QTcpSocket socket;
    QString ID;

private slots:
    void sendFileInformation();
};

#endif // FILESENDERTHREAD_H
