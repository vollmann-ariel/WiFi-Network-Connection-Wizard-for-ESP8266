#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>


const char PROGMEM indexHtml[] = R"(<!doctype html><html lang="es" data-bs-theme="dark"> <head> <meta charset="utf-8"> <meta name="viewport" content="width=device-width, initial-scale=1"> <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css" rel="stylesheet"> <link href="https://getbootstrap.com/docs/5.3/assets/css/docs.css" rel="stylesheet"> <title>Bootstrap Example</title> <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.bundle.min.js"></script> </head> <body class="p-3 m-0 border-0 bd-example m-0 border-0" data-bs-theme="dark"> <nav class="navbar navbar-expand-lg bg-body-tertiary"> <div class="container-fluid"> <a class="navbar-brand" href="#">Navbar</a> <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation"> <span class="navbar-toggler-icon"></span> </button> <div class="collapse navbar-collapse" id="navbarSupportedContent"> <ul class="navbar-nav me-auto mb-2 mb-lg-0"> <li class="nav-item"> <a class="nav-link active" aria-current="page" href="#">Home</a> </li> <li class="nav-item"> <a class="nav-link" id="refresh" href="#">Refresh</a> </li> <li class="nav-item dropdown"> <a class="nav-link dropdown-toggle" href="#" role="button" data-bs-toggle="dropdown" aria-expanded="false"> Dropdown </a> <ul class="dropdown-menu"> <li><a class="dropdown-item" href="#">Action</a></li> <li><a class="dropdown-item" href="#">Another action</a></li> <li><hr class="dropdown-divider"></li> <li><a class="dropdown-item" href="#">Something else here</a></li> </ul> </li> <li class="nav-item"> <a class="nav-link disabled" aria-disabled="true">Disabled</a> </li> </ul> </div> </div> </nav> <div id="container"></div> <script> var webSocket = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']); webSocket.addEventListener('open', () => { console.log('Connect'); }); webSocket.addEventListener('close', () => { console.log('Disconnect');}); webSocket.addEventListener('message', (event) => { const rxData = JSON.parse(event.data); if (rxData.request === LAST_REQUESTED) { if(rxData.selector === 'script'){ if(!document.getElementById(rxData.request)){ var Jscript = rxData.data; var bodyElement = document.querySelector("body"); var newScriptElement = document.createElement("script"); newScriptElement.id=rxData.request; newScriptElement.innerHTML = Jscript; bodyElement.appendChild(newScriptElement); } } else { if (rxData.selector === 'accordion'){ addAccordion(rxData.data); } else { container = document.getElementById(rxData.selector); if(rxData.append === false){ container.innerHTML = ''; } container.insertAdjacentHTML("beforeend", rxData.data); } } } else { console.log('unexpected request'); } }); document.querySelector('#refresh').addEventListener('click', (event) => { event.preventDefault(); LAST_REQUESTED = "ssids"; console.log("hola"); webSocket.send(JSON.stringify({ selector: "container", request: LAST_REQUESTED, data: {}, append: false })); }); </script> </body></html>)";

ESP8266WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void initAccordionHtml(uint8_t num, String request){
	JsonDocument jsonObj;
	jsonObj["selector"] = "container";
	jsonObj["request"] = request;
	jsonObj["append"] = false;
	jsonObj["data"] = "<div class=\"accordion\" id=\"wifiAccordion\"></div>";

	String jsonString;
	serializeJson(jsonObj, jsonString);
	webSocket.sendTXT(num, jsonString);
}

void initWifiScripts(uint8_t num, String request){
	JsonDocument jsonObj;
	jsonObj["selector"] = "script";
	jsonObj["request"] = request;
	jsonObj["append"] = false;
	jsonObj["data"] = R"(function togglePassword(showPassButton) { var passwordInput = showPassButton.parentNode.parentNode.querySelector("#floatingPassword"); if (showPassButton.getAttribute("aria-pressed") === "true") { passwordInput.type = "text"; console.log("text"); } else { console.log("password"); passwordInput.type = "password"; } } function addAccordion(jsonWifiInfo){ const wifiAccordion = document.getElementById("wifiAccordion"); var accordionHtml = '<div class=\"accordion-item\"> <h2 class=\"accordion-header\"> <button class=\"accordion-button collapsed\" type=\"button\" data-bs-toggle=\"collapse\" data-bs-target=\"#collapse#INDEX\" aria-expanded=\"false\" aria-controls=\"collapse#INDEX\"> #SSID </button> </h2> <div id=\"collapse#INDEX\" class=\"accordion-collapse collapse\" data-bs-parent=\"#wifiAccordion\" style=\"\"> <div class=\"accordion-body\"> <p> <small> Channel: #CHANNEL <br> Signal: #SIGNAL dB <br> Encryption: #ENCRYPTION <br> BSSID: #BSSID <br> Hidden: #HIDDEN <br> </small> </p> <div class=\"align-items-center\"> <div class=\"form-floating\"> <input type=\"password\" class=\"form-control\" id=\"floatingPassword\" placeholder=\"Password\"> <label for=\"floatingPassword\">Password</label> </div> <div class=\"btn-group-sm pt-2\" role=\"group\" aria-label=\"Small button group\"> <button id=\"showPass\" type=\"button\" class=\"btn btn-outline-danger\" data-bs-toggle=\"button\" aria-pressed=\"false\" onclick=\"togglePassword(this)\">Show Password</button> <button id=\"#SSID\" type=\"button\" class=\"btn btn-outline-success\">Connect</button> </div> </div> </div> </div></div>'; var modifiedHtml = accordionHtml.replaceAll("#INDEX", jsonWifiInfo.i); var modifiedHtml = modifiedHtml.replaceAll("#CHANNEL", jsonWifiInfo.channel); var modifiedHtml = modifiedHtml.replaceAll("#SIGNAL", jsonWifiInfo.rssi); var modifiedHtml = modifiedHtml.replaceAll("#ENCRYPTION", jsonWifiInfo.encryptionType); var modifiedHtml = modifiedHtml.replaceAll("#BSSID", jsonWifiInfo.bssid); var modifiedHtml = modifiedHtml.replaceAll("#HIDDEN", jsonWifiInfo.hidden ? "yes" : "no"); var modifiedHtml = modifiedHtml.replaceAll("#SSID", jsonWifiInfo.ssid); wifiAccordion.insertAdjacentHTML("beforeend", modifiedHtml); })";

	String jsonString;
	serializeJson(jsonObj, jsonString);
	webSocket.sendTXT(num, jsonString);
}

void publishWifiInfo(uint8_t num, String request){
	String ssid;
	int32_t rssi;
	uint8_t encryptionType;
	uint8_t *bssid;
	int32_t channel;
	bool hidden;
	int scanResult;
	JsonDocument jsonObj;
    JsonDocument networkInfo;
	String jsonString;
	char macStr[18];
	yield();
	scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
	Serial.printf("Scan done! %d networks found!\n", scanResult);
	if (scanResult == 0) {
	Serial.println("No WiFi networks found");
	} else if (scanResult > 0) {
		jsonObj["append"] = false;
		jsonObj["selector"] = "accordion";
		jsonObj["request"] = request;
		for (int8_t i = 0; i < scanResult; i++) {
			WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
			if (i==1) {
				jsonObj["append"] = true;
			}
			networkInfo["i"] = i;
			networkInfo["channel"] = channel;
			networkInfo["rssi"] = rssi;
			networkInfo["encryptionType"] = encryptionType;
			sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
			networkInfo["bssid"] = macStr;
			networkInfo["hidden"] = hidden;
			networkInfo["ssid"] = ssid;
			jsonObj["data"] = networkInfo;
			serializeJson(jsonObj, jsonString);
			webSocket.sendTXT(num, jsonString);
			Serial.printf("%2d) %s\n", i, ssid.c_str());
			yield();
		}
	} else {
	Serial.println("WiFi scan error " + scanResult);
	}
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
	IPAddress ip;

  switch(type) {
    case WStype_DISCONNECTED:
		Serial.printf("[%u] Disconnected!\n", num);
		break;
    case WStype_CONNECTED:
		ip = webSocket.remoteIP(num);
		Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
		break;
    case WStype_TEXT:
      	Serial.printf("[%u] <- This client sent this message -> %s\n", num, payload);
		JsonDocument doc;
		DeserializationError error = deserializeJson(doc, payload);
		if (error) {
			Serial.print("Error al deserializar JSON: ");
			Serial.println(error.c_str());
			return;
		}
		if(doc["request"] == "ssids") {
			initWifiScripts(num, doc["request"]);
			initAccordionHtml(num, doc["request"]);
			publishWifiInfo(num, doc["request"]);
		} else {
			Serial.print("Unexpected Request");
		}		
		break;
  }
}

void setup() {
	Serial.begin(115200);

	WiFi.mode(WIFI_AP_STA);

	Serial.println("");
	Serial.print("Soft AP Setup");
	while(!WiFi.softAP("WireLost")){
	Serial.print(".");
	delay(100);
	}

	///////////////////////////////////////////////////////  E L I M I N A R  ///////////////////////////////////////////////////////////////////
	Serial.println("\nConectándose a Wi-Fi...");
	WiFi.begin("TheLoudHouse", "romina260408");
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("Conectando...");
	}
	///////////////////////////////////////////////////////  E L I M I N A R  ///////////////////////////////////////////////////////////////////

	Serial.println();
	Serial.print("Soft AP IP address: ");
	Serial.println(WiFi.softAPIP());

	///////////////////////////////////////////////////////  E L I M I N A R  ///////////////////////////////////////////////////////////////////
	Serial.print("Station IP address: ");
	Serial.println(WiFi.localIP());
	///////////////////////////////////////////////////////  E L I M I N A R  ///////////////////////////////////////////////////////////////////

	webSocket.onEvent(webSocketEvent);
	webSocket.begin();
	webServer.on("/", []() {webServer.send(200,"text/html",indexHtml);});
	webServer.begin();

	Serial.println("Web Server and Web Socket Online");
}

void loop() {
	unsigned long last_10sec = 0;
	unsigned int counter = 0;
	while (true){
		unsigned long t = millis();
		webSocket.loop();
		webServer.handleClient();

		if((t - last_10sec) > 10 * 1000) {
			counter++;
			bool ping = (counter % 2);
			int i = webSocket.connectedClients(ping);
			//Serial.printf("%d Connected websocket clients ping: %d\n", i, ping);
			last_10sec = millis();
		}	
	}
}














// void handleConnect()
// {
//   String ssidArg = webServer.arg("ssid");
//   String passArg = webServer.arg("pass");
//   String message;
//   int contador = 0;
//   Serial.println(ssidArg);
//   Serial.println(passArg);
//   yield();
//   WiFi.begin(ssidArg, passArg);
//   Serial.println("Conectando");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//     contador++;
//     if (contador>=15)
//     {
//       webServer.send(200,"text/plain", "La conexión falló");
//       return;
//     }
//   }
//   message = "WiFi conectado. IP: " + WiFi.localIP().toString();
//   webServer.send(200, "text/plain", message); ///NO FUNCIONA POR EL CORTE, IMPLEMENTAR AJAX
//   Serial.println(message);
// }

