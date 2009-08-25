#ifndef FILERECIEVERTHREAD_H
#define FILERECIEVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include "peermanager.h"

class FileRecieverThread : public QThread
{
    Q_OBJECT

private:
    QString ID;
    QString peer;
    PeerManager *manager;
    QTcpSocket socket;
    bool readyToRecieve;
    bool startedRecieving;
    QString filename;
    QFile file;
    qint64 size;
    qint64 bytesCopied;

public:
    FileRecieverThread(PeerManager *peerManager, QString fileID, qint64 fileSize, QString sendingPeer, QString outputFilename, QObject *parent);
    ~FileRecieverThread();

    void run();

private slots:
    void sendFileID();
    void readFile();
    void cleanup();

signals:
    void progress(int bytesDone);
};

#endif // FILERECIEVERTHREAD_H
