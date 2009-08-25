/********************************************************************************
** Form generated from reading ui file 'chatdialog.ui'
**
** Created: Wed Aug 26 01:24:31 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CHATDIALOG_H
#define UI_CHATDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatDialog
{
public:
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidget;
    QWidget *tabChat;
    QGridLayout *gridLayout;
    QTextEdit *chatEdit;
    QLineEdit *messageEdit;
    QPushButton *fileButton;
    QPushButton *sendButton;
    QWidget *tabFileTransfer;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QLineEdit *fileNameEdit;
    QLabel *label_2;
    QLineEdit *fileSizeEdit;
    QPushButton *saveFileButton;
    QPushButton *cancelFileButton;
    QProgressBar *fileTransferProgress;
    QSpacerItem *verticalSpacer;
    QLineEdit *IDEdit;

    void setupUi(QDialog *ChatDialog)
    {
        if (ChatDialog->objectName().isEmpty())
            ChatDialog->setObjectName(QString::fromUtf8("ChatDialog"));
        ChatDialog->resize(453, 310);
        gridLayout_2 = new QGridLayout(ChatDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tabWidget = new QTabWidget(ChatDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabChat = new QWidget();
        tabChat->setObjectName(QString::fromUtf8("tabChat"));
        gridLayout = new QGridLayout(tabChat);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        chatEdit = new QTextEdit(tabChat);
        chatEdit->setObjectName(QString::fromUtf8("chatEdit"));
        chatEdit->setFocusPolicy(Qt::NoFocus);
        chatEdit->setReadOnly(true);

        gridLayout->addWidget(chatEdit, 0, 0, 1, 4);

        messageEdit = new QLineEdit(tabChat);
        messageEdit->setObjectName(QString::fromUtf8("messageEdit"));

        gridLayout->addWidget(messageEdit, 1, 0, 1, 1);

        fileButton = new QPushButton(tabChat);
        fileButton->setObjectName(QString::fromUtf8("fileButton"));
        fileButton->setAutoDefault(false);

        gridLayout->addWidget(fileButton, 1, 2, 1, 1);

        sendButton = new QPushButton(tabChat);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        sendButton->setAutoDefault(true);
        sendButton->setDefault(true);

        gridLayout->addWidget(sendButton, 1, 1, 1, 1);

        tabWidget->addTab(tabChat, QString());
        tabFileTransfer = new QWidget();
        tabFileTransfer->setObjectName(QString::fromUtf8("tabFileTransfer"));
        gridLayout_3 = new QGridLayout(tabFileTransfer);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label = new QLabel(tabFileTransfer);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        fileNameEdit = new QLineEdit(tabFileTransfer);
        fileNameEdit->setObjectName(QString::fromUtf8("fileNameEdit"));
        fileNameEdit->setReadOnly(true);

        gridLayout_3->addWidget(fileNameEdit, 0, 1, 1, 4);

        label_2 = new QLabel(tabFileTransfer);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        fileSizeEdit = new QLineEdit(tabFileTransfer);
        fileSizeEdit->setObjectName(QString::fromUtf8("fileSizeEdit"));
        fileSizeEdit->setReadOnly(true);

        gridLayout_3->addWidget(fileSizeEdit, 1, 1, 1, 1);

        saveFileButton = new QPushButton(tabFileTransfer);
        saveFileButton->setObjectName(QString::fromUtf8("saveFileButton"));

        gridLayout_3->addWidget(saveFileButton, 1, 4, 1, 1);

        cancelFileButton = new QPushButton(tabFileTransfer);
        cancelFileButton->setObjectName(QString::fromUtf8("cancelFileButton"));

        gridLayout_3->addWidget(cancelFileButton, 1, 3, 1, 1);

        fileTransferProgress = new QProgressBar(tabFileTransfer);
        fileTransferProgress->setObjectName(QString::fromUtf8("fileTransferProgress"));
        fileTransferProgress->setValue(0);

        gridLayout_3->addWidget(fileTransferProgress, 2, 0, 1, 5);

        verticalSpacer = new QSpacerItem(20, 172, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 3, 1, 1, 1);

        IDEdit = new QLineEdit(tabFileTransfer);
        IDEdit->setObjectName(QString::fromUtf8("IDEdit"));

        gridLayout_3->addWidget(IDEdit, 1, 2, 1, 1);

        tabWidget->addTab(tabFileTransfer, QString());

        gridLayout_2->addWidget(tabWidget, 1, 0, 1, 1);

        QWidget::setTabOrder(tabWidget, messageEdit);
        QWidget::setTabOrder(messageEdit, sendButton);
        QWidget::setTabOrder(sendButton, fileButton);
        QWidget::setTabOrder(fileButton, fileSizeEdit);
        QWidget::setTabOrder(fileSizeEdit, saveFileButton);
        QWidget::setTabOrder(saveFileButton, cancelFileButton);
        QWidget::setTabOrder(cancelFileButton, IDEdit);
        QWidget::setTabOrder(IDEdit, fileNameEdit);

        retranslateUi(ChatDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ChatDialog);
    } // setupUi

    void retranslateUi(QDialog *ChatDialog)
    {
        ChatDialog->setWindowTitle(QApplication::translate("ChatDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        fileButton->setText(QApplication::translate("ChatDialog", "File", 0, QApplication::UnicodeUTF8));
        sendButton->setText(QApplication::translate("ChatDialog", "&Send", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabChat), QApplication::translate("ChatDialog", "Chat", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ChatDialog", "Name", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ChatDialog", "Size", 0, QApplication::UnicodeUTF8));
        saveFileButton->setText(QApplication::translate("ChatDialog", "Save", 0, QApplication::UnicodeUTF8));
        cancelFileButton->setText(QApplication::translate("ChatDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabFileTransfer), QApplication::translate("ChatDialog", "File Transfer", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ChatDialog);
    } // retranslateUi

};

namespace Ui {
    class ChatDialog: public Ui_ChatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATDIALOG_H
