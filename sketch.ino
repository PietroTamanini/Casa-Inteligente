
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Servo.h>

const byte SERVO_PORTA = 3;
const byte LED_VERDE = 4;
const byte LED_VERMELHO = 5;
const byte LED_BRANCO = 6;
const byte INTERRUPTOR = 7;
const byte BUZZER = 8;
const byte RFID_RST = 9;
const byte RFID_SDA_SS = 10;
const byte TFT_CS = A0;
const byte TFT_DC = A1;

const byte UID_AUTORIZADO[] = {0x01, 0x02, 0x03, 0x04};
const byte ANGULO_FECHADO = 0;
const byte ANGULO_ABERTO = 90;

const unsigned long TEMPO_PORTA_ABERTA = 5000UL;
const unsigned long TEMPO_LED_VERMELHO = 1500UL;
const unsigned long TEMPO_AVISO_TELA = 1800UL;

const uint16_t COR_FUNDO = 0x0841;
const uint16_t COR_CARTAO = 0x18E3;
const uint16_t COR_RODAPE = 0x1082;
const uint16_t COR_CINZA = 0xBDF7;

MFRC522 rfid(RFID_SDA_SS, RFID_RST);
Adafruit_ILI9341 tela(TFT_CS, TFT_DC);
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
  tela.fillRect(0, 207, 320, 33, COR_RODAPE);
  tela.drawFastHLine(0, 207, 320, 0x29A5);

  tela.setTextWrap(false);
  tela.setTextSize(2);
  tela.setTextColor(cor);
  tela.setCursor(12, 211);
  tela.print(linha1);

  tela.setTextSize(1);
  tela.setTextColor(ILI9341_WHITE);
  tela.setCursor(12, 230);
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
    ILI9341_CYAN
  );
}

void desenharCartao(int16_t y, const char *titulo, const char *estado, uint16_t cor) {
  tela.fillRoundRect(10, y, 300, 72, 10, COR_CARTAO);
  tela.drawRoundRect(10, y, 300, 72, 10, 0x39A7);
  tela.fillCircle(279, y + 36, 11, cor);

  tela.setTextWrap(false);
  tela.setTextSize(2);
  tela.setTextColor(ILI9341_WHITE);
  tela.setCursor(24, y + 11);
  tela.print(titulo);

  tela.setTextColor(cor);
  tela.setCursor(24, y + 40);
  tela.print(estado);
}

void atualizarCartaoPorta() {
  desenharCartao(
    51,
    "PORTA",
    portaAberta ? "ABERTA" : "FECHADA",
    portaAberta ? ILI9341_GREEN : COR_CINZA
  );
}

void atualizarCartaoLuz() {
  desenharCartao(
    131,
    "LUZ DA SACADA",
    luzAcesa ? "LIGADA" : "DESLIGADA",
    luzAcesa ? ILI9341_YELLOW : COR_CINZA
  );
}

void iniciarTela() {
  tela.begin();
  tela.setRotation(1);
  tela.fillScreen(COR_FUNDO);

  tela.fillRect(0, 0, 320, 42, 0x10A4);
  tela.drawFastHLine(0, 41, 320, ILI9341_CYAN);

  tela.setTextWrap(false);
  tela.setTextColor(ILI9341_WHITE);
  tela.setTextSize(2);
  tela.setCursor(18, 13);
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

  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, LOW);

  atualizarCartaoPorta();
  mostrarAviso("ACESSO LIBERADO", "Porta aberta", ILI9341_GREEN);

  tone(BUZZER, 1800, 130);

  Serial.println(F("ACESSO LIBERADO - porta aberta"));
}

void fecharPorta() {
  porta.write(ANGULO_FECHADO);
  portaAberta = false;

  digitalWrite(LED_VERDE, LOW);

  atualizarCartaoPorta();
  mostrarAviso("PORTA FECHADA", "Aproxime um cartao", ILI9341_WHITE);

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
    digitalWrite(LED_VERMELHO, HIGH);

    avisoVermelhoAtivo = true;
    instanteNegado = millis();

    mostrarAviso(
      "ACESSO NEGADO",
      "Cartao nao autorizado",
      ILI9341_RED
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
      ILI9341_YELLOW
    );

    Serial.println(
      luzAcesa ? F("Luz ligada") : F("Luz desligada")
    );
  }

  ultimaLeituraInterruptor = leitura;
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_BRANCO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(INTERRUPTOR, INPUT_PULLUP);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);

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

  Serial.println(F("Casa inteligente pronta (ILI9341)!"));
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
    digitalWrite(LED_VERMELHO, LOW);
  }

  if (avisoTelaAtivo &&
      (agora - instanteAvisoTela >= TEMPO_AVISO_TELA)) {
    mostrarEstadoNormal();
  }
}
