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
#include "ui_filestatusdialog.h"
#include "pointers.h"

class FileStatusDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(FileStatusDialog)
public:
    explicit FileStatusDialog(Pointers *ptr, QWidget *parent = 0);

private:
    Ui::FileStatusDialog m_ui;

    QHash<QString,QGroupBox*> boxList;
    QHash<QString,QProgressBar*>progressList;

    QSignalMapper *pauseMapper;
    QSignalMapper *cancelMapper;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void cancelClicked(QString ID);
    void pauseClicked(QString ID);
    void fileRecieved(QString filename,qint64 size,QString ID,QString username);

public slots:
    void addTransfer(bool isUpload, QString filename, qint64 size, QString ID, QString senderName);
};

#endif // FILESTATUSDIALOG_H
