## 08 — Testes

### Validação executada

- Revisão estática. O JSON foi interpretado e as 13 peças, 49 ligações e redes elétricas foram comparadas com o esquema, incluindo os contatos da protoboard. Os pinos declarados e as bibliotecas foram conferidos com o código.
- Compilação. O Arduino CLI compilou o sketch para arduino:avr:uno, com Arduino AVR Boards 1.8.8. O resultado ocupou 19.078 bytes de flash (59%) e 722 bytes de RAM global (35%). Bibliotecas diretas usadas na compilação: MFRC522 1.4.12, Servo 1.3.0, Adafruit ILI9341 1.7.0 e Adafruit GFX Library 1.13.0.

- RFID e porta no Wokwi. O cartão azul 01:02:03:04 abriu a porta, acendeu o LED verde e exibiu ACESSO LIBERADO. O servo retornou e o LED verde apagou após cerca de cinco segundos no relógio simulado. O cartão verde 11:22:33:44 acendeu o LED vermelho e exibiu ACESSO NEGADO com a porta fechada.
- Iluminação e TFT no Wokwi. O painel exibiu os estados iniciais da porta e da sacada e acompanhou as duas posições do interruptor. A serial confirmou os eventos. O rodapé foi observado em SISTEMA PRONTO após o fechamento.

### Roteiro de reprodução no Wokwi

Ainda ficam pendentes a confirmação do aviso transitório PORTA FECHADA no TFT, a contagem e a medição dos tons do buzzer e os testes em bancada.

| Caso | Procedimento | Resultado esperado |
| --- | --- | --- |
| Acesso autorizado | Apresentar o cartão azul, UID 01:02:03:04. | Servo a 90°, LED verde aceso, um tom e ACESSO LIBERADO. |
| Acesso negado | Com a porta fechada, apresentar cartão com UID diferente. | Porta fechada, LED vermelho temporário, três tons e ACESSO NEGADO. |
| Fechamento automático | Após autorizar, aguardar cinco segundos sem apresentar outro cartão. | Servo volta a 0°, LED verde apaga e aparece PORTA FECHADA. |
| Iluminação | Alternar o interruptor e manter cada posição. | LED branco e painel LUZ DA SACADA acompanham a posição. |
| Interface TFT | Verificar a partida e observar cada evento anterior. | Tela horizontal, estados coerentes e retorno a SISTEMA PRONTO após o aviso. |
