#define DRAW_DELAY 50
#define TANK_STEP 3
#define MIN_TANK_POSITION 0
#define MAX_TANK_POSITION 75
#define TANK_LEFT_PADDING 3
#define BULLETS_NUMBER 5
#define BULLET_DELAY 200
#define CENTER_TANK 4
#define INVIDER_DELAY 500
#define INVADER_WIDTH 9
#define INVADER_HEIGHT 8
#define INVADER_LEFT_PADDING 3
// invader width + padding = 12
#define INVADER_PLACE_WIDTH 12
#define INVADER_PLACE_HEIGHT 12
#define INVADER_WIN_HEIGHT 45
#define INVADER_STEP_X 3
#define INVADER_STEP_Y 3
// 12 * 8 - all invaders width
#define MATRIX_WIDTH 96

static const unsigned char PROGMEM tankBitmap[] =
{
  B00000001, B00000000,
  B00000011, B10000000,
  B00000111, B11000000,
  B00000011, B10000000,
  B00001111, B11100000,
  B00011111, B11110000,
};
static const unsigned char PROGMEM invaderA1Bitmap[] =
{
  B00000011, B10000000,
  B00001011, B10100000,
  B00011011, B10110000,
  B00011111, B11110000,
  B00001100, B01100000,
  B00001100, B11000000,
  B00011000, B10000000,
  B00011001, B10000000,
};
static const unsigned char PROGMEM invaderA2Bitmap[] =
{
  B00000011, B10000000,
  B00001111, B11100000,
  B00001011, B10100000,
  B00011111, B11110000,
  B00001100, B01100000,
  B00001100, B01100000,
  B00000110, B00110000,
  B00000011, B00010000,
};
static const unsigned char PROGMEM invaderB1Bitmap[] =
{
  B00000111, B11000000,
  B00001111, B11100000,
  B00011110, B10110000,
  B00001111, B11100000,
  B00000111, B11000000,
  B00000100, B01000000,
  B00001000, B00100000,
  B00010000, B00010000,
};
static const unsigned char PROGMEM invaderB2Bitmap[] =
{
  B00000111, B11000000,
  B00001111, B11100000,
  B00011010, B11110000,
  B00001111, B11100000,
  B00000111, B11000000,
  B00001000, B00100000,
  B00001011, B10100000,
  B00000100, B01000000,
};
static const unsigned char PROGMEM invaderC1Bitmap[] =
{
  B00000011, B10000000,
  B00001011, B10100000,
  B00010111, B11010000,
  B00010011, B10010000,
  B00010100, B01010000,
  B00000100, B01000000,
  B00001000, B00100000,
  B00000100, B01000000,
};
static const unsigned char PROGMEM invaderC2Bitmap[] =
{
  B00010000, B00010000,
  B00010000, B00010000,
  B00010100, B01010000,
  B00001011, B10100000,
  B00000011, B10000000,
  B00000011, B10000000,
  B00000100, B01000000,
  B00011000, B00110000,
};
int invaderXShift;
int invaderYShift;
bool invaderDirectionRight;
byte invadersSpeed;
unsigned char invaderLines[3];
int tankPosition;
bool leftPressed;
bool rightPressed;
// 0 - nope, 1 - left, 2 - right
byte directionClicked;
bool topClicked;
bool loose;
bool win;
int bulletsX[BULLETS_NUMBER] = {0, 0, 0, 0, 0};
int bulletsY[BULLETS_NUMBER] = {0, 0, 0, 0, 0};


void btnTankTopHandler() {
  topClicked = true;
}
void btnTankLeftHandler() {
  directionClicked = 1;
}
void btnTankRightHandler() {
  directionClicked = 2;
}

byte calcLeftDeadInvaders() {
  unsigned char invadersMerge = invaderLines[0] | invaderLines[1] | invaderLines[2];
  byte count = 0;
  while (invadersMerge && !(invadersMerge & B10000000)) {
    count++;
    invadersMerge <<= 1;
  }
  return count;
}
byte calcRightDeadInvaders() {
  unsigned char invadersMerge = invaderLines[0] | invaderLines[1] | invaderLines[2];
  byte count = 0;
  while (invadersMerge && !(invadersMerge & 1)) {
    count++;
    invadersMerge >>= 1;
  }
  return count;
}

void setupTank() {
  // display settings
  display.begin();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.setRotation(2);

  // init vars
  tankPosition = SCREEN_WIDTH / 2;
  leftPressed = false;
  rightPressed = false;
  topClicked = false;
  directionClicked = 0;
  invaderLines[0] = B00011111;
  invaderLines[1] = B00011111;
  invaderLines[2] = B00011111;
  invaderDirectionRight = true;
  invaderXShift = 0 - calcLeftDeadInvaders() * INVADER_PLACE_WIDTH - INVADER_LEFT_PADDING;
  invaderYShift = 0;
  // invaders speed in frames per move
  invadersSpeed = 20;
  loose = false;
  win = false;
  for (byte i = 0; i < BULLETS_NUMBER; i++) {
    bulletsX[i] = 0;
    bulletsY[i] = 0;
  }
  
  // buttons handle
  buttonTop.pressHandler(btnTankTopHandler);
  buttonLeft.pressHandler(btnTankLeftHandler);
  buttonRight.pressHandler(btnTankRightHandler);
}

void drawTank() {
  byte direction = directionClicked;
  directionClicked = 0;
  if (leftPressed and rightPressed) {
    direction = 0;
  } else if (leftPressed) {
    direction = 1;
  } else if (rightPressed) {
    direction = 2;
  }
  
  if (direction == 1 and tankPosition > MIN_TANK_POSITION) {
    tankPosition -= TANK_STEP;
  }
  if (direction == 2 and tankPosition < MAX_TANK_POSITION) {
    tankPosition += TANK_STEP;
  }
  display.drawBitmap(tankPosition - TANK_LEFT_PADDING, 42, tankBitmap, 16, 6, BLACK);
}

bool checkHit(int x, int y) {
  for (byte i = 0; i < 3; i++) {
    int lineHeight = invaderYShift + i * INVADER_PLACE_HEIGHT;
    if (y > lineHeight and y < lineHeight + INVADER_HEIGHT) {
      int shiftedX = x - invaderXShift;
      if (shiftedX % 12 < 3) {
        // miss
      } else {
        unsigned char bullet = B10000000;
        bullet >>= shiftedX / 12;
        if (invaderLines[i] & bullet) {
          invaderLines[i] &= ~bullet;
          return true; 
        }
      }
    }
  }
  return false;
}

byte lastBulletFrame = 0;
void drawBullets() {
  lastBulletFrame++;
  for (byte i = 0; i < BULLETS_NUMBER; i++) {
    if (bulletsY[i] == 0) {
      if (topClicked && lastBulletFrame > 20) {
        lastBulletFrame = 0;
        topClicked = false;      
        bulletsY[i] = 42;
        bulletsX[i] = tankPosition + CENTER_TANK;
      }
    }
    if (bulletsY[i] > 0) {
      bulletsY[i] = bulletsY[i] - 1;
      if (bulletsY[i] < 0) {
        bulletsY[i] = 0;
      } else {
        display.drawRect(bulletsX[i], bulletsY[i], 1, 4, BLACK);
        if (checkHit(bulletsX[i], bulletsY[i])) {
          bulletsY[i] = 0;
        }
      }
    }
  }
}

void checkLoose() {
  byte lineNumber = 0;
  for (byte i = 0; i < 3; i++) {
    if (invaderLines[i]) {
      lineNumber++; 
    }
  }
  if (invaderYShift + lineNumber * INVADER_PLACE_HEIGHT 
      > INVADER_WIN_HEIGHT) {
    loose = true;
  }
}

bool firstInvaderFrame;
unsigned char bitmap[16];
void drawInvaider(int x, int y, int line) {
  if (line == 0) {
    display.drawBitmap(x, y, firstInvaderFrame ? invaderA1Bitmap : invaderA2Bitmap, 16, 8, BLACK);
  } else if (line == 1) {
    display.drawBitmap(x, y, firstInvaderFrame ? invaderB1Bitmap : invaderB2Bitmap, 16, 8, BLACK);
  } else if (line == 2) {
    display.drawBitmap(x, y, firstInvaderFrame ? invaderC1Bitmap : invaderC2Bitmap, 16, 8, BLACK);
  }
}
byte frameInvaderNumber = 0;
void drawInvaders() {
  if (frameInvaderNumber >= invadersSpeed) {
    frameInvaderNumber = 0;
    firstInvaderFrame = !firstInvaderFrame;

    if (invaderDirectionRight) {
      invaderXShift += INVADER_STEP_X;
      if (invaderXShift >= SCREEN_WIDTH - MATRIX_WIDTH + calcRightDeadInvaders() * INVADER_PLACE_WIDTH) {
        invaderDirectionRight = false;
        invaderYShift += INVADER_STEP_Y;
        checkLoose();
      }
    } else {
      invaderXShift -= INVADER_STEP_X;
      if (invaderXShift <= 0 - calcLeftDeadInvaders() * INVADER_PLACE_WIDTH - INVADER_LEFT_PADDING) {
        invaderDirectionRight = true;
        invaderYShift += INVADER_STEP_Y;
        checkLoose();
      }
    }
  }
//          display.println(invadersSpeed);
  frameInvaderNumber++;

  byte invadersNumber = 0;
  for (byte i = 0; i < 3; i++) {
    byte place = 0;
    unsigned char line = invaderLines[i];
    while (line) {
      if (line & B10000000) {
        invadersNumber++;
        drawInvaider(invaderXShift + place * INVADER_PLACE_WIDTH, invaderYShift + i * INVADER_PLACE_HEIGHT, i);
      }
      line <<= 1;
      place++;
    }
  }
  if (invadersNumber < 10) {
    invadersSpeed = 10;
  }
  if (invadersNumber < 5) {
    invadersSpeed = 5;
  }
  if (invadersNumber == 1) {
    invadersSpeed = 2;
  }
  if (invadersNumber == 0) {
    win = true;
  }
}

unsigned long lastDraw;

void loopTank() {
  // check buttons
  buttonTop.isPressed();
  leftPressed = buttonLeft.isPressed();
  rightPressed = buttonRight.isPressed();

  if (win) {
    display.clearDisplay();
    display.println("YOU WIN");
    display.display(); 
    delay(2000);
    setupTank();
  } else if (loose) {
    display.clearDisplay();
    display.println("GAME OVER");
    display.display(); 
    delay(2000);
    setupTank();
  } else {
    if (millis() - lastDraw > DRAW_DELAY) {
      lastDraw = millis();
      display.clearDisplay();
      drawTank();
      drawBullets();
      drawInvaders();
      display.display(); 
    }
  }
  // FPS
  // Serial.println(1000 / (millis() - lastDraw));
}
