# Wand-Light-Control
# ⚡ Wand Light Control

Sistem kontrol LED WS2812B berbasis ESP32/ESP8266 dengan antarmuka web yang elegan. Pilih warna dari berbagai preset atau buat warna kustom Anda sendiri!

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32%20%7C%20ESP8266-green.svg)

## ✨ Fitur

- 🎨 **Color Picker Intuitif** - Pilih warna dengan HSV color picker
- 🎯 **12 Preset Warna** - Warna siap pakai untuk berbagai suasana
- 📱 **Responsive Design** - Tampilan optimal di desktop, tablet, dan mobile
- 💾 **Penyimpanan Permanen** - Warna tersimpan di LittleFS dan tetap ada setelah restart
- 🔌 **WiFi Hotspot** - Buat hotspot sendiri, tidak perlu router
- ⚡ **Real-time Update** - Perubahan warna langsung terlihat
- 🎯 **Dual Input** - Input RGB dan HEX

## 📋 Kebutuhan Hardware

### Komponen Utama
- **ESP32** atau **ESP8266** (NodeMCU, Wemos D1 Mini, dll)
- **LED Strip WS2812B** (14 LED atau sesuaikan jumlahnya)
- **Relay Module 5V** (1 channel)
- **Power Supply 5V** (sesuai jumlah LED, minimal 2A untuk 14 LED)
- **Kabel Jumper**

### Spesifikasi LED Strip
- Tipe: WS2812B (NeoPixel)
- Voltage: 5V DC
- Konsumsi daya: ~60mA per LED (maksimal @ full white)
- Jumlah default: 14 LED (dapat diubah)

## 🔌 Koneksi Hardware

### Pin Mapping

#### ESP8266 (NodeMCU)
```
D1 (GPIO5)  → Relay IN
D4 (GPIO2)  → LED Data IN
GND         → Relay GND & LED GND
5V          → Relay VCC & LED 5V (gunakan power supply eksternal)
```

#### ESP32
```
GPIO5       → Relay IN
GPIO2       → LED Data IN
GND         → Relay GND & LED GND
5V          → Relay VCC & LED 5V (gunakan power supply eksternal)
```

### Diagram Koneksi
```
Power Supply 5V
    ├─→ ESP32/ESP8266 (5V)
    ├─→ Relay Module (VCC)
    └─→ LED Strip (5V) via Relay

ESP32/ESP8266
    ├─→ Relay (GPIO5/D1)
    ├─→ LED Strip Data (GPIO2/D4)
    └─→ GND (Common Ground)
```

⚠️ **Penting**: Gunakan power supply eksternal untuk LED strip. Jangan langsung dari pin ESP!

## 📦 Instalasi Software

### 1. Install Arduino IDE
Download dan install [Arduino IDE](https://www.arduino.cc/en/software) (versi 2.x atau 1.8.x)

### 2. Install Board ESP
**Untuk ESP32:**
1. Buka `File` → `Preferences`
2. Tambahkan URL berikut ke "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Buka `Tools` → `Board` → `Board Manager`
4. Cari "ESP32" dan install

**Untuk ESP8266:**
1. Tambahkan URL ke preferences:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
2. Install dari Board Manager

### 3. Install Library yang Diperlukan

Buka `Tools` → `Manage Libraries`, cari dan install:

- **Adafruit NeoPixel** (untuk kontrol LED WS2812B)
- **ArduinoJson** (versi 6.x untuk parsing JSON)
- **LittleFS** (sudah built-in di ESP32/ESP8266 core terbaru)

### 4. Upload Filesystem (LittleFS)

#### Install Plugin LittleFS
- **ESP32**: [Arduino ESP32 LittleFS Plugin](https://github.com/lorol/arduino-esp32littlefs-plugin)
- **ESP8266**: [Arduino ESP8266 LittleFS Plugin](https://github.com/earlephilhower/arduino-esp8266littlefs-plugin)

#### Upload File HTML
1. Buat folder `data` di folder project Arduino Anda
2. Copy file `index.html` ke folder `data`
3. Struktur folder:
   ```
   wand_light_control/
   ├── wand_light_control.ino
   └── data/
       └── index.html
   ```
4. Di Arduino IDE: `Tools` → `ESP32/ESP8266 LittleFS Data Upload`
5. Tunggu hingga upload selesai

### 5. Upload Sketch Arduino

1. Buka file `color_led.ino`
2. **Konfigurasi Pin** (jika perlu):
   ```cpp
   #define RELAY_PIN D1     // Relay pin
   #define LED_PIN D4       // WS2812B data pin
   #define NUM_LEDS 14      // Jumlah LED
   ```
3. **Konfigurasi WiFi**:
   ```cpp
   const char* ssid = "Wand_Light";      // Nama hotspot
   const char* password = "12345678";    // Password (min 8 karakter)
   ```
4. Pilih board: `Tools` → `Board` → Pilih ESP32/ESP8266 Anda
5. Pilih port: `Tools` → `Port` → Pilih port COM yang sesuai
6. Upload sketch: `Sketch` → `Upload` atau tekan `Ctrl+U`

## 🚀 Cara Penggunaan

1. **Power On** - Nyalakan ESP dan LED strip
2. **Hubungkan ke WiFi**:
   - SSID: `Wand_Light`
   - Password: `12345678`
3. **Buka Browser** - Akses `http://192.168.4.1`
4. **Pilih Warna**:
   - Gunakan color picker
   - Input RGB/HEX manual
   - Atau pilih dari 12 preset warna
5. **Klik OK** - Warna akan tersimpan dan LED akan update

### 12 Preset Warna Tersedia
- 🌟 **Lumos** - Warm golden light
- 🦌 **Patronum** - Silver-blue ethereal
- ⚡ **Stupefy** - Red stunning color
- 💚 **Avada** - Bright green
- 💜 **Crucio** - Purple violet
- 🔵 **Protego** - Protective blue
- 💛 **Expell** - Bright yellow
- 🔴 **Diffindo** - Cutting red
- Plus warna dasar: White, Red, Green, Blue

## 🛠️ Troubleshooting

### LED Tidak Menyala
- ✅ Cek koneksi power supply (5V dengan arus cukup)
- ✅ Pastikan pin `LED_PIN` sesuai dengan koneksi hardware
- ✅ Cek orientasi LED strip (Data IN ke ESP, bukan Data OUT)
- ✅ Test dengan jumlah LED lebih sedikit terlebih dahulu

### Tidak Bisa Connect ke WiFi
- ✅ Pastikan SSID dan password benar (cek Serial Monitor)
- ✅ Restart ESP dan coba lagi
- ✅ Cek apakah device WiFi Anda support 2.4GHz (ESP tidak support 5GHz)

### Website Tidak Muncul
- ✅ Pastikan file `index.html` sudah di-upload ke LittleFS
- ✅ Cek Serial Monitor untuk error LittleFS
- ✅ Akses `http://192.168.4.1/` (jangan lupa trailing slash)

### Warna Tidak Sesuai
- ✅ Pastikan `NEO_GRB` atau `NEO_RGB` sesuai dengan LED strip Anda
- ✅ Coba ubah di kode: `NEO_GRB + NEO_KHZ800` atau `NEO_RGB + NEO_KHZ800`

### Upload LittleFS Gagal
- ✅ Tutup Serial Monitor sebelum upload
- ✅ Pastikan plugin LittleFS terinstall dengan benar
- ✅ Coba gunakan baud rate lebih rendah

## 📝 Kustomisasi

### Mengubah Jumlah LED
```cpp
#define NUM_LEDS 14  // Ubah sesuai jumlah LED Anda
```

### Mengubah Pin
```cpp
#define RELAY_PIN D1  // Ubah sesuai koneksi relay
#define LED_PIN D4    // Ubah sesuai koneksi LED data
```

### Mengubah WiFi Credentials
```cpp
const char* ssid = "Wand_Light";
const char* password = "12345678";
```

### Mengubah Brightness
```cpp
strip.setBrightness(100);  // 0-255, default 100
```

### Menambah Preset Warna
Edit file `index.html`, tambahkan button baru di section presets:
```html
<button class="preset-btn" 
        style="background: rgb(255, 0, 255);" 
        data-r="255" data-g="0" data-b="255" 
        title="Custom">Custom</button>
```

## 🔍 Serial Monitor Output

Setelah upload, buka Serial Monitor (115200 baud) untuk melihat:
```
========================================
Board: ESP32
========================================
✅ Relay Activated
✅ LittleFS Mounted
✅ LED Strip Initialized
📂 Color loaded from LittleFS
🔍 Starting WiFi Hotspot: Wand_Light
✅ WiFi Hotspot Started!
📡 IP Address: 192.168.4.1
📌 SSID: Wand_Light
🔐 Password: 12345678
✅ HTTP Server Started

⚡ Akses di: http://192.168.4.1/
```
- Menggunakan Adafruit NeoPixel library
- UI design menggunakan modern gradient dan glassmorphism

---
