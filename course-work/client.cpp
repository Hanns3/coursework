#include <iostream>
#include <string>
#include <stdlib.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 1601
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'
#define BUFFER_SIZE 1024

bool is_client_connection_close(const char* msg);

int main(int argc, const char* arvg[])
{
    SOCKET client;

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0)
    {
        std::cout << "SERVER ERROR: establishing socket error";
        exit(0);
    }

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    std::cout << "inet_pton: " << inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    std::cout << "\nClient socket created.";

    SOCKET ret = connect(client, (SOCKADDR*)&server_address, sizeof(server_address));
    char buffer[BUFFER_SIZE];
    if ( ret == 0)
    {
        std::cout << "=> Connection to server "<< inet_ntop(AF_INET,reinterpret_cast<sockaddr*>(&server_address.sin_addr),buffer, BUFFER_SIZE) << "with port number: " << DEFAULT_PORT << "\n";
    }

    std::cout << "Waiting for server confirmation...\n";
    recv(client, buffer, BUFFER_SIZE, 0);
    std::cout << "=> Connection established.\n" << "Enter " << CLIENT_CLOSE_CONNECTION_SYMBOL << " to close the connection";

    while (true)
    {
        std::cout << "\nClient: ";
        std::cin.getline(buffer, BUFFER_SIZE);
        send(client, buffer, BUFFER_SIZE, 0);
        if (is_client_connection_close(buffer))
        {
            break;
        }

        std::cout << "Server: ";
        std::cin.getline(buffer, BUFFER_SIZE);
        recv(client, buffer, BUFFER_SIZE, 0);
        if (is_client_connection_close(buffer))
        {
            break;
        }
    }

    std::cout << "Goodbye...\n";

	return 0;
    closesocket(client);
}

bool is_client_connection_close(const char* msg)
{
    for (int i = 0; i < strlen(msg); i++)
    {
        if (msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL)
        {
            return true;
        }
    }
    return false;
}