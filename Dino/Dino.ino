#include <LiquidCrystal.h>
#include "Dinosaurio.h"
#include "Cactus.h"
#include "Pajaro.h"

LiquidCrystal lcd(22, 23, 5, 18, 19, 21);
Dinosaurio dino;
Cactus cactus(16);
Pajaro pajaro(29);
Pajaro pajaro1(39);
//nuevo cactus
Cactus cactu(10);

byte dinoReposo[8] = {
    B00000,
    B00000,
    B00111,
    B00101,
    B10111,
    B10100,
    B01101,
    B01100
};
byte dinoCamina1[8] = {
    B00000,
    B00111,
    B00101,
    B10111,
    B10100,
    B01110,
    B01100,
    B00010
};
byte dinoCamina2[8] = {
    B00000,
    B00111,
    B00101,
    B10111,
    B10100,
    B01110,
    B01100,
    B10000
};
byte dinoSalta[8] = {
    B00111,
    B00101,
    B10111,
    B10100,
    B01101,
    B01100,
    B10000,
    B00000
};
byte cactus1[8] = {
    B00000,
    B00000,
    B00001,
    B10101,
    B01101,
    B00110,
    B00100,
    B00100
};

byte pajaroAlaArriba[8] = {
    B00000,
    B00100,
    B01100,
    B01111,
    B11111,
    B01100,
    B00100,
    B00000
};

byte pajaroAlaAbajo[8] = {
    B00000,
    B00100,
    B00100,
    B01111,
    B11111,
    B01100,
    B00100,
    B00000
};

struct Button {
  const uint8_t PIN;
  bool pressed;
};

Button button1 = {4, false};

int velocidad = 300;
unsigned long startTime;
bool gameOver = false;

void IRAM_ATTR isr() {
  button1.pressed = true;
}

void setup() {

  lcd.begin(16, 2);
  Serial.begin(115200);
  Serial.println("test");

  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);

  lcd.createChar(0, dinoReposo);
  lcd.createChar(1, dinoCamina1);
  lcd.createChar(2, dinoCamina2);
  lcd.createChar(3, dinoSalta);
  lcd.createChar(4, cactus1);
  lcd.createChar(5, pajaroAlaArriba);
  lcd.createChar(6, pajaroAlaAbajo);

  iniciarJuego();
}

void iniciarJuego() {
  gameOver = false;
  startTime = millis();
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  delay(4000);
}

void loop() {
  if (gameOver) {
    if (button1.pressed) {
      button1.pressed = false;
      iniciarJuego();
    }
    return;
  }

  // Se borra la pantalla
  lcd.clear();

  // Mostrar contador de segundos en las posiciones 0 y 1 del LCD
  unsigned long currentTime = millis();
  int seconds = (currentTime - startTime) / 1000;
  lcd.setCursor(0, 0);
  lcd.print(seconds);

  // Se pinta la pantalla
  if (button1.pressed) {
    dino.saltar();
    lcd.setCursor(0, 0);
    button1.pressed = false;
  } else {
    lcd.setCursor(0, 1);
    dino.caminar();
  }

  lcd.write(byte(dino.obtenerEstado()));

  cactus.mover();
  lcd.setCursor(cactus.obtenerPosicion(), 1);
  lcd.write(byte(4));

  pajaro.mover();
  lcd.setCursor(pajaro.obtenerPosicion(), 0);
  lcd.write(byte(5));

  pajaro1.mover();
  lcd.setCursor(pajaro1.obtenerPosicion(), 1);
  lcd.write(byte(5));

  //implementacion de un nuevo cactus (puesto arriba para diferenciar)
  cactu.mover();
  lcd.setCursor(cactu.obtenerPosicion(), 0);
  lcd.write(byte(4));

  // Comprobar la colision
  if (dino.obtenerPosicion() == 0 && cactus.obtenerPosicion() == 0 ||
      dino.obtenerPosicion() == 1 && pajaro.obtenerPosicion() == 0 ||
      dino.obtenerPosicion() == 0 && pajaro1.obtenerPosicion() == 0|| 
      //comprobacion del nuevo cactus
      dino.obtenerPosicion() == 1 && cactu.obtenerPosicion() == 0 )
       {

    gameOver = true;
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("TRY AGAIN");

    // Mostrar la puntuación (tiempo en segundos)
    lcd.setCursor(0, 1);
    lcd.print("SCORE: ");
    lcd.print(seconds);

    Serial.println("Colisión!");
  }

  delay(velocidad);
}
