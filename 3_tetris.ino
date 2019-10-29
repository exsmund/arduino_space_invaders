#define TETRIS_DRAW_DELAY 50
#define TETRIS_LENGTH 20
#define FIGURE_LENGTH 4
#define TETRIS_WIDTH 10
#define TETRIS_DOT_SIZE 4
#define TETRIS_PADDING 2
#define TETRIS_MOVE_SPEED 2
static const unsigned char PROGMEM dotBitmap[] = 
{
  0b11110000,
  0b10010000,
  0b10110000,
  0b11110000,
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


// 0 - I
// 1 - L
// 2 - r
// 3 - T
// 4 - o
static const unsigned int figuresBitmap[] =
{
  0b0000000001111000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  
  0b0000000000111000,
  0b0000000000100000,
  0b0000000000000000,
  0b0000000000000000,
  
  0b0000000001110000,
  0b0000000000010000,
  0b0000000000000000,
  0b0000000000000000,
  
  0b0000000001110000,
  0b0000000000100000,
  0b0000000000000000,
  0b0000000000000000,
  
  0b0000000000110000,
  0b0000000000110000,
  0b0000000000000000,
  0b0000000000000000,
};
unsigned int field[TETRIS_LENGTH];
unsigned int figure[FIGURE_LENGTH];
int maskLine;
bool needNewFigure;
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

void btnTetrisLeftHandler() {
  needNewFigure = true;
}

void btnTetrisTopHandler() {
  figureDirectionClicked = 1;
}

void btnTetrisBottomHandler() {
  figureDirectionClicked = 2;
}

void btnTetrisRightHandler() {
  moveDownClicked = true;
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
  tetrisSpeed = 40;
  figurePosVert = 0;
  figurePosHor = 0;
  nextFigure = random(5);
  figureDirectionClicked = 0;
  moveDownClicked = false;
  tetrisPoints = 0;

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

void moveFigureHor() {
  byte direction = figureDirectionClicked;
  figureDirectionClicked = 0;
  if (leftPressed and rightPressed) {
    direction = 0;
  } else if (tetrisTopPressed) {
    direction = 1;
  } else if (tetrisBottomPressed) {
    direction = 2;
  }

  unsigned int newFigure[FIGURE_LENGTH];
  unsigned int merge = 0;
  if (direction == 2) {
    for (byte i = 0; i < FIGURE_LENGTH; i++) {
      merge |= figure[i];
      newFigure[i] = figure[i] << 1;
      if (newFigure[i]
          && field[figurePosVert + i] & newFigure[i]) {
        return false;
      }
    }
    Serial.println(merge);
    if (merge & 0b0000001000000000) {
      return;
    }
  } else if (direction == 1) {
    for (byte i = 0; i < FIGURE_LENGTH; i++) {
      merge |= figure[i];
      newFigure[i] = figure[i] >> 1;
      if (newFigure[i]
          && field[figurePosVert + i] & newFigure[i]) {
        return false;
      }
    }
    Serial.println(merge);
    if (merge & 1) {
      return;
    }
  } else {
    return;
  }
  for (byte i = 0; i < FIGURE_LENGTH; i++) {
    figure[i] = newFigure[i];
  }
}

void drawDot(int x, int y) {
  display.drawBitmap(TETRIS_PADDING + TETRIS_DOT_SIZE * x, TETRIS_DOT_SIZE * y, dotBitmap, TETRIS_DOT_SIZE, TETRIS_DOT_SIZE, BLACK);    
}

unsigned long lastTetrisDraw;
byte frameTetrisNumber = 0;
byte frameTetrisMoveNumber = 0;
void loopTetris() {
  tetrisTopPressed = buttonTop.isPressed();
  tetrisBottomPressed = buttonBottom.isPressed();
  tetrisRightPressed = buttonRight.isPressed();
  buttonLeft.isPressed();
  
  if (millis() - lastTetrisDraw > TETRIS_DRAW_DELAY) {
    lastTetrisDraw = millis();
    if (needNewFigure) {
      needNewFigure = false;
      currentFigure = nextFigure;
      nextFigure = random(5);
      figurePosVert = 0;
      figurePosHor = 0;
      
      for (byte i = 0; i < FIGURE_LENGTH; i++) {
        figure[i] = figuresBitmap[currentFigure * FIGURE_LENGTH + i];
//        Serial.println(currentFigure * FIGURE_LENGTH + i);
//        Serial.println(figuresBitmap[currentFigure * FIGURE_LENGTH + i]);
        if (figure[i] & field[i]) {
          // loose
          setupTetris();
          return;
        }
      }
    }
    if (frameTetrisNumber >= (moveDownClicked || tetrisRightPressed ? TETRIS_MOVE_SPEED : tetrisSpeed)) {
      moveDownClicked = false;
      frameTetrisNumber = 0;
      if (!moveFigureDown()) {
        needNewFigure = true;
        tetrisPoints += 10;
        for (int i = 0; i < FIGURE_LENGTH; i++) {
          if (figure[i] && (figurePosVert + i < TETRIS_LENGTH)) {
            field[figurePosVert + i] |= figure[i];
          }
        }
      }
    }    
    
    if (frameTetrisMoveNumber >= TETRIS_MOVE_SPEED) {
      frameTetrisMoveNumber = 0;
      moveFigureHor();
    }
    
    display.clearDisplay();
//    display.drawRect(0, 0, 84, 44, BLACK);
    
    display.drawBitmap(0, SCREEN_HEIGHT - 8, nextBitmap, 8, 8, BLACK);
    switch (nextFigure) {
      case 0:
        display.drawBitmap(10, SCREEN_HEIGHT - 8, mini0Bitmap, 8, 8, BLACK);
        break;
      case 1:
        display.drawBitmap(10, SCREEN_HEIGHT - 8, mini1Bitmap, 8, 8, BLACK);
        break;
      case 2:
        display.drawBitmap(10, SCREEN_HEIGHT - 8, mini2Bitmap, 8, 8, BLACK);
        break;
      case 3:
        display.drawBitmap(10, SCREEN_HEIGHT - 8, mini3Bitmap, 8, 8, BLACK);
        break;
      case 4:
        display.drawBitmap(10, SCREEN_HEIGHT - 8, mini4Bitmap, 8, 8, BLACK);
        break;
    }
    for (int i = 0; i < TETRIS_LENGTH; i++) {
      int maskLine = 0b0000000000000001;
      for (int j = 0; j < TETRIS_WIDTH; j++) {
        if (field[i] & maskLine) {
          drawDot(i, j);
        }
        maskLine <<= 1;
      }
    }
    for (int i = 0; i < FIGURE_LENGTH; i++) {
      int maskLine = 0b0000000000000001;
      for (int j = 0; j < TETRIS_WIDTH; j++) {
        if (figure[i] & maskLine) {
          drawDot(figurePosVert + i, j);
        }
        maskLine <<= 1;
      }
    }
    display.setCursor(1,3);
    display.println(tetrisPoints);
    display.display();
    frameTetrisNumber++;
    frameTetrisMoveNumber++;
  }
}
