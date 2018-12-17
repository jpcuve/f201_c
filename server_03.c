//
// Created by lucie on 17/12/18.
//
// Quatrième version. On va lire le nom d'utilisateur, année, mois, jour et nom de l'image
// et les stocker dans des buffer. De nouveau il manque la donnée du stop char, mais on va dire
// que le stop char est le char \t.
// Exemple de flux réçu du client: "lucie\t/chemin/vers/la/photo\t2014\t11\t17\t"
// A partir de maintenant on ne va plus utiliser telnet mais netcat avec un fichier préparé.
// Je t'ai fait un ficher 'test.txt' avec l'exemple de flux.
// 'netcat localhost 5555 < test.txt'

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

int next_char(int worker_socket){
    if ((int) (ptr - buffer) >= size){
        size = read(worker_socket, buffer, sizeof(buffer));
        ptr = buffer;
        if (size < 0){
            return -1;
        }
    }
    return (int) *ptr++;
}

int main() {
    struct sockaddr_in server_endpoint;
    bzero(&server_endpoint, sizeof(server_endpoint));
    server_endpoint.sin_family = AF_INET;
    server_endpoint.sin_addr.s_addr = htons(INADDR_ANY);
    server_endpoint.sin_port = htons(PORT);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        printf("Cannot create server socket");
        exit(1);
    }
    int binding = bind(server_socket, (struct sockaddr *) &server_endpoint, sizeof(server_endpoint));
    if (binding < 0){
        printf("Cannot bind server socket to endpoint");
        exit(1);
    }
    char username[256];
    bzero(username, sizeof(username));
    char path[256];
    bzero(path, sizeof(path));
    char year[5];
    bzero(year, sizeof(year));
    char month[3];
    bzero(month, sizeof(month));
    char day[3];
    bzero(day, sizeof(day));
    int state = 0;
    char *ptr = username;
    char *max = ptr + sizeof(username);
    listen(server_socket, 1);
    int worker_socket = accept(server_socket, NULL, NULL);
    while (state != 99){
        int read = next_char(worker_socket);
        if (read < 0){
            state = 99;
        } else {
            switch(state){
                case 0:
                    if (read == '\t'){
                        ptr = path;
                        max = ptr + sizeof(path);
                        state = 1;
                    } else {
                        *ptr++ = (char) read;
                        if (ptr >= max){
                            state = 99;
                        }
                    }
                    break;
                case 1:
                    if (read == '\t'){
                        ptr = year;
                        max = ptr + sizeof(year);
                        state = 2;
                    } else {
                        *ptr++ = (char) read;
                        if (ptr >= max){
                            state = 99;
                        }
                    }
                    break;
                case 2:
                    if (read == '\t'){
                        ptr = month;
                        max = ptr + sizeof(month);
                        state = 3;
                    } else {
                        *ptr++ = (char) read;
                        if (ptr >= max){
                            state = 99;
                        }
                    }
                    break;
                case 3:
                    if (read == '\t'){
                        ptr = day;
                        max = ptr + sizeof(day);
                        state = 4;
                    } else {
                        *ptr++ = (char) read;
                        if (ptr >= max){
                            state = 99;
                        }
                    }
                    break;
                case 4:
                    if (read == '\t'){
                        state = 5;
                    } else {
                        *ptr++ = (char) read;
                        if (ptr >= max){
                            state = 99;
                        }
                    }
                    break;
                case 5:
                    printf("Username: %s\n", username);
                    printf("Path: %s\n", path);
                    printf("Year: %s\n", year);
                    printf("Month: %s\n", month);
                    printf("Day: %s\n", day);
                    state = 99;
                    break;
            }
        }
    }
    close(worker_socket);
    close(server_socket);
    return 0;
}