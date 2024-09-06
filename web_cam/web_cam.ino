#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>

const char* WIFI_SSID = "blackKareem";
const char* WIFI_PASS = "Pass123#";

WebServer server(80);

static auto loRes = esp32cam::Resolution::find(640, 480);
static auto hiRes = esp32cam::Resolution::find(800, 600);

void serveJpg()
{
    auto frame = esp32cam::capture();
    if (frame == nullptr) {
        Serial.println("CAPTURE FAIL");
        server.send(503, "", "");
        return;
    }
    Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                  static_cast<int>(frame->size()));

    server.setContentLength(frame->size());
    server.send(200, "image/jpeg");
    WiFiClient client = server.client();
    frame->writeTo(client);
}

void serveMjpeg()
{
    WiFiClient client = server.client();

    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
    server.sendContent(response);

    while (client.connected()) {
        auto frame = esp32cam::capture();
        if (frame == nullptr) {
            Serial.println("CAPTURE FAIL");
            continue;
        }

        String header = "--frame\r\n";
        header += "Content-Type: image/jpeg\r\n";
        header += "Content-Length: " + String(frame->size()) + "\r\n\r\n";
        server.sendContent(header);
        frame->writeTo(client);

        String footer = "\r\n";
        server.sendContent(footer);

        delay(1);  // Adjust this value to increase frame rate (lower for faster streaming)
    }
}

void handleJpgLo()
{
    if (!esp32cam::Camera.changeResolution(loRes)) {
        Serial.println("SET-LO-RES FAIL");
    }
    serveJpg();
}

void handleJpgHi()
{
    if (!esp32cam::Camera.changeResolution(hiRes)) {
        Serial.println("SET-HI-RES FAIL");
    }
    serveJpg();
}

void handleMjpeg()
{
    if (!esp32cam::Camera.changeResolution(loRes)) {  // Use lower resolution for faster streaming
        Serial.println("SET-LO-RES FAIL");
    }
    serveMjpeg();
}

void setup() {
    Serial.begin(115200);
    Serial.println();

    {
        using namespace esp32cam;
        Config cfg;
        cfg.setPins(pins::AiThinker);
        cfg.setResolution(loRes);  // Start with low resolution for faster setup
        cfg.setBufferCount(2);
        cfg.setJpeg(10);  // Lower JPEG quality for faster transmission

        bool ok = Camera.begin(cfg);
        Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
    }

    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.print("http://");
    Serial.println(WiFi.localIP());
    Serial.println("  /cam-lo.jpg");
    Serial.println("  /cam-hi.jpg");
    Serial.println("  /stream");

    server.on("/cam-lo.jpg", handleJpgLo);
    server.on("/cam-hi.jpg", handleJpgHi);
    server.on("/stream", handleMjpeg);

    server.begin();
}

void loop() {
    server.handleClient();
}
