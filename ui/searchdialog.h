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

#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "ui_searchdialog.h"

#include <QDialog>

namespace Kapotah
{
    class Transfer;
}

using namespace Kapotah;

/** @brief This class displays the search dialog for sharing a folder or for searching for shared files over network
 * 
 * This class displays the search dialog for sharing a folder or for searching for shared files over network.
 * Result of a valid search is displayed on the dialog.
 * For sharing a folder we make index of the files inside the folder reccursively.
*/
class SearchDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit SearchDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual ~SearchDialog();

    private:
        Ui::SearchDialog ui;
        QList<Kapotah::Transfer*> m_transfers;

    private slots:
        
        /** Sends the announcer search pattern from the UI
         */
        void search();
        
        /** Tells FileSearcher to create the index of the directory to be shared and
         * keeps the index to itself.
         */
        void setSharedDir();
        
        /** It appends file names for each search response to the search list.
         */
        void addTransfer(Transfer *transfer);

        /** Starts the transfer
         */

        void startTransfer();
        /** Notify the user that indexing is complete
         */
        void notifyIndexingComplete(const QString& path);

    public slots:

        /**Whenever a new notification for message is shown and the user clicks on the notification,
         * this function shows the ChatDialog along with the new message just arrived.
         */
        void notificationActivated();
};

#endif // SEARCHDIALOG_H


