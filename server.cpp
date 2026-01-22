#include <iostream>
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define closesocket close
#endif

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    // Créer le socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configurer l'adresse
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    
    // Bind
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    
    // Listen
    listen(serverSocket, 1);
    
    std::cout << "Serveur en attente sur le port " << PORT << "..." << std::endl;
    
    // Accepter une connexion
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    std::cout << "Client connecté !" << std::endl;
    
    // Recevoir le message
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    buffer[bytesReceived] = '\0';
    
    std::cout << "Message reçu : " << buffer << std::endl;
    
    // Fermer
    closesocket(clientSocket);
    closesocket(serverSocket);
    
#ifdef _WIN32
    WSACleanup();
#endif
    
    return 0;
}