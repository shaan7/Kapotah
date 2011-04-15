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

class FileSearcher : public Singleton<FileSearcher>
{
    Q_OBJECT

public:
    void setSearchPath (const QString& searchPath);
    QString searchPath() const;

    void setMatchingFiles (QList<TransferFile> matches, QHostAddress address);

    FileSearcher();
    virtual ~FileSearcher();

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
    void initDone();

public slots:
    void addSearch (const QString& pattern, const QHostAddress& host);

};
}

#endif  //KAPOTAH_FILESEARCHER_H
