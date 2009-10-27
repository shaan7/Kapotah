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
#include <QSystemTrayIcon>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    Pointers ptr;
    Q_INIT_RESOURCE(systray);
    QApplication a(argc, argv);
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    ptr.server = new Server;

    if (!ptr.server->listen( QHostAddress::Any, 9876 )) {
        qDebug() << "SERVER LISTEN FAILED";
        return 2;
    }

    ptr.fserver = new FileServer;

    if (!ptr.fserver->listen( QHostAddress::Any, 9877 )) {
        qDebug() << "FILE SERVER LISTEN FAILED";
        return 2;
    }

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

//////////////////////TODO
//Implement messagesenderthread
//Take a look at all XML formats
//better to create an empty file in filerecieverthread.cpp
//********************* CONFORM TO W3C XML Standards ********************* <<--- IMPORTANT
