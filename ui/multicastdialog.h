/*
    This file is part of the Kapotah project.
    Copyright (C) 2011 Shantanu Tushar <shaan7in@gmail.com>
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

#ifndef MULTICASTDIALOG_H
#define MULTICASTDIALOG_H

#include "ui_multicastdialog.h"

#include <QDialog>

namespace Kapotah
{
    struct TransferFile;
}

/** @brief Displays a dialog for multicasting messages/files/folders over the network
 * 
 * It displays a dialog for multicasting messages/files/folders over the network
 * by selecting as many peers user wants from the list of peers.
*/
class MulticastDialog : public QDialog
{
        Q_OBJECT
    
    public:
        explicit MulticastDialog (QWidget* parent = 0, Qt::WindowFlags f = 0);
        
    private:
        Ui::MulticastDialog ui;
        
        /** We have considered folders to be list of files.So, this funtion takes
         * parameter fileList, and for each peer selected, sends the type of transfer i.e. outgoing,
         * fileList, address of peer to the TransferManager. Then tells the TransferManager to
         * start transfer.
         * 
         * @param fileList list of files to be transfered.
         */
        void sendFiles(QList<Kapotah::TransferFile> fileList);
    
    private slots:
        void sendMessage();
        void sendFileNeedsSourceDir();
    
    protected:
        bool eventFilter (QObject *obj, QEvent *event);
        virtual void dragEnterEvent (QDragEnterEvent *event);
        virtual void dragMoveEvent (QDragMoveEvent *event);
        virtual void dragLeaveEvent (QDragLeaveEvent*);
        virtual void dropEvent (QDropEvent *event);
               
    friend class PeerDialog;
};

#endif // MULTICASTDIALOG_H
