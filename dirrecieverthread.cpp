#include "dirrecieverthread.h"
#include <QDir>

DirRecieverThread::DirRecieverThread(Pointers *ptr, QString dir, QDomNodeList files, QDomNodeList dirs, QString peername, QObject *parent)
    : QThread(parent), manager(ptr->manager), fileList(files), dirList(dirs), peerName(peername), dirName(dir), m_ptr(ptr)
{
}

void DirRecieverThread::run()
{
    QDir dir(dirName);
    for (int i=0;i<dirList.count();i++) {
        dir.mkdir(dirList.at(i).toElement().attribute("path"));
    }

    for (int i=0;i<fileList.count();i++) {
        fileReciever = new FileRecieverThread(m_ptr, fileList.at(i).toElement().attribute("ID"),
                                          fileList.at(i).toElement().attribute("size").toInt(), peerName,
                                          dir.absoluteFilePath(fileList.at(i).toElement().attribute("path")), 0);
        qDebug() << "START " << dir.absoluteFilePath(fileList.at(i).toElement().attribute("path"));
        fileReciever->run();
        delete fileReciever;
    }
}
