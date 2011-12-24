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

#ifndef TRANSFERTHREAD_H
#define TRANSFERTHREAD_H

#include <QThread>
#include <QHostAddress>

/**
 * \brief Abstract class for deriving Transfer threads
 *
 * This class serves as an abstract base class for implementing
 * transfer threads and provides suggestions for must-have methods
 */
class TransferThread : public QThread
{
        Q_OBJECT

    public:
        explicit TransferThread (QHostAddress ip, QObject* parent = 0);

    protected:
        /**
         * The method which runs in the thread.
         * \note Must be impelemented in derived classes
         */
        virtual void run() = 0;
        QHostAddress m_ip;

    public slots:
        /**
         * Method which is supposed to stop the transfer
         * \note Must be impelemented in derived classes
         */
        virtual void stopTransfer() = 0;

    signals:
        /**
         * Emitted when a progress is made in this thread
         *
         * @param       done    number of bytes done
         * @param       total   total number of bytes
         */
        void progress (quint64 done, quint64 total);

        /**
         * Emitted when the transfer finishes execution
         */
        void done ();
};

#endif // TRANSFERTHREAD_H
