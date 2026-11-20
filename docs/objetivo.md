## 01 — Objetivo do projeto

### Objetivo geral

Desenvolver um protótipo de casa inteligente utilizando Arduino UNO e tecnologia RFID para demonstrar o controle de acesso a uma residência. O sistema identifica cartões, aciona a porta por servo motor, controla a luz da sacada por interruptor e apresenta os estados no display TFT ILI9341.

### Objetivos específicos

- Aplicar a leitura e validação de cartões RFID para distinguir acessos autorizados e negados. A TAG cadastrada é 01:02:03:04.
- Utilizar um servo motor SG90 para representar a abertura da porta e o fechamento automático após cinco segundos.
- Empregar LEDs, buzzer e display TFT como recursos de sinalização e apresentação de informações.
- Comandar a luz da sacada com um interruptor e acompanhar seu estado na tela.
- Integrar conceitos de eletrônica, programação embarcada e automação residencial em um protótipo didático de Internet das Coisas.

## 02 — Justificativa

O projeto foi escolhido para demonstrar uma aplicação da tecnologia RFID no controle de acesso residencial. A identificação automática e o acionamento da porta tornam a operação mais prática, enquanto as indicações na tela, nos LEDs e no buzzer ajudam a perceber o resultado de cada leitura. O controle da luz da sacada ilustra como a automação pode facilitar tarefas do dia a dia.

O protótipo opera localmente, sem conectividade de rede. A comparação de UID é um recurso didático, pois identificadores podem ser copiados. O estado da porta no display representa o comando enviado ao servo, sem sensor de posição.
