#define TETRIS_DRAW_DELAY 50
#define TETRIS_LENGTH 20
#define FIGURE_LENGTH 4
#define FIGURE_VARIANT_NUMBER 7
#define TETRIS_WIDTH 10
#define TETRIS_DOT_SIZE 4
#define TETRIS_PADDING 2
#define TETRIS_MOVE_SPEED 1
#define TERIS_PRESS_BUTTON_DELAY 200
#define START_SPEED 15
static const unsigned char PROGMEM dotBitmap[] = 
{
  0b11110000,
  0b10010000,
  0b10110000,
  0b11110000,
};

static const unsigned int bites[] = 
{
  0b1000000000000000,
  0b0100000000000000,
  0b0010000000000000,
  0b0001000000000000,
  0b0000100000000000,
  0b0000010000000000,
  0b0000001000000000,
  0b0000000100000000,
  0b0000000010000000,
  0b0000000001000000,
  0b0000000000100000,
  0b0000000000010000,
  0b0000000000001000,
  0b0000000000000100,
  0b0000000000000010,
  0b0000000000000001,
};

static const unsigned char PROGMEM levelBitmap[] = 
{
  0b00111100,
  0b00000010,
  0b00111100,
  0b00000000,
  0b00000010,
  0b00000010,
  0b01111110,
  0b00000000,
};

static const unsigned char PROGMEM pointsBitmap[] = 
{
  0b00000000,
  0b00000000,
  0b00010010,
  0b00111100,
  0b00000000,
  0b00110000,
  0b01001000,
  0b01111110,
};

static const unsigned char PROGMEM nextBitmap[] = 
{
  0b00110110,
  0b00001000,
  0b00110110,
  0b00000000,
  0b01111110,
  0b00000100,
  0b00001000,
  0b01111110,
};

static const unsigned char PROGMEM mini0Bitmap[] = 
{
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
};
static const unsigned char PROGMEM mini1Bitmap[] = 
{
  0b00000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11110000,
  0b11110000,
  0b00000000,
};
static const unsigned char PROGMEM mini2Bitmap[] = 
{
  0b00000000,
  0b11110000,
  0b11110000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b11000000,
  0b00000000,
};
static const unsigned char PROGMEM mini3Bitmap[] = 
{
  0b00000000,
  0b11000000,
  0b11000000,
  0b11110000,
  0b11110000,
  0b11000000,
  0b11000000,
  0b00000000,
};
static const unsigned char PROGMEM mini4Bitmap[] = 
{
  0b00000000,
  0b00000000,
  0b11110000,
  0b11110000,
  0b11110000,
  0b11110000,
  0b00000000,
  0b00000000,
};
static const unsigned char PROGMEM mini5Bitmap[] = 
{
  0b00000000,
  0b00110000,
  0b00110000,
  0b11110000,
  0b11110000,
  0b11000000,
  0b11000000,
  0b00000000,
};
static const unsigned char PROGMEM mini6Bitmap[] = 
{
  0b00000000,
  0b11000000,
  0b11000000,
  0b11110000,
  0b11110000,
  0b00110000,
  0b00110000,
  0b00000000,
};


// 0 - I
// 1 - L
// 2 - r
// 3 - T
// 4 - o
// 5 - z
// 6 - 5
static const unsigned int figuresBitmap[] =
{
  0b0000000000000000,
  0b0000001111000000,
  0b0000000000000000,
  0b0000000000000000,
  
  0b0000000000000000,
  0b0000000111000000,
  0b0000000100000000,
  0b0000000000000000,
  
  0b0000000000000000,
  0b0000001110000000,
  0b0000000010000000,
  0b0000000000000000,
  
  0b0000000000000000,
  0b0000001110000000,
  0b0000000100000000,
  0b0000000000000000,
  
  0b0000000000000000,
  0b0000000110000000,
  0b0000000110000000,
  0b0000000000000000,
  
  0b0000000000000000,
  0b0000001100000000,
  0b0000000110000000,
  0b0000000000000000,
  
  0b0000000000000000,
  0b0000000110000000,
  0b0000001100000000,
  0b0000000000000000,
};
unsigned int field[TETRIS_LENGTH];
unsigned int figure[FIGURE_LENGTH];
int maskLine;
bool needNewFigure;
bool needRotateFigure;
byte currentFigure;
byte nextFigure;
byte tetrisSpeed;
int figurePosHor;
byte figurePosVert;
bool tetrisTopPressed;
bool tetrisBottomPressed;
bool tetrisRightPressed;
byte figureDirectionClicked;
bool moveDownClicked;
long tetrisPoints;
bool pause;
int tetrisLevel;
int pointsToNextLevel;

void btnTetrisLeftHandler() {
  if (pause) {
    pause = false;
  } else {
    needRotateFigure = true;  
  }
}

void btnTetrisTopHandler() {
  if (pause) {
    pause = false;
  } else {
    figureDirectionClicked = 1;
  }
}

void btnTetrisBottomHandler() {
  if (pause) {
    pause = false;
  } else {
    figureDirectionClicked = 2;
  }
}

void btnTetrisRightHandler() {
  if (pause) {
    pause = false;
  } else {
    moveDownClicked = true;
  }
}

void setupTetris() {
  //setup display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  // setup vars
  for (byte i = 0; i < TETRIS_LENGTH; i++) {
    field[i] = 0b0000000000000000;
  }
  for (byte i = 0; i < FIGURE_LENGTH; i++) {
    figure[i] = 0b0000000000000000;
  }
  needNewFigure = true;
  // invaders speed in frames per move
  tetrisSpeed = START_SPEED;
  figurePosVert = 0;
  figurePosHor = 0;
  nextFigure = random(FIGURE_VARIANT_NUMBER);
  figureDirectionClicked = 0;
  moveDownClicked = false;
  tetrisPoints = 0;
  needRotateFigure = false;
  pause = true;
  tetrisLevel = 1;
  pointsToNextLevel = 0;

  // buttons handle
  buttonTop.pressHandler(btnTetrisTopHandler);
  buttonBottom.pressHandler(btnTetrisBottomHandler);
  buttonLeft.pressHandler(btnTetrisLeftHandler);
  buttonRight.pressHandler(btnTetrisRightHandler);
}

bool moveFigureDown() {
    for (int i = 0; i < FIGURE_LENGTH; i++) {
      if (figure[i]
          && (figurePosVert + i + 1 >= TETRIS_LENGTH 
              || field[figurePosVert + i + 1] & figure[i]
             )
         ) {
        return false;
      }
    }
    figurePosVert++;
    return true;
}

bool checkPlaceForFigure(int* someFigure) {
  for (byte i = 0; i < FIGURE_LENGTH; i++) {
    if (field[figurePosVert + i] & someFigure[i]) {
      return false;
    }
  }
  return true;
}

void rotateFigure() {
  if (needRotateFigure) {
    needRotateFigure = false;
    unsigned int newFigure[FIGURE_LENGTH];
    byte topSpace = 0;
    byte bottomSpace = 0;
    byte calcStep = 0;
    for (byte i = 0; i < FIGURE_LENGTH; i++) {
      if (calcStep == 0) {
        if (figure[i] == 0) {
          topSpace++;
        } else {
          calcStep++;
        }
      }
      if (calcStep == 1) {
        if (figure[i] == 0) {
          calcStep++;
        }
      }
      if (calcStep == 2) {
        if (figure[i] == 0) {
          bottomSpace++;
        }
      }
      newFigure[i] = 0b0000000000000000;
    }
    int horShift = figurePosHor;
    if (horShift + bottomSpace < 3) {
      horShift += 3 - horShift - bottomSpace; 
    }
    if (horShift - topSpace > 9) {
      horShift += topSpace + 9 - horShift;
    }
    for (byte i = 0; i < FIGURE_LENGTH; i++) {
      for (byte j = 0; j < 16; j++) {
        if (figure[i] & bites[j]) {
          newFigure[j - figurePosHor] |= bites[horShift + (3 - i)];
        }
      }
    }
      
    if (checkPlaceForFigure(newFigure)) {
      figurePosHor = horShift;
      for (byte i = 0; i < FIGURE_LENGTH; i++) {
        figure[i] = newFigure[i];
      } 
    }
  }
}

unsigned long lastMoveHor = 0;
void moveFigureHor() {
  if (figureDirectionClicked != 0) {
    lastMoveHor = millis(); 
  }
  byte direction = figureDirectionClicked;
  figureDirectionClicked = 0;
  if (millis() - lastMoveHor > TERIS_PRESS_BUTTON_DELAY) {
    if (leftPressed and rightPressed) {
      direction = 0;
    } else if (tetrisTopPressed) {
      direction = 1;
    } else if (tetrisBottomPressed) {
      direction = 2;
    }
  }

  unsigned int newFigure[FIGURE_LENGTH];
  if (direction == 2) {
    for (byte i = 0; i < FIGURE_LENGTH; i++) {
      newFigure[i] = figure[i] << 1;
      if (newFigure[i] & 0b1110000000000000) {
        return;
      }
      if (field[figurePosVert + i] & newFigure[i]) {
        return;
      }
    }
  } else if (direction == 1) {
    for (byte i = 0; i < FIGURE_LENGTH; i++) {
      newFigure[i] = figure[i] >> 1;
      if (field[figurePosVert + i] & newFigure[i]) {
        return;
      }
      if (newFigure[i] & 0b0000000000000111) {
        return;
      }
    }
  } else {
    return;
  }
  figurePosHor += direction == 1 ? 1 : -1;
  for (byte i = 0; i < FIGURE_LENGTH; i++) {
    figure[i] = newFigure[i];
  }
}

void drawDot(int x, int y, bool color) {
  display.drawBitmap(TETRIS_PADDING + TETRIS_DOT_SIZE * x, 8 + TETRIS_DOT_SIZE * y, dotBitmap, TETRIS_DOT_SIZE, TETRIS_DOT_SIZE, color);
}

void drawFigure(bool color) {
  for (int i = 0; i < FIGURE_LENGTH; i++) {
    int maskLine = 0b0000000000001000;
    for (int j = 0; j < TETRIS_WIDTH; j++) {
      if (figure[i] & maskLine) {
        drawDot(figurePosVert + i, j, color);
      }
      maskLine <<= 1;
    }
  }
}

void drawFullLine(int i, bool color) {
  for (int j = 0; j < TETRIS_WIDTH; j++) {
    drawDot(i, j, color);
  }
}

void checkFullLines() {
  int fullLinesCount = 0;
  for (int i = 0; i < TETRIS_LENGTH; i++) {
    if (field[i] == 0b0001111111111000) {
      fullLinesCount++;
      drawFullLine(i, WHITE);
    }
  }
  if (fullLinesCount == 0) {
    return;
  }
  display.display();
  delay(50);

  int shift = 0;
  for (int i = TETRIS_LENGTH - 1; i >= shift; i--) {
    while (shift <= i && field[i - shift] == 0b0001111111111000) {
      shift++;
    }
    if (shift <= i) {
      field[i] = field[i - shift];   
    } else {
      field[i] = 0b0000000000000000;
    }
  }
  addPoints(fullLinesCount * fullLinesCount * 13);
}

void addPoints(int points) {
  tetrisPoints += points;
  pointsToNextLevel += points;
  if (pointsToNextLevel > tetrisLevel * 400) {
    pointsToNextLevel -= tetrisLevel * 400;
    tetrisLevel++;
    if (tetrisLevel < START_SPEED) {
      tetrisSpeed = START_SPEED - tetrisLevel;
    }
  }
}

unsigned long lastTetrisDraw;
byte frameTetrisNumber = 0;
byte frameTetrisMoveNumber = 0;
void loopTetris() {
  tetrisTopPressed = buttonTop.isPressed();
  tetrisBottomPressed = buttonBottom.isPressed();
  tetrisRightPressed = buttonRight.isPressed();
  buttonLeft.isPressed();

  if (tetrisTopPressed && tetrisBottomPressed) {
    pause = true;
  }
  if (!pause && millis() - lastTetrisDraw > TETRIS_DRAW_DELAY) {
    lastTetrisDraw = millis();
    if (needNewFigure) {
      needNewFigure = false;
      currentFigure = nextFigure;
      nextFigure = random(FIGURE_VARIANT_NUMBER);
      figurePosVert = 0;
      figurePosHor = 6;
      
      for (byte i = 0; i < FIGURE_LENGTH; i++) {
        figure[i] = figuresBitmap[currentFigure * FIGURE_LENGTH + i];
        if (figure[i] & field[i]) {
          // loose
          setupTetris();
          return;
        }
      }
    }
    if (frameTetrisMoveNumber >= TETRIS_MOVE_SPEED) {
      frameTetrisMoveNumber = 0;
      moveFigureHor();
      rotateFigure();
    }    
    
    
    display.clearDisplay();
    display.drawRect(83, 8, 1, 40, BLACK);
    
//    display.drawBitmap(0, 0, nextBitmap, 8, 8, BLACK);
    if (tetrisLevel < 15) {
      switch (nextFigure) {
        case 0:
          display.drawBitmap(0, 0, mini0Bitmap, 8, 8, BLACK);
          break;
        case 1:
          display.drawBitmap(0, 0, mini1Bitmap, 8, 8, BLACK);
          break;
        case 2:
          display.drawBitmap(0, 0, mini2Bitmap, 8, 8, BLACK);
          break;
        case 3:
          display.drawBitmap(0, 0, mini3Bitmap, 8, 8, BLACK);
          break;
        case 4:
          display.drawBitmap(0, 0, mini4Bitmap, 8, 8, BLACK);
          break;
        case 5:
          display.drawBitmap(0, 0, mini5Bitmap, 8, 8, BLACK);
          break;
        case 6:
          display.drawBitmap(0, 0, mini6Bitmap, 8, 8, BLACK);
          break;
      }
    }
    for (int i = 0; i < TETRIS_LENGTH; i++) {
      int maskLine = 0b0000000000001000;
      for (int j = 0; j < TETRIS_WIDTH; j++) {
        if (field[i] & maskLine) {
          drawDot(i, j, BLACK);
        }
        maskLine <<= 1;
      }
    }
    if (frameTetrisNumber >= (moveDownClicked || tetrisRightPressed ? TETRIS_MOVE_SPEED : tetrisSpeed)) {
      moveDownClicked = false;
      frameTetrisNumber = 0;
      if (!moveFigureDown()) {
        drawFigure(WHITE);
        display.display();
        delay(50);
        drawFigure(BLACK);
        display.display();
        delay(50);
        needNewFigure = true;
        addPoints(currentFigure + 3);
        for (int i = 0; i < FIGURE_LENGTH; i++) {
          if (figure[i] && (figurePosVert + i < TETRIS_LENGTH)) {
            field[figurePosVert + i] |= figure[i];
          }
        }
        for (byte i = 0; i < FIGURE_LENGTH; i++) {
          figure[i] = 0b0000000000000000;
        }
        checkFullLines();
      }
    }
    drawFigure(BLACK);
    
    display.drawBitmap(5, 0, levelBitmap, 8, 8, BLACK);
    display.setCursor(14,0);
    display.print(tetrisLevel);
    display.print(" ");
    display.print(tetrisPoints);
    display.display();
    frameTetrisNumber++;
    frameTetrisMoveNumber++;
  }
}
