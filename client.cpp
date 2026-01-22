#include <iostream>
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define closesocket close
#endif

const int PORT = 8080;

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    // Créer le socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configurer l'adresse du serveur
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    
    // Connexion au serveur
    std::cout << "Connexion au serveur..." << std::endl;
    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    
    // Envoyer "Hello World!"
    std::string message = "Hello World!";
    send(clientSocket, message.c_str(), message.length(), 0);
    std::cout << "Message envoyé : " << message << std::endl;
    
    // Fermer
    closesocket(clientSocket);
    
#ifdef _WIN32
    WSACleanup();
#endif
    
    return 0;
}