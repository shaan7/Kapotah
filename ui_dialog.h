/********************************************************************************
** Form generated from reading ui file 'dialog.ui'
**
** Created: Tue Aug 25 18:12:04 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogClass
{
public:
    QGridLayout *gridLayout;
    QListWidget *peerList;
    QPushButton *refreshButton;
    QLineEdit *updateNumOfPeers;

    void setupUi(QDialog *DialogClass)
    {
        if (DialogClass->objectName().isEmpty())
            DialogClass->setObjectName(QString::fromUtf8("DialogClass"));
        DialogClass->resize(242, 495);
        gridLayout = new QGridLayout(DialogClass);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        peerList = new QListWidget(DialogClass);
        peerList->setObjectName(QString::fromUtf8("peerList"));

        gridLayout->addWidget(peerList, 0, 0, 1, 2);

        refreshButton = new QPushButton(DialogClass);
        refreshButton->setObjectName(QString::fromUtf8("refreshButton"));

        gridLayout->addWidget(refreshButton, 1, 0, 1, 1);

        updateNumOfPeers = new QLineEdit(DialogClass);
        updateNumOfPeers->setObjectName(QString::fromUtf8("updateNumOfPeers"));
        updateNumOfPeers->setReadOnly(true);

        gridLayout->addWidget(updateNumOfPeers, 1, 1, 1, 1);

        QWidget::setTabOrder(peerList, refreshButton);

        retranslateUi(DialogClass);

        QMetaObject::connectSlotsByName(DialogClass);
    } // setupUi

    void retranslateUi(QDialog *DialogClass)
    {
        DialogClass->setWindowTitle(QApplication::translate("DialogClass", "Peers", 0, QApplication::UnicodeUTF8));
        refreshButton->setText(QApplication::translate("DialogClass", "&Activate", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DialogClass);
    } // retranslateUi

};

namespace Ui {
    class DialogClass: public Ui_DialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
