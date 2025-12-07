#include <Wire.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <WebServer.h>
#include <TinyGPS++.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <DHTesp.h> // DHT11 sensörü için kütüphane

#define SCREEN_WIDTH 128      // OLED ekran genişliği, piksel cinsinden
#define SCREEN_HEIGHT 64      // OLED ekran yüksekliği, piksel cinsinden

#define OLED_RESET -1         // Reset pini (veya -1: Arduino reset piniyle paylaşılıyorsa)
#define SCREEN_ADDRESS 0x3C   // SSD1306 adresi (datasheet'e bakın)
#define OLED_SDA 19           // ESP32 pin: OLED SDA bağlantısı (GPIO 19)
#define OLED_SCL 18           // ESP32 pin: OLED SCL bağlantısı (GPIO 18)

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ OLED_SCL, /* data=*/ OLED_SDA, /* reset=*/ U8X8_PIN_NONE);

#define GPS_RX 25             // ESP32 pin: GPS modül RX bağlantısı
#define GPS_TX 26             // ESP32 pin: GPS modül TX bağlantısı
HardwareSerial gpsSerial(1);

TinyGPSPlus gps;

const char *ssid = "KULLANICI-ADINIZ";      // WiFi ağı adı
const char *password = "ŞİFRENİZ";  // WiFi şifresi

WebServer server(80);  // Web sunucusu için port numarası

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 10800);  // UTC+3 saat dilimi (3 * 3600 = 10800 saniye)

DHTesp dht; // DHTesp nesnesi oluştur

const int DHTPIN = 17; // DHT11 sensörünün bağlı olduğu pin (GPIO 15)
const int G15 = 17;    // DHTPIN olarak da kullanılacak (GPIO 15)

void handleRoot() {
    String content = "<html><head><meta charset='UTF-8'></head><body>";
    content += "<h1 style='text-align:center;font-size:30px;margin-top:10px;margin-bottom:10px;'>SWL 67 - TA2CAY <br>KONUM VE HAVA DURUMU TAKİP PROJESİ</h1>";
    content += "<p style='text-align:center;font-size:20px;margin-top:5px;margin-bottom:5px;'><B>ENLEM:</B> " + String(gps.location.lat(), 6) + "</p>";
    content += "<p style='text-align:center;font-size:20px;margin-top:5px;margin-bottom:5px;'>BOYLAM: " + String(gps.location.lng(), 6) + "</p>";
    content += "<p style='text-align:center;font-size:20px;margin-top:5px;margin-bottom:5px;'>YÜKSEKLİK: " + String(gps.altitude.meters(), 0) + "m</p>";
    content += "<p style='text-align:center;font-size:20px;margin-top:5px;margin-bottom:5px;'>HIZ: " + String(gps.speed.kmph()) + " km/h</p>";
    content += "<p style='text-align:center;font-size:20px;margin-top:5px;margin-bottom:5px;'>SICAKLIK: " + String(getTemperature()) + "°C NEM: " + String(getHumidity()) + "%</p>";
    content += "<div id='map' style='height: 300px; width: 100%;'></div>";
    content += "<script>function initMap() { var mapOptions = { center: { lat: " + String(gps.location.lat(), 6) + ", lng: " + String(gps.location.lng(), 6) + " }, zoom: 12 }; var map = new google.maps.Map(document.getElementById('map'), mapOptions); var marker = new google.maps.Marker({ position: { lat: " + String(gps.location.lat(), 6) + ", lng: " + String(gps.location.lng(), 6) + " }, map: map, title: 'Konumunuz' }); }</script>";
    content += "<script src='https://maps.googleapis.com/maps/api/js?key=API_KEY_BURAYA_YAZILACAK&callback=initMap' async defer></script>";
    content += "</body></html>";
    server.send(200, "text/html", content);
}

void setup() {
    Serial.begin(115200);
    u8g2.begin();
    delay(2000);

    // Bağlantıyı başlat
    WiFi.begin(ssid, password);
    Serial.println("Internet baglantisi gerceklestiriliyor...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Baglaniyor...");
    }

   // Bağlantı başarılı
Serial.println("BAGLANTI HAZIR");
Serial.print("TARAYICIYA YAPISTIRACAGINIZ IP ADRESI: ");
Serial.println(WiFi.localIP());

    // GPS modülünü başlat
    gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

    // NTP istemcisini başlat
    timeClient.begin();

    // DHT sensörünü başlat
    dht.setup(DHTPIN, DHTesp::DHT11); // DHT11 sensörü başlat

    // Sunucu rotalarını tanımla
    server.on("/", handleRoot);

    // Sunucuyu başlat
    server.begin();
    Serial.println("TARAYICI HAZIR VERILER OKUNABILIR");
}

void loop() {
    boolean newData = false;

    // GPS verilerini okumak için 1 saniye boyunca döngü
    for (unsigned long start = millis(); millis() - start < 1000;) {
        while (gpsSerial.available()) {
            if (gps.encode(gpsSerial.read())) {
                newData = true;
            }
        }
    }

    // Yeni GPS verisi varsa
    if (newData) {
        newData = false;
        printGPSData();
        handleRoot();  // Tarayıcı için güncellenmiş verileri gönder
    } else {
        drawNoGPSData();
    }

    // NTP istemcisini güncelle
    timeClient.update();

    server.handleClient();  // Web sunucusu isteklerini dinle
    delay(1000); // 1 saniye bekleme
}

float getTemperature() {
    return dht.getTemperature(); // Sıcaklık değerini al
}

float getHumidity() {
    return dht.getHumidity(); // Nem değerini al
}

void printGPSData() {
    u8g2.clearBuffer(); // Ekranı temizle

    // Yazı tipini değiştir
    u8g2.setFont(u8g2_font_6x12_tf); // Küçük yazı tipi

    // Tarih ve saat bilgisini al
    String dateTime = timeClient.getFormattedTime();

    // Saati ortalamak için font modunu ayarla
    u8g2.setFontMode(1); // 1: Orta

    // Tarih ve saat bilgisini ekrana yazdır
    u8g2.setCursor((SCREEN_WIDTH - u8g2.getStrWidth(dateTime.c_str())) / 2, 10); // Saati ortala
    u8g2.print(dateTime);

    // Diğer bilgileri ekrana yazdır
    u8g2.setFontMode(0); // Tekrar eski font modunu ayarla
    u8g2.setCursor(2, 20); 
    u8g2.print("Enlem: ");
    u8g2.print(gps.location.lat(), 6);

    u8g2.setCursor(2, 30);
    u8g2.print("Boylam: ");
    u8g2.print(gps.location.lng(), 6);

    u8g2.setCursor(2, 40);
    u8g2.print("Hiz: ");
    u8g2.print(gps.speed.kmph()); // Kilometre/saat cinsinden hız
    u8g2.print(" km/h");

    u8g2.setCursor(2, 50);
    u8g2.print("Rakim: ");
    u8g2.print(gps.altitude.meters(), 0); // Metreye göre yükseklik
    u8g2.print("m"); // "m" birimini ekle

    // Bağlanılan uydu sayısını ekrana yazdır
    u8g2.setCursor(2, 10); // Saat bilgisinin olduğu satırın başına gel
    u8g2.print("("); 
    u8g2.print(gps.satellites.value());
    u8g2.print(")"); 

    // Sıcaklık ve nem bilgilerini ekrana yazdır
    
    u8g2.setCursor(2, 60);
    u8g2.print("ISI:");
    u8g2.print(getTemperature());
    u8g2.print("C NEM: ");
    u8g2.print(static_cast<int>(getHumidity())); 
    u8g2.print("%");

    u8g2.sendBuffer(); // Ekranı güncelle
}

void drawNoGPSData() {
    u8g2.clearBuffer(); // Ekranı temizle

    // Yazı tipini değiştir
    u8g2.setFont(u8g2_font_6x12_tf); // Küçük yazı tipi

    u8g2.setCursor(0, 24); 
    u8g2.print("GPS UYDULARINA");
    u8g2.setCursor(0, 36); 
    u8g2.print("BAGLANAMIYOR");

    u8g2.sendBuffer(); // Ekranı güncelle
}
