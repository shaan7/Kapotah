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

class FileStatusDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(FileStatusDialog)
public:
    explicit FileStatusDialog(Pointers *ptr, QWidget *parent = 0);

private:
    Ui::FileStatusDialog m_ui;

    struct Transfer
    {
        QGroupBox *box;
        QProgressBar *progress;
        FileRecieverThread *reciever;
        bool inProgress;
        QString fileName;
        qint64 bytesDone;
        qint64 fileSize;
        QString savePath;
        QString senderName;
        QToolButton *pause;
        QToolButton *cancel;
        bool isDir;
        bool isUpload;
        QDomNodeList fileList;
        QDomNodeList dirList;
    };

    Pointers *m_ptr;

    QHash<QString,Transfer*> transfers;

    QSignalMapper *pauseMapper;
    QSignalMapper *cancelMapper;
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void cancelClicked(QString ID);
    void pauseClicked(QString ID);
    void fileRecieved(QString filename,qint64 size,QString ID,QString username);
    void dirRecieved(QDomNodeList fileList, QDomNodeList dirList, QString username);
    void progress(QString ID, QString peer, QString fileName, qint64 size, qint64 bytesDone);
    void fileDone(QString ID);

public slots:
    void addTransfer(QDomNodeList fileList, QDomNodeList dirList, QString filename, qint64 size, QString ID,
                     QString senderName, bool isUpload, bool isDir);
};

#endif // FILESTATUSDIALOG_H
