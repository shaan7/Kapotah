#ifndef DIRSENDERTHREAD_H
#define DIRSENDERTHREAD_H

#include <QThread>
#include <QStringList>
#include "peermanager.h"
#include "fileserver.h"

class DirSenderThread : public QThread
{
    Q_OBJECT

public:
    DirSenderThread(PeerManager *peermanager, FileServer *fileServer, QString dirToSend, PeerInfo destinationPeer, QObject *parent);
    ~DirSenderThread();

    void run();
private:
    PeerManager *manager;
    FileServer *fserver;
    QString dirPath;
    PeerInfo peer;
    QStringList fileList;
    QStringList dirList;

    void prepareItemsList(QString path);
};

#endif // DIRSENDERTHREAD_H
