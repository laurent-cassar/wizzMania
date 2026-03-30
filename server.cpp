#include <iostream>
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
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
    // Configuration UTF-8 pour les accents
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
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
    
    // Boucle pour recevoir plusieurs messages
    char buffer[BUFFER_SIZE];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        
        // Si la connexion est fermée ou erreur
        if (bytesReceived <= 0) {
            std::cout << "Client déconnecté" << std::endl;
            break;
        }
        
        buffer[bytesReceived] = '\0';
        std::cout << "Message reçu : " << buffer << std::endl;
    }
    
    // Fermer
    closesocket(clientSocket);
    closesocket(serverSocket);
    
#ifdef _WIN32
    WSACleanup();
#endif
    
    return 0;
}