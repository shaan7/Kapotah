#ifndef FILESERVER_H
#define FILESERVER_H

#include <QTcpServer>

class FileServer : public QTcpServer
{
    Q_OBJECT
public:
    FileServer();
    void addFile(QString ID, QString FileName);
    QHash<QString,QString> *getFileList()    {   return &fileList;   }

protected:
    void incomingConnection(int descriptor);

private:
    QHash<QString,QString> fileList;
};

#endif // FILESERVER_H
