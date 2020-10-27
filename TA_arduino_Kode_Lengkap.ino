/*****************************************************************************
Judul TA : Sistem Kontrol Suhu menggunakan DHT22 berbasis IOT
Program  : Kontrol Suhu Menggunakan DHT22 dan Dimmer Modul
Chip     : ESP8266 (NodeMCU V3 Lolin)
Nama     : Grefma Aurelya Orizanty
NIM      : E32170733
Prodi    : D3-Teknik Komputer
*****************************************************************************/

/*------- Panggil library library yang dibutuhkan -------*/  
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <RBDdimmer.h>
#include <dht.h>
/*------------------------------------------------------*/

/*------- Definisikan pin yang digunakan -------*/ 
#define outputPin  12 
#define zerocross  5 
#define PinDHT 15 // pin sensor suhu 
#define kipas 14 
/*--------------------------------------------*/

dimmerLamp dimmer(outputPin, zerocross); // inisialisasi port untuk dimmer pada ESP8266
dht DHT; // Buat DHT object

const char* ssid = "TA";
const char* password = "31089969";
const char* mqtt_server = "broker.hivemq.com";
char result_h[8]; // variabel untuk menampung karakter yang akan digunakan untuk merubah float menjadi string
char result_t[8];
char result_f[8];
unsigned long interval = 1000;
unsigned long waktuAwal = 0;
long lastMsg = 0;
char msg[50];
int value = 0;

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  pinMode(kipas, OUTPUT); //inisialisasi pin kipas sebagai output
  dimmer.begin(NORMAL_MODE, OFF); //inisialisasi dimmer
  digitalWrite(kipas, HIGH);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void prosesdata() 
{
  int bacaSensor1 = DHT.read22(PinDHT); // baca Data dari sensor
  float t = DHT.temperature; // Ambil nilai Suhu
  float h = DHT.humidity; // Ambil nilai Kelembaban
  dtostrf(h, 6, 2, result_h); //konversi nilai  humidity menjadi string
  dtostrf(t, 6, 2, result_t); //konversi nilai  temperature menjadi string
}

void minggu1()
{
  int bacaSensor1 = DHT.read22(PinDHT); // baca Data dari sensor
  float t = DHT.temperature; // Ambil nilai Suhu
  if (t >= 34.0 && t <= 35.5 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (50);
  } 
  else if (t >= 35.6 && t <= 36.0 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (30);
  }  
  else if (t >= 36.1 ){
    digitalWrite(kipas, LOW);
    dimmer.setState(ON);
    dimmer.setPower (20);
  }
  else if (t <= 33.9 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (80);
  }
}

void minggu2()
{
  int bacaSensor1 = DHT.read22(PinDHT); // baca Data dari sensor
  float t = DHT.temperature; // Ambil nilai Suhu
  if (t >= 31.0 && t <= 32.5 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (50);
  } 
  else if (t >= 32.6 && t <= 33.0 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (30);
  }  
  else if (t >= 33.1 ){
    digitalWrite(kipas, LOW);
    dimmer.setState(ON);
    dimmer.setPower (20);
  }
  else if (t <= 30.9 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (70);
  }
}

void minggu3()
{
  int bacaSensor1 = DHT.read22(PinDHT); // baca Data dari sensor
  float t = DHT.temperature; // Ambil nilai Suhu
  if (t >= 28.0 && t <= 29.5 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (50);
  } 
  else if (t >= 29.6 && t <= 30.0 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (30);
  }  
  else if (t >= 30.1 ){
    digitalWrite(kipas, LOW);
    dimmer.setState(ON);
    dimmer.setPower (20);
  }
  else if (t <= 27.9 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (60);
  }
}

void minggu4()
{
  int bacaSensor1 = DHT.read22(PinDHT); // baca Data dari sensor
  float t = DHT.temperature; // Ambil nilai Suhu
  if (t >= 25.0 && t <= 26.5 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (40);
  } 
  else if (t >= 26.6 && t <= 27.0 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (30);
  }  
  else if (t >= 27.1 ){
    digitalWrite(kipas, LOW);
    dimmer.setState(OFF);
  }
  else if (t <= 24.9 ){
    digitalWrite(kipas, HIGH);
    dimmer.setState(ON);
    dimmer.setPower (50);
  }
}
//test
void keluar()
{
  digitalWrite(kipas, HIGH);
  dimmer.setState(OFF);
  
}


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/**************************************************************
Fungsi call merupakan fungsi yang akan dipanggil oleh MQTT 
jika menerima data Pesan yang dikirim melalui MQTT tersimpan 
pada payload Oleh karena itu payload lah yang akan menampung 
perintah untuk device tersebut 
*************************************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (strcmp(topic,"kandang/kipas")==0){
    if((char)payload[0] == '1')
    {
      digitalWrite(kipas, LOW);
    }
    else if ((char)payload[0] == '0'){
      digitalWrite(kipas, HIGH);
    } 
  }
  else if (strcmp(topic,"kandang/lampu")==0){
    if((char)payload[0] == '1')
    {
      dimmer.setState(ON);
    }
    else if ((char)payload[0] == '0'){
      dimmer.setState(OFF);
    } 
  }
  else if(strcmp(topic,"kandang/dimmer")==0){
    payload[length] = '\0';
    int outVal = atoi((char *)payload);
    dimmer.setPower(outVal);
  }
  else if (strcmp(topic,"kandang/otomatis")==0){
    if((char)payload[0] == 'a' ) //jika pesan yang diterima adalah 'a' maka panggil fungsi minggu1()
    {
     minggu1();
     Serial.println("program minggu1 berjalan");
    }
    else if ((char)payload[0] == 'b') //jika pesan yang diterima adalah 'b' maka panggil fungsi minggu2()
    {
      minggu2();
      Serial.println("program minggu2 berjalan");
    } 
    else if ((char)payload[0] == 'c') //jika pesan yang diterima adalah 'c' maka panggil fungsi minggu3()
    {
      minggu3();
      Serial.println("program minggu3 berjalan");
    } 
    else if ((char)payload[0] == 'd') //jika pesan yang diterima adalah 'd' maka panggil fungsi minggu4()
    {
      minggu4();
      Serial.println("program minggu4 berjalan");
    }
    else if ((char)payload[0] == 'e'){
      Serial.println("waktunya keluar");
      keluar();
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("kandang/otomatis"); //subscribe topik (kandang/otomatis)
      client.subscribe("kandang/kipas");
      client.subscribe("kandang/dimmer");
      client.subscribe("kandang/lampu");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  prosesdata(); //panggil fungsi prosesdata untuk mengambil data suhu dan kelembapan secara berulang ulang
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now; 
    Serial.print ( "temperature:");
    Serial.print (result_t); //perintah untuk menampilkan nilai suhu ke serial monitor
    Serial.println ( "derajat celcius");
    Serial.print ( "humidity:");
    Serial.println (result_h); //perintah untuk menampilkan nilai kelembapan ke serial monitor 
    client.publish("kandang/suhu",result_t); //perintah untuk mempublish nilai suhu dari dalam variabel result_t
    client.publish("kandang/kelembapan",result_h); //perintah untuk mempublish nilai kelembapan dari dalam variabel result_h
  }
}
