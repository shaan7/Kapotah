#include "filestatusdialog.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QToolButton>
#include <QProgressBar>
#include <QLabel>

FileStatusDialog::FileStatusDialog(QWidget *parent) :
    QDialog(parent){
    m_ui.setupUi(this);
    addDownload();
}

void FileStatusDialog::addDownload()
{
    //Create the new widgets, similar to existing ones
    QGroupBox *box = new QGroupBox(m_ui.scrollArea);
    QGridLayout *layout = new QGridLayout();
    QProgressBar *progress = new QProgressBar();
    QToolButton *pause = new QToolButton();
    QToolButton *cancel = new QToolButton();

    //Layout the new widgets
    layout->addWidget(progress, 0, 0);
    layout->addWidget(pause, 0, 1);
    layout->addWidget(cancel, 0, 2);
    box->setLayout(layout);
    box->setTitle("Welcome File");

    //Add the widgets to the ScrollArea

    //Remove the spacer
    m_ui.scrollAreaWidgetContents->layout()->takeAt(m_ui.scrollAreaWidgetContents->layout()->count()-1);
    //Add the group box
    m_ui.scrollAreaWidgetContents->layout()->addWidget(box);
    //Add a new spacer
    dynamic_cast<QVBoxLayout*>(m_ui.scrollAreaWidgetContents->layout())->addStretch();

    connect(cancel, SIGNAL(clicked()), this, SLOT(addDownload()));
}
