#include "filestatusdialog.h"
#include "server.h"
#include <QDebug>
#include <QCloseEvent>
#include <QFileDialog>

FileStatusDialog::FileStatusDialog(Pointers *ptr, QWidget *parent) :
    QDialog(parent), m_ptr(ptr) {
    m_ui.setupUi(this);
    pauseMapper = new QSignalMapper(this);
    cancelMapper = new QSignalMapper(this);

    connect(cancelMapper, SIGNAL(mapped(QString)), this, SLOT(cancelClicked(QString)));
    connect(pauseMapper, SIGNAL(mapped(QString)), this, SLOT(pauseClicked(QString)));

    connect(ptr->server, SIGNAL(fileRecieved(QString,qint64,QString,QString)), this, SLOT(fileRecieved(QString,qint64,QString,QString)));
    connect(ptr->server, SIGNAL(dirRecieved(QDomNodeList,QDomNodeList,QString)), this, SLOT(dirRecieved(QDomNodeList,QDomNodeList,QString)));
}

void FileStatusDialog::dirRecieved(QDomNodeList fileList, QDomNodeList dirList, QString username)
{
    show();
    addTransfer(fileList, dirList, QString("Directory"), 0, QString("TODO"), username, false, true);   //TODO
}

void FileStatusDialog::fileRecieved(QString filename,qint64 size,QString ID,QString username)
{
    show();
    addTransfer(QDomNodeList(), QDomNodeList(), filename, size, ID, username, false, false);
}

void FileStatusDialog::addTransfer(QDomNodeList fileList, QDomNodeList dirList, QString filename, qint64 size, QString ID,
                     QString senderName, bool isUpload, bool isDir)
{
    Transfer *transfer = new Transfer;

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
    transfer->isDir = isDir;
    transfer->isUpload = isUpload;

    if (isDir) {
        transfer->fileList = fileList;
        transfer->dirList = dirList;
        qint64 totalSize=0;

        //Calculate total size of dir
        for (int i=0;i<fileList.count();i++) {
            totalSize += fileList.at(i).toElement().attribute("size").toLongLong();
        }
        transfer->fileSize = totalSize;
        transfer->bytesDone = 0;
    }
    else {
        transfer->fileSize = size;
    }

    transfers[ID] = transfer;

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
    connect(cancel, SIGNAL(clicked()), cancelMapper, SLOT(map()));
    connect(pause, SIGNAL(clicked()), pauseMapper, SLOT(map()));

    //Set mapping
    cancelMapper->setMapping(cancel, ID);
    pauseMapper->setMapping(pause, ID);
}

void FileStatusDialog::cancelClicked(QString ID)
{
    transfers[ID]->reciever->stopTransfer();
    transfers[ID]->progress->setMaximum(0);
    transfers[ID]->progress->setValue(0);
    transfers[ID]->inProgress = false;
}

void FileStatusDialog::pauseClicked(QString ID)
{
    if (transfers[ID]->inProgress) {        //If already started download
        transfers[ID]->reciever->start();
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "Select a file to save",
                                                    QDir::home().absoluteFilePath(transfers[ID]->fileName)); //CHANGES
    if (filename=="")
        return;
    transfers[ID]->savePath = filename;
    transfers[ID]->reciever = new FileRecieverThread(m_ptr, ID, transfers[ID]->fileSize,
                                                     transfers[ID]->senderName, transfers[ID]->savePath, this);
    connect(transfers[ID]->reciever, SIGNAL(progress(QString,QString,QString,qint64,qint64)),
            this, SLOT(progress(QString,QString,QString,qint64,qint64)));
    connect(transfers[ID]->reciever, SIGNAL(done(QString)), this, SLOT(fileDone(QString)));
    //startTime = QDateTime::currentDateTime();
    transfers[ID]->inProgress = true;
}

void FileStatusDialog::progress(QString ID, QString peer, QString fileName, qint64 size, qint64 bytesDone)
{
    if (!transfers[ID]->inProgress)
        return;

    if (transfers[ID]->isDir) {
        //TODO
    }
    transfers[ID]->progress->setMaximum(size);
    transfers[ID]->progress->setValue(bytesDone);
    transfers[ID]->box->setToolTip(QString::number(bytesDone) + " of " + QString::number(size));
}

void FileStatusDialog::fileDone(QString ID)
{
    transfers[ID]->pause->hide();
    transfers[ID]->cancel->hide();
}

void FileStatusDialog::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

