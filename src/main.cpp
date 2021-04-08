#include <Arduino.h>
#include "Nextion.h"
#include "max6675.h"
#include <SPI.h>
#include <SoftwareSerial.h>
#define thermoDO 9
#define thermoCS 8
#define thermoCLK 7

#define veloPin 2       //2
#define rotacaoPin A0   //A0
#define pitComfirmed 4  //4
#define sensor1 5       //5
#define sensor2 6       //6

#define tempoReset 2000

//MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

int vel = 0, rot = 0, prot = 0, combNvl = 0;
float tempo1 = 0, tempo2 = 0, tempo3 = 0, circRoda = 1.8221;
unsigned long tempo10 = 0, tempo20 = 0, tempo30 = 0;
byte isPage;
char txt0[10], txt1[10], txt2[10], txt3[10]; 

NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");
NexPage page2 = NexPage(2, 0, "page2");

//NexText txtBat                = NexText(1, 1, "bat");
//NexText txtTemperatura        = NexText(1, 2, "temperatura");
NexText txtVelo               = NexText(1, 3, "velo");
NexText txtComb               = NexText(1, 4, "comb");
NexProgressBar barRpm         = NexProgressBar(1, 5, "rpm");

void bordavel (void) {        // INTERRUPÇÃO E CALCULO DA VELOCIDADE DO CARRO NA RODA TRASEIRA
  tempo1=millis()-tempo10;
  vel = ((circRoda) / (tempo1 / 1000))*3.6;
  // horimetro = millis salva o tempo em millisegundo e passa para hora na eeprom
  tempo10=millis();  
}

void enviaVelo() {        // ENVIO DA VELOCIDADE AO DISPLAY NEXTION
  memset(txt0, 0, sizeof(txt0));
  itoa(vel, txt0, 10);
  txtVelo.setText(txt0);
}

/*void bordarot (void) {        // INTERRUPÇÃO DE SOMA PARA O CALCULO DO RPM
  prot++;
}

void calculaRpm() {        // CALCULO DE RPM PELA VELA DO MOTOR
  //detachInterrupt(rotacaoPin);
  tempo2=millis()-tempo20;
  rot = (prot/tempo2)*60000;
  tempo20=millis();
  
  int bar_value = map(rot, 0, 5000, 0, 100);
  barRpm.setValue(bar_value); // ENVIO PARA O DISPLAY
    
  prot=0;
  //attachInterrupt(digitalPinToInterrupt(rotacaoPin), bordarot, RISING);
}*/

/*void calculaTemperatura() {        // CALCULO DA TEMPERATURA NO MOTOR
    int temperatura = thermocouple.readCelsius();
    
    memset(txt2, 0, sizeof(txt2));         //
    itoa(temperatura, txt2, 10);           //Transforma inteiros para string
    txtTemperatura.setText(txt2);          //
}*/

void calculaComb() {        // CÁLCULO DO NÍVEL DO COMBUSTÍVEL 
  bool sens1 = digitalRead(sensor1), sens2 = digitalRead(sensor2);

  if (sens1 && sens2 == 0){
    combNvl  = 100; // ta cheio
  }
  if (sens1 == 0 && sens2 == 1) {
    combNvl  = 50; // meio tanque
  }
  if(sens1 && sens2 == 1) {
    combNvl = 10; // ta na reserva man!!
  }
  memset(txt3, 0, sizeof(txt3));
  itoa(combNvl, txt3, 10);
  txtComb.setText(txt3);
  
}

byte pinPitComfirmedApertado() {        // LEITURA DO BOTÃO PIT COMFIRMED EM DOIS ESTAGIOS 
  static bool estadoBotaoAnt;
  static unsigned long botaoApertado;
  static byte fase = 0;

  bool estadoBotao;
  byte estadoRet;
  
  estadoRet = 0;

  estadoBotao = digitalRead(pitComfirmed);
  if (!estadoBotao && (estadoBotao != estadoBotaoAnt)) { 
    botaoApertado = millis();
    fase = 1;
  }
  if ( (fase == 1) && (millis() - botaoApertado > tempoReset)) {
    fase = 0;
    estadoRet = 2;
  }
  if (estadoBotao && (estadoBotao != estadoBotaoAnt)) {
    if (fase == 1) {
      estadoRet = 1;
    }
    fase = 0;
  }
  estadoBotaoAnt = estadoBotao;
  return estadoRet;
}

void setup() {        // INICIO DO PROGRAMA ai carai ta funcionando o github!!!!!
  nexInit(); 

  delay(1500);
  page1.show();
  isPage = 1;
  
  pinMode(rotacaoPin, INPUT_PULLUP);
  pinMode(veloPin, INPUT_PULLUP);
  pinMode(sensor1, INPUT_PULLUP);
  pinMode(sensor2, INPUT_PULLUP);
  pinMode(pitComfirmed, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(rotacaoPin), bordarot, FALLING);
  //attachInterrupt(digitalPinToInterrupt(veloPin), bordavel, FALLING);
}

void loop() {        //INICIO DO LOOP
    if(isPage == 1 || isPage == 2) {
    //calculaRpm();
    delay(250);
    //calculaTemperatura();
    //enviaVelo();
    //calculaComb();
    //calculaBat();
    //calculaHour();
    byte estadoBotaoPit = pinPitComfirmedApertado();
    if(millis() - tempo10 > 2000)
      vel = 0;
    if(millis() - tempo20 > 2000)
      rot = 0;
    if(estadoBotaoPit == 1) {
      //1a função
      // enviar que o piloto apertou o botão para a telemetria
    }
    if(estadoBotaoPit == 2) {
      if(isPage == 1) {
        page2.show();
        isPage = 2;
      }
      else if(isPage == 2){
        page1.show();
        isPage = 1;
      }
    }    
  }  
}

/*void calculaBat() {        //CALCULO DA TENSÃO DA BATERIA
  int tensBat = map(analogRead(A0), 0, 1023, 0, 100);
  memset(txt3, 0, sizeof(txt3));
  itoa(tensBat, txt3, 10);
  txtBat.setText(txt3);
}
*/
void calculaHour() {
  if (rot != 0){
    //começa a conta o relogio
  }
}