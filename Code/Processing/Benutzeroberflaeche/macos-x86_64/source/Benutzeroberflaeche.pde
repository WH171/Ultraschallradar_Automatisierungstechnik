import processing.serial.*;

/**
 * @file RadarVisualisierung.pde
 * @brief High-End Radarbild exakt kalibriert auf einen Schwenkbereich von 120 Grad (30° bis 150°).
 */

Serial myPort;
String systemState = "TESTING";
String errorMessage = "";

float angle = 90;
float distance = 0;

int radarRadius = 400;
int maxDistanceCm = 40;

float[] radarWerte = new float[181]; 

void setup() {
  size(1000, 650); 
  smooth();
  for (int i = 0; i <= 180; i++) { radarWerte[i] = 0; }
  
  try {
    String portName = "/dev/cu.usbserial-A106PYCP"; 
    myPort = new Serial(this, portName, 9600);
    myPort.bufferUntil('\n');
  } catch (Exception e) {
    systemState = "ERR_PORT";
    errorMessage = "Port nicht gefunden!";
  }
}

void draw() {
  background(6, 12, 6); 
  
  pushMatrix();
  translate(width/2, height - 70);
  
  // 1. Grid-Kreise zeichnen & beschriften (Exakt im 120-Grad-Bogen)
  strokeWeight(1);
  for (int r = 100; r <= radarRadius; r += 100) {
    stroke(0, 100, 0, 150);
    noFill();
    // Bogen mathematisch begrenzt von 30° bis 150°
    arc(0, 0, r*2, r*2, radians(30 - 180), radians(155 - 180)); 
    
    fill(0, 180, 0);
    textSize(12);
    int cmText = int(map(r, 0, radarRadius, 0, maxDistanceCm));
    text(cmText + " cm", 10, -r + 5);
  }
  
  // 2. Winkel-Linien & Grad-Beschriftungen zeichnen (Kalibriert auf den 120°-Ausschnitt)
  int[] winkelCheck = {30, 60, 90, 120, 150}; // Äußere Grenzen bei 30° und 150°
  for (int w : winkelCheck) {
    stroke(0, 80, 0, 100);
    float rad = radians(w - 180);
    float lx = radarRadius * cos(rad);
    float ly = radarRadius * sin(rad);
    line(0, 0, lx, ly);
    
    float tx = (radarRadius + 25) * cos(rad);
    float ty = (radarRadius + 25) * sin(rad);
    fill(0, 230, 0);
    textAlign(CENTER, CENTER);
    text(w + "°", tx, ty);
  }
  textAlign(LEFT, BASELINE); 
  
  // 3. INTELLIGENTE KANTENERKENNUNG (Bereich angepasst auf 30° bis 150°)
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
        // Trennung bei abrupten Distanzsprüngen (> 3.5 cm)
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
  if (inObject) {
    drawCompensatedObject(startAngle, 150, sumDistance / countPins);
  }
  
  // 4. Aktueller Radarstrahl (Zeiger)
  strokeWeight(3);
  stroke(0, 255, 0, 220); 
  float pointerX = radarRadius * cos(radians(angle - 180));
  float pointerY = radarRadius * sin(radians(angle - 180));
  line(0, 0, pointerX, pointerY);
  
  popMatrix();
  
  // 5. Status-Dashboard
  fill(10, 30, 10, 220);
  stroke(0, 255, 0, 80);
  rect(15, 15, 260, 110, 8);
  
  fill(0, 255, 0);
  textSize(15);
  text("RADAR-STATUS: " + systemState, 30, 40);
  
  if (systemState.equals("OK")) {
    text("Winkel: " + int(angle) + "°", 30, 70);
    text("Entfernung: " + int(distance) + " cm", 30, 100);
  } else if (systemState.equals("TESTING")) {
    fill(255, 220, 0);
    text("Selbsttest läuft...", 30, 70);
    text("LEDs & Sensor prüfen.", 30, 100);
  } else {
    fill(255, 50, 50);
    text("HARDWARE FEHLER!", 30, 70);
    text(errorMessage, 30, 100);
  }
}

/**
 * @brief Zeichnet das gefilterte Objekt auf dem Sonar-Display.
 */
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
    ellipse(x, y, 7, 7);
  } else {
    strokeWeight(4);
    noFill();
    arc(0, 0, r*2, r*2, radians(startA - 180), radians(endA - 180));
    strokeWeight(1); 
  }
  
  float cx = r * cos(radians(centerAngle - 180));
  float cy = r * sin(radians(centerAngle - 180));
  stroke(255, 0, 0, 40);
  line(cx, cy, 0, 0);
}

void serialEvent(Serial p) {
  try {
    String inString = p.readStringUntil('\n');
    if (inString != null) {
      inString = trim(inString);
      
      if (inString.startsWith("S:")) {
        systemState = inString.substring(2);
        if (systemState.equals("ERR_SENSOR")) {
          errorMessage = "Sensor-Echo fehlt!";
        }
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
    println("Fehler: " + e.getMessage());
  }
}
