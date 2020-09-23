/*Preojeto Sensoriamento temperatura, umidade e luminosidade com Esp8266 e Firebase
 * Silas Almeida Santos
 * 24/08/2020
 * 
 */
//=============== Bibliotecas =========================================================================================================
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Ticker.h>
#include <DHT.h>
//==========================================================================================================================================
//=============== Diretivas ==========================================================================================================
// Set these to run example.
#define FIREBASE_HOST "...................................."           // URL do seu banco no Firebase
#define FIREBASE_AUTH "..............................." // Segredo Firebase
#define WIFI_SSID "............................................."              // Nome da sua rede
#define WIFI_PASSWORD "......................................"                           // Senha da Rede

//DHT Sensor - GPIO 5 = D1 on ESP-12E
#define DHT_PIN D1
#define DHTTYPE DHT11
#define LEDT_PIN D2

//Define LDR como A1
#define LDR A0 

// Publique a cada 5 min
#define PUBLISH_INTERVAL 1000*60*5

DHT dht(DHT_PIN, DHTTYPE);
Ticker ticker;
bool publishNewState = true;
int wifiStatus;  
//=========== Protótipos funções =========================================================================================================

void publish();
void setupWifi();
void setupFirebase();
void testwifi(); 

//============================================================================================================================================
void publish(){
  publishNewState = true;
}
//============== Inicialização =======================================================================================================
void setup() {
  Serial.begin(9600);
   dht.begin();
  pinMode(LEDT_PIN, OUTPUT);   
  setupWifi();    
  setupFirebase();

  digitalWrite(LEDT_PIN, LOW);
  // Registra o ticker para publicar de tempos em tempos
  ticker.attach_ms(PUBLISH_INTERVAL, publish);
}

//====== Função principakl ============================================================================================================
void loop() {

   testwifi();
   
  // Apenas publique quando passar o tempo determinado
  if(publishNewState){
    Serial.println("Publish new State");
    float luminosity = analogRead(LDR); //Armazena o valor lido pelo LDR   
    luminosity = (luminosity / 1024) * 100; //Transforma o valor lido em porcentagem, resoluçãoADC do Esp8266 e 10bit com tensao máxima de 1v
   
    // Obtem os dados do sensor DHT 
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    
   
    if(!isnan(humidity) && !isnan(temperature)){
      // Manda para o firebase 
      
      Firebase.pushFloat("humidity", humidity);
      Firebase.pushFloat("humidity", humidity);
      Firebase.pushFloat("temperature", temperature);
      Firebase.pushFloat("luminosity", luminosity); 
      Serial.println(temperature);
      Serial.println(humidity);
      Serial.println(luminosity);
     
      publishNewState = false;

      
      
     
      
      
    }else{
      Serial.println("Error Publishing");
    }
  }

  
  delay(2000);
}
//======================================================================================================================================

//======== Função responsável pela conexão a rede WIFI ==================================================================================
void setupWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

//====== Inicialização com Firebase =====================================================================================================
void setupFirebase(){
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 
}
//====================================================================================================================================
void testwifi() { // Verifica a conexão a rede
  wifiStatus = WiFi.status();
 
      if(wifiStatus == WL_CONNECTED){ // conectado a internet led acesso
          digitalWrite(LEDT_PIN, HIGH);
        }  
      else{
          digitalWrite(LEDT_PIN, LOW);// Sem conexão , led apagado
      }
  
}
