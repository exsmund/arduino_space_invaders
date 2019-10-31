#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Button.h>

// Init display
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
#define SCREEN_WIDTH 84
#define SCREEN_HEIGHT 48

// Init buttons
Button buttonTop = Button(8, INPUT_PULLUP);
Button buttonBottom = Button(9, INPUT_PULLUP);
Button buttonLeft = Button(11, INPUT_PULLUP);
Button buttonRight = Button(10, INPUT_PULLUP);

// Switch programs
// 1 - tank
// 2 - flappybird
// 3 - tetris
int program = 0;
int startProgram = 3;

void btnTankBottomHandler() {
  if (program == 1) {
    program = 0;
    startProgram = 2;
  }
  if (program == 2) {
    program = 0;
    startProgram = 1;
  }
}

void setup() {
  // Log setup
  Serial.begin(9600);
  
  // Display settings
  display.begin();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.setRotation(2);
  buttonBottom.pressHandler(btnTankBottomHandler);

  // Random setup
  randomSeed(analogRead(0));
}

void loop() {
  if (startProgram == 1) {
    startProgram = 0;
    setupTank();  
    program = 1;
  }
  if (startProgram == 2) {
    startProgram = 0;
    setupBird();  
    program = 2;
  }
  if (startProgram == 3) {
    startProgram = 0;
    setupTetris();  
    program = 3;
  }
  if (program == 1) {
    loopTank();  
  }
  if (program == 2) {
    loopBird();  
  }
  if (program == 3) {
    loopTetris();  
  }
  buttonBottom.isPressed();
}
