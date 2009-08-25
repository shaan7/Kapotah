#include "fileserver.h"
#include "fileserverthread.h"

FileServer::FileServer() : QTcpServer()
{
}

void FileServer::addFile(QString ID, QString FileName)
{
    fileList[ID] = FileName;
}

void FileServer::incomingConnection(int descriptor)
{
    FileServerThread *thread = new FileServerThread(descriptor, this);
    thread->start();
}
