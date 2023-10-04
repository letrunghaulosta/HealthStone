#ifndef _SOCKET_HPP_INC_
#define _SOCKET_HPP_INC_

#include <sys/socket.h>
#include <cstdint>
#include <list>

class Socket
{
public:
    enum GameMessageType : uint16_t
    {
        HERO_LIST_ID_P1 = 1,
        HERO_LIST_ID_P2,
        CLIENT_REQUEST_UNIT_INFO,
        CLIENT_REQUEST_ATTACK,
        RESPOND_UNIT_INFO,
        INFORM_WINNER,
        DONE_MESSAGE,
        MESSAGE_LENGTH,
        ACK_KNOWLEDGE,
        CLIENT_PLACE_CARD
    };
    
    struct SockMessage
    {
        GameMessageType messageType;
        uint16_t size;
        uint8_t buffer[1020];
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
    void CopyData(std::list<uint8_t> src, uint8_t* dest, uint16_t* size);
    void Receive(SockMessage*, int);
    void Send(SockMessage*,int);
    void Close();
    static Socket* GetInstance(){return socketInstance;}
private:
    static Socket *socketInstance;
    int socket_fd;
    int client_fd;
};

#endif