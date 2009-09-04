#include "filestatusdialog.h"
#include "server.h"
#include <QDebug>
#include <QCloseEvent>
#include <QFileDialog>
#include <QCryptographicHash>

//THIS FILE IS TOOOOOOOOOOOO LARGE, IT WILL BE MADE SMALLER

FileStatusDialog::FileStatusDialog(Pointers *ptr, QWidget *parent) :
    QDialog(parent), m_ptr(ptr) {
    m_ui.setupUi(this);
    filePauseMapper = new QSignalMapper(this);
    fileCancelMapper = new QSignalMapper(this);
    dirPauseMapper = new QSignalMapper(this);
    dirCancelMapper = new QSignalMapper(this);


    connect(fileCancelMapper, SIGNAL(mapped(QString)), this, SLOT(fileCancelClicked(QString)));
    connect(filePauseMapper, SIGNAL(mapped(QString)), this, SLOT(filePauseClicked(QString)));
    connect(dirCancelMapper, SIGNAL(mapped(QString)), this, SLOT(dirCancelClicked(QString)));
    connect(dirPauseMapper, SIGNAL(mapped(QString)), this, SLOT(dirPauseClicked(QString)));

    connect(ptr->server, SIGNAL(fileRecieved(QString,qint64,QString,QString)), this, SLOT(fileRecieved(QString,qint64,QString,QString)));
    connect(ptr->server, SIGNAL(dirRecieved(QDomNodeList,QDomNodeList,QString)), this, SLOT(dirRecieved(QDomNodeList,QDomNodeList,QString)));
}

void FileStatusDialog::dirRecieved(QDomNodeList fileList, QDomNodeList dirList, QString username)
{
    show();
    addDirTransfer(fileList, dirList, username, false);
}

void FileStatusDialog::fileRecieved(QString filename,qint64 size,QString ID,QString username)
{
    show();
    addFileTransfer(filename, size, ID, username, false);
}

void FileStatusDialog::addFileTransfer(QString filename, qint64 size, QString ID, QString senderName, bool isUpload)
{
    FileTransfer *transfer = new FileTransfer;

    //Create the new widgets
    QGroupBox *box = new QGroupBox(m_ui.scrollArea);
    QGridLayout *layout = new QGridLayout();
    QProgressBar *progress = new QProgressBar();
    QToolButton *pause = new QToolButton();
    QToolButton *cancel = new QToolButton();
    QToolButton *type = new QToolButton();

    //Store info
    transfer->box = box;
    transfer->progress = progress;
    transfer->fileName = filename;
    transfer->inProgress = false;
    transfer->senderName = senderName;
    transfer->cancel = cancel;
    transfer->pause = pause;
    transfer->isUpload = isUpload;

    transfer->fileSize = size;
    fileTransfers[ID] = transfer;

    //Set the widgets

    pause->setAutoRaise(true);
    cancel->setAutoRaise(true);
    type->setAutoRaise(true);

    pause->setArrowType(Qt::RightArrow);
    if (isUpload) {
        type->setArrowType(Qt::UpArrow);
        type->setToolTip("This Is A Upload");
    }
    else {
        type->setIcon(QIcon(":/images/kget.png"));
        type->setToolTip("This Is A Download");
    }

    //Layout the new widgets
    layout->addWidget(progress, 0, 0);
    layout->addWidget(pause, 0, 1);
    layout->addWidget(cancel, 0, 2);
    layout->addWidget(type, 0, 3);
    box->setLayout(layout);
    box->setTitle(filename + " with " + senderName);

    //Add the widgets to the ScrollArea

    //Remove the spacer
    m_ui.scrollAreaWidgetContents->layout()->takeAt(m_ui.scrollAreaWidgetContents->layout()->count()-1);
    //Add the group box
    m_ui.scrollAreaWidgetContents->layout()->addWidget(box);
    //Add a new spacer
    dynamic_cast<QVBoxLayout*>(m_ui.scrollAreaWidgetContents->layout())->addStretch();

    //Connect signals
    connect(cancel, SIGNAL(clicked()), fileCancelMapper, SLOT(map()));
    connect(pause, SIGNAL(clicked()), filePauseMapper, SLOT(map()));

    //Set mapping
    fileCancelMapper->setMapping(cancel, ID);
    filePauseMapper->setMapping(pause, ID);
}

void FileStatusDialog::addDirTransfer(QDomNodeList fileList, QDomNodeList dirList, QString senderName, bool isUpload)
{
    DirTransfer *transfer = new DirTransfer;

    //Create the new widgets
    QGroupBox *box = new QGroupBox(m_ui.scrollArea);
    QGridLayout *layout = new QGridLayout();
    QProgressBar *progress = new QProgressBar();
    QToolButton *pause = new QToolButton();
    QToolButton *cancel = new QToolButton();
    QToolButton *type = new QToolButton();

    //Store info
    transfer->box = box;
    transfer->progress = progress;
    transfer->inProgress = false;
    transfer->senderName = senderName;
    transfer->cancel = cancel;
    transfer->pause = pause;
    transfer->isUpload = isUpload;

    transfer->fileList = fileList;
    transfer->dirList = dirList;
    qint64 totalSize=0;

    //Calculate ID and total size of dir
    QString items;
    for (int i=0;i<dirList.count();i++) {
        items += dirList.at(i).toElement().attribute("path");  //Arbitrary method
    }
    QString ID(QCryptographicHash::hash(items.toUtf8(), QCryptographicHash::Md5).toHex());
    for (int i=0;i<fileList.count();i++) {
        totalSize += fileList.at(i).toElement().attribute("size").toLongLong();
    }
    transfer->totalSize = totalSize;
    transfer->bytesDone = 0;
    transfer->bytesDoneTillNow = 0;
    transfer->filesDone = 0;
    dirTransfers[ID] = transfer;

    //Set the widgets

    progress->setMaximum(fileList.count());

    pause->setAutoRaise(true);
    cancel->setAutoRaise(true);
    type->setAutoRaise(true);

    pause->setArrowType(Qt::RightArrow);
    if (isUpload) {
        type->setArrowType(Qt::UpArrow);
        type->setToolTip("This Is A Upload");
    }
    else {
        type->setIcon(QIcon(":/images/kget.png"));
        type->setToolTip("This Is A Download");
    }

    //Layout the new widgets
    layout->addWidget(progress, 0, 0);
    layout->addWidget(pause, 0, 1);
    layout->addWidget(cancel, 0, 2);
    layout->addWidget(type, 0, 3);
    box->setLayout(layout);
    box->setTitle("Directory with " + senderName);

    //Add the widgets to the ScrollArea

    //Remove the spacer
    m_ui.scrollAreaWidgetContents->layout()->takeAt(m_ui.scrollAreaWidgetContents->layout()->count()-1);
    //Add the group box
    m_ui.scrollAreaWidgetContents->layout()->addWidget(box);
    //Add a new spacer
    dynamic_cast<QVBoxLayout*>(m_ui.scrollAreaWidgetContents->layout())->addStretch();

    //Connect signals
    connect(cancel, SIGNAL(clicked()), dirCancelMapper, SLOT(map()));
    connect(pause, SIGNAL(clicked()), dirPauseMapper, SLOT(map()));

    //Set mapping
    dirCancelMapper->setMapping(cancel, ID);
    dirPauseMapper->setMapping(pause, ID);
}

void FileStatusDialog::dirCancelClicked(QString ID)
{
    DirTransfer *transfer = dirTransfers[ID];
    transfer->reciever->stopTransfer();
    transfer->progress->setMaximum(0);
    transfer->progress->setValue(0);
    transfer->inProgress = false;
}

void FileStatusDialog::dirPauseClicked(QString ID)
{
    DirTransfer *transfer = dirTransfers[ID];
    if (transfer->inProgress) {        //If already started download
        transfer->reciever->start();
        return;
    }
    QString dirname = QFileDialog::getExistingDirectory(this, "Select a dir");
    if (dirname=="")
        return;

    transfer->reciever = new DirRecieverThread(m_ptr, ID, dirname, transfer->fileList, transfer->dirList, transfer->senderName, this);
    transfer->savePath = dirname;
    connect(transfer->reciever, SIGNAL(progress(QString,int)), this, SLOT(dirProgress(QString,int)));
    connect(transfer->reciever, SIGNAL(done(QString)), this, SLOT(dirDone(QString)));
    //startTime = QDateTime::currentDateTime();
    transfer->inProgress = true;
}

void FileStatusDialog::dirProgress(QString ID, int filesDone)
{
    DirTransfer *transfer = dirTransfers[ID];
    if (!transfer->inProgress)
        return;

    transfer->progress->setValue(filesDone);
}

void FileStatusDialog::dirDone(QString ID)
{
    DirTransfer *transfer = dirTransfers[ID];
    transfer->pause->hide();
    transfer->cancel->hide();
}

void FileStatusDialog::fileCancelClicked(QString ID)
{
    FileTransfer *transfer = fileTransfers[ID];
    transfer->reciever->stopTransfer();
    transfer->progress->setMaximum(0);
    transfer->progress->setValue(0);
    transfer->inProgress = false;
}

void FileStatusDialog::filePauseClicked(QString ID)
{
    FileTransfer *transfer = fileTransfers[ID];
    if (transfer->inProgress) {        //If already started download
        transfer->reciever->start();
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "Select a file to save",
                                                    QDir::home().absoluteFilePath(transfer->fileName)); //CHANGES
    if (filename=="")
        return;
    transfer->savePath = filename;
    transfer->reciever = new FileRecieverThread(m_ptr, ID, transfer->fileSize,
                                                     transfer->senderName, transfer->savePath, this);
    connect(transfer->reciever, SIGNAL(progress(QString,QString,QString,qint64,qint64)),
            this, SLOT(fileProgress(QString,QString,QString,qint64,qint64)));
    connect(transfer->reciever, SIGNAL(done(QString)), this, SLOT(fileDone(QString)));
    //startTime = QDateTime::currentDateTime();
    transfer->inProgress = true;
}

void FileStatusDialog::fileProgress(QString ID, QString peer, QString fileName, qint64 size, qint64 bytesDone)
{
    FileTransfer *transfer = fileTransfers[ID];
    if (!transfer->inProgress)
        return;

    transfer->progress->setMaximum(size);
    transfer->progress->setValue(bytesDone);
}

void FileStatusDialog::fileDone(QString ID)
{
    FileTransfer *transfer = fileTransfers[ID];
    transfer->pause->hide();
    transfer->cancel->hide();
}

void FileStatusDialog::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

