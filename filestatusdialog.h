#ifndef FILESTATUSDIALOG_H
#define FILESTATUSDIALOG_H

#include "ui_filestatusdialog.h"

class FileStatusDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(FileStatusDialog)
public:
    explicit FileStatusDialog(QWidget *parent = 0);

private:
    Ui::FileStatusDialog m_ui;

public slots:
    void addDownload();
};

#endif // FILESTATUSDIALOG_H
