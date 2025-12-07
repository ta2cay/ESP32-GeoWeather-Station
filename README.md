# ESP32 GeoWeather Station (Hava ve Konum İstasyonu) 🛰️🌡️

[![Project Video](https://img.youtube.com/vi/EmByUC7QB1E/0.jpg)](https://www.youtube.com/watch?v=EmByUC7QB1E)
*(Projeyi çalışırken görmek için videoya tıklayın / Click the image to watch the project video)*

## 🇹🇷 Proje Hakkında (Turkish)

Bu proje, bir ESP32 mikrodenetleyicisi kullanarak taşınabilir bir hava durumu ve GPS konum izleme istasyonu oluşturur. Sistem, NEO-6M modülünden gerçek zamanlı GPS verilerini (enlem, boylam, hız, yükseklik) ve DHT11 sensöründen ortam sıcaklık/nem bilgilerini okur. Tüm bu veriler, anlık olarak 0.96 inçlik bir OLED ekranda kullanıcıya sunulur.

Açık havada veri toplamak, araç içi takip veya taşınabilir IoT uygulamaları için temel bir örnektir.

### Özellikler
* 📍 **Gerçek Zamanlı Konum:** Enlem, Boylam, Hız ve Yükseklik takibi.
* 🌡️ **Hava Durumu Verileri:** Ortam Sıcaklığı (°C) ve Nem (%) ölçümü.
* 📟 **Net Ekran:** Verilerin 0.96" OLED (SSD1306) üzerinde net gösterimi.
* 🔋 **Taşınabilir:** ESP32 ve uygun bir pil ile tamamen taşınabilir hale getirilebilir.

### Gerekli Malzemeler
* ESP32 Geliştirme Kartı (Örn: ESP32 DevKit V1)
* NEO-6M GPS Modülü (Antenli)
* DHT11 Sıcaklık ve Nem Sensörü
* 0.96" I2C OLED Ekran (SSD1306 Sürücülü)
* Breadboard ve Jumper Kablolar

---

## 🇬🇧 About The Project (English)

This project creates a portable weather and GPS location tracking station using an ESP32 microcontroller. The system reads real-time GPS data (latitude, longitude, speed, altitude) from a NEO-6M module and ambient temperature/humidity information from a DHT11 sensor. All this data is presented instantly to the user on a 0.96-inch OLED screen.

It is a fundamental example for outdoor data collection, in-vehicle tracking, or portable IoT applications.

### Features
* 📍 **Real-Time Location:** Tracking Latitude, Longitude, Speed, and Altitude.
* 🌡️ **Weather Data:** Measuring Ambient Temperature (°C) and Humidity (%).
* 📟 **Clear Display:** Clear indication of data on a 0.96" OLED (SSD1306).
* 🔋 **Portable:** Can be made completely portable with an ESP32 and a suitable battery.

### Hardware Required
* ESP32 Development Board (e.g., ESP32 DevKit V1)
* NEO-6M GPS Module (with Antenna)
* DHT11 Temperature and Humidity Sensor
* 0.96" I2C OLED Display (SSD1306 Driver)
* Breadboard and Jumper Wires

---

## 🛠️ Kurulum ve Bağlantı (Setup & Wiring)

### Gerekli Kütüphaneler (Required Libraries)

Bu projeyi derlemek için Arduino IDE'nize aşağıdaki kütüphaneleri (veya kodda kullanılan benzerlerini) yüklemeniz gerekir:
*(You need to install the following libraries (or similar ones used in the code) to your Arduino IDE to compile this project:)*

1.  **TinyGPS++** (GPS verilerini işlemek için / For parsing NMEA data)
2.  **Adafruit SSD1306** & **Adafruit GFX** (OLED Ekran için / For OLED display)
3.  **DHT sensor library** (by Adafruit) (DHT sensörü için / For DHT sensor)

### Bağlantı Şeması Örneği (Example Wiring)

*Not: Pin numaraları kullandığınız koda göre değişiklik gösterebilir. Lütfen `.ino` dosyasındaki tanımlamaları kontrol edin.*
*(Note: Pin numbers may vary depending on the code used. Please check the definitions in the `.ino` file.)*

| ESP32 Pin | Component | Pin On Component | Açıklama / Description |
| :--- | :--- | :--- | :--- |
| 3.3V / 5V | GPS NEO-6M | VCC | Güç / Power |
| GND | GPS NEO-6M | GND | Toprak / Ground |
| **RX2 (GPIO 16)**| GPS NEO-6M | TX | Seri Veri Alımı (Serial RX) |
| **TX2 (GPIO 17)**| GPS NEO-6M | RX | Seri Veri Gönderimi (Serial TX) |
| | | | |
| 3.3V / 5V | OLED Display| VCC | Güç / Power |
| GND | OLED Display| GND | Toprak / Ground |
| **GPIO 22 (SCL)**| OLED Display| SCL | I2C Saat (Clock) |
| **GPIO 21 (SDA)**| OLED Display| SDA | I2C Veri (Data) |
| | | | |
| 3.3V / 5V | DHT11 | VCC (+)| Güç / Power |
| GND | DHT11 | GND (-)| Toprak / Ground |
| **GPIO [PIN_NO]**| DHT11 | DATA/OUT| Veri Sinyali (Data Signal) |

**Önemli Not:** GPS modülünün dış mekanda veya pencere kenarında olması uydu sinyali alabilmesi için önemlidir. İlk açılışta sinyal bulması birkaç dakika sürebilir.
**(Important Note:** The GPS module needs to be outdoors or near a window to receive a satellite signal. It may take a few minutes to get a fix on first startup.)

## 📝 Lisans (License)

Bu proje MIT Lisansı altında lisanslanmıştır. Detaylar için `LICENSE` dosyasına bakınız.
(This project is licensed under the MIT License. See the `LICENSE` file for details.)
