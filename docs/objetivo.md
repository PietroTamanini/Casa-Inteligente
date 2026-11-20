## 02 — Objetivo

### Objetivo geral

Desenvolver um protótipo didático de automação residencial que integre controle de acesso à porta e comando da iluminação da sacada em um Arduino UNO R3, com indicação local no display TFT ILI9341.

### Objetivos específicos

- Identificar cartões pelo UID e liberar somente a TAG 01:02:03:04.
- Acionar a porta com um servo e fechá-la automaticamente após cinco segundos.
- Sinalizar acessos autorizados e negados com LEDs, buzzer e mensagens no TFT.
- Controlar a luz da sacada com um interruptor e apresentar seu estado na tela.

## 03 — Justificativa

A automação residencial reduz tarefas repetitivas e facilita a consulta ao estado de portas e luzes. Neste protótipo, a identificação por cartão restringe a abertura a um UID cadastrado, e os sinais visuais e sonoros tornam o resultado perceptível no ponto de acesso.

A atividade aplica conceitos de Internet das Coisas ao integrar entrada de dados, processamento, comunicação entre periféricos e atuação. O protótipo opera localmente, sem conectividade de rede. A comparação de UID é um recurso didático, pois identificadores podem ser copiados. O estado da porta no display representa o comando enviado ao servo, sem sensor de posição.
