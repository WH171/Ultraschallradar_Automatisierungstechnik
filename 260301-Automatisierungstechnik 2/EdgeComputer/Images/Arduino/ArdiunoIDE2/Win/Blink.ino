// Die setup Funktion laeuft einmal durch, 
//bis der reset gedrueckt wird oder die stomversorgung weg faellt
void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
 // initialisiert den digitalen pin LED_Builtin als output.
 
}

// die loop Funktion laeuft in einer Dauerschleife
void loop() {

  digitalWrite(LED_BUILTIN, HIGH);  
  //stellt die LED auf an (HIGH ist das Spannungslevel)
  delay(1000);                      
  // wartet fuer eine Sekunde
  digitalWrite(LED_BUILTIN, LOW);   
  // stellt die LED auf aus (LOW ist das Spannungslevel)
  delay(1000);                     
  // wartet fuer eine Sekunde

}
