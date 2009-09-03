/***************************************************************************
 *   Copyright 2009 Shantanu Tushar <jhahoneyk@gmail.com>                  *
 *   Copyright 2009 Sudhendu Roy <sudhendu_roy@yahoo.co.in>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include <QtGui/QApplication>
#include "dialog.h"
#include "server.h"
#include "fileserver.h"
#include "pointers.h"
#include "filestatusdialog.h"

int main(int argc, char *argv[])
{
    Pointers ptr;

    QApplication a(argc, argv);

    ptr.server = new Server;

    if (!ptr.server->listen( QHostAddress::Any, 9876 ))
        qDebug() << "SERVER LISTEN FAILED";

    ptr.fserver = new FileServer;

    if (!ptr.fserver->listen( QHostAddress::Any, 9877 ))
        qDebug() << "FILE SERVER LISTEN FAILED";

    ptr.manager = new PeerManager(&ptr);    //Create the peer manager

    ptr.fileStatusDialog =  new FileStatusDialog(&ptr);

    Dialog d(&ptr);
    d.show();
    return a.exec();

    delete ptr.fileStatusDialog;
    delete ptr.manager;
    delete ptr.fserver;
    delete ptr.server;
}
