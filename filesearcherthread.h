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

#ifndef FILESEARCHERTHREAD_H
#define FILESEARCHERTHREAD_H

#include <QThread>
#include "announcer.h"
#include "peer.h"

class QFileSystemModel;

namespace Kapotah
{
    class FileSearcher;
}

class FileSearcherThread : public QThread
{
public:
    explicit FileSearcherThread ( QFileSystemModel &model, const QString &pattern, const Kapotah::Peer &peer,
                                  Kapotah::FileSearcher &fileSearcher, QObject* parent = 0);
protected:
    virtual void run();

private:
    QFileSystemModel &m_model;
    QString m_pattern;
    Kapotah::Peer m_peer;
    Kapotah::FileSearcher &m_fileSearcher;
};

#endif // FILESEARCHERTHREAD_H
