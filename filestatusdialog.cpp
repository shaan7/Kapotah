/***************************************************************************
 *   Copyright 2009 Shantanu Tushar <jhahoneyk@gmail.com>                  *
 *   Copyright 2009 Sudhendu Roy <sudhendu_roy@yahoo.co.in>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "filestatusdialog.h"
#include "server.h"
#include "fileserver.h"
#include <QDebug>
#include <QCloseEvent>
#include <QFileDialog>
#include <QCryptographicHash>
#include <QFileInfo>

//THIS FILE IS TOOOOOOOOOOOO LARGE, IT WILL BE MADE SMALLER
//ASSIGN PARENTS TO WIDGETS

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

    connect(ptr->server, SIGNAL(fileRecieved(QString,qint64,QString,QHostAddress)), this, SLOT(fileRecieved(QString,qint64,QString,QHostAddress)));
    connect(ptr->server, SIGNAL(dirRecieved(QDomNodeList,QDomNodeList,QHostAddress)), this, SLOT(dirRecieved(QDomNodeList,QDomNodeList,QHostAddress)));
    connect(ptr->fserver, SIGNAL(startedTransfer(QString,QString)), this, SLOT(fileSendStarted(QString,QString)));
    connect(ptr->fserver, SIGNAL(finishedTransfer(QString,QString)), this, SLOT(fileSendFinished(QString,QString)));
}

QString FileStatusDialog::returnToolTipHTML(QString title, QString description)
{
    return QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">") +
        QString("<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }") +
        QString("</style></head><body style=\" font-family:'DejaVu Sans'; font-size:9pt; font-weight:400; font-style:normal;\">") +
        QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">") +
        QString("<span style=\" font-size:11pt; font-weight:600; text-decoration: underline; color:#00007f;\">" + title + "</span></p>") +
        QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">") +
        description + QString("</p></body></html>");
}

void FileStatusDialog::fileSendStarted(QString ID, QString filename)
{
    if (upFileTransfers.contains(ID))
        return;
    if (ID.left(3) == "DIR")
        return;

    addTransferEntry(ID, QFileInfo(filename).fileName(), QHostAddress("0.0.0.0"), true, false);
}

void FileStatusDialog::fileSendFinished(QString ID, QString filename)
{
    if (ID.left(3) == "DIR")
        return;

    upFileTransfers[ID]->progress->setMaximum(1);
    upFileTransfers[ID]->progress->setValue(1);
}

void FileStatusDialog::dirRecieved(QDomNodeList fileList, QDomNodeList dirList, QHostAddress ipAddress)
{
    show();
    addDirTransfer(fileList, dirList, ipAddress, false);
}

void FileStatusDialog::fileRecieved(QString filename,qint64 size,QString ID, QHostAddress senderIP)
{
    show();
    addFileTransfer(filename, size, ID, senderIP, false);
}

void FileStatusDialog::addTransferEntry(QString ID, QString title, QHostAddress senderIP, bool isUpload, bool isDir)
{
    //Create the new widgets
    QGroupBox *box = new QGroupBox(m_ui.scrollArea);
    QGridLayout *layout = new QGridLayout();
    QProgressBar *progress = new QProgressBar();
    QToolButton *pause;
    QToolButton *cancel;

    if (!isUpload) {    //We don't yet support manual control of uploads
        pause = new QToolButton();
        cancel = new QToolButton();
    }
    QToolButton *type = new QToolButton();

    //Set the widgets

    if (!isUpload) {
        pause->setAutoRaise(true);
        cancel->setAutoRaise(true);
    }
    type->setAutoRaise(true);

    if (isUpload) {
        type->setIcon(QIcon(":/images/arrow-up-double.png"));
        type->setToolTip(returnToolTipHTML("Transfer Type", "This transfer is an upload"));
        progress->setMaximum(0);
        progress->setValue(0);
    }
    else {
        pause->setIcon(QIcon(":/images/kget.png"));
        pause->setToolTip(returnToolTipHTML("Download", "Click here to select a location and download"));
        cancel->setIcon(QIcon(":/images/dialog-cancel.png"));
        cancel->setToolTip(returnToolTipHTML("Cancel", "Cancels the transfer without confirmation"));
        type->setIcon(QIcon(":/images/arrow-down-double.png"));
        type->setToolTip(returnToolTipHTML("Transfer Type", "This transfer is a download"));
    }

    cancel->setEnabled(false);
    //Layout the widgets
    layout->addWidget(progress, 0, 0);
    if (!isUpload) {
        layout->addWidget(pause, 0, 1);
        layout->addWidget(cancel, 0, 2);
        layout->addWidget(type, 0, 3);
        box->setTitle(title.left(25) + " from " + senderIP.toString());
    }
    else {
        layout->addWidget(type, 0, 1);
        box->setTitle(title.left(25) + " to " + senderIP.toString());
    }
    box->setLayout(layout);

    //Add the widgets to the ScrollArea

    //Remove the spacer
    m_ui.scrollAreaWidgetContents->layout()->takeAt(m_ui.scrollAreaWidgetContents->layout()->count()-1);
    //Add the group box
    m_ui.scrollAreaWidgetContents->layout()->addWidget(box);
    //Add a new spacer
    dynamic_cast<QVBoxLayout*>(m_ui.scrollAreaWidgetContents->layout())->addStretch();

    //Store info
    if (isDir) {
        DirTransfer *transfer = new DirTransfer;

        //Store info
        transfer->box = box;
        transfer->progress = progress;
        if (!isUpload) {
            transfer->cancel = cancel;
            transfer->pause = pause;
            dirTransfers[ID] = transfer;
        }
        else
            upDirTransfers[ID] = transfer;
    }
    else {
        FileTransfer *transfer = new FileTransfer;

        //Store info
        transfer->box = box;
        transfer->progress = progress;
        if (!isUpload) {
            transfer->cancel = cancel;
            transfer->pause = pause;
            fileTransfers[ID] = transfer;
        }
        else
            upFileTransfers[ID] = transfer;
    }
}

void FileStatusDialog::addFileTransfer(QString filename, qint64 size, QString ID, QHostAddress senderIP, bool isUpload)
{
    if (fileTransfers.contains(ID))
        return;

    addTransferEntry(ID, filename, senderIP, isUpload, false);

    FileTransfer *transfer = fileTransfers[ID];

    //Store info
    transfer->fileName = filename;
    transfer->inProgress = false;
    transfer->senderIP = senderIP;
    transfer->isUpload = isUpload;
    transfer->fileSize = size;

    //Connect signals
    connect(transfer->cancel, SIGNAL(clicked()), fileCancelMapper, SLOT(map()));
    connect(transfer->pause, SIGNAL(clicked()), filePauseMapper, SLOT(map()));

    //Set mapping
    fileCancelMapper->setMapping(transfer->cancel, ID);
    filePauseMapper->setMapping(transfer->pause, ID);
}

void FileStatusDialog::addDirTransfer(QDomNodeList fileList, QDomNodeList dirList, QHostAddress senderIP, bool isUpload)
{
    qint64 totalSize=0;

    //Calculate ID and total size of dir
    QString items;
    for (int i=0;i<dirList.count();i++) {
        items += dirList.at(i).toElement().attribute("path");  //Arbitrary method
    }

    //Calculate ID for dir
    QString ID(QCryptographicHash::hash(items.toUtf8(), QCryptographicHash::Md5).toHex());

    if (dirTransfers.contains(ID))
        return;

    for (int i=0;i<fileList.count();i++) {
        totalSize += fileList.at(i).toElement().attribute("size").toLongLong();
    }

    addTransferEntry(ID, "Folder", senderIP, isUpload, true);
    DirTransfer *transfer = dirTransfers[ID];

    //Store info
    transfer->inProgress = false;
    transfer->senderIP= senderIP;
    transfer->isUpload = isUpload;

    transfer->fileList = fileList;
    transfer->dirList = dirList;

    transfer->totalSize = totalSize;
    transfer->bytesDone = 0;
    transfer->bytesDoneTillNow = 0;
    transfer->filesDone = 0;
    dirTransfers[ID] = transfer;

    //Set the widgets
    transfer->progress->setMaximum(fileList.count());

    //Connect signals
    connect(transfer->cancel, SIGNAL(clicked()), dirCancelMapper, SLOT(map()));
    connect(transfer->pause, SIGNAL(clicked()), dirPauseMapper, SLOT(map()));

    //Set mapping
    dirCancelMapper->setMapping(transfer->cancel, ID);
    dirPauseMapper->setMapping(transfer->pause, ID);
}

void FileStatusDialog::dirCancelClicked(QString ID)
{
    DirTransfer *transfer = dirTransfers[ID];
    transfer->reciever->stopTransfer();
    transfer->progress->setMaximum(0);
    transfer->progress->setValue(0);
    transfer->inProgress = false;
    transfer->pause->setEnabled(true);
}

void FileStatusDialog::dirPauseClicked(QString ID)
{
    DirTransfer *transfer = dirTransfers[ID];

    QString dirname = QFileDialog::getExistingDirectory(this, "Select a dir");
    if (dirname=="")
        return;

    transfer->pause->setEnabled(false);
    transfer->reciever = new DirRecieverThread(m_ptr, ID, dirname, transfer->fileList, transfer->dirList, transfer->senderIP, this);
    transfer->savePath = dirname;
    connect(transfer->reciever, SIGNAL(progress(QString,int)), this, SLOT(dirProgress(QString,int)));
    connect(transfer->reciever, SIGNAL(done(QString)), this, SLOT(dirDone(QString)));
    //startTime = QDateTime::currentDateTime();
    transfer->inProgress = true;
    transfer->cancel->setEnabled(true);
    transfer->reciever->start();
}

void FileStatusDialog::dirProgress(QString ID, int filesDone)
{
    DirTransfer *transfer = dirTransfers[ID];
    if (!transfer->inProgress)
        return;

    transfer->progress->setValue(filesDone);
    //qDebug() << QString::number(filesDone) + " of " + QString::number(transfer->totalSize);
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
    transfer->pause->setEnabled(true);
}

void FileStatusDialog::filePauseClicked(QString ID)
{
    FileTransfer *transfer = fileTransfers[ID];

    QString filename = QFileDialog::getSaveFileName(this, "Select a file to save",
                                                    QDir::home().absoluteFilePath(transfer->fileName)); //CHANGES
    if (filename=="")
        return;

    transfer->pause->setEnabled(false);
    transfer->savePath = filename;
    transfer->reciever = new FileRecieverThread(m_ptr, ID, transfer->fileSize,
                                                     transfer->senderIP, transfer->savePath, this);
    connect(transfer->reciever, SIGNAL(progress(QString,QHostAddress,QString,qint64,qint64)),
            this, SLOT(fileProgress(QString,QHostAddress,QString,qint64,qint64)));
    connect(transfer->reciever, SIGNAL(done(QString)), this, SLOT(fileDone(QString)));
    //startTime = QDateTime::currentDateTime();
    transfer->inProgress = true;
    transfer->cancel->setEnabled(true);
    transfer->reciever->start();
}

void FileStatusDialog::fileProgress(QString ID, QHostAddress peer, QString fileName, qint64 size, qint64 bytesDone)
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

