## 03 — Lista de componentes

| ID no Wokwi | Componente / modelo | Qtde. | Função |
| --- | --- | --- | --- |
| uno | Arduino UNO R3 | 1 | Executa o programa e controla os periféricos. |
| lcd2 | TFT ILI9341, 320 × 240, SPI | 1 | Mostra estados e mensagens de acesso. |
| rfid1 | Leitor RFID RC522 (MFRC522) | 1 | Lê o UID dos cartões. |
| servo1 | Microservo SG90 | 1 | Representa a abertura e o fechamento da porta. |
| led1 | LED vermelho | 1 | Indica acesso negado. |
| led2 | LED verde | 1 | Indica abertura autorizada. |
| led3 | LED branco | 1 | Representa a luz da sacada. |
| r1, r2, r3 | Resistor de 330 Ω | 3 | Limita a corrente dos LEDs. |
| sw1 | Interruptor deslizante SPDT | 1 | Comanda a luz da sacada. |
| bz1 | Buzzer piezoelétrico | 1 | Emite sinais de acesso. |
| bb1 | Meia protoboard | 1 | Interliga terminais e distribui a alimentação. |
| — | Fios de conexão | Conforme montagem | Interligam as redes elétricas do circuito. |
| — | Cartões RFID de teste | 2 no roteiro | Um UID autorizado e um UID diferente. |

O circuito inclui 13 peças em diagram.json. Os cartões são selecionados na interface do RC522 no Wokwi e os fios são representados na lista de conexões. O servo é descrito pelo modelo SG90, representado pelo componente genérico de microservo na simulação.
