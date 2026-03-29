# AS Radar Projekt: Umgebungs-Scan mittels Ultraschall und Servo
**Automatisierungssysteme | Prof. Dr. Wings**

---

## 📝 Projektbeschreibung
Dieses Projekt realisiert ein automatisiertes Radarsystem auf Basis des **Arduino Nano 33 BLE Sense Lite**. Ein Servomotor rotiert einen Ultraschall-Abstandssensor kontinuierlich in einem Bereich von 180°, um Hindernisse in der Umgebung zu erfassen. Die gemessenen Distanzwerte werden in Echtzeit verarbeitet und zur grafischen Darstellung an ein Host-System übermittelt. 

Das Ziel ist ein präziser, autonomer Scanvorgang zur digitalen Abbildung des unmittelbaren Umfelds.

## 🛠 Hardware-Komponenten
Für den Aufbau wurden folgende Komponenten verwendet:
* **Mikrocontroller:** [Arduino Nano 33 BLE Sense Lite](https://store.arduino.cc/products/arduino-nano-33-ble-sense-lite)
* **Aktorik:** 180°-Servomotor (SG90 oder kompatibel)
* **Sensorik:** Ultraschall-Abstandssensor (Grove - Ultrasonic Distance Sensor / HC-SR04)
* **Erweiterung:** TinyML Shield / Grove Base Shield
* **Gehäuse/Montage:** Eigens entwickelte Halterung für die Sensor-Servo-Einheit

## 💻 Software & Abhängigkeiten
Die Software ist modular aufgebaut und befindet sich im Ordner `/04_Software`.
* **Entwicklungsumgebung:** Arduino IDE 2.x
* **Bibliotheken:** * `Servo.h` (Standardbibliothek)
  * `Ultrasonic.h` (für Grove-Sensorik)
* **Visualisierung:** [Link zum Skript/Tool, z.B. Processing oder Python]

## 🚀 Installation & Inbetriebnahme
1. **Hardware-Setup:** Verbinden Sie den Servo mit Pin D3 und den Ultraschallsensor mit Pin D2 am Shield.
2. **Repository klonen:** ```bash
   git clone [https://github.com/](https://github.com/)[Dein-Benutzername]/[Repo-Name].git
