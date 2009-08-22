#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent=0);

protected:
     void incomingConnection(int socketDescriptor);

private slots:
     void readIncomingData();

signals:
     void messageRecieved(QString message, QString username);
};

#endif // SERVER_H
