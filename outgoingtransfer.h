/*
    This file is part of the Kapotah project.
    Copyright (C) 2010 Shantanu Tushar <jhahoneyk@gmail.com>
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

#ifndef KAPOTAH_OUTGOINGTRANSFER_H
#define KAPOTAH_OUTGOINGTRANSFER_H

#include "transfer.h"

namespace Kapotah
{

    class OutgoingTransfer : public Transfer
    {
            Q_OBJECT

        public:
            enum State { Stopped, GeneratingFileList, SendingFileList, FileListSent, Sending, Done };
            explicit OutgoingTransfer (QList< TransferFile > files, QHostAddress peer,
                                       QObject* parent = 0);
            virtual ~OutgoingTransfer();
            virtual void start();
            virtual TransferType type();

        private:
            State m_state;

        private slots:
            void onThreadStartPreparingList();
            void onThreadStartSendingList();
            void onThreadDoneSendingList();
    };

}

#endif // KAPOTAH_OUTGOINGTRANSFER_H
