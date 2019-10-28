#define pipeDelay 50
static const unsigned char PROGMEM bird_up[] =
{
  B00000110,
  B00001101,
  B00011110,
  B00001100
};
static const unsigned char PROGMEM birdf[] =
{
B00000110,
B00011101,
B00001110,
B00000100
};
static const unsigned char PROGMEM birdd[] =
{
B00000100,
B00011110,
B00001101,
B00000110
};
static const unsigned char PROGMEM bird_dead[] =
{
B00000100,
B00001110,
B00001111,
B00000101,
B00000010
};


int points = 0;
int birdPosition = 24;
bool gameStart = true;
unsigned long lastGameover = 0;
int birdSpeed = 0;
int pipes[3];
int pipeIndex = 0;
int pipeShift = 9;
int pipeNumber = 0;


void drawPoints() {
   display.println(points);
}

void loopBird() {
  if (gameStart) {
    display.clearDisplay();
  }
  buttonTop.isPressed();
  if (gameStart) {
    drawBird();
  }
  if (gameStart) {
    drawPipes();
    drawPoints();
    display.display();  
  }
}


void gameover() {
  lastGameover = millis();
  gameStart = false;
  
  delay(1000);
//  display.clearDisplay();
//  display.setTextSize(2);
//  display.setTextColor(BLACK);
//  display.setCursor(0,10);
//  display.println(points);
//  display.display();  
}

unsigned long lastBirdDraw = 0;

//int birdDelays[10] = [];
int birdDelay = 100;
void drawBird() {
  if (birdSpeed == 0) {
    display.drawBitmap(10, 48 - birdPosition, birdf, 8, 4, BLACK);
  } else {
    if (birdSpeed > 0) {
    display.drawBitmap(10, 48 - birdPosition, bird_up, 8, 4, BLACK); 
    } else {
    display.drawBitmap(10, 48 - birdPosition, birdd, 8, 4, BLACK);   
    }
  }
  if (pipeNumber == 3 and pipeShift > 5 and pipeShift < 15) {
    int ind = (3 + pipeIndex - 2) % 3;
    if (48 - birdPosition < pipes[ind] + 1) {
        gameover();
        return;
    }
    if (48 - birdPosition > pipes[ind] + 13) {
        gameover();
        return;
    }
  }
  if (millis() - lastBirdDraw > birdDelay) {
    lastBirdDraw = millis();
    birdSpeed--;
//    birdDelay = birdDelay / (birdSpeed*birdSpeed);
  birdDelay = (2305 - (birdSpeed*birdSpeed)) / 100;
    if (birdSpeed > 0) {
      birdPosition++;
      if (birdPosition == 48) {
        gameover();
        return;
      }
    } else {
      birdPosition--;
      if (birdPosition == 0) {
        gameover();
        return;
      }
    }
  }
}


unsigned long lastPipeDraw = 0;
void drawPipes() {
  for (int i = 0; i < pipeNumber; i++) {
    int ind = (3 + pipeIndex - i) % 3;
    // нижняя труба
    display.drawRect(84 - pipeShift - i*30, - 1, 5, pipes[ind], BLACK);
    display.drawRect(84 - pipeShift - i*30 - 1, pipes[ind] - 2, 7, 3, BLACK);

    // верхняя труба
    display.drawRect(84 - pipeShift - i*30, pipes[ind] + 19, 5, 30 - pipes[ind], BLACK);
    display.drawRect(84 - pipeShift - i*30 - 1, pipes[ind] + 17, 7, 3, BLACK);
  }
  if (millis() - lastPipeDraw > pipeDelay) {
    lastPipeDraw = millis();
    pipeShift++;
    if (pipeShift == 20 and pipeNumber == 3) {
        points++;
    }
    if (pipeShift > 29) {
      pipeShift = 0;
      if (pipeNumber < 3) {
        pipeNumber++;
      }
      if (pipeIndex < 2) {
        pipeIndex++;
      } else {
        pipeIndex = 0;
      }
      pipes[pipeIndex] = random(20) + 4;
    }
  }
}

void setupBird() {
  display.begin();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  
  buttonTop.pressHandler(btnBirdTopHandler);
}
void btnBirdTopHandler() {
  if (!gameStart) {
    if (millis() - lastGameover > 1000) {
      points = 0;
      gameStart = true; 
      birdPosition = 24;
      birdSpeed = 0;
      display.setTextSize(1);
      display.setCursor(0,0);
      pipeIndex = 0;
      pipeShift = 9;
      pipeNumber = 0;
    }
  } else {
    birdSpeed = 10;
  }
}
