/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <shaan7in@gmail.com>
    Copyright (C) 2010 Sudhendu Kumar <sudhendu.kumar.roy@gmail.com>

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

#ifndef KAPOTAH_INCOMINGTRANSFER_H
#define KAPOTAH_INCOMINGTRANSFER_H

#include "transfer.h"

#include <QString>

namespace Kapotah
{

    /**
     * \brief Handles an incoming transfer collection
     *
     * Handles <b>one</b> incoming transfer, that is a collection of files which
     * a peer has sent at once (together)
     */
    class IncomingTransfer : public Transfer
    {
            Q_OBJECT

        public:
            explicit IncomingTransfer (QList< TransferFile > files, quint64 totalSize, quint64 numFiles,
                                       quint64 numDirs, QHostAddress peer, QString id, bool isSearchResponse,
                                       QObject* parent = 0);
            virtual void start();
            virtual void stop();
            virtual TransferType type();

        private:
            QString m_destinationDir;
            quint64 m_doneSinceLastSpeedEstimate;
            quint64 m_doneTillLastProgressReport;
            quint64 m_prevTime;
            quint64 m_speed;

        protected slots:
            void startNextFile();
            void currentFileFinished();
            void reportProgress (quint64 done, quint64 size);

        public slots:
            /**
             * Set the destination where the files will be saved
             */
            void setDestinationDir (QString path);

        signals:
            /**
             * Emitted when there is no destination dir defined
             */
            void needDestinationDir();

        protected:
            virtual void timerEvent (QTimerEvent* event);
    };

}

#endif // KAPOTAH_INCOMINGTRANSFER_H
