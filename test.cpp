#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <thread>

int main() {
    // 创建 Socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // 绑定 IP 和端口
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind to port 8080." << std::endl;
        close(serverSocket);
        return 1;
    }

    // 监听端口
    if (listen(serverSocket, SOMAXCONN) == -1) {
        std::cerr << "Failed to listen on port 8080." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(10));
    // 接受连接请求
    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept client connection." << std::endl;
        close(serverSocket);
        return 1;
    }

    // 处理客户端请求
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        std::cerr << "Failed to read from client socket." << std::endl;
        close(clientSocket);
        close(serverSocket);
        return 1;
    }

    // 打印客户端发送的数据
    std::cout << "Received data from client: " << buffer << std::endl;

    // 关闭连接
    close(clientSocket);
    close(serverSocket);

    return 0;
}
