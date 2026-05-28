#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TOUCH_PIN 4

// Game constants
#define GROUND_Y 54
#define DINO_X 15
#define DINO_W 12
#define DINO_H 14

#define CACTUS_W 8
#define CACTUS_H 16

int dinoY = GROUND_Y - DINO_H;
int velocity = 0;
bool jumping = false;

int cactusX = SCREEN_WIDTH;
int score = 0;
int highScore = 0;

bool gameOver = false;
unsigned long lastFrame = 0;
int gameSpeed = 4;

bool isTouched() {
  return digitalRead(TOUCH_PIN) == HIGH;
}

void resetGame() {
  dinoY = GROUND_Y - DINO_H;
  velocity = 0;
  jumping = false;

  cactusX = SCREEN_WIDTH;
  score = 0;
  gameSpeed = 4;
  gameOver = false;
}

void drawDino() {
  display.fillRect(DINO_X, dinoY, DINO_W, DINO_H, SSD1306_WHITE);

  // Eye
  display.drawPixel(DINO_X + 8, dinoY + 3, SSD1306_BLACK);

  // Legs
  display.drawLine(DINO_X + 2, dinoY + DINO_H, DINO_X + 2, dinoY + DINO_H + 3, SSD1306_WHITE);
  display.drawLine(DINO_X + 9, dinoY + DINO_H, DINO_X + 9, dinoY + DINO_H + 3, SSD1306_WHITE);
}

void drawCactus() {
  display.fillRect(cactusX, GROUND_Y - CACTUS_H, CACTUS_W, CACTUS_H, SSD1306_WHITE);
  display.fillRect(cactusX - 3, GROUND_Y - 10, 3, 6, SSD1306_WHITE);
  display.fillRect(cactusX + CACTUS_W, GROUND_Y - 13, 3, 6, SSD1306_WHITE);
}

void drawGround() {
  display.drawLine(0, GROUND_Y + 3, SCREEN_WIDTH, GROUND_Y + 3, SSD1306_WHITE);

  // Small moving ground dots
  for (int i = 0; i < SCREEN_WIDTH; i += 16) {
    int x = (i - score * 2) % SCREEN_WIDTH;
    if (x < 0) x += SCREEN_WIDTH;
    display.drawPixel(x, GROUND_Y + 6, SSD1306_WHITE);
  }
}

void drawScore() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(80, 0);
  display.print("S:");
  display.print(score);

  display.setCursor(0, 0);
  display.print("HI:");
  display.print(highScore);
}

bool checkCollision() {
  int dinoLeft = DINO_X;
  int dinoRight = DINO_X + DINO_W;
  int dinoTop = dinoY;
  int dinoBottom = dinoY + DINO_H;

  int cactusLeft = cactusX;
  int cactusRight = cactusX + CACTUS_W;
  int cactusTop = GROUND_Y - CACTUS_H;
  int cactusBottom = GROUND_Y;

  return dinoRight > cactusLeft &&
         dinoLeft < cactusRight &&
         dinoBottom > cactusTop &&
         dinoTop < cactusBottom;
}

void showGameOver() {
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(12, 14);
  display.print("GAME OVER");

  display.setTextSize(1);
  display.setCursor(35, 38);
  display.print("Score: ");
  display.print(score);

  display.setCursor(18, 52);
  display.print("Touch to restart");

  display.display();
}

void updateGame() {
  if (isTouched() && !jumping) {
    jumping = true;
    velocity = -10;
  }

  if (jumping) {
    dinoY += velocity;
    velocity += 1;

    if (dinoY >= GROUND_Y - DINO_H) {
      dinoY = GROUND_Y - DINO_H;
      jumping = false;
      velocity = 0;
    }
  }

  cactusX -= gameSpeed;

  if (cactusX < -CACTUS_W) {
    cactusX = SCREEN_WIDTH + random(20, 70);
    score++;

    if (score > highScore) {
      highScore = score;
    }

    if (score % 5 == 0 && gameSpeed < 9) {
      gameSpeed++;
    }
  }

  if (checkCollision()) {
    gameOver = true;
    showGameOver();
  }
}

void drawGame() {
  display.clearDisplay();

  drawScore();
  drawGround();
  drawDino();
  drawCactus();

  display.display();
}

void setup() {
  pinMode(TOUCH_PIN, INPUT);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  randomSeed(analogRead(34));

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(20, 18);
  display.print("DINO");
  display.setTextSize(1);
  display.setCursor(18, 45);
  display.print("Touch to start");
  display.display();

  while (!isTouched()) {
    delay(10);
  }

  resetGame();
}

void loop() {
  if (gameOver) {
    if (isTouched()) {
      delay(300);
      resetGame();
    }
    return;
  }

  unsigned long now = millis();

  if (now - lastFrame >= 35) {
    lastFrame = now;
    updateGame();
    drawGame();
  }
}