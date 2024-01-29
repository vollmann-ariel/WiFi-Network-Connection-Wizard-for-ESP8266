#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>


const char *indexHtml = R"(

<!DOCTYPE html>
<html lang="en" data-darkmode="true" data-lt-installed="true">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>OpenWrt - Wireless - LuCI</title>
	<meta name="viewport" content="initial-scale=1.0">
	<style>
		:root {
			--background-color-high-hsl: var(--background-color-h), var(--background-color-s), var(--background-color-l);
			--background-color-high: hsl(var(--background-color-high-hsl));
			--background-color-medium-hsl: var(--background-color-h), var(--background-color-s), calc(var(--background-color-l) + var(--background-color-delta-l-sign) * calc(6 / 255 * 100%));
			--background-color-medium: hsl(var(--background-color-medium-hsl));
			--background-color-low-hsl: var(--background-color-h), var(--background-color-s), calc(var(--background-color-l) + var(--background-color-delta-l-sign) * calc(10 / 255 * 100%));
			--background-color-low: hsl(var(--background-color-low-hsl));
			--text-color-highest-hsl: var(--text-color-h), var(--text-color-s), var(--text-color-l);
			--text-color-highest: hsl(var(--text-color-highest-hsl));
			--text-color-high-hsl: var(--text-color-h), var(--text-color-s), calc(var(--text-color-l) + var(--text-color-delta-l-sign) * calc(64 / 255 * 100%));
			--text-color-high: hsl(var(--text-color-high-hsl));
			--text-color-medium-hsl: var(--text-color-h), var(--text-color-s), calc(var(--text-color-l) + var(--text-color-delta-l-sign) * calc(128 / 255 * 100%));
			--text-color-medium: hsl(var(--text-color-medium-hsl));
			--text-color-low-hsl: var(--text-color-h), var(--text-color-s), calc(var(--text-color-l) + var(--text-color-delta-l-sign) * calc(191 / 255 * 100%));
			--text-color-low: hsl(var(--text-color-low-hsl));
			--border-color-h: var(--background-color-h);
			--border-color-s: var(--background-color-s);
			--border-color-l: var(--background-color-l);
			--border-color-high-hsl: var(--border-color-h), var(--border-color-s), calc(var(--border-color-l) + var(--border-color-delta-l-sign) * calc(51 / 255 * 100%));
			--border-color-high: hsl(var(--border-color-high-hsl));
			--border-color-medium-hsl: var(--border-color-h), var(--border-color-s), calc(var(--border-color-l) + var(--border-color-delta-l-sign) * calc(34 / 255 * 100%));
			--border-color-medium: hsl(var(--border-color-medium-hsl));
			--border-color-low-hsl: var(--border-color-h), var(--border-color-s), calc(var(--border-color-l) + var(--border-color-delta-l-sign) * calc(17 / 255 * 100%));
			--border-color-low: hsl(var(--border-color-low-hsl));
			--on-primary-color: var(--background-color-high);
			--error-color-high: rgb(var(--error-color-high-rgb));
			--on-error-color: var(--background-color-high);
			--on-success-color: var(--background-color-high);
			--background-color-delta-l-sign: 1;
			--background-color-h: 0;
			--background-color-s: 0%;
			--background-color-l: calc(34 / 255 * 100%);
			--text-color-delta-l-sign: -1;
			--text-color-h: 0;
			--text-color-s: 0%;
			--text-color-l: 100%;
			--border-color-delta-l-sign: 1;
			--primary-color-high: #4da1c0;
			--primary-color-low: #3c7a8d;
			--error-color-high-rgb: 209, 86, 83;
			--error-color-medium: #bf4e4c;
			--error-color-low: #b14946;
			--success-color-high-rgb: 0, 166, 108;
			--success-color-medium: #00945e;
			--success-color-low: #008252;
			--warn-color-high: #a69461;
			--warn-color-medium: #a68d45;
			--warn-color-low: #a68732;
			--on-warn-color: var(--background-color-high);
			--disabled-opacity: .4;
			color-scheme: dark
		}

		*,
		::before,
		::after {
			border: 0;
			box-sizing: border-box;
			margin: 0;
			padding: 0
		}

		table {
			border-collapse: collapse;
			border-spacing: 0
		}

		html {
			font-size: 100%;
			-webkit-text-size-adjust: 100%;
			-ms-text-size-adjust: 100%;
			height: 100%
		}

		a:focus {
			outline: thin dotted
		}

		a:hover,
		a:active {
			outline: 0
		}

		button {
			font-size: 100%;
			box-sizing: border-box;
			vertical-align: baseline;
			line-height: 2em;
			margin: 0
		}

		button::-moz-focus-inner {
			border: 0;
			padding: 0
		}

		button {
			cursor: pointer;
			-webkit-appearance: button;
			word-break: break-all
		}

		body {
			background-color: var(--background-color-high);
			font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
			font-size: 13px;
			font-weight: 400;
			line-height: 18px;
			color: var(--text-color-high);
			min-height: 100%;
			display: flex;
			flex-direction: column;
			padding: 5px
		}

		.container {
			width: 100%;
			max-width: 940px;
			margin-left: auto;
			margin-right: auto;
			zoom: 1
		}

		a {
			color: var(--primary-color-high);
			text-decoration: none;
			line-height: inherit;
			font-weight: inherit
		}

		a:hover {
			color: var(--primary-color-low);
			text-decoration: underline
		}

		h3 {
			font-weight: 700;
			color: var(--text-color-high)
		}

		h3 {
			line-height: 36px
		}

		ul {
			margin: 0 0 18px 25px
		}

		ul ul {
			margin-bottom: 0
		}

		ul {
			list-style: disc
		}

		li {
			line-height: 18px;
			color: var(--text-color-medium)
		}

		strong {
			font-style: inherit;
			font-weight: 700
		}

		button {
			font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
			font-size: 13px;
			font-weight: 400;
			line-height: normal
		}

		.cbi-value-field {
			margin-left: 20px;
			flex: 1
		}

		.cbi-button,
		button {
			transition: border linear 0.2s, box-shadow linear 0.2s;
			box-shadow: inset 0 1px 3px hsla(var(--border-color-low-hsl), .01)
		}

		.cbi-button:hover,
		.cbi-button:focus,
		button:hover {
			--focus-color-rgb: 82, 168, 236;
			outline: 0;
			box-shadow: inset 0 1px 3px hsla(var(--border-color-low-hsl), .01), 0 0 8px rgba(var(--focus-color-rgb), 0.6);
			text-decoration: none;
			border-color: rgba(var(--focus-color-rgb), 0.8) !important
		}

		.cbi-page-actions {
			background: var(--background-color-low);
			margin-bottom: 18px;
			border-top: 1px solid var(--border-color-medium);
			border-radius: 0 0 3px 3px;
			text-align: right;
			padding: 17px 20px 18px 17px
		}

		.tr {
			display: table-row
		}

		.table {
			display: table;
			width: 100%;
			margin-bottom: 18px;
			font-size: 13px;
			border-collapse: collapse;
			position: relative;
			padding: 0
		}

		.table .td {
			display: table-cell;
			vertical-align: middle;
			line-height: 18px;
			text-align: left;
			padding: 10px 10px 9px
		}

		.table .td {
			border-top: 1px solid var(--border-color-medium)
		}

		header {
			position: sticky;
			top: 0;
			z-index: 800;
			overflow: visible;
			color: #BFBFBF;
			display: flex;
			background: linear-gradient(#333, #222);
			box-shadow: 0 1px 3px hsla(var(--border-color-low-hsl), .25), inset 0 -1px 0 hsla(var(--border-color-low-hsl), .01);
			margin: -5px -5px 15px;
			padding: 0 calc((100% - 940px) / 2) 0 calc((100% - 940px) / 2)
		}

		header a {
			color: #bfbfbf;
			text-shadow: 0 -1px 0 hsla(var(--border-color-low-hsl), .25)
		}

		header .brand:hover {
			background-color: rgba(255, 255, 255, 0.05);
			color: #fff;
			text-decoration: none
		}

		header .brand {
			float: left;
			display: block;
			margin-left: -15px;
			color: #fff;
			font-size: 20px;
			font-weight: 200;
			line-height: 1;
			padding: 8px 20px 12px
		}

		.nav {
			display: block;
			float: left;
			position: relative;
			left: 0;
			margin: 0 10px 0 0
		}

		.nav>li {
			display: block;
			float: left
		}

		.nav a {
			display: block;
			float: none;
			line-height: 19px;
			text-decoration: none;
			padding: 10px 10px 11px
		}

		.nav a:hover {
			color: #fff;
			text-decoration: none
		}

		.nav a.menu:hover {
			background: rgba(255, 255, 255, 0.05)
		}

		.nav .dropdown-menu {
			background-color: #333
		}

		.nav .dropdown-menu li a {
			color: #999;
			text-shadow: 0 1px 0 rgba(0, 0, 0, 0.5);
			padding: 4px 15px
		}

		.nav .dropdown-menu li a:hover {
			background-color: #191919;
			background-repeat: repeat-x;
			background-image: linear-gradient(to bottom, #292929, #191919);
			color: #fff
		}

		a.menu:after {
			width: 0;
			height: 0;
			display: inline-block;
			content: "&darr;";
			text-indent: -99999px;
			vertical-align: top;
			margin-top: 8px;
			margin-left: 4px;
			border-left: 4px solid transparent;
			border-right: 4px solid transparent;
			border-top: 4px solid #fff;
			opacity: 0.5
		}

		.dropdown-menu {
			background-color: #fff;
			float: left;
			position: absolute;
			top: 40px;
			left: -9999px;
			z-index: 900;
			min-width: 160px;
			max-width: 220px;
			margin-left: 0;
			margin-right: 0;
			zoom: 1;
			border-radius: 0 0 6px 6px;
			box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
			background-clip: padding-box;
			border-color: rgba(0, 0, 0, 0.2);
			border-style: solid;
			border-width: 0 1px 1px;
			padding: 6px 0
		}

		.dropdown-menu li {
			float: none;
			display: block;
			background-color: transparent;
			position: relative
		}

		.dropdown-menu a {
			display: block;
			clear: both;
			font-weight: 400;
			line-height: 18px;
			color: gray;
			text-shadow: 0 1px 0 #fff;
			padding: 4px 15px
		}

		.dropdown-menu a:hover {
			background-color: #ddd;
			background-repeat: repeat-x;
			background-image: linear-gradient(to bottom, #eee, #ddd);
			color: #404040;
			text-decoration: none;
			box-shadow: inset 0 1px 0 rgba(0, 0, 0, 0.025), inset 0 -1px rgba(0, 0, 0, 0.025)
		}

		.dropdown:hover ul.dropdown-menu {
			left: 0
		}

		.cbi-button {
			--default-btn-background: linear-gradient(var(--background-color-high), var(--background-color-high) 25%, var(--background-color-low));
			--on-color: var(--text-color-high);
			cursor: pointer;
			display: inline-block;
			background: var(--default-btn-background);
			color: var(--on-color);
			font-size: 13px;
			line-height: 2em;
			border: 1px solid var(--border-color-high);
			border-radius: 4px;
			white-space: pre;
			padding: 0 14px
		}

		.cbi-button-save {
			--on-color: var(--success-color-high);
			border-color: var(--on-color)
		}

		.cbi-button-reset {
			--on-color: var(--error-color-high);
			border-color: var(--on-color)
		}

		.cbi-page-actions::after {
			display: table;
			content: "";
			clear: both
		}

		.cbi-page-actions .cbi-button-save {
			--on-color: var(--on-success-color);
			background: linear-gradient(var(--success-color-medium), var(--success-color-low));
			border-color: var(--success-color-high)
		}

		.middle {
			vertical-align: middle !important
		}

		.td.cbi-section-actions {
			text-align: right;
			vertical-align: middle;
			width: 15%
		}

		.td.cbi-section-actions> :not(.cbi-dropdown)>* {
			flex: 1 1 4em;
			margin: 0 1px
		}

		ul {
			list-style: none
		}

		h3 {
			font-size: 18px
		}

		.td.cbi-section-actions>* {
			display: flex
		}

		.tabs>li>a:focus-visible,
		.cbi-tabmenu>li>a:focus-visible {
			text-decoration: underline
		}

		.dropdown {
			position: relative
		}

		.cbi-button-neutral {
			color: var(--text-color-high)
		}

		.cbi-page-actions>* {
			vertical-align: middle
		}

		header .brand {
			display: none !important
		}

		@media screen and (max-device-width: 600px) {

			.table {
				display: flex;
				flex-direction: column;
				width: 100%
			}

			.tr {
				display: flex;
				flex-direction: row;
				flex-wrap: wrap;
				align-items: flex-end;
				border-top: 1px solid var(--border-color-medium);
				margin: 0 -3px;
				padding: 5px 0
			}

			.table .td,
			.table .tr::before {
				flex: 1 1 50%;
				align-self: flex-start;
				overflow: hidden;
				text-overflow: ellipsis;
				word-wrap: break-word;
				display: inline-block;
				border-top: none;
				box-sizing: border-box;
				padding: 3px
			}

			.td.cbi-value-field {
				align-self: flex-start;
				flex-basis: 100%;
				margin-bottom: .3em
			}

			.table.cbi-section-table {
				border: none;
				background: none;
				margin: 0
			}

			.cbi-section-table-row {
				display: flex;
				flex-direction: row;
				flex-wrap: wrap;
				margin: 0 0 .5em
			}

			.td.cbi-section-actions {
				overflow: initial;
				max-width: 100%;
				flex-basis: 100%;
				padding: 3px 2px
			}

			.td.cbi-value-field[data-widget="CBI.DummyValue"] {
				flex-basis: 50%;
				text-align: left
			}

			.cbi-section-table .tr:hover .td,
			.cbi-section-table .tr:hover::before {
				background-color: transparent
			}

			.cbi-value-field {
				width: 100%;
				margin: 0
			}

			button,
			.cbi-button {
				font-size: 14px !important;
				padding: 0 8px
			}

			.cbi-page-actions {
				border-top: none;
				margin-top: -.5em;
				padding: 8px
			}

			header .nav {
				flex: 3 3 80%;
				display: flex;
				flex-wrap: wrap;
				justify-content: flex-start;
				margin: 2px 5px 2px 0
			}

			header .nav a {
				padding: 2px 6px
			}

			.dropdown-menu {
				top: 23px
			}
		}

		@media screen and (max-device-width: 375px) {
			.cbi-page-actions {
				display: flex;
				justify-content: space-between;
				margin: 0 -1px;
				padding: 0
			}

			.cbi-page-actions button {
				overflow: hidden;
				text-overflow: ellipsis
			}

			.cbi-page-actions .cbi-button {
				flex: 1;
				line-height: 2em;
				margin: 1px !important
			}

			.cbi-section-actions .cbi-button {
				overflow: hidden;
				text-overflow: ellipsis;
				margin: 1px !important
			}

			body[data-page="admin-network-wireless"] .td[data-name="_stat"] {
				flex-basis: 60%
			}

			body[data-page="admin-network-wireless"] .td.cbi-section-actions::before {
				content: none !important
			}
		}
	</style>
</head>
<body class="lang_en Wireless" data-page="admin-network-wireless">
	<header>
		<a class="brand">WireLost</a>
		<ul class="nav" id="topmenu" style="">
			<li class="dropdown"><a class="menu">Options</a>
				<ul class="dropdown-menu">
					<li><a>Menu 1</a></li>
					<li><a>Menu 2</a></li>
					<li><a>Menu 3</a></li>
					<li><a>Menu 4</a></li>
				</ul>
			</li>
			<li><a>Logout</a></li>
		</ul>	
	</header>

	<div id="maincontent" class="container">
		<div id="tabmenu" style="display:none"></div>
		<div id="view">
			<div id="cbi-wireless" class="cbi-map" data-idref="528af03c">
				<div id="cbi-wireless-wifi-device" class="cbi-section cbi-tblsection" data-idref="3de5dc87">
					<h3>Wireless Overview</h3>
					<table class="table cbi-section-table" id="body-table">


					</table>
				</div>
			</div>
			<div class="cbi-page-actions">
				<button class="cbi-button cbi-button-save" id="refresh">Refresh</button>
				<button class="cbi-button cbi-button-reset">De muestra</button>
			</div>
		</div>

	</div>
	<script>
		var webSocket = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
		webSocket.addEventListener('open', () => {
			console.log('Connect');
      webSocket.send("Prueba");
		});
		webSocket.addEventListener('close', () => {
			console.log('Disconnect');
		});
		webSocket.addEventListener('message', (event) => {
		console.log(event);
		const NEW_DATA = JSON.parse(event.data);
		if (NEW_DATA.request === LAST_REQUESTED) {
			const rangeHTML = document.createRange().createContextualFragment(NEW_DATA.html);
			document.querySelector(NEW_DATA.selector).innerHTML = '';
			document.querySelector(NEW_DATA.selector).appendChild(rangeHTML);
			}
		});
		document.querySelector('#refresh').addEventListener('click', (event) => {
			event.preventDefault();
			LAST_REQUESTED = "ssids";
			webSocket.send(JSON.stringify({
				selector: "#body-table",
				request: LAST_REQUESTED,
				data: {}
			}));
			});
	</script>
</body>

</html>

)";

ESP8266WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP_STA);

  Serial.print("Soft AP Setup");
  while(!WiFi.softAP("WireLost")){
    Serial.print(".");
    delay(100);
  }

  Serial.println("Conectándose a Wi-Fi...");
  WiFi.begin("TheLoudHouse", "romina260408");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }

  Serial.println();
  Serial.print("Soft AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Station IP address: ");
  Serial.println(WiFi.localIP());


  webSocket.onEvent(webSocketEvent);
  webSocket.begin();
  webServer.on("/", []() {webServer.send(200,"text/html",indexHtml);});
  webServer.begin();

  Serial.println("Web Server and Web Socket Online");
}

unsigned long last_10sec = 0;
unsigned int counter = 0;

void loop() {
  unsigned long t = millis();
  webSocket.loop();
  webServer.handleClient();

  if((t - last_10sec) > 10 * 1000) {
      counter++;
      bool ping = (counter % 2);
      int i = webSocket.connectedClients(ping);
      Serial.printf("%d Connected websocket clients ping: %d\n", i, ping);
      last_10sec = millis();
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
      webSocket.sendTXT(num, "Connected");
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      
      const size_t bufferSize = JSON_OBJECT_SIZE(3) + 40;
      DynamicJsonDocument doc(bufferSize);

      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("Error al deserializar JSON: ");
        Serial.println(error.c_str());
        return;
      }

      const char* selector = doc["selector"];
      const char* request = doc["request"];
      JsonObject data = doc["data"];

      // Imprimir los valores
      Serial.print("Selector: ");
      Serial.println(selector);

      Serial.print("Request: ");
      Serial.println(request);

      Serial.print("Data: ");
      serializeJson(data, Serial);
      Serial.println();

      break;
  }
}

// void handleRoot()
// {
//   String ssid;
//   int32_t rssi;
//   uint8_t encryptionType;
//   uint8_t *bssid;
//   int32_t channel;
//   bool hidden;
//   int scanResult;
//   String message;
//   scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
//   message = HtmlHead;
//   if (scanResult == 0) {
//     message += F("No se hallaron redes");
//   } else if (scanResult > 0) {
//     // Sending unsorted scan results
//     for (int8_t i = 0; i < scanResult; i++) {
//       WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
//       message += "<label for=\"red1\"><input type=\"radio\" id=\"red" + String(i) + "\" name=\"ssid\" value=\"" + ssid + "\"><div class=\"wn\">" + ssid + "</div><div class=\"wi\">Canal: " + String(channel) + " | Potencia:" + String(rssi) + "dBm</div></label>";
//       yield();
//     }
//   } else {
//     message += PSTR("WiFi scan error ") + scanResult;
//   }
//   message += HtmlScript;
//   webServer.send(200, "text/html", message);
// }

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

/* ToDo:
- setup
- web page
  - Luci
  - Socket reconnect
- handleRoot
*/