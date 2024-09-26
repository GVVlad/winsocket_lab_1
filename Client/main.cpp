#include <iostream>
#include <Ws2tcpip.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 27015

void startClient() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    SetConsoleOutputCP(CP_UTF8);

    std::string message = "Повідомлення яке передається на сервер.";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Не вдалося ініціалізувати Winsock. Код помилки: " << WSAGetLastError() << std::endl;
        return;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Не вдалося створити сокет. Код помилки: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    InetPton(AF_INET, SERVER_IP, &serverAddr.sin_addr.s_addr);
    serverAddr.sin_port = htons(PORT);

    int result = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();
        std::cerr << "Не вдалося підключитися до сервера. Код помилки: " << errorCode << std::endl;
        return;
    }

    std::cout << "Підключено до сервера." << std::endl;

    int bytesSent = send(clientSocket, message.c_str(), message.size(), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Не вдалося відправити повідомлення. Код помилки: " << WSAGetLastError() << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();
}

int main() {
    startClient();
    return 0;
}
