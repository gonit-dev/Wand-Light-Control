#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

// Deteksi board dan include library yang sesuai
#ifdef ESP32
  #include <WiFi.h>
  #include <WebServer.h>
  #include <LittleFS.h>
  #define BOARD_NAME "ESP32"
  WebServer server(80);
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <LittleFS.h>
  #define BOARD_NAME "ESP8266"
  ESP8266WebServer server(80);
#else
  #error "Board harus ESP32 atau ESP8266!"
#endif

// Pin definitions
// Untuk ESP8266: D0=GPIO16, D1=GPIO5, D2=GPIO4, D3=GPIO0, D4=GPIO2, D5=GPIO14, D6=GPIO12, D7=GPIO13, D8=GPIO15
// Untuk ESP32: GPIO0-GPIO39
#define RELAY_PIN D1     // Relay pin untuk mengaktifkan LED
#define LED_PIN D4       // WS2812B data pin - Ganti jika perlu (D0, D1, D2, D3, D5, D6, D7, D8, atau GPIO lainnya di ESP32 atau ESP8266)
#define NUM_LEDS 14      // Jumlah LED - Ganti sesuai jumlah LED Anda

// WiFi Hotspot credentials
const char* ssid = "Wand_Light";          // Nama WiFi Hotspot
const char* password = "12345678";        // Password WiFi (minimal 8 karakter)

// NeoPixel object (sama seperti kode animasi lama)
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Color structure
struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

Color currentColor = {255, 200, 80}; // Default warna Lumos Gold
const char* CONFIG_FILE = "/config.json";

// Function prototypes
void connectToWiFi();
void handleRoot();
void handleGetColor();
void handleSetColor();
void updateLEDs();
void saveColor();
void loadColor();

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.print("Board: ");
  Serial.println(BOARD_NAME);
  Serial.println("========================================");
  
  // Initialize Relay Pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  delay(5000);
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("✅ Relay Activated");
  
  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("❌ LittleFS Mount Failed");
    return;
  }
  Serial.println("✅ LittleFS Mounted");
  
  // Initialize LED strip DULU
  strip.begin();
  strip.clear();
  strip.show();
  strip.setBrightness(100);
  Serial.println("✅ LED Strip Initialized");
  
  // Load saved color
  loadColor();
  
  // Update LED dengan warna yang di-load
  updateLEDs();
  
  // Connect to WiFi
  connectToWiFi();
  
  // Setup web routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/color", HTTP_GET, handleGetColor);
  server.on("/api/color", HTTP_POST, handleSetColor);
  
  // 404 handler
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });
  
  server.begin();
  Serial.println("✅ HTTP Server Started");
  Serial.print("\n⚡ Akses di: http://");
  Serial.print(WiFi.softAPIP());
  Serial.println("/\n");
}

void loop() {
  server.handleClient();
  
  #ifdef ESP8266
    yield(); // Give ESP8266 time to process background tasks
  #endif
}

void connectToWiFi() {
  Serial.print("🔍 Starting WiFi Hotspot: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  
  #ifdef ESP32
    IPAddress IP = WiFi.softAPIP();
  #elif defined(ESP8266)
    IPAddress IP = WiFi.softAPIP();
  #endif
  
  Serial.println("✅ WiFi Hotspot Started!");
  Serial.print("📡 IP Address: ");
  Serial.println(IP);
  Serial.print("🔌 SSID: ");
  Serial.println(ssid);
  Serial.print("🔐 Password: ");
  Serial.println(password);
}

void handleRoot() {
  // Baca file HTML dari LittleFS
  if (LittleFS.exists("/index.html")) {
    File file = LittleFS.open("/index.html", "r");
    if (file) {
      server.streamFile(file, "text/html");
      file.close();
    } else {
      server.send(500, "text/plain", "Failed to open index.html");
    }
  } else {
    server.send(404, "text/plain", "index.html not found. Please upload it to LittleFS.");
  }
}

void handleGetColor() {
  StaticJsonDocument<100> doc;
  doc["r"] = currentColor.r;
  doc["g"] = currentColor.g;
  doc["b"] = currentColor.b;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleSetColor() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    StaticJsonDocument<100> doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (!error) {
      currentColor.r = doc["r"] | 255;
      currentColor.g = doc["g"] | 200;
      currentColor.b = doc["b"] | 80;
      
      updateLEDs();
      saveColor();
      
      server.send(200, "application/json", "{\"status\":\"success\"}");
      Serial.println("✅ Color received and saved");
      return;
    }
  }
  
  server.send(400, "application/json", "{\"status\":\"error\"}");
  Serial.println("❌ Invalid color data");
}

void updateLEDs() {
  // Matiin semua LED dulu
  strip.clear();
  
  // Nyalain semua 14 LED dengan warna yang sama
  for(int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(currentColor.r, currentColor.g, currentColor.b));
  }
  strip.show();
  
  Serial.print("🎨 All ");
  Serial.print(NUM_LEDS);
  Serial.print(" LEDs updated: RGB(");
  Serial.print(currentColor.r);
  Serial.print(", ");
  Serial.print(currentColor.g);
  Serial.print(", ");
  Serial.print(currentColor.b);
  Serial.println(")");
}

void saveColor() {
  StaticJsonDocument<100> doc;
  doc["r"] = currentColor.r;
  doc["g"] = currentColor.g;
  doc["b"] = currentColor.b;
  
  File file = LittleFS.open(CONFIG_FILE, "w");
  if (file) {
    serializeJson(doc, file);
    file.close();
    Serial.println("💾 Color saved to LittleFS");
  } else {
    Serial.println("❌ Failed to save color");
  }
}

void loadColor() {
  if (LittleFS.exists(CONFIG_FILE)) {
    File file = LittleFS.open(CONFIG_FILE, "r");
    if (file) {
      StaticJsonDocument<100> doc;
      DeserializationError error = deserializeJson(doc, file);
      file.close();
      
      if (!error) {
        currentColor.r = doc["r"] | 255;
        currentColor.g = doc["g"] | 200;
        currentColor.b = doc["b"] | 80;
        Serial.println("📂 Color loaded from LittleFS");
      } else {
        Serial.println("⚠️  Failed to parse config file, using default");
      }
    }
  } else {
    Serial.println("⚠️  Config file not found, using default color");
  }
}