#include <iostream>
#include <string>
#include <stdlib.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <thread>
#include <mutex>

#pragma comment (lib, "Ws2_32.lib")

#define ERROR_S "SERVER ERROR: "
#define DEFAULT_PORT 1601
#define BUFFER_SIZE 1024
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

bool is_client_connection_close(const char* msg, int &size);

void set_up_socket(SOCKET &client, sockaddr_in &server_address)
{
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0)
    {
        std::cout << "SERVER ERROR: establishing socket error\n";
        exit(0);
    }

    std::cout << "SERVER: Socket for server was succesfully created\n";

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    int ret = bind(client, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address));

    if (ret < 0)
    {
        std::cout << ERROR_S << "binding connection. Socket has already been establishing.\n";
        return;
    }
}


void handle_client(SOCKET &server,int &size)
{
    if (server < 0)
    {
        std::cout << ERROR_S << "Can't accepting client.\n";
    }
    std::thread t([&server, &size]
        {
            char buffer[BUFFER_SIZE];
            bool IsExit = false;
            while (server > 0)
            {

                strcpy_s(buffer, "=> Server connected");
                send(server, buffer, BUFFER_SIZE, 0);
                std::cout << "Connected to the clien #1 " << std::endl << "Enter " << CLIENT_CLOSE_CONNECTION_SYMBOL << " to end connection\n";
                std::cout << "Client: ";
                recv(server, buffer, BUFFER_SIZE, 0);
                std::cout << buffer << std::endl;
                if (is_client_connection_close(buffer,size))
                {
                    IsExit = true;
                }
                while (!IsExit)
                {
                    std::cout << "Server: ";
                    std::cin.getline(buffer, BUFFER_SIZE);
                    send(server, buffer, BUFFER_SIZE, 0);
                    if (is_client_connection_close(buffer,size))
                    {
                        break;
                    }
                    std::cout << "Client: ";
                    recv(server, buffer, BUFFER_SIZE, 0);
                    std::cout << buffer << std::endl;
                    if (is_client_connection_close(buffer,size))
                    {
                        break;
                    }
                }
                std::cout << "\nGoodbye..." << std::endl;
                IsExit = false;

            }
        });
    t.join();
}

int main()
{
    SOCKET client;
    SOCKET server;
    WSADATA wsaData;
    int size = 0;

    struct sockaddr_in server_address;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "SERVER ERROR: Initializing WinSock" << std::endl;
        return 1;
    }
    
    set_up_socket(client, server_address);
    int size_of_address = sizeof(server_address);
    std::cout << "SERVER: " << "Listening clients...\n";
    listen(client, 1);
    server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size_of_address);
    size++;
    std::thread handling(handle_client, std::ref(server),std::ref(size));
    handling.detach();
    while (true)
    {
        listen(client, 1);
        server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size_of_address);
        std::cout << "\nNew client accepted, server: "<<server;
        size++;
        handle_client(server, size);
    }
    
    return 0;
    
}

bool is_client_connection_close(const char* msg, int &size)
{
    for (int i = 0; i < strlen(msg); i++)
    {
        if (msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL)
        {
            size--;
        }
        
    }

    for (int i = 0; i < strlen(msg); i++)
    {
        if (msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL && size == 0)
        {
            exit(0);
        }
    }
    return false;
}

