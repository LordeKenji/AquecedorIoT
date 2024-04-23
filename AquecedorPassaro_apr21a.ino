/*
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/f310cad7-1205-4d14-912a-57263b9dc3fc

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  CloudSwitch rele;
  CloudTemperatureSensor temperatura;
  CloudRelativeHumidity umidade;
  bool estadoPorta;
  CloudTemperature tempDesejada;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"

//Configurações do DHT11
#include "DHT.h" //Biblioteca do DHT11
#define DHTTYPE DHT11
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  //Configurando DHT11
  dht.begin();

  //Pino do Rele(LED por enquanto)
  pinMode(2, OUTPUT);
  //Sensor Porta
  pinMode(21, INPUT);

}

int processo;
int evento;

void loop() {
  ArduinoCloud.update();
  // Your code here
  leituraSensor();

  int estadPorta = digitalRead(21);

  if (estadPorta == 1) {
    Serial.println("Porta Fechada");
    if (processo == 1) {
      Serial.println("Mudando processo de 1 para 0");
      digitalWrite(2, LOW);
      rele = 0;
      processo = 0;
      Serial.println("Feito com sucesso");
    }
  }
  if (estadPorta == 0) {
    Serial.println("Porta Aberta");
    digitalWrite(2, HIGH);
    rele = 1;
    processo = 1;
  }

  estadoPorta = estadPorta;

  Serial.println();
  Serial.print("Processo:");
  Serial.print(processo);
  Serial.println();

  Serial.println("");
  Serial.println("");

  //Controle Temperatura (Verificar mais tarde)
  float tempAtual = dht.readTemperature();
  if (tempDesejada > tempAtual) {
    digitalWrite(2, HIGH);
    rele = 1;
    //evento onde ocorre o aquecimento
    evento = 1;
  }
  if (tempDesejada < tempAtual && estadPorta == 1 && evento == 1) {
    digitalWrite(2, LOW);
    rele = 0;
    //sinaliza o fim do evento lâmpada aquecida
    evento = 0;
  }
  Serial.print("Temperatura escolhida:");
  Serial.println(tempDesejada);
  Serial.print("Situação Evento:");
  Serial.println(evento);


}

/*
  Since Rele is READ_WRITE variable, onReleChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onReleChange()  {
  if (rele == 1) {
    digitalWrite(2, HIGH);
    Serial.println(F("LED ON"));
  }
  else {
    digitalWrite(2, LOW);
    Serial.println(F("LED OFF"));
  }

}

void leituraSensor() {

  float temp = dht.readTemperature();
  float umid = dht.readHumidity();

  //Printar no monitor serial:
  Serial.print(F("Temperature: "));
  Serial.print(temp);
  Serial.println("");

  Serial.print(F("Humidity: "));
  Serial.print(umid);
  Serial.println("");

  //Transferir para things
  temperatura = temp;
  umidade = umid;
  delay(1000);
}


/*
  Since TempDesejada is READ_WRITE variable, onTempDesejadaChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTempDesejadaChange()  {
  // Add your code here to act upon TempDesejada change
}