#ifndef FILESTATUSDIALOG_H
#define FILESTATUSDIALOG_H

#include <QDialog>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QToolButton>
#include <QProgressBar>
#include <QLabel>
#include <QDomNodeList>
#include "ui_filestatusdialog.h"
#include "pointers.h"
#include "filereceiverthread.h"
#include "dirreceiverthread.h"

class FileStatusDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(FileStatusDialog)
public:
    explicit FileStatusDialog(Pointers *ptr, QWidget *parent = 0);

private:
    Ui::FileStatusDialog m_ui;

    struct FileTransfer
    {
        QGroupBox *box;
        QProgressBar *progress;
        QToolButton *start;
        QToolButton *cancel;
        QString savePath;
        QHostAddress senderIP;
        QString fileName;
        FileReceiverThread *receiver;
        bool inProgress;
        quint64 bytesDone;
        quint64 fileSize;
        bool isUpload;
    };

    struct DirTransfer
    {
        QGroupBox *box;
        QProgressBar *progress;
        QToolButton *start;
        QToolButton *cancel;
        QString savePath;
        QHostAddress senderIP;
        QDomNodeList fileList;
        QDomNodeList dirList;
        DirReceiverThread *receiver;
        FileReceiverThread *fReceiver;
        bool inProgress;
        bool isUpload;
        quint64 bytesDone;
        quint64 totalSize;
        quint64 fileCount;
        quint64 bytesDoneTillNow;
        int filesDone;
    };

    Pointers *m_ptr;

    QHash<QString,FileTransfer*> fileTransfers;
    QHash<QString,DirTransfer*> dirTransfers;

    QHash<QString,FileTransfer*> upFileTransfers;
    QHash<QString,DirTransfer*> upDirTransfers;

    QSignalMapper *fileStartMapper;
    QSignalMapper *fileCancelMapper;
    QSignalMapper *dirStartMapper;
    QSignalMapper *dirCancelMapper;

    void addTransferEntry(QString ID, QString title, QHostAddress senderIP, bool isUpload, bool isDir);
    QString returnToolTipHTML(QString title, QString description);
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void fileCancelClicked(QString ID);
    void fileStartClicked(QString ID);
    void fileRecieved(QString filename,quint64 size,QString ID,QHostAddress senderIP);
    void fileProgress(QString ID, QHostAddress peer, QString fileName, quint64 size, quint64 bytesDone);
    void fileDone(QString ID);

    void dirCancelClicked(QString ID);
    void dirStartClicked(QString ID);
    void dirRecieved(QDomNodeList fileList, QDomNodeList dirList, QHostAddress senderIP);
    void dirProgress(QString ID, int filesDone);
    void dirDone(QString ID);

    void fileSendStarted(QString ID, QString filename);
    void fileSendFinished(QString ID, QString filename);
    void fileProgress(QString ID, quint64 bytes);

public slots:
    void addFileTransfer(QString filename, quint64 size, QString ID, QHostAddress senderIP, bool isUpload);
    void addDirTransfer(QDomNodeList fileList, QDomNodeList dirList, QHostAddress senderIP, bool isUpload);
};

#endif // FILESTATUSDIALOG_H
