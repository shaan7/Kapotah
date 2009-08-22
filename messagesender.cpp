#include "messagesender.h"
#include <QDomDocument>
#include <QTcpSocket>

MessageSender::MessageSender(PeerManager *peerManager, QObject *parent) : QObject(parent)
{
    manager = peerManager;
}

void MessageSender::sendMessage(QString message, PeerInfo destinationPeer)
{
    QDomDocument document;

    QDomElement doc = document.createElement("document");
    QDomElement action = document.createElement("action");
    action.setAttribute( "type", "message" );
    QDomElement msg = document.createElement("message");
    msg.setAttribute("senderName", manager->username());
    msg.setAttribute("content", message);

    document.appendChild(doc);
    doc.appendChild(action);
    action.appendChild(msg);

    QTcpSocket socket;
    socket.abort();
    socket.connectToHost(destinationPeer.ipAddress(), 9876);
    socket.waitForConnected();
    socket.write(document.toByteArray());
    socket.disconnectFromHost();
    socket.waitForDisconnected();
    deleteLater();
}
