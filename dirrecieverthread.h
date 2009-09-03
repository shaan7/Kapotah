#ifndef DIRRECIEVERTHREAD_H
#define DIRRECIEVERTHREAD_H

#include <QThread>
#include <QDomNodeList>
#include "peermanager.h"
#include "filerecieverthread.h"
#include "pointers.h"

class DirRecieverThread : public QThread
{
    Q_OBJECT

private:
    Pointers *m_ptr;
    PeerManager *manager;
    QDomNodeList fileList;
    QDomNodeList dirList;
    QString peerName;
    QString dirName;
    FileRecieverThread *fileReciever;
public:
    DirRecieverThread(Pointers *ptr, QString dir, QDomNodeList files, QDomNodeList dirs, QString peername, QObject *parent);

    void run();
};

#endif // DIRRECIEVERTHREAD_H
