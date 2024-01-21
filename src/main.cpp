#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <StreamString.h>

const char* HtmlHead PROGMEM = "<!DOCTYPE html><html lang=\"es\"><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>Seleccionar Red WiFi</title> <style> body{font-family:Arial,sans-serif;background-color:#f0f0f0;color:#333;margin:20px;} form{max-width:400px;margin:0 auto;background-color:#fff;padding:20px;border-radius:8px;box-shadow:0 0 10px rgba(0,0,0,0.1);} h2{text-align:center;margin-bottom:20px;} label{display:block;margin-bottom:16px;} input[type=\"text\"],input[type=\"password\"]{width:85%;padding:8px;margin-bottom:16px;box-sizing:border-box;} #pass{width:70%;} .spb{background:none;border:none;font-size:1.5em;cursor:pointer;padding:0;margin-left:5px;} input[type=\"submit\"]{background-color:#3498db;color:#fff;padding:10px 15px;border:none;border-radius:4px;cursor:pointer;} input[type=\"submit\"]:hover{background-color:#2980b9;} @media screen and (max-width:480px){form{width:100%;}} .wn{font-weight:bold;margin-bottom:4px;} .wi{font-size:.8em;} </style></head><body> <form action=\"/conectar\" method=\"post\"> <h2>Seleccione la red</h2>";
const char* HtmlScript PROGMEM = " <label for=\"pass\">Contraseña:</label> <input type=\"password\" id=\"pass\" name=\"pass\" disabled> <button type=\"button\" class=\"spb\" onclick=\"tpv()\">🙈</button> <input type=\"submit\" value=\"Conectar\" disabled> </form> <script> function tpv(){const e=document.getElementById('pass'),t=document.querySelector('.spb');e.type==='password'?(e.type='text',t.textContent='🙉'):(e.type='password',t.textContent='🙈');} const e=document.querySelectorAll('input[type=\"radio\"]'),t=document.getElementById('pass'),n=document.querySelector('input[type=\"submit\"]');e.forEach(e=>{e.addEventListener('change',function(){t.disabled=!1,n.disabled=!1;});}); </script></body></html>";

ESP8266WebServer server(80);

void handleRoot();
void handleConnect();

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);

  //IPAddress apIP(172, 217, 28, 1);
  //WiFi.softAPConfig(apIP,apIP,IPAddress(255,255,255,0));

  while(!WiFi.softAP("Test"))
  {
  Serial.println(".");
    delay(100);
  }

  Serial.println();
  Serial.println("IP address:\t");
  Serial.println(WiFi.softAPIP());
  digitalWrite(2,0);

  server.on("/", handleRoot);
  server.on("/conectar", handleConnect);
  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {
  server.handleClient();
}

void handleRoot()
{
  String ssid;
  int32_t rssi;
  uint8_t encryptionType;
  uint8_t *bssid;
  int32_t channel;
  bool hidden;
  int scanResult;
  String message;

  scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
  message = HtmlHead;

  if (scanResult == 0) {
    message += F("No se hallaron redes");
  } else if (scanResult > 0) {
    // Sending unsorted scan results
    for (int8_t i = 0; i < scanResult; i++) {
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
      message += "<label for=\"red1\"><input type=\"radio\" id=\"red" + String(i) + "\" name=\"ssid\" value=\"" + ssid + "\"><div class=\"wn\">" + ssid + "</div><div class=\"wi\">Canal: " + String(channel) + " | Potencia:" + String(rssi) + "dBm</div></label>";
      yield();
    }
  } else {
    message += PSTR("WiFi scan error ") + scanResult;
  }
  message += HtmlScript;
  server.send(200, "text/html", message);
}

void handleConnect()
{
  String ssidArg = server.arg("ssid");
  String passArg = server.arg("pass");
  Serial.println(ssidArg);
  Serial.println(passArg);

  yield();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidArg, passArg);
  Serial.println("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi conectado. IP: ");
  Serial.println(WiFi.localIP());
}






































