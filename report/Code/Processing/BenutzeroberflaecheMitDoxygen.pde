import processing.serial.*;

/**
 * @file BenutzeroberflaecheMitDoxygen.pde
 * @brief Grafische Benutzeroberfläche (Frontend) für ein 120-Grad-Ultraschallradar.
 * @details Diese Software empfängt Messdaten (Winkel und Distanz) in Echtzeit über die 
 * serielle Schnittstelle von einem Arduino Nano V3. Die Daten werden gefiltert, 
 * mittels einer intelligenten Kantenerkennung mathematisch von der physikalischen 
 * Schallkegel-Streuung bereinigt und auf einem animierten Sonar-Bildschirm dargestellt.
 * * @author Wilko Hinrichs
 * @date 2026-06-08
 * @version 1.2
 */

/* --- GLOBALE OBJEKTE & STATUSVARIABLEN --- */
Serial myPort;                      ///< Das serielle Schnittstellen-Objekt zur Hardware-Kommunikation.
String systemState = "TESTING";     ///< Aktueller Systemstatus des Radars ("TESTING", "OK", "ERR_SENSOR", "ERR_PORT").
String errorMessage = "";           ///< Klartext-Fehlermeldung bei Hardware- oder Portproblemen.

float angle = 90;                   ///< Der aktuell vom Arduino übermittelte Schwenkwinkel (30° bis 150°).
float distance = 0;                 ///< Die aktuell gemessene Entfernung des Hindernisses in Zentimetern.

/* --- VISUALISIERUNGS-EINSTELLUNGEN --- */
int radarRadius = 400;              ///< Maximaler Zeichnungsradius des Sonar-Rasters in Pixeln.
int maxDistanceCm = 40;             ///< Skalierungsgrenze: Maximale Messdistanz des Radars in Zentimetern.

/** * @brief Speicher-Array für das visuelle "Nachleuchten" des Radars.
 * @details Der Index entspricht dem Winkel (0-180°). Der gespeicherte Wert ist die Distanz in cm.
 */
float[] radarWerte = new float[181]; 

/**
 * @brief Initialisiert das Anwendungsfenster und konfiguriert die serielle Kommunikation.
 * @details Das Fenster wird auf eine feste Auflösung gesetzt, das Antialiasing (smooth) aktiviert 
 * und versucht, die Verbindung zum USB-Treiber des Arduino Nano mit 9600 Baud aufzubauen.
 */
void setup() {
  size(1000, 650); 
  smooth();
  
  // Initialisiere das Daten-Array mit Nullen (keine Hindernisse beim Start)
  for (int i = 0; i <= 180; i++) { 
    radarWerte[i] = 0; 
  }
  
  // Verbindungsaufbau zur Hardware
  try {
    String portName = "/dev/cu.usbserial-A106PYCP"; 
    myPort = new Serial(this, portName, 9600);
    myPort.bufferUntil('\n'); // Event wird erst bei vollständiger Zeile ausgelöst
  } catch (Exception e) {
    systemState = "ERR_PORT";
    errorMessage = "Port nicht gefunden!";
  }
}

/**
 * @brief Hauptzeichenschleife (Render-Engine) der Benutzeroberfläche.
 * @details Draw wird ununterbrochen ausgeführt und zeichnet das Radar-Layout in fünf Schichten:
 * 1. Das kreisförmige Echolot-Raster inklusive Zentimeter-Angaben.
 * 2. Die radialen Richtungs-Vektoren inklusive Gradzahlen.
 * 3. Die gefilterten Objekt-Cluster (Echtzeit-Hindernisse).
 * 4. Den rotierenden, leuchtenden Radar-Abtaststrahl.
 * 5. Das mathematische Status-Dashboard in der oberen linken Ecke.
 */
void draw() {
  background(6, 12, 6); // Dunkles Sci-Fi-Radar-Grün
  
  // Verschiebe den Koordinatenursprung (0,0) in das untere Zentrum des Fensters
  pushMatrix();
  translate(width/2, height - 70);
  
  // ==========================================
  // SCHRITT 1: GRID-KREISE ZEICHNEN & BESCHRIFTEN
  // ==========================================
  strokeWeight(1);
  for (int r = 100; r <= radarRadius; r += 100) {
    stroke(0, 100, 0, 150);
    noFill();
    // Zeichne einen Kreisbogen exakt im aktiven Arbeitsfenster (30° bis 150°)
    // Processing nutzt Radiant: 0° ist rechts, mathematisch negativ drehend
    arc(0, 0, r*2, r*2, radians(30 - 180), radians(150 - 180)); 
    
    // Distanz-Beschriftung an den Kreisringen platzieren
    fill(0, 180, 0);
    textSize(12);
    int cmText = int(map(r, 0, radarRadius, 0, maxDistanceCm));
    text(cmText + " cm", 10, -r + 5);
  }
  
  // ==========================================
  // SCHRITT 2: WINKEL-LINIEN & GRAD-BESCHRIFTUNGEN
  // ==========================================
  int[] winkelCheck = {30, 60, 90, 120, 150}; 
  for (int w : winkelCheck) {
    stroke(0, 80, 0, 100);
    float rad = radians(w - 180);
    float lx = radarRadius * cos(rad);
    float ly = radarRadius * sin(rad);
    line(0, 0, lx, ly);
    
    // Gradzahlen leicht außerhalb des äußeren Rings platzieren
    float tx = (radarRadius + 25) * cos(rad);
    float ty = (radarRadius + 25) * sin(rad);
    fill(0, 230, 0);
    textAlign(CENTER, CENTER);
    text(w + "°", tx, ty);
  }
  textAlign(LEFT, BASELINE); // Textausrichtung für nachfolgende Elemente zurücksetzen
  
  // ==========================================
  // SCHRITT 3: INTELLIGENTE KANTENERKENNUNG (OBJEKT-COMPILER)
  // ==========================================
  boolean inObject = false;       ///< Flag, ob der Scan-Algorithmus sich gerade innerhalb eines Objekts befindet.
  int startAngle = 0;             ///< Der Startwinkel des aktuell erfassten Objekt-Clusters.
  float sumDistance = 0;          ///< Akkumulierte Distanzwerte zur späteren Mittelwertbildung.
  int countPins = 0;              ///< Anzahl der Messpunkte innerhalb dieses Objekt-Blocks.
  float lastValidDistance = 0;    ///< Distanzwert des vorhergehenden Winkelschritts zur Sprunganalyse.
  
  for (int i = 30; i <= 150; i++) {
    float d = radarWerte[i];
    
    // Prüfen, ob ein physikalisches Objekt im Messbereich liegt
    if (d > 2 && d <= maxDistanceCm) {
      if (!inObject) {
        // Eintrittsflanke: Ein neues Objekt wurde entdeckt
        inObject = true;
        startAngle = i;
        sumDistance = d;
        countPins = 1;
        lastValidDistance = d;
      } else {
        /* MATHEMATISCHER FILTER GEGEN SCHALLSTREUUNG:
           Wenn sich der Abstand zum vorherigen Gradschritt schlagartig um mehr als 3.5 cm 
           ändert, detektiert der Filter eine Objektkante. Das bedeutet, zwei dicht 
           nebeneinander stehende Objekte werden hier sauber getrennt! */
        if (abs(d - lastValidDistance) > 3.5) {
          drawCompensatedObject(startAngle, i - 1, sumDistance / countPins);
          // Unmittelbarer Neustart des Algorithmus für das nächste Objekt
          startAngle = i;
          sumDistance = d;
          countPins = 1;
        } else {
          // Keine Kante -> Der Punkt gehört zur Oberfläche desselben Objekts
          sumDistance += d;
          countPins++;
        }
        lastValidDistance = d;
      }
    } else {
      // Austrittsflanke: Der Strahl wandert wieder ins Leere
      if (inObject) {
        drawCompensatedObject(startAngle, i - 1, sumDistance / countPins);
        inObject = false;
      }
    }
  }
  // Letztes aktives Objekt am Gehäuserand (150°) sichern
  if (inObject) {
    drawCompensatedObject(startAngle, 150, sumDistance / countPins);
  }
  
  // ==========================================
  // SCHRITT 4: AKTUELLER RADARSTRAHL (ZEIGER)
  // ==========================================
  strokeWeight(3);
  stroke(0, 255, 0, 220); // Leuchtendes Neon-Grün
  float pointerX = radarRadius * cos(radians(angle - 180));
  float pointerY = radarRadius * sin(radians(angle - 180));
  line(0, 0, pointerX, pointerY);
  
  popMatrix(); // Lokales Koordinatensystem schließen
  
  // ==========================================
  // SCHRITT 5: STATUS-DASHBOARD (USER INTERFACE)
  // ==========================================
  fill(10, 30, 10, 220);
  stroke(0, 255, 0, 80);
  rect(15, 15, 260, 110, 8); // Abgerundete Sci-Fi Box
  
  fill(0, 255, 0);
  textSize(15);
  text("RADAR-STATUS: " + systemState, 30, 40);
  
  if (systemState.equals("OK")) {
    text("Winkel: " + int(angle) + "°", 30, 70);
    text("Entfernung: " + int(distance) + " cm", 30, 100);
  } else if (systemState.equals("TESTING")) {
    fill(255, 220, 0); // Warnfarbe Gelb während des Bootens
    text("Selbsttest läuft...", 30, 70);
    text("LEDs & Sensor prüfen.", 30, 100);
  } else {
    fill(255, 50, 50); // Alarmfarbe Rot bei Störung
    text("HARDWARE FEHLER!", 30, 70);
    text(errorMessage, 30, 100);
  }
}

/**
 * @brief Berechnet die reale Objektgeometrie und zeichnet das kompensierte Hindernis.
 * @details Diese Funktion entscheidet anhand der akkumulierten Breite im Gradbereich, 
 * wie das Objekt dargestellt wird. Winzige Echos werden zu einem messerscharfen Kern-Punkt zentriert. 
 * Echte, breite Objekte werden als flächige Kreisbogen-Struktur abgebildet, um die physikalische 
 * Realität perfekt zu spiegeln.
 * * @param startA Der mathematische Eintrittswinkel des Objekts.
 * @param endA Der mathematische Austrittswinkel des Objekts.
 * @param avgDist Die gemittelte, fehlerbereinigte Distanz des Objekts in Zentimetern.
 */
void drawCompensatedObject(int startA, int endA, float avgDist) {
  int objectWidthDegrees = endA - startA;
  
  // Extrem schmales Rauschen (unter 2 Grad Breite) als Fehlsignal blockieren
  if (objectWidthDegrees < 2) return;
  
  float centerAngle = startA + (objectWidthDegrees / 2.0);
  float r = map(avgDist, 0, maxDistanceCm, 0, radarRadius);
  
  stroke(255, 30, 30, 220); // Gefahrenfarbe Rot für Hindnisse
  fill(255, 0, 0, 200);
  
  if (objectWidthDegrees <= 7) {
    // Schmaler Modus: Objekt wird als fokussierter Zielpunkt komprimiert
    float x = r * cos(radians(centerAngle - 180));
    float y = r * sin(radians(centerAngle - 180));
    ellipse(x, y, 7, 7);
  } else {
    // Breiter Modus: Objekt wird als reale, flächige Kontur gezeichnet
    strokeWeight(4);
    noFill();
    arc(0, 0, r*2, r*2, radians(startA - 180), radians(endA - 180));
    strokeWeight(1); // Standard-Dicke wiederherstellen
  }
  
  // Eine feine Peillinie zieht die Vektor-Richtung zum Objektzentrum
  float cx = r * cos(radians(centerAngle - 180));
  float cy = r * sin(radians(centerAngle - 180));
  stroke(255, 0, 0, 40);
  line(cx, cy, 0, 0);
}

/**
 * @brief Asynchroner Interrupt-Handler für eingehende serielle Daten.
 * @details Diese Funktion wird von Processing automatisch im Hintergrund getriggert, sobald 
 * ein Zeilenumbruch ('\n') im Puffer des USB-Ports registriert wird. Sie parst das proprietäre 
 * Übertragungsprotokoll des Arduinos:
 * - "S:[STATUS]" -> Schaltet den globalen Systemzustand um.
 * - "D:[WINKEL],[DISTANZ]" -> Trennt Winkel und Distanz und speichert sie im globalen Array.
 * * @param p Das Objekt des seriellen Ports, welcher das Event ausgelöst hat.
 */
void serialEvent(Serial p) {
  try {
    String inString = p.readStringUntil('\n');
    if (inString != null) {
      inString = trim(inString); // Steuerzeichen und Leerzeichen abschneiden
      
      // Parser-Zweig 1: Systemmeldungen extrahieren
      if (inString.startsWith("S:")) {
        systemState = inString.substring(2);
        if (systemState.equals("ERR_SENSOR")) {
          errorMessage = "Sensor-Echo fehlt!";
        }
      } 
      // Parser-Zweig 2: Numerische Messdaten dekodieren
      else if (inString.startsWith("D:") && systemState.equals("OK")) {
        String[] data = split(inString.substring(2), ',');
        if (data.length == 2) {
          angle = float(data[0]);
          distance = float(data[1]);
          
          // Datenbereich absichern und im Ringpuffer-Array hinterlegen
          int index = int(angle);
          if (index >= 0 && index <= 180) {
            radarWerte[index] = distance;
          }
        }
      }
    }
  } catch (Exception e) {
    println("Fehler beim seriellen Parsing: " + e.getMessage());
  }
}
