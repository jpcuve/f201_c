//
// Created by lucie on 17/12/18.
//
// Je suis obligé d'y aller progressivement parce que je n'ai plus programmé en C
// depuis 30 ans.
// Normalement tu commences un problème comme ça par le plus simple.
// Un serveur qui écoute sur un port la demande de connection d'un client
// quand la connection du client arrive le serveur lui envoie une chaîne de
// caractères Hello World! et puis s'éteint.
// Tu peux prendre le client 'telnet' de linux pour tester facilement, en faisant
// 'telnet localhost 5555'

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 5555

int main() {
    // structure qui contient l'adresse ip et le port
    struct sockaddr_in server_endpoint;
    bzero(&server_endpoint, sizeof(server_endpoint)); // mettre la structure à zéro
    server_endpoint.sin_family = AF_INET;
    server_endpoint.sin_addr.s_addr = htons(INADDR_ANY);
    server_endpoint.sin_port = htons(PORT);
    // créer le socket qui va servir au serveur
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // lier l'endpoint au socket
    bind(server_socket, (struct sockaddr *) &server_endpoint, sizeof(server_endpoint));
    // écouter les demandes de connection venant de clients.
    // Le deuxième argument est 'backlog', un indicateur du nombre de clients qui peuvent être en attente
    // de traitement par le serveur. Ici nous laissons 1 car nous ne traitons qu'un client, puis arrêtons
    // le serveur.
    listen(server_socket, 1);
    // fonction bloquante, qui accepte une connection de client
    int worker_socket = accept(server_socket, NULL, NULL);
    char* message = "Hello, World!";
    write(worker_socket, message, strlen(message));
    close(server_socket);
    return 0;
}