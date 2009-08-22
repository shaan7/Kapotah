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
