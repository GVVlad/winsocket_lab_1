#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 27015
#define BUFFER_SIZE 1024

void startServer() {
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    SetConsoleOutputCP(CP_UTF8);
    BOOLEAN flag = TRUE;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Не вдалося ініціалізувати Winsock. Код помилки: " << WSAGetLastError() << std::endl;
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Не вдалося створити сокет. Код помилки: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);


    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();
        std::cerr << "Не вдалося виконати прив'язку. Код помилки: " << errorCode << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Сервер запущений на порту " << PORT << std::endl;

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Не вдалося почати прослуховування. Код помилки: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Очікування підключення клієнта..." << std::endl;

    while (flag) {
        SOCKET clientSocket;
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);

        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Не вдалося прийняти підключення. Код помилки: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "Клієнт підключено." << std::endl;

        char buffer[BUFFER_SIZE];
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        std::cout << "Отримано повідомлення від клієнта: ";
        for (int i = 0; i < bytesReceived; i++) {
            std::cout << buffer[i];
        }
        std::cout << std::endl;

        closesocket(clientSocket);
    }

    WSACleanup();
}

int main() {
    startServer();
    return 0;
}
