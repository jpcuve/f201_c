//
// Created by lucie on 17/12/18.
//
// La troisième version est plus compliquée. Je vais lire les données du client dans
// un buffer, mais je vais écrire une fonction qui me lit le data caractère par caractère.
// cette fonction va placer le caractère dans un int, de façon à pouvoir me donner
// un signal de fermeture de la connection (donc: si int est entre 0 et 255, c'est du data
// si int = -1, la connection est fermée.
// A tester avec 'telnet localhost 5555'
// Le serveur renvoie les codes des caractères écrits par le client.

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 5555
#define BUFFER_SIZE 100

char buffer[BUFFER_SIZE];
char *ptr = buffer;
int size = 0;

// ceci est probablement la fonction qui te manque...
int next_char(int worker_socket){
    if ((int) (ptr - buffer) >= size){ // nous sommes arrivés au bout de ce qu'il y a à lire, il faut re-remplir le buffer
        size = read(worker_socket, buffer, sizeof(buffer));
        printf("Read %d characters from socket\n", size);
        ptr = buffer;
        if (size < 0){
            return -1;
        }
    }
    return (int) *ptr++;
}

int main() {
    // structure qui contient l'adresse ip et le port
    struct sockaddr_in server_endpoint;
    bzero(&server_endpoint, sizeof(server_endpoint)); // mettre la structure à zéro
    server_endpoint.sin_family = AF_INET;
    server_endpoint.sin_addr.s_addr = htons(INADDR_ANY);
    server_endpoint.sin_port = htons(PORT);
    // créer le socket qui va servir au serveur
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        printf("Cannot create server socket");
        exit(1);
    }
    // lier l'endpoint au socket
    int binding = bind(server_socket, (struct sockaddr *) &server_endpoint, sizeof(server_endpoint));
    if (binding < 0){
        printf("Cannot bind server socket to endpoint");
        exit(1);
    }
    // écouter les demandes de connection venant de clients.
    // Le deuxième argument est 'backlog', un indicateur du nombre de clients qui peuvent être en attente
    // de traitement par le serveur. Ici nous laissons 1 car nous ne traitons qu'un client, puis arrêtons
    // le serveur.
    listen(server_socket, 1);
    // fonction bloquante, qui accepte une connection de client
    int worker_socket = accept(server_socket, NULL, NULL);
    char snum[10];
    while (1){
        int read = next_char(worker_socket);
        if (read < 0){
            break;
        }
        int size = sprintf(snum, "%d ", read);
        write(worker_socket, snum, size);
    }
    close(worker_socket);
    close(server_socket);
    return 0;
}