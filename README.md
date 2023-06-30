# Backdoor
Apresentamos aqui a implementação de um módulo de kernel que atua como backdoor, coletando informações dentro do kernel e as transmitindo para outra máquina( no caso utilizamos uma máquina virtual em ubunto). As informações que coletamos foram as entradas do usuário via teclado.

Requerimentos
Para o funcionamento do módulo de kernel foi utilizado uma máquina virtual com ubunto 64-bit, e o fedora como sistema operacional local, portanto entre o requisitos temos:
.Máquina Virtual Ubunto 64-bit
.Sistema Operacional Fedora 36
.Oracle Virtual Box(ou qualquer outro software para vizualisação).
.Compilador GCC
Vale ressaltar que os comando para obter o log de informações do kernel são específicos do fedora, como será visto no código, bem diferente de outros sistemas Unix, portatno provavlemnete não irpa funcionar fora dele.

Instalação da Oracle Virtual box
Está disponível em https://www.oracle.com/virtualization/technologies/vm/downloads/virtualbox-downloads.html; basta escolher seu sistema operacional.
