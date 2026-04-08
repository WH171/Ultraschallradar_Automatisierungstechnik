# Ultraschallradar Projekt: Umgebungs-Scan mittels Ultraschall und Servo-Motor
**Automatisierungssysteme | Prof. Dr. Wings**

---

## 📝 Projektbeschreibung
Dieses Projekt realisiert ein automatisiertes Radarsystem auf Basis des **Arduino Nano 33 BLE Sense**. Ein Servomotor rotiert einen Ultraschallsensor kontinuierlich in einem Bereich von 180°, um Hindernisse in der Umgebung zu erfassen. Die gemessenen Distanzwerte werden in Echtzeit verarbeitet und zur grafischen Darstellung an eine Benutzerpberfläche übermittelt. 

Das Ziel ist ein autonomer Scanvorgang zur digitalen Abbildung des unmittelbaren Umfelds.

## 🛠 Hardware-Komponenten
Für den Aufbau wurden folgende Komponenten verwendet:
* **Mikrocontroller:** [Arduino Nano 33 BLE Sense Lite](https://store.arduino.cc/products/arduino-nano-33-ble-sense-lite)
* **Aktorik:** Servomotor SG90
* **Sensorik:** Ultraschallsensor HC-SR04
* **Erweiterung:** Spannungsteiler, Jumperkabel
* **Gehäuse/Montage:** Eigens entwickeltes Gehäuse mit drehbarem Aufsatz in dem Ultraschallsensor integriert ist

## 💻 Software & Abhängigkeiten
Die Software ist modular aufgebaut und befindet sich im Ordner `Code`.
* **Entwicklungsumgebung:** Arduino IDE 2.x
* **Bibliotheken:** * `Servo.h` (Standardbibliothek)
  * `Ultrasonic.h` (für Grove-Sensorik)
* **Visualisierung:** Eine über Processing entwickelte Anwendung
