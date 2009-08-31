#include "dirrecieverthread.h"
#include <QDir>

DirRecieverThread::DirRecieverThread(PeerManager *peerManager, QString dir, QDomNodeList files, QDomNodeList dirs, QString peername, QObject *parent)
    : QThread(parent), manager(peerManager), fileList(files), dirList(dirs), peerName(peername), dirName(dir)
{
}

void DirRecieverThread::run()
{
    QDir dir(dirName);
    for (int i=0;i<dirList.count();i++) {
        dir.mkdir(dirList.at(i).toElement().attribute("path"));
    }

    for (int i=0;i<fileList.count();i++) {
        fileReciever = new FileRecieverThread(manager, fileList.at(i).toElement().attribute("ID"),
                                          fileList.at(i).toElement().attribute("size").toInt(), peerName,
                                          dir.absoluteFilePath(fileList.at(i).toElement().attribute("path")), 0);
        qDebug() << "START " << dir.absoluteFilePath(fileList.at(i).toElement().attribute("path"));
        fileReciever->run();
        delete fileReciever;
    }
}
