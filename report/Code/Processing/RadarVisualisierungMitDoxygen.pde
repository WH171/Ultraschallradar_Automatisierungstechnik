import processing.serial.*;

/**
 * @file RadarVisualisierung.pde
 * @brief Empfaengt serielle Daten vom Arduino und visualisiert diese als Radar.
 * * Diese Software dient als grafisches Frontend fuer ein Ultraschallradar.
 * Sie kommuniziert ueber den seriellen Port und stellt Hindernisse in 
 * Echtzeit auf einem simulierten Radar-Display dar.
 */

Serial myPort;
String systemState = "TESTING"; 
String errorMessage = "";

float angle = 0;
float distance = 0;

// Einstellungen fuer das Radar
int radarRadius = 400; 
int maxDistanceCm = 40; 

/**
 * @brief Initialisiert das Fenster und die serielle Kommunikation.
 */
void setup() {
  size(1000, 600);
  smooth();
  
  // COM-PORT EINRICHTUNG
  try {
    // Hier den direkten Pfad zum Mac-USB-Port eintragen
    String portName = "/dev/cu.usbmodem1401"; 
    
    myPort = new Serial(this, portName, 9600);
    myPort.bufferUntil('\n'); 
  } catch (Exception e) {
    systemState = "ERROR";
    errorMessage = "Konnte COM-Port nicht oeffnen. Falscher Port?";
  }
}

/**
 * @brief Hauptschleife der Benutzeroberflaeche. Steuert die Zustandsmaschine.
 */
void draw() {
  if (systemState.equals("ERROR")) {
    drawErrorScreen();
  } 
  else if (systemState.equals("TESTING")) {
    drawTestingScreen();
  } 
  else if (systemState.equals("OK")) {
    drawRadarScreen();
  }
}

// --- ZEICHNUNGS-FUNKTIONEN ---

/**
 * @brief Zeichnet das Radarraster, die Scan-Linie und erkannte Hindernisse.
 */
void drawRadarScreen() {
  // Simuliert das Nachleuchten eines Radars
  fill(0, 0, 0, 15); 
  noStroke();
  rect(0, 0, width, height);
  
  // Nullpunkt in die Mitte unten verschieben
  pushMatrix();
  translate(width/2, height - 20); 
  
  // 1. Raster zeichnen
  strokeWeight(1);
  stroke(0, 150, 0); // Dunkelgruen
  noFill();
  
  // Halbkreise (40cm, 30cm, 20cm, 10cm)
  arc(0, 0, radarRadius * 2, radarRadius * 2, PI, TWO_PI);
  arc(0, 0, radarRadius * 1.5, radarRadius * 1.5, PI, TWO_PI);
  arc(0, 0, radarRadius, radarRadius, PI, TWO_PI);
  arc(0, 0, radarRadius * 0.5, radarRadius * 0.5, PI, TWO_PI);
  
  // Linien fuer die Winkel zeichnen
  for (int a = 0; a <= 180; a += 30) {
    float x = radarRadius * cos(radians(a));
    float y = -radarRadius * sin(radians(a));
    line(0, 0, x, y);
  }

  // --- BESCHRIFTUNG DES RASTERS ---
  fill(0, 180, 0); 
  
  // A) Distanzen an der mittleren Achse eintragen
  textAlign(LEFT, BOTTOM);
  textSize(14);
  text("10cm", 5, -(radarRadius * 0.25));
  text("20cm", 5, -(radarRadius * 0.5));
  text("30cm", 5, -(radarRadius * 0.75));
  text("40cm", 5, -radarRadius);

  // B) Winkelzahlen am aeusseren Rand eintragen
  textSize(16);
  for (int a = 0; a <= 180; a += 30) {
    float textX = (radarRadius + 20) * cos(radians(a));
    float textY = -(radarRadius + 20) * sin(radians(a));
    
    if (a == 0) {
      textAlign(LEFT, CENTER);
    } else if (a == 180) {
      textAlign(RIGHT, CENTER);
    } else {
      textAlign(CENTER, BOTTOM); 
    }
    text(a + "°", textX, textY);
  }

  // 2. Aktuelle Scan-Linie zeichnen
  strokeWeight(4);
  stroke(0, 255, 0); // Hellgruen
  float lineX = radarRadius * cos(radians(angle));
  float lineY = -radarRadius * sin(radians(angle));
  line(0, 0, lineX, lineY);
  
  // 3. Hindernisse einzeichnen
  if (distance < maxDistanceCm && distance > 0) {
    // Pixel-Distanz berechnen
    float pixDistance = map(distance, 0, maxDistanceCm, 0, radarRadius);
    float objX = pixDistance * cos(radians(angle));
    float objY = -pixDistance * sin(radians(angle));
    
    fill(255, 0, 0); // Rot
    noStroke();
    ellipse(objX, objY, 15, 15);
  }
  popMatrix();
  
  // --- WARNMELDUNG OBEN RECHTS ---
  if (distance < maxDistanceCm && distance > 0) {
    // Blink-Effekt
    if (frameCount % 60 < 30) {
      fill(255, 0, 0);   
    } else {
      fill(100, 0, 0);   
    }
    
    stroke(255); 
    strokeWeight(2);
    rect(width - 280, 15, 260, 50, 5); 
    
    fill(255); 
    textAlign(CENTER, CENTER);
    textSize(22);
    text("OBJEKT ERKANNT!", width - 150, 40); 
  }
  
  // 4. UI Text Overlay links
  fill(0, 0, 0, 255); 
  noStroke();
  rect(15, 15, 260, 95); 
  
  stroke(0, 150, 0); 
  strokeWeight(1);
  noFill();
  rect(15, 15, 260, 95);
  
  fill(0, 255, 0);
  textAlign(LEFT, TOP);
  textSize(20);
  text("Status: SYSTEM OK", 20, 20);
  text("Winkel: " + int(angle) + "°", 20, 50);
  
  if(distance < maxDistanceCm && distance > 0) {
    text("Distanz: " + int(distance) + " cm", 20, 80);
  } else {
    text("Distanz: Ausser Reichweite", 20, 80);
  }
}

/**
 * @brief Zeigt den Ladebildschirm waehrend der Kalibrierung des Arduinos.
 */
void drawTestingScreen() {
  background(30);
  fill(255, 200, 0);
  textAlign(CENTER, CENTER);
  textSize(36);
  text("System faehrt hoch...", width/2, height/2 - 20);
  textSize(20);
  text("Hardware-Selbsttest wird durchgefuehrt.", width/2, height/2 + 30);
}

/**
 * @brief Zeigt einen Fehlerbildschirm an (z.B. fehlender COM-Port oder Sensor-Error).
 */
void drawErrorScreen() {
  background(150, 0, 0);
  fill(255);
  textAlign(CENTER, CENTER);
  textSize(50);
  text("SYSTEMFEHLER", width/2, height/2 - 60);
  
  textSize(24);
  text(errorMessage, width/2, height/2 + 20);
  
  if (frameCount % 60 < 30) {
    fill(255, 255, 0);
    text("Anlage ueberpruefen und Arduino neustarten.", width/2, height/2 + 80);
  }
}

// --- SERIELLE KOMMUNIKATION ---

/**
 * @brief Interrupt-Funktion. Wird aufgerufen, sobald serielle Daten eintreffen.
 * @param p Das Serial-Objekt, welches die Daten empfaengt.
 */
void serialEvent(Serial p) {
  try {
    String incoming = p.readStringUntil('\n');
    if (incoming != null) {
      incoming = trim(incoming);
      String[] parts = split(incoming, ':');
      
      if (parts.length == 2) {
        String type = parts[0];
        String value = parts[1];
        
        // Status verarbeiten
        if (type.equals("S")) {
          if (value.equals("OK")) systemState = "OK";
          else if (value.equals("TESTING")) systemState = "TESTING";
          else if (value.equals("ERR_SENSOR")) {
            systemState = "ERROR";
            errorMessage = "Ultraschallsensor antwortet nicht";
          }
        }
        // Radar-Daten verarbeiten
        else if (type.equals("D") && systemState.equals("OK")) {
          String[] data = split(value, ',');
          if (data.length == 2) {
            angle = float(data[0]);
            distance = float(data[1]);
          }
        }
      }
    }
  } catch (Exception e) {
    println("Fehler beim Lesen der Schnittstelle");
  }
}
