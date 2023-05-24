#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

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
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    std::cout << "Client socket created.";

    SOCKET iResult = connect(client, (SOCKADDR*)&server_address, sizeof(server_address));

    if ( iResult==0)
    {
        std::cout << "=> Connection to server " << inet_ntoa(server_address.sin_addr) << "with port number: " << DEFAULT_PORT << "\n";

    }

    char buffer[BUFFER_SIZE];
    std::cout << "Waiting for server confirmation...\n";
    recv(client, buffer, BUFFER_SIZE, 0);
    std::cout << "=> Connection established.\n" << "Enter " << CLIENT_CLOSE_CONNECTION_SYMBOL << " to close the connection";

    while (true)
    {
        std::cout << "CLient: ";
        std::cin.getline(buffer, BUFFER_SIZE, 0);
        send(client, buffer, BUFFER_SIZE, 0);
        if (is_client_connection_close(buffer))
        {
            break;
        }

        std::cout << "Server: ";
        std::cin.getline(buffer, BUFFER_SIZE, 0);
        recv(client, buffer, BUFFER_SIZE, 0);
        if (is_client_connection_close(buffer))
        {
            break;
        }
    }

    std::cout << "Goodbye...\n";


	return 0;
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