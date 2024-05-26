#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <string>
#include <thread>
#include <fstream>
#include <filesystem>
#include <mutex>

using namespace std;

mutex logMutex;

void logRequest(const string& logMessage) {
    lock_guard<mutex> guard(logMutex);
    ofstream logFile("server.log", ios::app);
    logFile << logMessage << endl;
}

string readFile(const string& filePath) {
    ifstream file(filePath, ios::in | ios::binary);
    if (file) {
        string contents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        return contents;
    }
    return "<html><h1>404 Not Found</h1></html>";
}

void handleClient(SOCKET clientSocket) {
    const int BUFFER_SIZE = 30720;
    char buffer[BUFFER_SIZE] = {0};
    int bytes = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytes < 0) {
        cerr << "Cannot read client request" << endl;
        closesocket(clientSocket);
        return;
    }

    string request(buffer);
    cout << "Received request: " << request << endl;
    logRequest("Received request: " + request);

    string response;
    size_t pos = request.find(" ");
    string method = request.substr(0, pos);
    size_t pos2 = request.find(" ", pos + 1);
    string path = request.substr(pos + 1, pos2 - pos - 1);

    if (method == "GET") {
        if (path == "/") {
            path = "/index.html";
        }
        string filePath = "." + path;
        response = readFile(filePath);
        string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + to_string(response.size()) + "\r\n\r\n";
        response = header + response;
    } else {
        response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n";
    }

    send(clientSocket, response.c_str(), response.size(), 0);
    cout << "Sent response to client" << endl;
    logRequest("Sent response to client");

    closesocket(clientSocket);
}

int main() {
    cout << "Creating a Web server" << endl;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Initialization Failed" << endl;
        return 1;
    }

    SOCKET wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (wsocket == INVALID_SOCKET) {
        cerr << "Cannot create socket" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);
    int server_len = sizeof(server);

    if (bind(wsocket, (SOCKADDR *)&server, server_len) != 0) {
        cerr << "Cannot bind socket" << endl;
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    if (listen(wsocket, 20) != 0) {
        cerr << "Could not start listening" << endl;
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    cout << "Listening on 127.0.0.1:8080" << endl;

    while (true) {
        SOCKET clientSocket = accept(wsocket, (SOCKADDR *)&server, &server_len);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Cannot accept" << endl;
            continue;
        }
        thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

    closesocket(wsocket);
    WSACleanup();

    return 0;
}
