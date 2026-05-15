
# 🌱 Sistem Monitoring Kelembapan Tanah dan Suhu Berbasis ESP32

## 👥 Anggota Kelompok

| Nama | NIM | Tugas |
|---|---|---|
| Nashwan Faiz Nandana Astaman | 2309106125 | Pemrograman Sistem |
| Ahmad Fauzan Ramadhan | 2309106094 | Perakitan Alat |
| Aditya Wedakarna | 2309106100 | Penyusunan Laporan |
| Muhammad Akhyat Tariq Razan | 2309106119 | Penyusunan Laporan dan Membantu Tim |

---

# 📖 Deskripsi Projek

Projek ini merupakan sistem monitoring kelembapan tanah dan suhu berbasis ESP32. Sistem menggunakan sensor soil moisture untuk membaca kondisi tanah dan sensor DS18B20 untuk membaca suhu lingkungan secara real-time.

Hasil pembacaan sensor ditampilkan pada LCD 16x2 I2C dan indikator LED. Pada projek ini, 3 LED digunakan sebagai indikator kondisi kelembapan tanah.

Sistem ini dapat membantu pengguna dalam memantau kondisi tanah sehingga lebih mudah dalam mengetahui kondisi tanaman.

---

# ⚙️ Fitur Sistem

- Monitoring kelembapan tanah secara real-time
- Monitoring suhu lingkungan
- Tampilan data pada LCD 16x2 I2C
- Indikator LED kondisi tanah
- Menggunakan ESP32 sebagai mikrokontroler utama

---

# 🌡️ Threshold Suhu

| Kondisi | Rentang Suhu |
|---|---|
| OPTIMAL | 24°C – 28°C |
| TERLALU DINGIN | < 15°C |
| TERLALU PANAS | > 33°C |
| MASIH DAPAT DITOLERANSI | Selain kondisi di atas |

---

# 💧 Threshold Kelembapan Tanah

| Nilai Sensor | Kondisi Tanah |
|---|---|
| < 1200 | SANGAT BASAH |
| < 2200 | BASAH |
| < 3200 | LEMBAB |
| ≥ 3200 | KERING |

---

# 💡 Indikator LED

| LED | Kondisi |
|---|---|
| LED Hijau | Tanah Basah / Sangat Basah |
| LED Kuning | Tanah Lembab |
| LED Merah | Tanah Kering |

---

# 🧩 Komponen yang Digunakan

- ESP32
- Sensor Temperatur DS18B20
- MH Sensor Series Soil Moisture Sensor
- LCD 16x2 I2C
- LED Merah
- LED Kuning
- LED Hijau
- Breadboard
- Kabel Jumper
- Resistor

---

# 🔌 Board Schematic

Board schematic dirancang menggunakan:
- Wokwi

## 📍 Koneksi Komponen

| Komponen | ESP32 |
|---|---|
| Soil Moisture Sensor | Pin Analog |
| DS18B20 | Pin Digital |
| LCD SDA | SDA ESP32 |
| LCD SCL | SCL ESP32 |
| LED Hijau | Pin Digital |
| LED Kuning | Pin Digital |
| LED Merah | Pin Digital |

---

# 🖥️ Alur Sistem

1. Sensor soil moisture membaca kelembapan tanah  
2. Sensor DS18B20 membaca suhu lingkungan  
3. ESP32 memproses data sensor  
4. Data ditampilkan pada LCD 16x2 I2C  
5. LED menyala sesuai kondisi tanah  

---

# 🚀 Cara Menjalankan

1. Hubungkan seluruh komponen sesuai schematic  
2. Upload program ke ESP32 menggunakan Arduino IDE  
3. Install library berikut:
   - OneWire
   - DallasTemperature
   - LiquidCrystal_I2C
4. Jalankan alat  
5. Data sensor akan tampil pada LCD  

---

# 📷 Dokumentasi

Tambahkan foto alat, schematic, dan hasil pengujian pada bagian ini.
