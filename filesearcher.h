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


#ifndef KAPOTAH_FILESEARCHER_H
#define KAPOTAH_FILESEARCHER_H

#include "singleton.h"
#include <QFileSystemModel>
#include "peer.h"
#include <QQueue>

class FileSearcherThread;

namespace Kapotah
{

struct TransferFile;

/**
 * \brief       This class handles the file search requests and processing
 *
 * The class does the intial indexing for files, and
 * uses FileSearcherThread to process searches received from
 * other peers
 *
 * \note At any given time, <b>only one</b> search is in progress,
 * this is done for performance reasons.
 */
class FileSearcher : public Singleton<FileSearcher>
{
    Q_OBJECT

public:
    /**
     * Set the path which should be searched when a request is recieved
     *
     * @param   searchPath      path of local directory
     */
    void setSearchPath (const QString& searchPath);

    /**
     * Returns the current path which should be searched when a request is recieved
     *
     * @return  path of local directory
     */
    QString searchPath() const;

    /**
     * Used internally by FileSearcherThread to report results
     *
     * @param   matches         the files that match the pattern
     * @param   address         the peer to whom the matches should be sent
     */
    void setMatchingFiles (QList<TransferFile> matches, QHostAddress address);

    FileSearcher();

private:
    class SearchItem
    {
    public:
        SearchItem (const QString& pattern, const QHostAddress &host) : pattern (pattern), host (host) { }
        QString pattern;
        QHostAddress host;
    };

    QFileSystemModel m_fsModel;
    QQueue<SearchItem> m_queue;
    QString m_searchPath;
    FileSearcherThread* m_thread;
    bool m_isProcessing;
    QStringList m_pendingDirs;
    QList<TransferFile> m_matches;
    QHostAddress m_hostToSendMatches;

    void processIndex (QModelIndex index);

private slots:
    void threadDone();
    void doScheduling();
    void initFileSystemModel();
    void directoryLoaded (const QString& path);

signals:
    /**
     * Emitted when the initial indexing of searchPath is complete
     *
     * @param   path    the local directory path
     */
    void initDone(const QString &path);

public slots:
    /**
     * Add a search to the search queue
     *
     * @param   pattern         Search pattern
     * @param   host    peer who requested the search
     */
    void addSearch (const QString& pattern, const QHostAddress& host);

};
}

#endif  //KAPOTAH_FILESEARCHER_H
