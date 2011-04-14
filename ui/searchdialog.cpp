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

#include "searchdialog.h"

#include "announcer.h"
#include "filesearcher.h"
#include <QFileDialog>

SearchDialog::SearchDialog(QWidget* parent, Qt::WindowFlags f) : QDialog (parent, f)
{
    ui.setupUi(this);
    setWindowTitle("Search");
    ui.searchButton->setIcon(QIcon(":/images/search.png"));
    ui.downloadButton->setIcon(QIcon(":/images/download.png"));
    ui.settingsButton->setIcon(QIcon(":/images/folder-development.png"));
    ui.searchButton->setToolTip("Search");
    ui.downloadButton->setToolTip("Download");

    connect(ui.searchButton, SIGNAL(clicked()), SLOT(search()));
    connect(ui.settingsButton, SIGNAL(clicked()), SLOT(setSharedDir()));
}

void SearchDialog::search()
{
    qDebug() << "Searching for " << ui.searchPatternEdit->text();
    Kapotah::Announcer::instance()->searchPeersForFile(ui.searchPatternEdit->text());
}

void SearchDialog::setSharedDir()
{
    QString dirname = QFileDialog::getExistingDirectory(this, "Select a directory to share");
    if (dirname.isEmpty())
        return;
    Kapotah::FileSearcher::instance()->setSearchPath(dirname);
}

#include "searchdialog.moc"