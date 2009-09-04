#include "dirrecieverthread.h"
#include <QDir>

DirRecieverThread::DirRecieverThread(Pointers *ptr, QString ID, QString dir, QDomNodeList files, QDomNodeList dirs, QString peername, QObject *parent)
    : QThread(parent), manager(ptr->manager), fileList(files), dirList(dirs), peerName(peername), dirName(dir), m_ptr(ptr), m_ID(ID)
{
    cancel = false;
}

void DirRecieverThread::run()
{
    QDir dir(dirName);
    for (int i=0;i<dirList.count();i++) {
        dir.mkdir(dirList.at(i).toElement().attribute("path"));
    }

    for (int i=0;i<fileList.count();i++) {
        if (cancel)
            break;
        fileReciever = new FileRecieverThread(m_ptr, fileList.at(i).toElement().attribute("ID"),
                                          fileList.at(i).toElement().attribute("size").toInt(), peerName,
                                          dir.absoluteFilePath(fileList.at(i).toElement().attribute("path")), 0);
        qDebug() << "START " << dir.absoluteFilePath(fileList.at(i).toElement().attribute("path"));
        //fileReciever->setPId(m_ID);
        //emit recieverChanged(m_ID, fileReciever, i+1);
        fileReciever->run();
        delete fileReciever;

        emit progress(m_ID, i+1);
    }
    emit done(m_ID);
}

void DirRecieverThread::stopTransfer()
{
    cancel = true;
}
