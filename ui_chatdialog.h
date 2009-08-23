/********************************************************************************
** Form generated from reading ui file 'chatdialog.ui'
**
** Created: Sun Aug 23 09:33:02 2009
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
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ChatDialog
{
public:
    QGridLayout *gridLayout;
    QTextEdit *chatEdit;
    QLineEdit *messageEdit;
    QPushButton *sendButton;

    void setupUi(QDialog *ChatDialog)
    {
        if (ChatDialog->objectName().isEmpty())
            ChatDialog->setObjectName(QString::fromUtf8("ChatDialog"));
        ChatDialog->resize(400, 300);
        gridLayout = new QGridLayout(ChatDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        chatEdit = new QTextEdit(ChatDialog);
        chatEdit->setObjectName(QString::fromUtf8("chatEdit"));
        chatEdit->setFocusPolicy(Qt::NoFocus);
        chatEdit->setReadOnly(true);

        gridLayout->addWidget(chatEdit, 0, 0, 1, 2);

        messageEdit = new QLineEdit(ChatDialog);
        messageEdit->setObjectName(QString::fromUtf8("messageEdit"));

        gridLayout->addWidget(messageEdit, 1, 0, 1, 1);

        sendButton = new QPushButton(ChatDialog);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        sendButton->setAutoDefault(false);
        sendButton->setDefault(true);

        gridLayout->addWidget(sendButton, 1, 1, 1, 1);


        retranslateUi(ChatDialog);

        QMetaObject::connectSlotsByName(ChatDialog);
    } // setupUi

    void retranslateUi(QDialog *ChatDialog)
    {
        ChatDialog->setWindowTitle(QApplication::translate("ChatDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        sendButton->setText(QApplication::translate("ChatDialog", "&Send", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ChatDialog);
    } // retranslateUi

};

namespace Ui {
    class ChatDialog: public Ui_ChatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATDIALOG_H
