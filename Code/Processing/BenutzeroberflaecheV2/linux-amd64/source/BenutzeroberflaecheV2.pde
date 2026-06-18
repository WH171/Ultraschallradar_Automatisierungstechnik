import processing.serial.*;

/**
 * @file Ultraschallradar-BenutzeroberflaecheV2.pde
 * @brief Ausfallsicheres Radar-UI mit Live-Watchdog gegen Kabelbruch und Trennung im Betrieb.
 * @details Robuste Cross-Platform-Schnittstelle für den reibungslosen Wechsel zwischen Mac und Windows.
 */

Serial myPort;
String systemState = "BOOT";        
String errorMessage = "";           
String portName = ""; 

float angle = 90;
float distance = 0;

int radarRadius;                    
int maxDistanceCm = 40;
float[] radarWerte = new float[181]; 

int lastConnectionCheck = 0;        
int lastReconnectAttempt = 0;       
int lastDataReceivedTime = 0;       ///< Zeitstempel des letzten gültigen Datenpakets (Watchdog)

void setup() {
  fullScreen();
  smooth();
  
  radarRadius = int(height * 0.65);
  for (int i = 0; i <= 180; i++) { radarWerte[i] = 0; }
  
  // Dynamic Port-Discovery vor dem ersten Verbindungsaufbau
  updateSerialPort();
  connectSerial();
}

/**
 * @brief Analysiert das Betriebssystem und filtert die verfügbaren Hardware-Ports.
 */
void updateSerialPort() {
  String[] ports = Serial.list();
  
  if (ports.length > 0) {
    if (platform == WINDOWS) {
      // ==========================================
      // WINDOWS-LOGIK: Höchsten COM-Port suchen
      // ==========================================
      int highestComIndex = -1;
      int highestComNumber = -1;
      
      for (int i = 0; i < ports.length; i++) {
        if (ports[i].toUpperCase().startsWith("COM")) {
          try {
            int comNum = int(ports[i].substring(3));
            if (comNum > highestComNumber) {
              highestComNumber = comNum;
              highestComIndex = i;
            }
          } catch (Exception e) {
            if (highestComIndex == -1) highestComIndex = i;
          }
        }
      }
      portName = (highestComIndex != -1) ? ports[highestComIndex] : ports[0];
      
    } else {
      // ==========================================
      // MAC-LOGIK: Nach USB-Serial Treffern filtern
      // ==========================================
      int macTargetIndex = -1;
      for (int i = 0; i < ports.length; i++) {
        String pLower = ports[i].toLowerCase();
        // Typische Präfixe für Arduinos/CH340/FTDI auf dem Mac
        if (pLower.contains("usbserial") || pLower.contains("usbmodem") || pLower.contains("tty.usb")) {
          macTargetIndex = i;
          break; // Ersten echten USB-Treffer direkt nehmen
        }
      }
      
      // Falls ein USB-Port gefunden wurde, nimm ihn, andernfalls Fallback auf das erste Element
      portName = (macTargetIndex != -1) ? ports[macTargetIndex] : ports[0];
    }
  }
}

void connectSerial() {
  if (portName.equals("")) return;
  try {
    myPort = new Serial(this, portName, 9600);
    myPort.bufferUntil('\n');
    systemState = "BOOT"; 
    lastDataReceivedTime = millis(); // Watchdog beim Verbinden zurücksetzen
  } catch (Exception e) {
    systemState = "ERR_PORT";
    errorMessage = "Sicherstellen, dass der Computer mit dem Radarsystem verbunden ist!";
  }
}

void draw() {
  // ==========================================
  // LIVE-WATCHDOG: Wenn im Zustand OK länger als 500ms keine Daten kommen -> Verbindungsverlust!
  // ==========================================
  if (systemState.equals("OK") && (millis() - lastDataReceivedTime > 500)) {
    systemState = "ERR_PORT";
    try {
      myPort.stop(); // Alten, toten Port sauber schließen
    } catch (Exception e) {}
  }

  // ==========================================
  // SCREEN 1 & 2: PORT-FEHLER ODER HARDWARE-FEHLER (ROTER SCREEN)
  // ==========================================
  if (systemState.equals("ERR_PORT") || systemState.equals("ERR_SENSOR")) {
    background(130, 0, 0); 
    textAlign(CENTER, CENTER);
    
    fill(255);
    textSize(height * 0.07); 
    text("SYSTEMFEHLER", width/2, height * 0.35);
    
    textSize(height * 0.035);
    if (systemState.equals("ERR_PORT")) {
      text("Sicherstellen, dass der Computer mit dem Radarsystem verbunden ist!", width/2, height * 0.52);
      
      // Versuche alle 2 Sekunden im Hintergrund neu zu verbinden
      if (millis() - lastReconnectAttempt > 2000) {
        updateSerialPort(); // Liste aktualisieren (wichtig bei nachträglichem Einstecken)
        connectSerial();
        lastReconnectAttempt = millis();
      }
    } else {
      text("Ultraschallsensor liefert keine Echosignale!", width/2, height * 0.52);
    }
    
    fill(255, 255, 0); 
    textSize(height * 0.032);
    text("Anlage überprüfen und Arduino neustarten.", width/2, height * 0.65);
    return; 
  }

  // ==========================================
  // SCREEN 3: VERBINDUNGSAUFBAU & HARDWARE-HOCHFAHREN (GRAUER SCREEN)
  // ==========================================
  if (systemState.equals("BOOT") || systemState.equals("TESTING")) {
    background(30, 30, 30); 
    textAlign(CENTER, CENTER);
    
    fill(255, 200, 0); 
    textSize(height * 0.06);
    text("System fährt hoch...", width/2, height * 0.45);
    
    fill(255, 180, 0, 180);
    textSize(height * 0.035);
    text("Hardware-Selbsttest wird durchgeführt.", width/2, height * 0.55);
    
    if (systemState.equals("BOOT") && millis() - lastConnectionCheck > 200) {
      try {
        myPort.write("P:START\n");
      } catch (Exception e) {
        systemState = "ERR_PORT";
      }
      lastConnectionCheck = millis();
    }
    return; 
  }

  // ==========================================
  // SCREEN 4: DER NORMALE LIVE-RADAR-MODUS (GRÜNER SCREEN)
  // ==========================================
  background(6, 12, 6); 
  textAlign(LEFT, BASELINE); 
  
  pushMatrix();
  translate(width/2, height - 80);
  
  // Grid-Kreise zeichnen & beschriften
  strokeWeight(1);
  for (int r = int(radarRadius*0.25); r <= radarRadius; r += int(radarRadius*0.25)) {
    stroke(0, 100, 0, 150);
    noFill();
    arc(0, 0, r*2, r*2, radians(30 - 180), radians(150 - 180)); 
    
    fill(0, 180, 0);
    textSize(18); // VERGRÖSSERT: Von 13 auf 18 für bessere Lesbarkeit am Ring
    int cmText = int(map(r, 0, radarRadius, 0, maxDistanceCm));
    text(cmText + " cm", 15, -r + 6);
  }
  
  // Winkel-Linien & Grad-Beschriftungen
  int[] winkelCheck = {30, 60, 90, 120, 150}; 
  for (int w : winkelCheck) {
    stroke(0, 80, 0, 100);
    float rad = radians(w - 180);
    float lx = radarRadius * cos(rad);
    float ly = radarRadius * sin(rad);
    line(0, 0, lx, ly);
    
    // Abstand leicht erhöht (+42 statt +30), damit die größere Schrift nicht das Grid schneidet
    float tx = (radarRadius + 42) * cos(rad);
    float ty = (radarRadius + 42) * sin(rad);
    fill(0, 230, 0);
    textAlign(CENTER, CENTER);
    textSize(20); // VERGRÖSSERT: Von 13 auf 20 für deutliche Winkelanzeige
    text(w + "°", tx, ty);
  }
  textAlign(LEFT, BASELINE); 
  
  // Kantenerkennung und Cluster-Darstellung
  boolean inObject = false;
  int startAngle = 0;
  float sumDistance = 0;
  int countPins = 0;
  float lastValidDistance = 0;
  
  for (int i = 30; i <= 150; i++) {
    float d = radarWerte[i];
    if (d > 2 && d <= maxDistanceCm) {
      if (!inObject) {
        inObject = true;
        startAngle = i;
        sumDistance = d;
        countPins = 1;
        lastValidDistance = d;
      } else {
        if (abs(d - lastValidDistance) > 3.5) {
          drawCompensatedObject(startAngle, i - 1, sumDistance / countPins);
          startAngle = i;
          sumDistance = d;
          countPins = 1;
        } else {
          sumDistance += d;
          countPins++;
        }
        lastValidDistance = d;
      }
    } else {
      if (inObject) {
        drawCompensatedObject(startAngle, i - 1, sumDistance / countPins);
        inObject = false;
      }
    }
  }
  if (inObject) { drawCompensatedObject(startAngle, 150, sumDistance / countPins); }
  
  // Aktueller Abtaststrahl
  strokeWeight(4);
  stroke(0, 255, 0, 220); 
  float pointerX = radarRadius * cos(radians(angle - 180));
  float pointerY = radarRadius * sin(radians(angle - 180));
  line(0, 0, pointerX, pointerY);
  
  popMatrix();
  
  // Dashboard im Vordergrund (Box leicht vergrößert für optimierte Textgrößen)
  fill(10, 30, 10, 220);
  stroke(0, 255, 0, 80);
  rect(30, 30, 330, 135, 8); // Box verbreitert und erhöht
  
  fill(0, 255, 0);
  textSize(20); // VERGRÖSSERT: Von 16 auf 20
  text("RADAR-SYSTEM AKTIV", 45, 65);
  textSize(18); // VERGRÖSSERT: Von 15 auf 18
  text("Winkel: " + int(angle) + "°", 45, 100);
  text("Entfernung: " + int(distance) + " cm", 45, 130);
}

void drawCompensatedObject(int startA, int endA, float avgDist) {
  int objectWidthDegrees = endA - startA;
  if (objectWidthDegrees < 2) return;
  
  float centerAngle = startA + (objectWidthDegrees / 2.0);
  float r = map(avgDist, 0, maxDistanceCm, 0, radarRadius);
  
  stroke(255, 30, 30, 220);
  fill(255, 0, 0, 200);
  
  if (objectWidthDegrees <= 7) {
    float x = r * cos(radians(centerAngle - 180));
    float y = r * sin(radians(centerAngle - 180));
    ellipse(x, y, 8, 8);
  } else {
    strokeWeight(5);
    noFill();
    arc(0, 0, r*2, r*2, radians(startA - 180), radians(endA - 180));
    strokeWeight(1); 
  }
}

void serialEvent(Serial p) {
  try {
    String inString = p.readStringUntil('\n');
    if (inString != null) {
      inString = trim(inString);
      
      // JEDES Mal, wenn IRGENDWELCHE Daten kommen, setzen wir den Watchdog zurück!
      lastDataReceivedTime = millis(); 
      
      if (inString.startsWith("S:")) {
        String state = inString.substring(2);
        if (state.equals("TESTING")) systemState = "TESTING";
        else if (state.equals("OK")) systemState = "OK";
        else if (state.equals("ERR_SENSOR")) systemState = "ERR_SENSOR";
      } 
      else if (inString.startsWith("D:") && systemState.equals("OK")) {
        String[] data = split(inString.substring(2), ',');
        if (data.length == 2) {
          angle = float(data[0]);
          distance = float(data[1]);
          int index = int(angle);
          if (index >= 0 && index <= 180) {
            radarWerte[index] = distance;
          }
        }
      }
    }
  } catch (Exception e) {
    systemState = "ERR_PORT";
  }
}
