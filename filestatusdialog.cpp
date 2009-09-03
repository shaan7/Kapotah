#include "filestatusdialog.h"
#include "server.h"
#include <QDebug>
#include <QCloseEvent>

FileStatusDialog::FileStatusDialog(Pointers *ptr, QWidget *parent) :
    QDialog(parent){
    m_ui.setupUi(this);
    pauseMapper = new QSignalMapper(this);
    cancelMapper = new QSignalMapper(this);

    connect(cancelMapper, SIGNAL(mapped(QString)), this, SLOT(cancelClicked(QString)));
    connect(pauseMapper, SIGNAL(mapped(QString)), this, SLOT(pauseClicked(QString)));

    connect(ptr->server, SIGNAL(fileRecieved(QString,qint64,QString,QString)), this, SLOT(fileRecieved(QString,qint64,QString,QString)));
    qDebug() << "connected";
}

void FileStatusDialog::fileRecieved(QString filename,qint64 size,QString ID,QString username)
{
    show();
    addTransfer(false, filename, size, ID, username);
}

void FileStatusDialog::addTransfer(bool isUpload, QString filename, qint64 size, QString ID, QString senderName)
{
    //Create the new widgets, similar to existing ones
    QGroupBox *box = new QGroupBox(m_ui.scrollArea);
    QGridLayout *layout = new QGridLayout();
    QProgressBar *progress = new QProgressBar();
    QToolButton *pause = new QToolButton();
    QToolButton *cancel = new QToolButton();
    QToolButton *type = new QToolButton();

    //Store widgets
    boxList[ID] = box;
    progressList[ID] = progress;

    //Set the widgets

    if (isUpload) {
        type->setArrowType(Qt::UpArrow);
        type->setToolTip("This Is A Upload");
    }
    else {
        type->setArrowType(Qt::DownArrow);
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
    qDebug() << "CANCEL " << ID;
}

void FileStatusDialog::pauseClicked(QString ID)
{
    qDebug() << "PAUSE " << ID;
}

void FileStatusDialog::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

