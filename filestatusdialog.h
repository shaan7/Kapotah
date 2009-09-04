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
#include "filerecieverthread.h"
#include "dirrecieverthread.h"

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
        QToolButton *pause;
        QToolButton *cancel;
        QString savePath;
        QString senderName;
        QString fileName;
        FileRecieverThread *reciever;
        bool inProgress;
        qint64 bytesDone;
        qint64 fileSize;
        bool isUpload;
    };

    struct DirTransfer
    {
        QGroupBox *box;
        QProgressBar *progress;
        QToolButton *pause;
        QToolButton *cancel;
        QString savePath;
        QString senderName;
        QDomNodeList fileList;
        QDomNodeList dirList;
        DirRecieverThread *reciever;
        FileRecieverThread *fReciever;
        bool inProgress;
        bool isUpload;
        qint64 bytesDone;
        qint64 totalSize;
        qint64 fileCount;
        qint64 bytesDoneTillNow;
        int filesDone;
    };

    Pointers *m_ptr;

    QHash<QString,FileTransfer*> fileTransfers;
    QHash<QString,DirTransfer*> dirTransfers;

    QSignalMapper *filePauseMapper;
    QSignalMapper *fileCancelMapper;
    QSignalMapper *dirPauseMapper;
    QSignalMapper *dirCancelMapper;
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void fileCancelClicked(QString ID);
    void filePauseClicked(QString ID);
    void fileRecieved(QString filename,qint64 size,QString ID,QString username);
    void fileProgress(QString ID, QString peer, QString fileName, qint64 size, qint64 bytesDone);
    void fileDone(QString ID);

    void dirCancelClicked(QString ID);
    void dirPauseClicked(QString ID);
    void dirRecieved(QDomNodeList fileList, QDomNodeList dirList, QString username);
    void dirProgress(QString ID, int filesDone);
    void dirDone(QString ID);

public slots:
    void addFileTransfer(QString filename, qint64 size, QString ID, QString senderName, bool isUpload);
    void addDirTransfer(QDomNodeList fileList, QDomNodeList dirList, QString senderName, bool isUpload);
};

#endif // FILESTATUSDIALOG_H
