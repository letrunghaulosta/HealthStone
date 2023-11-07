#include "Socket.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <list>

#define PORT 8080

Socket* Socket::socketInstance = new Socket();

void Socket::CreateSocket()
{
    int valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // Forcefully attaching socket to the port 8080
    if (setsockopt(socket_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
  
    // Forcefully attaching socket to the port 8080
    if (bind(socket_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(socket_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((client_fd
         = accept(socket_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
}

void Socket::Connect()
{
    int status, valread;
    struct sockaddr_in serv_addr;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
  
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return;
    }
  
    if ((status
         = ::connect(socket_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return;
    }
}

void Socket::Receive(SockMessage* message, uint16_t* length)
{
    int new_sock = client_fd < 0?socket_fd:client_fd;
    memset(message,0,255);
    *length = 0;
    recv(new_sock, message, 3, 0);
    if(message->messageType == MESSAGE_LENGTH)
    {
        *length = message->buffer[0] | message->buffer[1] << 8;
        recv(new_sock, message, *length + 1, 0);
    }

    // std::cout << "#####################" << std::endl;
    // std::cout << "#RECV, ID: " << (int)message->messageType << " ,LEN: " << (int)*length << "#" << std::endl;
    // std::cout << "#####################" << std::endl;
}

void Socket::Send(SockMessage* message, uint16_t _size)
{
    int new_sock = client_fd < 0?socket_fd:client_fd;
    
    // std::cout << "#####################" << std::endl;
    // std::cout << "#SEND, ID: " << (int)message->messageType << " ,LEN: " << (int)_size << "#" << std::endl;
    // std::cout << "#####################" << std::endl;

    if(message->messageType != DONE_MESSAGE)
    {
        Socket::SockMessage tempMessage;
        tempMessage.messageType = Socket::MESSAGE_LENGTH;
        tempMessage.buffer[0] = _size&0xff;
        tempMessage.buffer[1] = (_size >> 8)&0xff;
        send(new_sock,&tempMessage,3,0);
    }
    send(new_sock, message, _size + 1, 0);
    memset(message, 0 ,255);
}

void Socket::Close()
{
    if(client_fd < 0)
    {
        close(socket_fd);
        return;
    }
    // closing the connected socket
    close(client_fd);
    // closing the listening socket
    shutdown(socket_fd, SHUT_RDWR);
}