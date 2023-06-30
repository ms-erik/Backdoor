#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <termios.h>
#include <fcntl.h>

#define SERVER_IP "127.0.0.1" // Substitua pelo endereço IP da sua máquina local 
#define SERVER_PORT 137   // Aqui pode escolher qualquer porta

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Cria o socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Falha ao criar o socket");
        exit(1);
    }

    // Configura os detalhes do servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr)) <= 0) {
        perror("Endereço IP inválido");
        exit(1);
    }

    // Conecta ao servidor na máquina virtual
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Falha ao conectar");
        close(clientSocket);
        exit(1);
    } else {
        printf("Conectado!\n");
    }

    while (1) {
        // Verifica se houve entrada do usuário
        if (kbhit()) {
            int input = getchar();
            if (input == '1') {
                // Se a entrada for '1', sai do loop
                break;
            }
        }

        // Recebe dados da máquina virtual
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived < 0) {
            perror("Falha ao receber dados");
            close(clientSocket);
            exit(1);
        } else if (bytesReceived == 0) {
            printf("Conexão encerrada pelo servidor.\n");
            break;
        } else {
            printf("%s", buffer);
        }
    }

    // Fecha a conexão
    close(clientSocket);

    return 0;
}

