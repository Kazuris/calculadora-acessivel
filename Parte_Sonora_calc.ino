//Autores: Gustavo Henrique, Luan Carusi, Vinicius Lopes

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Inicia a serial por software nos pinos 12 e 13
SoftwareSerial mySoftwareSerial(12, 13); // RX, TX

DFRobotDFPlayerMini myDFPlayer;

char buf;
int volume = 24; // Inicializa o volume com 5

#define botaoMaisVolumePin  A0 // Pino do botão de aumentar volume
#define botaoMenosVolumePin A1 // Pino do botão de diminuir volume

void aumentarVolume()
{
  if (volume < 30) // Defina um limite máximo para o volume, se desejar
  {
    volume++;
    myDFPlayer.volume(volume);
    Serial.print("Volume atual: ");
    Serial.println(volume);
  }
}

void diminuirVolume()
{
  if (volume > 0) // Defina um limite mínimo para o volume, se desejar
  {
    volume--;
    myDFPlayer.volume(volume);
    Serial.print("Volume atual: ");
    Serial.println(volume);
  }
}

void setup()
{
  // Comunicacao serial com o modulo
  mySoftwareSerial.begin(9600);
  // Inicializa a serial do Arduino
  Serial.begin(115200);

  pinMode(botaoMaisVolumePin, INPUT_PULLUP); // Configura o pino como entrada com pull-up interno
  pinMode(botaoMenosVolumePin, INPUT_PULLUP); // Configura o pino como entrada com pull-up interno

  // Verifica se o modulo esta respondendo e se o
  // cartao SD foi encontrado
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini"));
  Serial.println(F("Inicializando modulo DFPlayer... (3~5 segundos)"));
  if (!myDFPlayer.begin(mySoftwareSerial))
  {
    Serial.println(F("Nao inicializado:"));
    Serial.println(F("1. Cheque as conexoes do DFPlayer Mini"));
    Serial.println(F("2. Insira um cartao SD"));
    while (true);
  }
  Serial.println();
  Serial.println(F("Modulo DFPlayer Mini inicializado!"));

  // Definicoes iniciais
  myDFPlayer.setTimeOut(500); // Timeout serial 500ms
  myDFPlayer.volume(volume); // Configura o volume inicial
  myDFPlayer.EQ(0); // Equalizacao normal

  // Mostra o menu de comandos
  Serial.println();
  Serial.print("Numero de arquivos no cartao SD: ");
  Serial.println(myDFPlayer.readFileCounts(DFPLAYER_DEVICE_SD));
  menu_opcoes();
}

void loop()
{
  // Verifica se o botão de aumentar volume foi pressionado
  if (digitalRead(botaoMaisVolumePin) == LOW)
  {
    aumentarVolume();
    delay(500); // Debounce
    menu_opcoes();
  }

  // Verifica se o botão de diminuir volume foi pressionado
  if (digitalRead(botaoMenosVolumePin) == LOW)
  {
    diminuirVolume();
    delay(500); // Debounce
    menu_opcoes();
  }

  // Aguarda a entrada de dados pela serial
  while (Serial.available() > 0)
  {
    buf = Serial.read();
    
    //Reproducao
    if ((buf >= '1') && (buf <= '9'))
    {
      Serial.print("Reproduzindo musica: ");
      Serial.println(buf);
      buf = buf - 48;
      myDFPlayer.play(buf);
      menu_opcoes();
    }
  }
}

//Exibe o menu de opções
void menu_opcoes()
{
  Serial.println();
  Serial.println(F("Comandos:"));
  Serial.println(F(" [1-9] Para selecionar o arquivo MP3"));
  Serial.println();
}
