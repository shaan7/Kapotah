#include "peerinfo.h"

bool PeerInfo::operator==(PeerInfo peer)
{
    if (peer.ipAddress() == m_ipAddress && peer.name() == m_name)
        return true;
    else
        return false;
}
