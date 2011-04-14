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

#include "filesearcher.h"

#include "filesearcherthread.h"

using namespace Kapotah;

template<> FileSearcher *Kapotah::Singleton<FileSearcher>::m_instance = 0;

FileSearcher::FileSearcher() : m_isProcessing(true)
{
    connect(&m_fsModel, SIGNAL(directoryLoaded(QString)), SLOT(directoryLoaded(QString)));
}

FileSearcher::~FileSearcher()
{

}

void FileSearcher::addSearch (const QString& pattern, const Peer& peer)
{
    m_queue.enqueue(SearchItem(pattern, peer));
    doScheduling();
}

QString FileSearcher::searchPath() const
{
    return m_searchPath;
}

void FileSearcher::setSearchPath (const QString& searchPath)
{
    m_searchPath = searchPath;
    initFileSystemModel();
}

void FileSearcher::doScheduling()
{
    if (m_queue.length() == 0)
        return;

    if (m_isProcessing)
        return;


    SearchItem item = m_queue.dequeue();
    m_thread = new FileSearcherThread(m_fsModel, item.pattern, item.peer, *this);
    connect(m_thread, SIGNAL(finished()), SLOT(threadDone()));
    m_isProcessing = true;
    m_thread->start();
}

void FileSearcher::threadDone()
{
    m_thread->quit();
    m_thread->wait();
    m_isProcessing = false;
    disconnect(m_thread);
    delete m_thread;

    doScheduling();
}

void FileSearcher::initFileSystemModel()
{
    qDebug() << "INITIALIZING " << m_searchPath;
    m_fsModel.setRootPath(m_searchPath);
}

void FileSearcher::directoryLoaded (const QString& path)
{
    qDebug() << path << " LOADED";
    m_pendingDirs.removeOne(path);
    processIndex(m_fsModel.index(path));

    if (m_pendingDirs.length()==0) {
        emit initDone();
    }
}

void FileSearcher::processIndex (QModelIndex index)
{
    qDebug() << index.data();

    if (m_fsModel.hasChildren(index)) {
        if (m_fsModel.rowCount(index) == 0) {     //Not yet fetched
            if (m_fsModel.canFetchMore(index)) {
                m_pendingDirs.append(m_fsModel.filePath(index));
                m_fsModel.fetchMore(index);
            }
        } else {
            for (int i=0; i<m_fsModel.rowCount(index); i++) {
                processIndex(m_fsModel.index(i, 0, index));
            }
        }
    }
}

#include "filesearcher.moc"