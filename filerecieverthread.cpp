#include <QFile>
#include <QByteArray>
#include "filerecieverthread.h"
#include "peerinfo.h"

FileRecieverThread::FileRecieverThread(PeerManager *peerManager, QString fileID, qint64 fileSize, QString sendingPeer, QString outputFilename, QObject *parent)
    : QThread(parent)
{
    ID = fileID;
    peer = sendingPeer;
    manager = peerManager;
    readyToRecieve = false;
    startedRecieving = false;
    filename = outputFilename;
    size = fileSize;
    bytesCopied = 0;
}

void FileRecieverThread::run()
{
    socket.connectToHost(manager->peerInfo(peer).ipAddress(), 9877);
    connect(&socket, SIGNAL(connected()), this, SLOT(sendFileID()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(cleanup()));
}

void FileRecieverThread::sendFileID()
{
    socket.write(ID.toUtf8());
    connect(&socket, SIGNAL(readyRead()), this, SLOT(readFile()));
}

void FileRecieverThread::readFile()
{
    QByteArray data = socket.readAll();
    QString response = data;
    
    if (readyToRecieve) {
        if (startedRecieving) {
            bytesCopied += file.write(data);
            emit progress(bytesCopied);
        }
        else {
            file.setFileName(filename);

            if (!file.open(QIODevice::WriteOnly)) {
                qDebug() << "ERROR OPENING FILE";
                return;
            }

            bytesCopied += file.write(data);
            emit progress(bytesCopied);
            startedRecieving = true;
        }
        if (bytesCopied < size)
            socket.write("SENDMORE");
        else {
            file.close();
            socket.disconnectFromHost();
        }
    }
    else if (response == "OK") {
        readyToRecieve = true;
        socket.write("READY");
    }
}

void FileRecieverThread::cleanup()
{
    socket.close();
    deleteLater();
}

FileRecieverThread::~FileRecieverThread()
{
    cleanup();
}
