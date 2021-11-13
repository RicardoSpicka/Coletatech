#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Configurações Adafruit.io *********************************/
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "usuário_do_site_adafuit"
#define AIO_KEY "chave_gerada_no_site"

/******************* Configurações da Rede Wi-Fi ***************************/
const char* ssid     = "sua_SSID";
const char* password = "sua_senha";
 float val1;
 float val2;
 float val3;
 float val4;

/*************************** Configurações Globais  ***********************************/

WiFiClient client;                                                                       
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);  

/****************************** Feeds ***************************************/

Adafruit_MQTT_Publish porcentagem1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/coletora01"); 
Adafruit_MQTT_Publish porcentagem2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/coletora02"); 
Adafruit_MQTT_Publish porcentagem3 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/coletora03"); 
Adafruit_MQTT_Publish porcentagem4 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/coletora04"); 

/*************************** Laço de conexão MQTT ************************************/

void MQTT_connect();   

// Definir os números dos pinos
const int trigger = 32;
const int eco = 33;

// Definir as variáveis
long duracao;
float dist;
float aux;

/*************************** Laço de configuração************************************/

void setup() {
  
 pinMode(trigger, OUTPUT); // Configura o pino trigger como saída
 pinMode(eco, INPUT); // Configura o pino eco como entrada.
 Serial.begin(9600); // Inicia a comunicação serial
 
    // Aqui começa a conexão com a rede Wifi

    Serial.println();
    Serial.println();
    Serial.print("Connectando ao WIFI: ");
    Serial.println(ssid);
    delay(1000);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
    }

    Serial.println("");
    Serial.println("WiFi connectado");
    Serial.print("Endereço IP dinâmico: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    Serial.println();
    delay(5000);
        val1 = 0;

    
}

void loop() {


    MQTT_connect();   

         aux = 0;
         for (int a = 0; a < 3; a++){
         
         // Limpa o trigger
         digitalWrite(trigger, LOW);
         delayMicroseconds(10);
         
         // Configurar o trigger para nível alto para transmissão de sinais
         digitalWrite(trigger, HIGH);
         delayMicroseconds(10); // tempo para envio do sinal
         digitalWrite(trigger, LOW);
                   
         // Inicia contagem de tempo e lê o pino de eco
         duracao = pulseIn(eco, HIGH);
        
         // Calcular a distância
         dist = duracao * 0.034 / 2;
        
         aux = aux + dist; 
         Serial.print(dist);
         Serial.print(" <---> ");
         Serial.println(aux);

         delay (3000);
         
         }

  dist = aux/5;
  
  Serial.println();
  Serial.print("A distância média é: ");
  Serial.println(dist);

 float val1 = (((dist - 3) * 100 / 20.50) - 100) * (-1);
 float val2 = (((dist - 3) * 100 / 20.50) - 100) * (-1);
 float val3 = (((dist - 3) * 100 / 20.50) - 100) * (-1);
 float val4 = (((dist - 3) * 100 / 20.50) - 100) * (-1);

 
 if (val1 > 100) val1 = 100;
 if (val1 < 0) val1 = 0;

   Serial.print("porcentagem 01: ");
   Serial.println(val1);
      if (! porcentagem1.publish(val1))        
        Serial.println(F("Falha na publicação da porcentagem")); 
      else                                        
        Serial.println(F("Porcentagem publicada!")); 
        
   Serial.print("porcentagem 02: ");
   Serial.println(val2);
       if (! porcentagem2.publish(val2))        
          Serial.println(F("Falha na publicação da porcentagem")); 
       else                                        
          Serial.println(F("Porcentagem publicada!")); 
   
   Serial.print("porcentagem 03: ");
   Serial.println(val3);

       if (! porcentagem3.publish(val3))        
          Serial.println(F("Falha na publicação da porcentagem")); 
      else                                        
         Serial.println(F("Porcentagem publicada!")); 

   Serial.print("porcentagem 04: ");
   Serial.println(val4);

       if (! porcentagem4.publish(val4))        
          Serial.println(F("Falha na publicação da porcentagem")); 
      else                                        
         Serial.println(F("Porcentagem publicada!")); 
         

 // Aguardar 100ms antes da próxima leitura para evitar interferência
 Serial.println();
}

/*************************** Laço de conexão************************************/

void MQTT_connect() {        
  
  int8_t ret;             

  if (mqtt.connected()) {   
    return;                 
  }

  Serial.println("Conectando ao Servidor WEB ");   
  delay(2000);

  uint8_t retries = 3;      
  while ((ret = mqtt.connect()) != 0) { 
       Serial.println(mqtt.connectErrorString(ret)); 
       Serial.println("Reconectando em 5s..."); 
       mqtt.disconnect();    
       retries--;     
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("Servidor Conectado!");  
  Serial.println("");
  Serial.println("");
  Serial.println("");
  delay(2000); 
}
