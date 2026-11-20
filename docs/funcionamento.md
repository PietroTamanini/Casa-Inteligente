## 06 — Funcionamento

### Inicialização e leitura RFID

O Arduino inicia a serial a 9.600 bit/s, configura o interruptor com pull-up interno e comanda o servo a 0°. O programa lê a posição inicial do interruptor, desativa os dois sinais CS em nível alto e inicializa o SPI, o TFT e o RC522. Em cada ciclo, consulta o leitor e, ao obter um cartão, imprime o UID em hexadecimal. A autorização exige quatro bytes iguais a 01:02:03:04, na mesma ordem. Ao terminar, o programa encerra a comunicação com o cartão.

### Porta e sinalização

- Acesso autorizado. O servo recebe o comando de 90°, o LED verde acende e o vermelho apaga. O buzzer emite um tom de 1.800 Hz por 130 ms e o TFT mostra ACESSO LIBERADO. Uma nova autorização reinicia a contagem.
- Fechamento. Ao atingir 5.000 ms desde a última abertura, o programa volta o servo a 0°, apaga o LED verde e exibe PORTA FECHADA. A checagem ocorre no laço principal e pode sofrer atraso durante a sinalização sonora de uma recusa.
- Acesso negado. O servo conserva o estado em que estava. O LED vermelho é temporizado em 1.500 ms e o buzzer emite três tons de 950 Hz, com duração de 90 ms cada. O TFT mostra ACESSO NEGADO.

### Iluminação e interface

Na posição que conecta D7 ao GND, o nível LOW acende o LED branco. Na outra posição, o contato externo está aberto e o pull-up interno leva D7 a HIGH, apagando a luz. O programa aguarda 40 ms de estabilidade para aceitar uma transição.

O ILI9341 opera em orientação horizontal, com 320 × 240 pixels. Os painéis PORTA e LUZ DA SACADA informam os estados. O rodapé exibe eventos por 1.800 ms e volta a SISTEMA PRONTO. Um evento mais recente pode substituir a mensagem anterior.
