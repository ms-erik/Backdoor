#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 137

void shiftStringByCount(char* string, int count) {
    if (count >= 0 && count < strlen(string)) {
        memmove(string, string + count, strlen(string) - count + 1);
    } else {
        string[0] = '\0';
    }
}

int findSemicolon(const char* string) {
    int position = -1;  // Default return value if semicolon is not found
    int i = 0;

    while (string[i] != '\0') {
        if (string[i] == ';') {
            position = i;
            break;
        }
        i++;
    }

    return position + 1;
}

int main() {
    int fd[2];
    int sockfd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    fd_set fds;
    struct timeval timeout;

    // Cria o pipe para capturar a saída do journalctl -f
    if (pipe(fd) == -1) {
        perror("Erro ao criar o pipe");
        exit(1);
    }

    // Cria o socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Erro ao criar o socket");
        exit(1);
    }

    // Configura os detalhes do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Associa o socket ao endereço e porta
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erro ao fazer o bind");
        exit(1);
    }

    // Aguarda por conexões
    if (listen(sockfd, 1) == -1) {
        perror("Erro ao ouvir por conexões");
        exit(1);
    }

    printf("Servidor aguardando por conexões...\n");

    // Aceita a conexão do cliente
    client_addr_len = sizeof(client_addr);
    client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_sock == -1) {
        perror("Erro ao aceitar a conexão");
        exit(1);
    }

    printf("Cliente conectado!\n");

    // Fork para executar o journalctl -f em um processo filho
    pid_t pid = fork();

    if (pid < 0) {
        perror("Erro ao criar o processo filho");
        exit(1);
    } else if (pid == 0) {
        // Processo filho
        // Redireciona a saída do journalctl -f para o pipe
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        // Executa o journalctl -f
        execlp("journalctl", "journalctl", "-f", NULL);

        perror("Erro ao executar o journalctl");
        exit(1);
    } else {
        // Processo pai
        close(fd[1]);

        while (1) {
            FD_ZERO(&fds);
            FD_SET(fd[0], &fds);
            FD_SET(sockfd, &fds);

            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            // Aguardar até que haja dados disponíveis para leitura
            if (select(fd[0] + sockfd + 1, &fds, NULL, NULL, &timeout) > 0) {
                // Verificar se a saída do journalctl -f está pronta para leitura
                if (FD_ISSET(fd[0], &fds)) {
                    char buffer[BUFFER_SIZE];
                    ssize_t bytesRead;

                    // Ler os dados do pipe
                    bytesRead = read(fd[0], buffer, sizeof(buffer) - 1);
                    if (bytesRead < 0) {
                        perror("Erro ao ler do pipe");
                        exit(1);
                    }

                    if (bytesRead > 0) {
                        // Adicionar um terminador de string no final dos dados lidos
                        buffer[bytesRead] = '\0';

                        // Imprimir os dados lidos
                        printf("%s", buffer);

                        // Enviar dados para o cliente
                        int start = findSemicolon(buffer);
                        shiftStringByCount(buffer, start);
                        if (send(client_sock, buffer, bytesRead, 0) == -1) {
                            perror("Erro ao enviar dados para o cliente");
                            exit(1);
                        }
                    }
                }

                // Verificar se há conexões de clientes pendentes
                if (FD_ISSET(sockfd, &fds)) {
                    // Aceitar a conexão do cliente
                    client_addr_len = sizeof(client_addr);
                    client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
                    if (client_sock == -1) {
                        perror("Erro ao aceitar a conexão");
                        exit(1);
                    }

                    printf("Cliente conectado!\n");
                }
            }
        }
    }

    // Fecha o socket do servidor
    close(sockfd);

    return 0;
}

