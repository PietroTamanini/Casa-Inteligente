## 07 — Tabela de pinagem

| Arduino UNO | Ligação | Função |
| --- | --- | --- |
| D3 | servo1 PWM | Comando de posição da porta. |
| D4 | r2 → led2 A | LED verde, acesso autorizado. |
| D5 | r1 → led1 A | LED vermelho, acesso negado. |
| D6 | r3 → led3 A | LED branco, iluminação. |
| D7 | sw1 pino 2 | Comum do interruptor em INPUT_PULLUP. |
| D8 | bz1 pino 2 | Sinal sonoro. |
| D9 | rfid1 RST | Reinicialização do RC522. |
| D10 | rfid1 SDA/SS | Seleção SPI do RC522. |
| D11 | rfid1 e lcd2 MOSI | Dados do Arduino para os periféricos. |
| D12 | rfid1 e lcd2 MISO | Dados dos periféricos para o Arduino. |
| D13 | rfid1 e lcd2 SCK | Relógio SPI compartilhado. |
| A0 | lcd2 CS | Seleção SPI do TFT. |
| A1 | lcd2 D/C | Seleção entre dados e comandos. |
| 5 V | bb1 bp.1 → tp.1 pelo jumper bp.3; lcd2 LED direto | Alimenta V+ do servo no trilho inferior e VCC do TFT no superior. |
| 3,3 V | rfid1 3.3V | Alimentação direta do RC522. |
| GND.2 | bb1 bn.1 → tn.1 pelo jumper bn.3 | Retorno comum do servo, RC522, LEDs, interruptor, buzzer e TFT. |

Os cátodos dos três LEDs, sw1 pino 1 e bz1 pino 1 vão ao GND. O pino 3 do interruptor, o IRQ do RC522 e o RESET do TFT ficam sem ligação no esquema. Os demais pinos do Arduino não têm conexões externas registradas.
