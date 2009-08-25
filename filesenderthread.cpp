#include <QFileInfo>
#include <QDomDocument>
#include <QTime>
#include "filesenderthread.h"

FileSenderThread::FileSenderThread(PeerManager *peermanager, FileServer *fileServer, QString fileToSend, PeerInfo destinationPeer)
{
    manager = peermanager;
    fserver = fileServer;
    filename = fileToSend;
    destination = destinationPeer;

    qsrand(1000);
}

void FileSenderThread::run()
{
    socket.abort();
    socket.connectToHost(destination.ipAddress(), 9876);
    connect(&socket, SIGNAL(connected()), this, SLOT(sendFileInformation()));
}

void FileSenderThread::sendFileInformation()
{
    //Get info about the file
    QFileInfo info(filename);
    QDomDocument document;

    QDomElement doc = document.createElement("document");
    QDomElement action = document.createElement("action");
    action.setAttribute( "type", "file" );
    QDomElement file = document.createElement("file");
    file.setAttribute("senderName", manager->username());
    file.setAttribute("fileName", info.fileName());
    file.setAttribute("size", QString::number(info.size()));
    ID = QTime::currentTime().toString() + "::" + QString::number(qrand());
    file.setAttribute("ID", ID);

    if (!fserver->getFileList()->contains(ID)) {
        fserver->addFile(ID, filename);
    }

    document.appendChild(doc);
    doc.appendChild(action);
    action.appendChild(file);

    socket.write(document.toByteArray());
    socket.disconnectFromHost();
    socket.waitForDisconnected();
    deleteLater();
}
