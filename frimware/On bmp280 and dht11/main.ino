#define AP_SSID "SSID"
#define AP_PASS "Пароль"

#include <DHT.h>
#include <GyverPortal.h>
#include <OneWire.h>
#include <Adafruit_BMP280.h>
GyverPortal ui;



#define DHTPIN2 15
#define DHTTYPE2 DHT11
DHT dht11(DHTPIN2, DHTTYPE2);


int valSelect1, valSelect2, val1 = 0, val2 = 0;
float temp1 = 0.0, temp2 = 0.0;
float p =0.0;
float hum = 0.0;
bool flag = 0;
String s;
//BMP280
Adafruit_BMP280 bmp;

void build() {
  GP.BUILD_BEGIN(GP_DARK);
  GP.TITLE("Умный двор", "t1");
  GP.HR();

  for (int i = 0; i < 5; i++) {
    s += "lbl/";
    s += i;
    s += ',';
  }

  GP.UPDATE(s);
  GP.BREAK();

  GP.BLOCK_BEGIN(GP_THIN, "", "Общие данные");
  GP.LABEL("Температура: ");
  GP.LABEL_BLOCK("", String("lbl/") + 0);
  GP.BREAK();
  GP.BREAK();
  GP.LABEL("Влажность: ");
  GP.LABEL_BLOCK("", String("lbl/") + 1);
  GP.BREAK();
  GP.BREAK();
  GP.LABEL("Тепература2: ");
  GP.LABEL_BLOCK("", String("lbl/") + 2);
  GP.BREAK();
  GP.LABEL("Давление: ");
  GP.LABEL_BLOCK("", String("lbl/") + 3);
  GP.BREAK();

  GP.BLOCK_END();
   M_BLOCK_TAB(                    // Блок с OTA-апдейтом
      "ESP UPDATE",                 // Имя + тип DIV
      GP.OTA_FIRMWARE();            // Кнопка с OTA начинкой
    );
  
  GP.BUILD_END();


  
}

void action() {
  String s;
  temp2 = dht11.readTemperature();
  hum = dht11.readHumidity();
   temp1 = bmp.readTemperature();
   p = bmp.readPressure();
  if (ui.update()) {
    if (ui.updateSub("lbl/0")) {
      s = temp1;
      s += " *C";
      ui.answer(s);
    } else if (ui.updateSub("lbl/1")){
      s = hum;
      s += " %";
      ui.answer(s);
    
      ui.answer(s);
    } else if (ui.updateSub("lbl/2")){
      s = temp2;
      s += " *C";
      ui.answer(s);
    }
    else if (ui.updateSub("lbl/3")){
      s = p;
      s += " bar";
      ui.answer(s);
    }
  }

}

void setup() {
 
  dht11.begin();
  bmp.begin();
  WiFi.mode(WIFI_AP);                               // Если STA режим пропущен врубаем AP
  WiFi.softAP(AP_SSID);      
       Wire.pins(0, 2);
    Wire.begin(0, 2);

  
  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
}

void loop() {
  ui.tick();
}

