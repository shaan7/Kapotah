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

#include "filesearcherthread.h"

#include "filesearcher.h"
#include "transfermanager.h"
#include <QFileSystemModel>

#include <QDebug>

using namespace Kapotah;

FileSearcherThread::FileSearcherThread (QFileSystemModel& model, const QString& pattern,
                                        const QHostAddress &host, FileSearcher &fileSearcher,
                                        QObject* parent)
                                        : m_model(model), m_pattern(pattern), m_host(host),
                                        m_fileSearcher(fileSearcher), QThread (parent)
{

}

void FileSearcherThread::run()
{
    traverseModelIndex(m_model.index(m_model.rootPath()));

    if (!m_matches.empty()) {
        QList<TransferFile> fileList;

        foreach (QString path, m_matches) {
            TransferFile file;
            file.path = path;
            fileList.append(file);
            qDebug() << "MATCH " << file.path << " SIZE " << file.path;
        }

        m_fileSearcher.setMatchingFiles(fileList, m_host);
    }
}

void FileSearcherThread::traverseModelIndex (QModelIndex index)
{
    if (m_model.fileInfo(index).isDir()) {
        for (int i=0; i<m_model.rowCount(index); i++) {
            traverseModelIndex(m_model.index(i, 0, index));
        }
    } else {
        if (index.data().toString().contains(m_pattern, Qt::CaseInsensitive)) {
            m_matches.append(m_model.filePath(index));
        }
    }
}

FileSearcherThread::~FileSearcherThread()
{

}


#include "filesearcherthread.moc"
