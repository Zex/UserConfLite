#include "translate.h"

std::string get_family_name(int fa)
{
    switch (fa)
    {
        case PF_UNSPEC: return "Unspecified.";
        case PF_LOCAL: return "Local to host (pipes and file-domain).";
//        case PF_UNIX: return "POSIX name for PF_LOCAL.";
//        case PF_FILE: return "Another non-standard name for PF_LOCAL."
        case PF_INET: return "IP protocol family.";
        case PF_AX25: return "Amateur Radio AX.25.";
        case PF_IPX: return "Novell Internet Protocol.";
        case PF_APPLETALK: return "Appletalk DDP.";
        case PF_NETROM: return "Amateur radio NetROM.";
        case PF_BRIDGE: return "Multiprotocol bridge.";
        case PF_ATMPVC: return "ATM PVCs.";
        case PF_X25: return "Reserved for X.25 project.";
        case PF_INET6: return "IP version 6.";
        case PF_ROSE: return "Amateur Radio X.25 PLP.";
        case PF_DECnet: return "Reserved for DECnet project.";
        case PF_NETBEUI: return "Reserved for 802.2LLC project.";
        case PF_SECURITY: return "Security callback pseudo AF.";
        case PF_KEY: return "PF_KEY key management API.";
//        case PF_NETLINK:
        case PF_ROUTE: return "Alias to emulate 4.4BSD.";
        case PF_PACKET: return "Packet family.";
        case PF_ASH: return "Ash.";
        case PF_ECONET: return "Acorn Econet.";
        case PF_ATMSVC: return "ATM SVCs.";
        case PF_RDS: return "RDS sockets.";
        case PF_SNA: return "Linux SNA Project.";
        case PF_IRDA: return "IRDA sockets.";
        case PF_PPPOX: return "PPPoX sockets.";
        case PF_WANPIPE: return "Wanpipe API sockets.";
        case PF_LLC: return "Linux LLC.";
        case PF_CAN: return "Controller Area Network.";
        case PF_TIPC: return "TIPC sockets.";
        case PF_BLUETOOTH: return "Bluetooth sockets.";
        case PF_IUCV: return "IUCV sockets.";
        case PF_RXRPC: return "RxRPC sockets.";
        case PF_ISDN: return "mISDN sockets.";
        case PF_PHONET: return "Phonet sockets.";
        case PF_IEEE802154: return "IEEE 802.15.4 sockets.";
        case PF_CAIF: return "CAIF sockets.";
        case PF_ALG: return "Algorithm sockets.";
        case PF_MAX: return "PF_MAX";
        default: return "UNKNOWN";
    }
}

std::string get_sock_level(int so)
{
    switch (so)
    {
        case SOL_RAW: return "SOL_RAW";
        case SOL_DECNET: return "SOL_DECNET";
        case SOL_X25: return "SOL_X25";
        case SOL_PACKET: return "SOL_PACKET";
        case SOL_ATM: return "SOL_ATM";
        case SOL_AAL: return "SOL_AAL";
        case SOL_IRDA: return "SOL_IRDA";
        default: return "UNKNOWN";
    }
}

std::string get_sock_type(int so)
{
    std::string ret;

    if (so & SOCK_STREAM) ret.append("SOCK_STREAM,");
    if (so & SOCK_DGRAM) ret.append("SOCK_DGRAM,");
    if (so & SOCK_RAW) ret.append("SOCK_RAW,");
    if (so & SOCK_RDM) ret.append("SOCK_RDM,");
    if (so & SOCK_SEQPACKET) ret.append("SOCK_SEQPACKET,");
    if (so & SOCK_DCCP) ret.append("SOCK_DCCP,");
    if (so & SOCK_PACKET) ret.append("SOCK_PACKET,");
    if (so & SOCK_CLOEXEC) ret.append("SOCK_CLOEXEC,");
    if (so & SOCK_NONBLOCK) ret.append("SOCK_NONBLOCK,");

    if (ret.at(ret.size()-1) == ',')
        ret = ret.substr(0, ret.size()-1);

    return ret;
}

std::string get_ai_flag(int f)
{
    std::string ret;

    if (f & AI_PASSIVE) ret.append("AI_PASSIVE,");
    if (f & AI_CANONNAME) ret.append("AI_CANONNAME,");
    if (f & AI_NUMERICHOST) ret.append("AI_NUMERICHOST,");
    if (f & AI_V4MAPPED) ret.append("AI_V4MAPPED,");
    if (f & AI_ALL) ret.append("AI_ALL,");
    if (f & AI_ADDRCONFIG) ret.append("AI_ADDRCONFIG,");
# ifdef __USE_GNU
    if (f & AI_IDN) ret.append("AI_IDN,");
    if (f & AI_CANONIDN) ret.append("AI_CANONIDN,");
    if (f & AI_IDN_ALLOW_UNASSIGNED) ret.append("AI_IDN_ALLOW_UNASSIGNED,");
    if (f & AI_IDN_USE_STD3_ASCII_RULES) ret.append("AI_IDN_USE_STD3_ASCII_RULES,");
# endif
    if (f & AI_NUMERICSERV) ret.append("AI_NUMERICSERV,");

    if (ret.at(ret.size()-1) == ',')
        ret = ret.substr(0, ret.size()-1);

    return ret;
}


std::string get_utmp_type(int t)
{
    std::string ret;

    switch (t)
    {
        case EMPTY        : return "EMPTY";
        case RUN_LVL      : return "RUN_LVL";
        case BOOT_TIME    : return "BOOT_TIME";
        case NEW_TIME     : return "NEW_TIME";
        case OLD_TIME     : return "OLD_TIME";
        case INIT_PROCESS : return "INIT_PROCESS";
        case LOGIN_PROCESS: return "LOGIN_PROCESS";
        case USER_PROCESS : return "USER_PROCESS";
        case DEAD_PROCESS : return "DEAD_PROCESS";
        case ACCOUNTING   : return "ACCOUNTING";
        default: return "UNKNOWN";
    }

    return ret;

}


std::string get_inotify_mask(uint32_t m)
{
    std::string ret;

    if (m & IN_ACCESS) ret.append("IN_ACCESS,");
    if (m & IN_MODIFY) ret.append("IN_MODIFY,");
    if (m & IN_ATTRIB) ret.append("IN_ATTRIB,");
    if (m & IN_CLOSE_WRITE) ret.append("IN_CLOSE_WRITE,");
    if (m & IN_CLOSE_NOWRITE) ret.append("IN_CLOSE_NOWRITE,");
    if (m & IN_OPEN) ret.append("IN_OPEN,");
    if (m & IN_MOVED_FROM) ret.append("IN_MOVED_FROM,");
    if (m & IN_MOVED_TO) ret.append("IN_MOVED_TO,");
    if (m & IN_CREATE) ret.append("IN_CREATE,");
    if (m & IN_DELETE) ret.append("IN_DELETE,");
    if (m & IN_DELETE_SELF) ret.append("IN_DELETE_SELF,");
    if (m & IN_MOVE_SELF) ret.append("IN_MOVE_SELF,");

    if (ret.at(ret.size()-1) == ',')
        ret = ret.substr(0, ret.size()-1);

    return ret;
}

