#include "server.h"
#include <QTcpSocket>
#include <QDomDocument>

Server::Server(QObject *parent) :
        QTcpServer(parent)
{
}

void Server::incomingConnection(int socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readIncomingData()));
}

void Server::readIncomingData()
{
    //Parse the datagram as XML
    QDomDocument document;
    document.setContent(dynamic_cast<QTcpSocket*>(sender())->readAll(), false, 0, 0, 0);
    QDomElement documentElement = document.documentElement();
    QDomNode node = documentElement.firstChild();
    QDomElement action = node.toElement();

    //If data is type message
    if (action.attribute("type") == "message") {
        QDomElement message = action.firstChild().toElement();
        emit messageRecieved(message.attribute("content", "ERROR"),message.attribute("senderName", "unknown"));
    }
}
