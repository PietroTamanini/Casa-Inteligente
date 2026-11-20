
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Servo.h>

const byte SERVO_PORTA = 3;
const byte PIN_LED_VERDE_ACESSO = 4;
const byte PIN_LED_VERMELHO_ACESSO = 5;
const byte LED_BRANCO = 6;
const byte INTERRUPTOR = 7;
const byte BUZZER = 8;
const byte RFID_RST = 9;
const byte RFID_SDA_SS = 10;
const byte TFT_RST = A0;
const byte TFT_CS = A2;
const byte TFT_DC = A1;

const byte UID_AUTORIZADO[] = {0x01, 0x02, 0x03, 0x04};
const byte ANGULO_FECHADO = 0;
const byte ANGULO_ABERTO = 90;

const unsigned long TEMPO_PORTA_ABERTA = 5000UL;
const unsigned long TEMPO_LED_VERMELHO = 1500UL;
const unsigned long TEMPO_AVISO_TELA = 1800UL;

const uint16_t COR_FUNDO = 0x0000;
const uint16_t COR_CARTAO = 0x2A2A;
const uint16_t COR_RODAPE = 0x2929;
const uint16_t COR_CINZA = 0xB5B5;
const uint16_t COR_CIANO = 0x1F1F;
const uint16_t COR_VERDE = 0x0707;
const uint16_t COR_VERMELHO = 0xE0E0;
const uint16_t COR_AMARELO = 0xE7E7;

MFRC522 rfid(RFID_SDA_SS, RFID_RST);
Adafruit_ST7735 tela(TFT_CS, TFT_DC, TFT_RST);
Servo porta;

bool portaAberta = false;
bool luzAcesa = false;
unsigned long instanteAbertura = 0;

bool avisoVermelhoAtivo = false;
unsigned long instanteNegado = 0;

bool avisoTelaAtivo = false;
unsigned long instanteAvisoTela = 0;

bool ultimaLeituraInterruptor = HIGH;
bool estadoEstavelInterruptor = HIGH;
unsigned long instanteMudancaInterruptor = 0;

void mostrarRodape(const char *linha1, const char *linha2, uint16_t cor) {
  tela.fillRect(0, 102, tela.width(), tela.height() - 102, COR_RODAPE);
  tela.drawFastHLine(0, 102, tela.width(), 0x3333);

  tela.setTextWrap(false);
  tela.setTextSize(1);
  tela.setTextColor(cor);
  tela.setCursor(8, 106);
  tela.print(linha1);

  tela.setTextColor(ST7735_WHITE);
  tela.setCursor(8, 117);
  tela.print(linha2);
}

void mostrarAviso(const char *linha1, const char *linha2, uint16_t cor) {
  mostrarRodape(linha1, linha2, cor);
  avisoTelaAtivo = true;
  instanteAvisoTela = millis();
}

void mostrarEstadoNormal() {
  avisoTelaAtivo = false;
  mostrarRodape(
    "SISTEMA PRONTO",
    portaAberta ? "Porta aberta" : "Aproxime um cartao",
    COR_CIANO
  );
}

void desenharCartao(int16_t y, const char *titulo, const char *estado, uint16_t cor) {
  tela.fillRoundRect(5, y, tela.width() - 10, 37, 6, COR_CARTAO);
  tela.drawRoundRect(5, y, tela.width() - 10, 37, 6, 0x3333);
  tela.fillCircle(tela.width() - 19, y + 19, 5, cor);

  tela.setTextWrap(false);
  tela.setTextSize(1);
  tela.setTextColor(ST7735_WHITE);
  tela.setCursor(12, y + 5);
  tela.print(titulo);

  tela.setTextColor(cor);
  tela.setTextSize(2);
  tela.setCursor(12, y + 16);
  tela.print(estado);
}

void atualizarCartaoPorta() {
  desenharCartao(
    22,
    "PORTA",
    portaAberta ? "ABERTA" : "FECHADA",
    portaAberta ? COR_VERDE : COR_CINZA
  );
}

void atualizarCartaoLuz() {
  desenharCartao(
    62,
    "LUZ DA SACADA",
    luzAcesa ? "LIGADA" : "DESLIGADA",
    luzAcesa ? COR_AMARELO : COR_CINZA
  );
}

void iniciarTela() {
  tela.initR(INITR_BLACKTAB);
  tela.setRotation(1);
  tela.fillScreen(COR_FUNDO);

  tela.fillRect(0, 0, tela.width(), 18, COR_RODAPE);
  tela.drawFastHLine(0, 17, tela.width(), COR_CIANO);

  tela.setTextWrap(false);
  tela.setTextColor(ST7735_WHITE);
  tela.setTextSize(1);
  tela.setCursor(8, 5);
  tela.print("CASA INTELIGENTE");

  atualizarCartaoPorta();
  atualizarCartaoLuz();
  mostrarEstadoNormal();
}

void abrirPorta() {
  porta.write(ANGULO_ABERTO);
  portaAberta = true;
  instanteAbertura = millis();

  avisoVermelhoAtivo = false;

  digitalWrite(PIN_LED_VERDE_ACESSO, HIGH);
  digitalWrite(PIN_LED_VERMELHO_ACESSO, LOW);

  atualizarCartaoPorta();
  mostrarAviso("ACESSO LIBERADO", "Porta aberta", COR_VERDE);

  tone(BUZZER, 1800, 130);

  Serial.println(F("ACESSO LIBERADO - porta aberta"));
}

void fecharPorta() {
  porta.write(ANGULO_FECHADO);
  portaAberta = false;

  digitalWrite(PIN_LED_VERDE_ACESSO, LOW);

  atualizarCartaoPorta();
  mostrarAviso("PORTA FECHADA", "Aproxime um cartao", ST7735_WHITE);

  Serial.println(F("Porta fechada"));
}

bool cartaoAutorizado() {
  if (rfid.uid.size != sizeof(UID_AUTORIZADO)) {
    return false;
  }

  for (byte i = 0; i < sizeof(UID_AUTORIZADO); i++) {
    if (rfid.uid.uidByte[i] != UID_AUTORIZADO[i]) {
      return false;
    }
  }

  return true;
}

void verificarRFID() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  Serial.print(F("UID recebido: "));

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 16) {
      Serial.print('0');
    }

    Serial.print(rfid.uid.uidByte[i], HEX);

    if (i + 1 < rfid.uid.size) {
      Serial.print(':');
    }
  }

  Serial.println();

  if (cartaoAutorizado()) {
    abrirPorta();
  } else {
    digitalWrite(PIN_LED_VERMELHO_ACESSO, HIGH);

    avisoVermelhoAtivo = true;
    instanteNegado = millis();

    mostrarAviso(
      "ACESSO NEGADO",
      "Cartao nao autorizado",
      COR_VERMELHO
    );

    Serial.println(F("ACESSO NEGADO"));

    for (byte i = 0; i < 3; i++) {
      tone(BUZZER, 950, 90);
      delay(160);
    }

    noTone(BUZZER);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void verificarInterruptor() {
  bool leitura = digitalRead(INTERRUPTOR);
  unsigned long agora = millis();

  if (leitura != ultimaLeituraInterruptor) {
    instanteMudancaInterruptor = agora;
  }

  if ((agora - instanteMudancaInterruptor) >= 40UL &&
      leitura != estadoEstavelInterruptor) {

    estadoEstavelInterruptor = leitura;
    luzAcesa = (estadoEstavelInterruptor == LOW);

    digitalWrite(LED_BRANCO, luzAcesa ? HIGH : LOW);

    atualizarCartaoLuz();

    mostrarAviso(
      "LUZ DA SACADA",
      luzAcesa ? "Ligada" : "Desligada",
      COR_AMARELO
    );

    Serial.println(
      luzAcesa ? F("Luz ligada") : F("Luz desligada")
    );
  }

  ultimaLeituraInterruptor = leitura;
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_LED_VERDE_ACESSO, OUTPUT);
  pinMode(PIN_LED_VERMELHO_ACESSO, OUTPUT);
  pinMode(LED_BRANCO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(INTERRUPTOR, INPUT_PULLUP);

  digitalWrite(PIN_LED_VERDE_ACESSO, LOW);
  digitalWrite(PIN_LED_VERMELHO_ACESSO, LOW);

  ultimaLeituraInterruptor = digitalRead(INTERRUPTOR);
  estadoEstavelInterruptor = ultimaLeituraInterruptor;

  luzAcesa = (estadoEstavelInterruptor == LOW);

  digitalWrite(LED_BRANCO, luzAcesa ? HIGH : LOW);

  porta.attach(SERVO_PORTA);
  porta.write(ANGULO_FECHADO);

  pinMode(RFID_SDA_SS, OUTPUT);
  pinMode(TFT_CS, OUTPUT);

  digitalWrite(RFID_SDA_SS, HIGH);
  digitalWrite(TFT_CS, HIGH);

  SPI.begin();

  iniciarTela();
  rfid.PCD_Init();

  Serial.println(F("Casa inteligente pronta (ST7735)!"));
  Serial.println(F("No Wokwi, clique no RC522 e apresente um cartao."));
  Serial.println(F("Cartao azul = 01:02:03:04 (autorizado)."));
}

void loop() {
  verificarRFID();
  verificarInterruptor();

  unsigned long agora = millis();

  if (portaAberta &&
      (agora - instanteAbertura >= TEMPO_PORTA_ABERTA)) {
    fecharPorta();
  }

  if (avisoVermelhoAtivo &&
      (agora - instanteNegado >= TEMPO_LED_VERMELHO)) {

    avisoVermelhoAtivo = false;
    digitalWrite(PIN_LED_VERMELHO_ACESSO, LOW);
  }

  if (avisoTelaAtivo &&
      (millis() - instanteAvisoTela >= TEMPO_AVISO_TELA)) {
    mostrarEstadoNormal();
  }
}
