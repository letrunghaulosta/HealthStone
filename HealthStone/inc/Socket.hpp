#ifndef _SOCKET_HPP_INC_
#define _SOCKET_HPP_INC_

#include <sys/socket.h>
#include <cstdint>
#include <list>

class Socket
{
public:
    enum GameMessageType : uint8_t
    {
        HERO_LIST_ID_P1 = 1,
        HERO_LIST_ID_P2,
        CARD_ON_HAND_LIST_ID_P1,
        CARD_ON_HAND_LIST_ID_P2,
        CARD_ON_TABLE_LIST_ID_P1,
        CARD_ON_TABLE_LIST_ID_P2,
        UNIT_ON_BUFF_P1,
        UNIT_ON_BUFF_P2,
        CLIENT_REQUEST_UNIT_INFO,
        INFORM_WINNER,
        DONE_MESSAGE,
        MESSAGE_LENGTH,
        ACK_KNOWLEDGE,
        CLIENT_PLACE_CARD,
        READY_MESSAGE
    };
    
    struct SockMessage
    {
        GameMessageType messageType;
        uint8_t buffer[1022];
    };

    Socket(){
        socket_fd = -1;
        client_fd = -1;
    }
    ~Socket()
    {
        Close();
    }
    void CreateSocket();
    void Connect();
    void Receive(SockMessage*, uint16_t*);
    void Send(SockMessage*,uint16_t);
    void Close();
    static Socket* GetInstance(){return socketInstance;}
private:
    static Socket *socketInstance;
    int socket_fd;
    int client_fd;
};

#endif