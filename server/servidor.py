import os
import select
import socket

BUFFER_SIZE = 1024
SERVER_PORT = 1234

def shift_string_by_count(string, count):
    if count >= 0 and count < len(string):
        string[:] = string[count:]  # Shift the string in-place
    else:
        string[:] = []

def find_semicolon(string):
    for i, char in enumerate(string):
        if char == ';':
            return i + 1
    return -1

# Abrir o arquivo /dev/kmsg
fd = os.open("/dev/kmsg", os.O_RDONLY)
if fd < 0:
    print("Erro ao abrir o arquivo /dev/kmsg")
    exit(1)

# Cria o socket
sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sockfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Configura os detalhes do servidor
server_addr = ("", SERVER_PORT)

# Associa o socket ao endereço e porta
try:
    sockfd.bind(server_addr)
except socket.error as e:
    print("Erro ao fazer o bind:", e)
    exit(1)

# Aguarda por conexões
sockfd.listen(1)
print("Servidor aguardando por conexões...")

# Aceita a conexão do cliente
client_sock, client_addr = sockfd.accept()
print("Cliente conectado!")

while True:
    readable, _, _ = select.select([fd], [], [], 1)

    if readable:
        buffer = os.read(fd, BUFFER_SIZE)

        if buffer:
            buffer = buffer.decode("utf-8")
            print(buffer, end="")

            start = find_semicolon(buffer)
            shift_string_by_count(buffer, start)

            try:
                client_sock.sendall(buffer.encode("utf-8"))
            except socket.error as e:
                print("Erro ao enviar dados para o cliente:", e)
                exit(1)

# Fechar o descritor de arquivo
os.close(fd)

# Fecha a conexão com o cliente
client_sock.close()

# Fecha o socket do servidor
sockfd.close()
