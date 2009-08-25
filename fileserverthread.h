#ifndef FILESERVERTHREAD_H
#define FILESERVERTHREAD_H

#include <QFile>
#include <QThread>
#include <QTcpSocket>

class FileServerThread : public QThread
{
    Q_OBJECT
public:
    FileServerThread(int descriptor, QObject *parent);

    void run();

private:
    QTcpSocket socket;
    QString ID;
    QFile file;
    QString filename;

private slots:
    void readResponse();
};

#endif // FILESERVERTHREAD_H
