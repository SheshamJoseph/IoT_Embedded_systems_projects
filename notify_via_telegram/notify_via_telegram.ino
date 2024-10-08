#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";

const String bot_token = "";
const String chat_id = "";

void sendTelegramMessage(String message){
	if (WiFi.status() == WL_CONNECTED){
		// send message via http request
		HTTPClient http;
		String url = "https://api.telegram.org/bot" + bot_token + "/sendMessage?chat_id=" + chat_id + "&text=" + message;
		http.begin(url);
		int httpCode = http.GET();
		if (httpCode > 0){
			Serial.println("Message sent...");
		} else {
			Serial.println("Error in sending message...");
		}
	} else {
		Serial.println("WiFi not connected...");
	}
}


void setup(){
	delay(1000);
	Serial.begin(115200);

	WiFi.begin(ssid, password);
	Serial.print("Connecting to WiFi...");
	while(WiFi.status() != WL_CONNECTED) {
		Serial,print(".");
		delay(1000);
	}
	Serial.println("WiFi connected..");;
}

void loop() {
	sendTelegramMessage("hello");
	delay(1000);
}
