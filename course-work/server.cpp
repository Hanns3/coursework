#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

#define ERROR_S "SERVER ERROR: "
#define DEFAULT_PORT 1601
#define BUFFER_SIZE 1024
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

bool is_client_connection_close(const char* msg);

int main()
{
    SOCKET client;
    SOCKET server;

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0)
    {
        std::cout << "SERVER ERROR: establishing socket error";
        exit(0);
    }

    std::cout << "SERVER: Socket for server was succesfully created\n";

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    SOCKET ret = bind(client, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address));

    if (ret < 0)
    {
        std::cout << ERROR_S << "binding connection. Socket has already been establishing.\n";
        return -1;
    }
    int size = sizeof(server_address);
    std::cout << "SERVER: " << "Listening clients...\n";
    listen(client, 1);

    server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size);
    if (server < 0)
    {
        std::cout << ERROR_S << "Can't accepting client.\n";
    }

    char buffer[BUFFER_SIZE];
    bool IsExit = false;
    while (server > 0)
    {
        strcpy_s(buffer, "=> Server connected");
        send(server, buffer, BUFFER_SIZE, 0);
    std::cout << "Connected to the clien #1 " << std::endl << "Enter " << CLIENT_CLOSE_CONNECTION_SYMBOL << " to end connection\n\n";

        std::cout << "Client: ";
        recv(server, buffer, BUFFER_SIZE, 0);
        std::cout << buffer << std::endl;
        if (is_client_connection_close(buffer))
        {
            IsExit = true;
        }

        while (IsExit)
        {
            std::cout << "Server: ";
            std::cin.getline(buffer, BUFFER_SIZE);
            send(server, buffer, BUFFER_SIZE, 0);
            if (is_client_connection_close(buffer))
            {
                break;
            }

            std::cout << "Client: ";
            recv(server, buffer, BUFFER_SIZE, 0);
            std::cout << buffer << std::endl;
            if (is_client_connection_close(buffer))
            {
                break;
            }
        }
        std::cout << "\nGoodbye..." << std::endl;
        IsExit = false;
        exit(1);

    }
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
}

