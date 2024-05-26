#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <string>

using namespace std;

int main() {
    cout << "Creating a Web server" << endl;

    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsaData;
    struct sockaddr_in server;
    int server_len;
    const int BUFFER_SIZE = 30720;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Initialization Failed" << endl;
        return 1;
    }

    // Create a socket
    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (wsocket == INVALID_SOCKET) {
        cerr << "Cannot create socket" << endl;
        WSACleanup();
        return 1;
    }

    // Bind socket to address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);
    server_len = sizeof(server);

    if (bind(wsocket, (SOCKADDR *)&server, server_len) != 0) {
        cerr << "Cannot bind socket" << endl;
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    // Listen to address
    if (listen(wsocket, 20) != 0) {
        cerr << "Could not start listening" << endl;
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    cout << "Listening on 127.0.0.1:8080" << endl;
    int bytes = 0;

    while (true) {
        // Accept client request
        new_wsocket = accept(wsocket, (SOCKADDR *)&server, &server_len);
        if (new_wsocket == INVALID_SOCKET) {
            cerr << "Cannot Accept" << endl;
            continue; // Try to accept next connection
        }

        // Read request
        char buff[BUFFER_SIZE] = {0};
        bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
        if (bytes < 0) {
            cerr << "Cannot read client request" << endl;
            closesocket(new_wsocket);
            continue; // Try to accept next connection
        }

        // Prepare and send response
        string response = "<html><h1>Hello World</h1></html>";
        string serverMessage = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
        serverMessage.append(to_string(response.size()));
        serverMessage.append("\r\n\r\n");
        serverMessage.append(response);

        int bytesSent = send(new_wsocket, serverMessage.c_str(), serverMessage.size(), 0);
        if (bytesSent < 0) {
            cerr << "Could not send response" << endl;
        } else {
            cout << "Sent response to client" << endl;
        }

        closesocket(new_wsocket);
    }

    closesocket(wsocket);
    WSACleanup();

    return 0;
}
