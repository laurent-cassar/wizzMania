#include <iostream>
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
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
    // Configuration UTF-8 pour les accents
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
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
    std::cout << "Connecté !" << std::endl;
    std::cout << "Tapez 'quit' pour quitter\n" << std::endl;
    
    // Boucle pour envoyer plusieurs messages
    while (true) {
        // Demander à l'utilisateur d'entrer un message
        std::cout << "Message : ";
        std::string message;
        std::getline(std::cin, message);
        
        // Quitter si l'utilisateur tape "quit"
        if (message == "quit" || message == "exit") {
            std::cout << "Déconnexion..." << std::endl;
            break;
        }
        
        // Envoyer le message
        if (!message.empty()) {
            send(clientSocket, message.c_str(), message.length(), 0);
            std::cout << "✓ Envoyé" << std::endl;
        }
    }
    
    // Fermer
    closesocket(clientSocket);
    
#ifdef _WIN32
    WSACleanup();
#endif
    
    return 0;
}