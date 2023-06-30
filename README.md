# Backdoor
Apresentamos aqui a implementação de um módulo de kernel que atua como backdoor, coletando informações dentro do kernel e as transmitindo para outra máquina( no caso utilizamos uma máquina virtual em ubunto). As informações que coletamos foram as entradas do usuário via teclado.

Pré-requerimentos
Para o funcionamento do módulo de kernel foi utilizado uma máquina virtual com ubunto 64-bit, e o fedora como sistema operacional local, portanto entre o requisitos temos:
.Máquina Virtual Ubunto 64-bit
.Sistema Operacional Fedora 36
.Oracle Virtual Box(ou qualquer outro software para vizualisação).
.Compilador GCC
Vale ressaltar que os comando para obter o log de informações do kernel são específicos do fedora, como será visto no código, bem diferente de outros sistemas Unix, portatno provavlemnete não irpa funcionar fora dele.


Instalação
1.Instalação da Oracle Virtual box
Está disponível em https://www.oracle.com/virtualization/technologies/vm/downloads/virtualbox-downloads.html; basta escolher seu sistema operacional.

2.Clone o repositório do git no diretório desejado.


Configuração
    Etapa realizada na máquina do usuário.
1. Mova para o diretório backdoor, nele utilize os seguintes comandos:
    -make
    -sudo insmod keylloger.ko
Os quais compilam o módulo de kernel e o carrega.

2. Em seguida mova para o diretório do servidor, e novamente de os comandos:
    -make
    -sudo ./server
Para compilar e iniciar o servidor.

    Etapa na máquina virtual

1. Compilar o programa cliente dando make
2. Rodar o executável.

Uso

Após a instalação e configuração, o módulo de kernel será carregado e o servidor estará em execução. O cliente poderá se conectar ao servidor e obter as informações sobre as teclas pressionadas no teclado na máquina local.

- O módulo de kernel 'keylogger' captura as teclas pressionadas e imprimirá as informações no log do kernel.
- O servidor aguarda pela conexão do cliente.
- O cliente se conectá ao servidor e exibe as informações sobre as teclas pressionadas.

Observações 

- Grande parte dos comandos necessitam de privilégios de root, tenha certeza de estar utilizando.
- O servidor e o cliente estão configurados em um IP e em uma porta específicas, caso deseje modificar, basta alterar os arquivos server.c e client.c e recompilar os programas.
- O módulo de kernel foi desenvolvido no fedora 36, não há nenhuma garantia de que irá funcionar em outros sistemas operacionais.
