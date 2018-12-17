//
// Created by lucie on 17/12/18.
//
// La deuxième version va lire une chaîne de caractères du client
// et la renvoyer au client, puis fermer le serveur.
// C'est le programme le plus classique d'exemple de serveur: echo.
// La lecture se fait dans un buffer.
// De nouveau tu peux tester par 'telnet localhost 5555' puis en tapant n'importe quoi suivi de return
// Telnet envoie ce que tu as tapé dans son socket client quand tu presse return.

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
    char buffer[100];
    bzero(buffer, sizeof(buffer)); // nécessaire pour que le string lu soit suivi par 0
    read(worker_socket, buffer, sizeof(buffer));
    write(worker_socket, buffer, sizeof(buffer) + 1); // +1 pour inclure un 0 final
    close(server_socket);
    return 0;
}