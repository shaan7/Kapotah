/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <jhahoneyk@gmail.com>
    Copyright (C) 2011 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TRANSFERWIDGET_H
#define TRANSFERWIDGET_H

#include <QWidget>

namespace Kapotah
{
    class Transfer;
    class IncomingTransfer;
    class OutgoingTransfer;
}

class QVBoxLayout;

class QHBoxLayout;

class QLabel;

class QProgressBar;

class QToolButton;

class TransferWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit TransferWidget (Kapotah::Transfer* transfer, QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual ~TransferWidget();

    private:
        QLabel *m_peerLabel;
        QProgressBar *m_progress;
        QToolButton *m_startStop;
        QLabel *m_progressLabel;
        QVBoxLayout *m_verticalLayout;
        QHBoxLayout *m_horizontalLayout;
        Kapotah::Transfer *m_transfer;
        Kapotah::IncomingTransfer *m_incomingTransfer;
        Kapotah::OutgoingTransfer *m_outgoingTransfer;

    private slots:
        void startTransfer();
        void transferFinished();
        void transferNeedsDestinationDir();
        void updateProgress(quint64 done, quint64 total, quint32 speed);
};

#endif // TRANSFERWIDGET_H
