#include "fileserverthread.h"
#include "fileserver.h"

static const unsigned bytesPerBlock = 1000;
FileServerThread::FileServerThread(int descriptor, QObject *parent) : QThread(parent)
{
    socket.setSocketDescriptor(descriptor);
    connect(&socket, SIGNAL(readyRead()), this, SLOT(readResponse()));
}

void FileServerThread::run()
{
    ;
}

void FileServerThread::readResponse()
{
    QString response = socket.readAll();

    if (response == "READY") {
        file.setFileName(filename);
        if (!file.open(QIODevice::ReadOnly))
            return;

        socket.write(file.read(bytesPerBlock));
    }
    else if (response == "SENDMORE") {
        socket.write(file.read(bytesPerBlock));
    }
    else if (dynamic_cast<FileServer*>(parent())->getFileList()->contains(response)) {
        ID = response;
        filename = dynamic_cast<FileServer*>(parent())->getFileList()->value(ID);
        socket.write("OK");
    }
}
