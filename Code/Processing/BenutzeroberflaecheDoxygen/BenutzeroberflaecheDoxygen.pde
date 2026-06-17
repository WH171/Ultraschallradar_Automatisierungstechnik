import processing.serial.*;

/**
 * @file RadarVisualisierung.pde
 * @brief Full-screen high-end Radar UI with automated hot-plugging and watchdog filtering.
 * @details This graphical user interface acts as the frontend interface for an ultrasonic 
 * radar tracking station. Running in true fullscreen mode, it implements an active handshake 
 * protocol during boot, visualizes data streams inside a modern 120-degree telemetry matrix, 
 * compensates for signal divergence using dynamic hysteresis edge-tracking, and maintains 
 * an active software watchdog to trigger a fullscreen recovery warning upon hardware unplug events.
 * * @author [Your Name]
 * @date 2026-06-17
 * @version 1.3
 */

/* --- SERIAL CONNECTION VARIABLES --- */
Serial myPort;                      ///< Serial communication pipeline instance.
String systemState = "BOOT";        ///< System tracking machine states: "BOOT", "TESTING", "OK", "ERR_PORT", "ERR_SENSOR".
String errorMessage = "";           ///< Human-readable storage for active hardware error traces.
String portName = "/dev/cu.usbserial-A106PYCP"; ///< Pre-configured explicit path to target Mac serial device controller.

float angle = 90;                   ///< Current scanline projection coordinate updated by hardware telemetry.
float distance = 0;                 ///< Current obstacle range buffer computed in centimeters.

/* --- DISPLAY CONFIGURATIONS --- */
int radarRadius;                    ///< Scale constraints computed adaptively during boot routines to fit screen size.
int maxDistanceCm = 40;             ///< Maximum telemetry threshold mapped directly onto outer matrix boundary.

/** * @brief Persistent telemetry sweep map array.
 * @details Uses raw angle coordinates as direct access index references to implement radar persistent phosphor glow effects.
 */
float[] radarWerte = new float[181]; 

/* --- TIMER WATCHDOGS --- */
int lastConnectionCheck = 0;        ///< Timer registry evaluating interval loops for handshake token delivery.
int lastReconnectAttempt = 0;       ///< Timer registry enforcing a 2-second cooldown on background port polling.
int lastDataReceivedTime = 0;       ///< Heartbeat storage tracking the last valid payload arrival timestamp.

/**
 * @brief Configures display parameters, calculates geometry, and triggers initial connect routines.
 */
void setup() {
  fullScreen(); // Force viewport initialization into native fullscreen workspace
  smooth();     // Activate hardware anti-aliasing pipelines
  
  // Automatically configure graphic radar geometry size relative to vertical viewport constraints (65%)
  radarRadius = int(height * 0.65);
  for (int i = 0; i <= 180; i++) { radarWerte[i] = 0; }
  
  // Execute opening serial interface hook
  connectSerial();
}

/**
 * @brief Handles automated background serial discovery and runtime re-allocations.
 * @details Wraps port instantiations inside robust try-catch traps to isolate critical OS 
 * I/O exceptions when launching without plugged devices or tracking active runtime line-break drops.
 */
void connectSerial() {
  try {
    myPort = new Serial(this, portName, 9600);
    myPort.bufferUntil('\n'); // Enforce interrupt triggering only on complete packet returns
    systemState = "BOOT"; 
    lastDataReceivedTime = millis(); // Refresh heartbeat clock on fresh connects
  } catch (Exception e) {
    systemState = "ERR_PORT";
    errorMessage = "Sicherstellen, dass der Computer mit dem Radarsystem verbunden ist!";
  }
}

/**
 * @brief Primary graphics render execution loop.
 * @details Evaluates active system flags sequentially to project either the fullscreen critical alert screen, 
 * the bootloader handshake layout, or the running 120-degree tactical HUD display layers.
 */
void draw() {
  // ==========================================
  // FATAL RECOVERY MODE: SYSTEM DISCONNECTED OR ERROR DETECTED (RED SCREEN)
  // ==========================================
  if (systemState.equals("ERR_PORT") || systemState.equals("ERR_SENSOR")) {
    background(130, 0, 0); // Dark crimson panic tone matching target specification
    textAlign(CENTER, CENTER);
    
    fill(255);
    textSize(height * 0.07); 
    text("SYSTEMFEHLER", width/2, height * 0.35);
    
    textSize(height * 0.035);
    if (systemState.equals("ERR_PORT")) {
      text("Sicherstellen, dass der Computer mit dem Radarsystem verbunden ist!", width/2, height * 0.52);
      
      // ACTIVE HOT-PLUG INTERRUPT LOOP: Attempt background port recovery every 2000 milliseconds
      if (millis() - lastReconnectAttempt > 2000) {
        connectSerial();
        lastReconnectAttempt = millis();
      }
    } else {
      text("Ultraschallsensor liefert keine Echosignale!", width/2, height * 0.52);
    }
    
    fill(255, 255, 0); 
    textSize(height * 0.032);
    text("Anlage überprüfen und Arduino neustarten.", width/2, height * 0.65);
    return; // Halt routine drawing to freeze display within alert loop
  }

  // ==========================================
  // HANDSHAKE BOOTLOADER MODE: DEVICE SYNCHRONIZATION RUNNING (GREY SCREEN)
  // ==========================================
  if (systemState.equals("BOOT") || systemState.equals("TESTING")) {
    background(30, 30, 30); // Charcoal gray specification template layout
    textAlign(CENTER, CENTER);
    
    fill(255, 200, 0); 
    textSize(height * 0.06);
    text("System fährt hoch...", width/2, height * 0.45);
    
    fill(255, 180, 0, 180);
    textSize(height * 0.035);
    text("Hardware-Selbsttest wird durchgeführt.", width/2, height * 0.55);
    
    // STREAM CONNECT TOKENS: Broadcast "P:START" every 200ms until micro-controller escapes its boot block
    if (systemState.equals("BOOT") && millis() - lastConnectionCheck > 200) {
      try {
        myPort.write("P:START\n");
      } catch (Exception e) {
        systemState = "ERR_PORT";
      }
      lastConnectionCheck = millis();
    }
    return; // Deflect execution to secure screen locking
  }

  // ==========================================
  // LIVE RADAR ACTIVE SCANNING INTERFACE (GREEN SCREEN MATRIX)
  // ==========================================
  
  // RUNTIME CRITICAL LIVE WATCHDOG: Force immediate drop to failure screen if heartbeat slips past 500ms
  if (systemState.equals("OK") && (millis() - lastDataReceivedTime > 500)) {
    systemState = "ERR_PORT";
    try { myPort.stop(); } catch (Exception e) {} // Kill dead stream resource mapping handles
  }

  background(6, 12, 6); 
  textAlign(LEFT, BASELINE); 
  
  // Transform absolute space matrix to map (0,0) directly to center bottom positions
  pushMatrix();
  translate(width/2, height - 80);
  
  // LAYER 1: DRAW RANGE RANGE-GRID RINGS (30° to 150°)
  strokeWeight(1);
  for (int r = int(radarRadius*0.25); r <= radarRadius; r += int(radarRadius*0.25)) {
    stroke(0, 100, 0, 150);
    noFill();
    arc(0, 0, r*2, r*2, radians(30 - 180), radians(150 - 180)); 
    
    fill(0, 180, 0);
    textSize(13);
    int cmText = int(map(r, 0, radarRadius, 0, maxDistanceCm));
    text(cmText + " cm", 10, -r + 5);
  }
  
  // LAYER 2: DRAW RADIAL AZIMUTH LINES & DEGREE LABELS
  int[] winkelCheck = {30, 60, 90, 120, 150}; 
  for (int w : winkelCheck) {
    stroke(0, 80, 0, 100);
    float rad = radians(w - 180);
    float lx = radarRadius * cos(rad);
    float ly = radarRadius * sin(rad);
    line(0, 0, lx, ly);
    
    float tx = (radarRadius + 30) * cos(rad);
    float ty = (radarRadius + 30) * sin(rad);
    fill(0, 230, 0);
    textAlign(CENTER, CENTER);
    text(w + "°", tx, ty);
  }
  textAlign(LEFT, BASELINE); 
  
  // LAYER 3: COMPENSATED SIGNAL CLUSTERING (HYSTERESIS DATA PARSER)
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
        // RADIAL CONVERGENCE SEGREGATION FILTER: Splits dense adjacent object echoes if distance spikes (>3.5cm)
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
  
  // LAYER 4: ACTIVE SCAN SWEEP LINE
  strokeWeight(4);
  stroke(0, 255, 0, 220); 
  float pointerX = radarRadius * cos(radians(angle - 180));
  float pointerY = radarRadius * sin(radians(angle - 180));
  line(0, 0, pointerX, pointerY);
  
  popMatrix(); // Close viewport mapping coordinate isolation
  
  // LAYER 5: TACTICAL TELEMETRY DASHBOARD OVERLAY
  fill(10, 30, 10, 220);
  stroke(0, 255, 0, 80);
  rect(30, 30, 280, 110, 8);
  
  fill(0, 255, 0);
  textSize(16);
  text("RADAR-SYSTEM AKTIV", 45, 60);
  textSize(15);
  text("Winkel: " + int(angle) + "°", 45, 90);
  text("Entfernung: " + int(distance) + " cm", 45, 115);
}

/**
 * @brief Renders isolated obstacle structures based on clustered geometry widths.
 * @details Filters out sparse single-degree anomalies. If an obstacle exhibits a slim signature, 
 * it is compressed down to a pin-point vector ellipse. Wide targets are mapped along a continuous arc 
 * segment representing true face geometry width.
 * * @param startA Initial vector entrance sweep angle.
 * @param endA Exit vector edge boundary sweep angle.
 * @param avgDist Averaged raw distance calculation mapped across the cluster size.
 */
void drawCompensatedObject(int startA, int endA, float avgDist) {
  int objectWidthDegrees = endA - startA;
  if (objectWidthDegrees < 2) return; // Discard isolated white noise pings
  
  float centerAngle = startA + (objectWidthDegrees / 2.0);
  float r = map(avgDist, 0, maxDistanceCm, 0, radarRadius);
  
  stroke(255, 30, 30, 220);
  fill(255, 0, 0, 200);
  
  if (objectWidthDegrees <= 7) {
    // Sharp target focus mode
    float x = r * cos(radians(centerAngle - 180));
    float y = r * sin(radians(centerAngle - 180));
    ellipse(x, y, 8, 8);
  } else {
    // Broad profile arc tracing mode
    strokeWeight(5);
    noFill();
    arc(0, 0, r*2, r*2, radians(startA - 180), radians(endA - 180));
    strokeWeight(1); 
  }
}

/**
 * @brief Serial Port telemetry interrupt service routine.
 * @details Evaluates string line updates. Restarts the processing watchdog whenever data streams 
 * enter the input buffers. Filters headers to split status flags ('S:') from telemetry packets ('D:').
 * * @param p Reference access link to the throwing thread port instance.
 */
void serialEvent(Serial p) {
  try {
    String inString = p.readStringUntil('\n');
    if (inString != null) {
      inString = trim(inString);
      
      // DATA COMPLIANCE SIGNAL DISPATCH: Keep awake the system watchdog on every valid transaction
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
    systemState = "ERR_PORT"; // Catch thread crash anomalies and force red alert dropouts
  }
}
