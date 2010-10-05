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

#ifndef PEERINFO_H
#define PEERINFO_H

#include <QString>
#include <QHostAddress>

class PeerInfo
{
private:
    QString m_name;
    QHostAddress m_ipAddress;
    int m_age;
public:
    PeerInfo()  {;}
    PeerInfo(QString name)  {   m_name = name;  }
    PeerInfo(QString name, QHostAddress ipAddress)  {   m_name = name; m_ipAddress = ipAddress; }
    QString name()  {   return m_name;  }
    QHostAddress ipAddress() {   return m_ipAddress; }
    int age()   {   return m_age;   }
    void setAge(int age) {   m_age = age;    }
    void incrementAge() {   m_age++;    }
    bool operator==(PeerInfo peer);
};

#endif // PEERINFO_H
