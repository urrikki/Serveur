//Empeche d'avoir certains avertissements liés à l'utilisation de fonctions risquées
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

//On se lie à la bibliothèque ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

//Include que nous allons utiliser
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>

//Définie le port
#define PORT 80
#define default PORT 14843

int main() {
    int iResult;

    // initialisation de winsock
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    // Création de la socket
    SOCKET hsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hsocket == INVALID_SOCKET) {
        printf("socket failed");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("74.125.24.101");  //Adresse ip du site 

    // Connexion au serveur Google
    if (connect(hsocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0) {
        printf("connect failed");
        closesocket(hsocket);
        WSACleanup();
        return 1;
    }

    // Envoi de la requête HTTP à Google
    const char* httpRequest = "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
    send(hsocket, httpRequest, strlen(httpRequest), 0);

    // Réception et affichage de la réponse
    char buffer[4096];
    int valread;
    while ((valread = recv(hsocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        // subtract 1 for the null terminator at the end        
        buffer[valread] = '\0';
        printf("%s", buffer);
    }

    closesocket(hsocket);

    WSACleanup();

    return 0;
}