//https://compacttool.ru/wemos-ttgo-modul-wifi-peredatchika-esp8266-nodemcu-s-oled-ekranom-091-dyujma
/*U8g2 library: https://github.com/olikraus/u8g2
SDA D4 / GPIO2
SCL D5 / GPIO14
RST D2 / GPIO4*/

/*Arduino code for ESP8266 AJAX Webserver www.circuitdigest.com */
#define AP // Работа в режиме точки доступа (или клиента - закомментировать)

#include <ESP8266WiFi.h> // вообще всегда нужон
//#include <WiFiClient.h> // Для работы с имеющейся сетью
#include <ESP8266WebServer.h> // Для работы сервера
#include "index.h"



//////////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>

const int SensorDataPin = 2;     

OneWire oneWire(SensorDataPin);
 
DallasTemperature sensors(&oneWire);
//////////////////////////////////////////


#ifdef AP // Режим работы точки доступа
#define AP_SSID "Temperature sensor"
//#define AP_PASS "0000" // Если что - просто раскомментировать
#else // Подключение к имеющейся сети
const char* ssid = "RPI-NET";// Имя имеющейся сети
const char* password = "18598940"; // и её пароль
#endif

ESP8266WebServer server(80);
void handleRoot()
{
  String s = webpage;
  //  String s = index;
  server.send(200, "text/html", s);
}
void sensor_data()
{
  sensors.requestTemperatures(); 
  float temperature_Celsius = sensors.getTempCByIndex(0);
  float temperature_Fahrenheit = sensors.getTempFByIndex(0);
 // String sensor_value = String(temperature_Celsius);
  Serial.print("Temperature: ");
  Serial.print(temperature_Celsius);
  Serial.println(" ºC");
  Serial.print("Temperature: ");
  Serial.print(temperature_Fahrenheit);
  Serial.println(" ºF");
//  Serial.print(sensor_value);
  Serial.println("");
  delay(100);
  
  //int a = analogRead(A0);
  //int temp = a / 4.35;
  //float temp = float(random(0, 1000)) / 100;
  String sensor_value = String(temperature_Celsius);
  server.send(200, "text/plane", sensor_value);
}
void led_control()
{
  String state = "OFF";
  String act_state = server.arg("state");
  if (act_state == "1")
  { // Для есп почему-то состояния инвертированы
    digitalWrite(LED_BUILTIN, HIGH); //LED ON (включение светодиода)
    state = "ON";
    /* delay(100);
      digitalWrite(LED_BUILTIN, HIGH); //LED OFF (выключение светодиода)*/
  }  else  {// Для есп почему-то состояния инвертированы
    digitalWrite(LED_BUILTIN, LOW); //LED OFF (выключение светодиода)
    state = "OFF";
  }
  server.send(200, "text/plane", state);
}
void setup(void)
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); //LED OFF (выключение светодиода)
#ifdef AP // Если работаем в режиме точки доступа:
  WiFi.mode(WIFI_AP);
  Serial.println("...");
  Serial.print("WiFi: ");
  Serial.println(AP_SSID);
#ifdef AP_PASS // для точки доступа задан пароль...
  WiFi.softAP(AP_SSID, AP_PASS) // сеть с паролем
  Serial.print("Password: ");
  Serial.println(AP_PASS);
#else // или для точки доступа не задан пароль...
  WiFi.softAP(AP_SSID); // сеть без пароля. Указываем только имя
#endif
  Serial.print("IP address: 192.168.4.1"); // Обычный адрес есп в этой сети
#else // Работаем в режиме клиента, подключившись к имеющейся сети
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting...");
    delay(250);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif

  server.on("/", handleRoot);
  server.on("/led_set", led_control);
  server.on("/adcread", sensor_data);
  server.begin();
}
void loop(void)
{
  server.handleClient();
}
