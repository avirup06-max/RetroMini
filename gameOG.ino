#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Button pins
const int btnUp = 32;
const int btnDown = 33;
const int btnLeft = 25;
const int btnRight = 26;
const int btnCenter = 27;

const int buzzerPin = 15;  // Add this near the top with your pin definitions


// Joystick pins
#define joyXPin 34  // Replace with actual X-axis analog pin
#define joyYPin 35  // Replace with actual Y-axis analog pin
const int joySW = 14; // Optional (joystick button)

int selectedGame = 0;


// Functions declarations
void drawMenu();
void runSelectedGame();
void ticTacToe();
void snakeGame();
void tetrisGame();
void flappyBird();
void carRacing();
void asteroidShooting();
void pongGame();

const char* gameList[] = {
  "Tic-Tac-Toe",
  "Snake",
  "Tetris",
  "Flappy Bird",
  "Car Racing",
  "Asteroid Shooting",
  "Pong"
};
const int totalGames = sizeof(gameList) / sizeof(gameList[0]);


void setup() {
  Serial.begin(115200);

  // OLED init
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();

  showStartupMessage();
  
  // Buttons init
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnLeft, INPUT_PULLUP);
  pinMode(btnRight, INPUT_PULLUP);
  pinMode(btnCenter, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);


  pinMode(joySW, INPUT_PULLUP); // Optional

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  drawMenu();
}

void loop() {
  // MENU navigation
  if(digitalRead(btnUp) == LOW) {
    selectedGame--;
    if(selectedGame < 0) selectedGame = totalGames - 1;
    drawMenu();
    delay(200);
  }
  if(digitalRead(btnDown) == LOW) {
    selectedGame++;
    if(selectedGame >= totalGames) selectedGame = 0;
    drawMenu();
    delay(200);
  }
  if(digitalRead(btnCenter) == LOW) {
    runSelectedGame();
  }
}

void showStartupMessage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int16_t x, y;
  uint16_t w, h;

  // Line 1: "Developed By:"
  String line1 = "Developed By:";
  display.getTextBounds(line1, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 5);
  display.println(line1);

  // Line 2: "--- IPESP ---"
  String line2 = "--- IPESP ---";
  display.getTextBounds(line2, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 25);
  display.println(line2);

  // Line 3: "Development Solution"
  String line3 = "Development";
  display.getTextBounds(line3, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 40);
  display.println(line3);

  String line4 = "Solution";
  display.getTextBounds(line4, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 50);
  display.println(line4);

  display.display();
  tone(buzzerPin, 523, 200);  // C4 (523 Hz) for 200 ms
  delay(220);                 // Wait for the first note to finish
  tone(buzzerPin, 659, 200);  // E4 (659 Hz) for 200 ms
  delay(220);                 // Wait for the second note to finish
  tone(buzzerPin, 784, 200);  // G4 (784 Hz) for 200 ms
  delay(220);                 // Wait for the third note to finish
  tone(buzzerPin, 880, 200);  // A4 (880 Hz) for 200 ms
  delay(220);                 // Wait for the fourth note to finish
  tone(buzzerPin, 523, 300);  // C5 (523 Hz) for 300 ms (higher octave)
  delay(350); 
  noTone(buzzerPin);
  delay(1500); // Show for 2.5 seconds
}


// =========== SOUNDS ============

void playSelectSound() {
  tone(buzzerPin, 1000, 100); // Cursor move sound
}

void playPlaceMoveSound() {
  tone(buzzerPin, 1200, 150); // Piece placed
}

void playWinSound() {
  tone(buzzerPin, 880, 300);  // First tone
  delay(300);
  tone(buzzerPin, 1320, 300); // Second tone
  delay(300);
  noTone(buzzerPin);
}

void playDrawSound() {
  tone(buzzerPin, 500, 200);
  delay(200);
  tone(buzzerPin, 400, 200);
  delay(200);
  noTone(buzzerPin);
}


void playEatSound() {
  tone(buzzerPin, 1500, 100);
  delay(100);
  noTone(buzzerPin);
}

// Called on game over
void playGameOverSound() {
  tone(buzzerPin, 500, 200);
  delay(200);
  tone(buzzerPin, 400, 200);
  delay(200);
  tone(buzzerPin, 300, 400);
  delay(400);
  noTone(buzzerPin);
}

void playMoveSound() {
  tone(buzzerPin, 900, 50); // Quick move sound
}

void playRotateSound() {
  tone(buzzerPin, 1100, 80); // Rotate sound
}

void playLockSound() {
  tone(buzzerPin, 700, 100); // Piece lock sound
}

void playLineClearSound() {
  tone(buzzerPin, 1000, 150);
  delay(100);
  tone(buzzerPin, 1200, 150);
  delay(100);
  noTone(buzzerPin);
}


// ============ Menu ============

void drawMenu() {
  // play a quick click whenever the menu is redrawn
  tone(buzzerPin, 800, 50);  // 800 Hz for 50 ms

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Select Game:");

  const int maxVisibleItems = 5; // max lines OLED can show (excluding title)
  int startIdx = selectedGame - selectedGame % maxVisibleItems;

  for (int i = 0; i < maxVisibleItems && (startIdx + i) < totalGames; i++) {
    display.setCursor(0, 10 + i * 10); // x=0 for arrow
    if ((startIdx + i) == selectedGame) {
      display.print("> ");
    } else {
      display.print("  ");
    }
    display.println(gameList[startIdx + i]);
  }

  display.display();
}




void runSelectedGame() {
  tone(buzzerPin, 1000, 200);  
  switch (selectedGame) {
    case 0: ticTacToe(); break;
    case 1: snakeGame(); break;
    case 2: tetrisGame(); break;
    case 3: flappyBird(); break;
    case 4: carRacing(); break;
    case 5: asteroidShooting(); break;
    case 6: pongGame(); break;
  }
}


void showButtonControlInfo(String gameName) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int16_t x, y;
  uint16_t w, h;

  display.getTextBounds(gameName, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 15);
  display.println(gameName);

  String msg = "Use BUTTONS to play";
  display.getTextBounds(msg, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 35);
  display.println(msg);

  display.display();
  delay(2000);
}


void showJoystickControlInfo(String gameName) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int16_t x, y;
  uint16_t w, h;

  display.getTextBounds(gameName, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 15);
  display.println(gameName);

  String msg = "Use JOYSTICK to play";
  display.getTextBounds(msg, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 35);
  display.println(msg);

  display.display();
  delay(2000);
}


// =================== TIC-TAC-TOE ==================================================================================================

void ticTacToe() {
  showButtonControlInfo("Tic-Tac-Toe Game");
game_start:
  while (digitalRead(btnCenter) == LOW ||
         digitalRead(btnUp) == LOW ||
         digitalRead(btnDown) == LOW ||
         digitalRead(btnLeft) == LOW ||
         digitalRead(btnRight) == LOW) {
    delay(10);
  }

  int board[3][3] = {0}; // 0=empty, 1=X, 2=O
  int curX = 0, curY = 0;
  int player = 1;
  bool gameEnded = false;
  int winner = 0;
  int moveCount = 0;

  while (!gameEnded) {
    // Input
    if (digitalRead(btnUp) == LOW && curY > 0) {
      curY--; playSelectSound(); delay(150);
    }
    if (digitalRead(btnDown) == LOW && curY < 2) {
      curY++; playSelectSound(); delay(150);
    }
    if (digitalRead(btnLeft) == LOW && curX > 0) {
      curX--; playSelectSound(); delay(150);
    }
    if (digitalRead(btnRight) == LOW && curX < 2) {
      curX++; playSelectSound(); delay(150);
    }

    if (digitalRead(btnCenter) == LOW) {
      if (board[curX][curY] == 0) {
        board[curX][curY] = player;
        moveCount++;
        playPlaceMoveSound();

        // Check win
        for (int i = 0; i < 3; i++) {
          if (board[i][0] && board[i][0] == board[i][1] && board[i][1] == board[i][2]) winner = board[i][0];
          if (board[0][i] && board[0][i] == board[1][i] && board[1][i] == board[2][i]) winner = board[0][i];
        }
        if (board[0][0] && board[0][0] == board[1][1] && board[1][1] == board[2][2]) winner = board[0][0];
        if (board[2][0] && board[2][0] == board[1][1] && board[1][1] == board[0][2]) winner = board[2][0];

        if (winner != 0 || moveCount == 9) gameEnded = true;
        player = (player == 1) ? 2 : 1;
        delay(150);
      }
    }

    // Draw board
    display.clearDisplay();
    display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
    display.drawLine(43, 0, 43, 64, SSD1306_WHITE);
    display.drawLine(85, 0, 85, 64, SSD1306_WHITE);
    display.drawLine(0, 21, 128, 21, SSD1306_WHITE);
    display.drawLine(0, 43, 128, 43, SSD1306_WHITE);

    for (int x = 0; x < 3; x++) {
      for (int y = 0; y < 3; y++) {
        int px = x * 43 + 14;
        int py = y * 21 + 6;
        if (board[x][y] == 1)
          display.drawChar(px, py, 'X', SSD1306_WHITE, BLACK, 2);
        if (board[x][y] == 2)
          display.drawChar(px, py, 'O', SSD1306_WHITE, BLACK, 2);
      }
    }

    display.drawRect(curX * 43 + 1, curY * 21 + 1, 41, 19, SSD1306_WHITE);
    display.display();
    delay(50);
  }

  // Game Over Screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor((128 - 90) / 2, 10);
  display.println("== Game Over ==");

  display.setCursor((128 - 95) / 2, 25);
  if (winner) {
    display.print("Winner: Player ");
    display.print(winner);
    playWinSound();
  } else {
    display.print("Draw");
    playDrawSound();
  }

  display.setCursor((128 - 120) / 2, 40);
  display.println("Click Center = Menu");
  display.setCursor((128 - 120) / 2, 52);
  display.println("Other Button = Retry");
  display.display();

  while (true) {
    if (digitalRead(btnCenter) == LOW) {
      delay(300);
      drawMenu();
      return;
    }
    if (digitalRead(btnUp) == LOW || digitalRead(btnDown) == LOW ||
        digitalRead(btnLeft) == LOW || digitalRead(btnRight) == LOW) {
      delay(300);
      goto game_start;
    }
  }
}

// =================== SNAKE ==================================================================================================

int snakeScore = 0;
int snakeHighScore = 0;
void snakeGame() {
  showButtonControlInfo("Snake Game");
  const int gridSize = 4;
  const int width = SCREEN_WIDTH / gridSize;
  const int height = (SCREEN_HEIGHT - 10) / gridSize;  // Leave space for score bar

  int snakeX[128], snakeY[64];
  int snakeLength = 3;
  int foodX = random(width), foodY = random(height);
  int dirX = 1, dirY = 0;

  snakeScore = 0;

  // Initialize snake in center
  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = width / 2 - i;
    snakeY[i] = height / 2;
  }

  while (true) {
    // —— Read input & play cursor sound ——
    if (digitalRead(btnUp) == LOW && dirY != 1)    { dirX = 0;  dirY = -1; playSelectSound(); }
    if (digitalRead(btnDown) == LOW && dirY != -1)  { dirX = 0;  dirY = 1;  playSelectSound(); }
    if (digitalRead(btnLeft) == LOW && dirX != 1)   { dirX = -1; dirY = 0;  playSelectSound(); }
    if (digitalRead(btnRight) == LOW && dirX != -1) { dirX = 1;  dirY = 0;  playSelectSound(); }

    // —— Move snake body ——
    for (int i = snakeLength - 1; i > 0; i--) {
      snakeX[i] = snakeX[i - 1];
      snakeY[i] = snakeY[i - 1];
    }
    snakeX[0] += dirX;
    snakeY[0] += dirY;

    // —— Check wall collision ——
    if (snakeX[0] < 0 || snakeY[0] < 0 || snakeX[0] >= width || snakeY[0] >= height)
      goto gameOver;

    // —— Check self-collision ——
    for (int i = 1; i < snakeLength; i++) {
      if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
        goto gameOver;
    }

    // —— Eating food ——
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
      if (snakeLength < 128) snakeLength++;
      foodX = random(width);
      foodY = random(height);
      snakeScore++;
      if (snakeScore > snakeHighScore) snakeHighScore = snakeScore;
      playEatSound();
    }

    // —— Draw everything ——
    display.clearDisplay();

    // Score bar
    display.drawRect(0, 0, SCREEN_WIDTH, 10, SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(2, 1);
    display.print("S:"); display.print(snakeScore);
    display.setCursor(SCREEN_WIDTH - 40, 1);
    display.print("HS:"); display.print(snakeHighScore);

    // Snake
    for (int i = 0; i < snakeLength; i++) {
      int x = snakeX[i] * gridSize;
      int y = snakeY[i] * gridSize + 10;
      if (i == 0)
        display.fillRect(x, y, gridSize, gridSize, SSD1306_INVERSE);
      else
        display.fillRect(x, y, gridSize, gridSize, SSD1306_WHITE);
    }

    // Food
    display.fillRect(foodX * gridSize, foodY * gridSize + 10, gridSize, gridSize, SSD1306_BLACK);
    display.drawRect(foodX * gridSize, foodY * gridSize + 10, gridSize, gridSize, SSD1306_WHITE);

    display.display();
    delay(100);
  }

gameOver:
  playGameOverSound();

  display.clearDisplay();
  display.setTextSize(1);

  // Centered "GAME OVER!"
  display.setCursor((128 - 90) / 2, 5);
  display.println("== GAME OVER ==");

  // Centered Score
  display.setCursor((128 - 85) / 2, 20);
  display.print("Your Score: ");
  display.println(snakeScore);

  // Centered High Score
  display.setCursor((128 - 85) / 2, 31);
  display.print("High Score: ");
  display.println(snakeHighScore);

  // Instructions
  display.setCursor((128 - 120) / 2, 43);
  display.println("Click Center = Menu");
  display.setCursor((128 - 120) / 2, 53);
  display.println("Other Button = Retry");
  display.display();

  // Wait for user choice
  while (true) {
    if (digitalRead(btnCenter) == LOW) {
      delay(300);
      drawMenu();
      return;
    }
    if (digitalRead(btnUp) == LOW || digitalRead(btnDown) == LOW ||
        digitalRead(btnLeft) == LOW || digitalRead(btnRight) == LOW) {
      delay(300);
      snakeGame();  // restart game
      return;
    }
  }
}


// =================== TETRIS ==================================================================================================


void tetrisGame() {
  showButtonControlInfo("Tetris Game");
  const int gridW = 10, gridH = 16;
  const int cellW = 5, cellH = 4;
  const int gridOffsetX = 10;  // Shift grid to center horizontally
  const int gridOffsetY = 0;
  int grid[gridW][gridH] = {0};

  // Tetromino shapes (4x4)
  const byte shapes[7][4][4] = {
    { // I
      {0, 0, 0, 0},
      {1, 1, 1, 1},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    { // O
      {0, 1, 1, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    { // T
      {0, 1, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    { // S
      {0, 1, 1, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    { // Z
      {1, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    { // J
      {1, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    { // L
      {0, 0, 1, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    }
  };

  int currentShape = random(0, 7);
  int px = 3, py = 0;
  int score = 0;
  static int highScore = 0;
  unsigned long lastFall = millis();
  int fallSpeed = 500;

  auto rotateShape = [](const byte shape[4][4], byte rotated[4][4]) {
    for (int y = 0; y < 4; y++)
      for (int x = 0; x < 4; x++)
        rotated[x][3 - y] = shape[y][x];
  };

  auto drawGrid = [&]() {
    display.clearDisplay();

    // Grid boundary lines
    int gridPixelW = gridW * cellW;
    int gridPixelH = gridH * cellH;
    display.drawRect(gridOffsetX - 1, gridOffsetY - 1, gridPixelW + 2, gridPixelH + 2, SSD1306_WHITE);

    // Draw static blocks
    for (int y = 0; y < gridH; y++) {
      for (int x = 0; x < gridW; x++) {
        if (grid[x][y])
          display.fillRect(gridOffsetX + x * cellW, gridOffsetY + y * cellH, cellW - 1, cellH - 1, SSD1306_WHITE);
      }
    }

    // Draw current shape
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (shapes[currentShape][i][j]) {
          int x = px + j;
          int y = py + i;
          if (x >= 0 && x < gridW && y >= 0 && y < gridH)
            display.fillRect(gridOffsetX + x * cellW, gridOffsetY + y * cellH, cellW - 1, cellH - 1, SSD1306_WHITE);
        }
      }
    }

    // Score display
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(95, 0);
    display.print("S:");
    display.println(score);
    display.setCursor(95, 10);
    display.print("H:");
    display.println(highScore);

    display.display();
  };

  auto canMove = [&](int dx, int dy, const byte shape[4][4]) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (shape[i][j]) {
          int x = px + j + dx;
          int y = py + i + dy;
          if (x < 0 || x >= gridW || y >= gridH) return false;
          if (y >= 0 && grid[x][y]) return false;
        }
      }
    }
    return true;
  };

  auto mergeToGrid = [&]() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (shapes[currentShape][i][j]) {
          int x = px + j;
          int y = py + i;
          if (x >= 0 && x < gridW && y >= 0 && y < gridH)
            grid[x][y] = 1;
        }
      }
    }
  };

  auto clearLines = [&]() {
    bool cleared = false;
    for (int y = gridH - 1; y >= 0; y--) {
      bool full = true;
      for (int x = 0; x < gridW; x++) {
        if (!grid[x][y]) {
          full = false;
          break;
        }
      }
      if (full) {
        cleared = true;
        for (int ty = y; ty > 0; ty--)
          for (int x = 0; x < gridW; x++)
            grid[x][ty] = grid[x][ty - 1];
        for (int x = 0; x < gridW; x++) grid[x][0] = 0;
        score += 10;
        y++;  // recheck same line
      }
    }
    if (cleared) playWinSound();
  };

  while (true) {
    // Input
    if (digitalRead(btnLeft) == LOW && canMove(-1, 0, shapes[currentShape])) {
      px--;
      playSelectSound();
      delay(100);
    }
    if (digitalRead(btnRight) == LOW && canMove(1, 0, shapes[currentShape])) {
      px++;
      playSelectSound();
      delay(100);
    }
    if (digitalRead(btnDown) == LOW && canMove(0, 1, shapes[currentShape])) {
      py++;
      playPlaceMoveSound();
      delay(50);
    }
    if (digitalRead(btnCenter) == LOW) {
      byte rotated[4][4];
      rotateShape(shapes[currentShape], rotated);
      if (canMove(0, 0, rotated)) {
        memcpy((void*)shapes[currentShape], rotated, sizeof(rotated));
        playSelectSound();
        delay(150);
      }
    }

    // Auto fall
    if (millis() - lastFall > fallSpeed) {
      if (canMove(0, 1, shapes[currentShape])) {
        py++;
      } else {
        mergeToGrid();
        playEatSound();  // Piece locked in place
        clearLines();
        currentShape = random(0, 7);
        px = 3; py = 0;
        if (!canMove(0, 0, shapes[currentShape])) {
          playGameOverSound();  // GAME OVER sound
          if (score > highScore) highScore = score;

          while (true) {
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);

            display.setCursor((128 - 90) / 2, 5);
            display.println("== GAME OVER ==");

            display.setCursor((128 - 75) / 2, 20);
            display.print("Your Score: ");
            display.println(score);

            display.setCursor((128 - 75) / 2, 30);
            display.print("High Score: ");
            display.println(highScore);
          
            display.setCursor((128 - 120) / 2, 43);
            display.println("Click Center = Menu");
            display.setCursor((128 - 120) / 2, 53);
            display.println("Other Button = Retry");

            display.display();

            // Button handling
            if (digitalRead(btnCenter) == LOW) {
              delay(300);
              drawMenu();
              return;
            }
            if (digitalRead(btnLeft) == LOW || digitalRead(btnRight) == LOW || digitalRead(btnDown) == LOW || digitalRead(btnUp) == LOW) {
              delay(300);
              tetrisGame();
              return;
            }
          }
        }
      }
      lastFall = millis();
    }

    drawGrid();
  }
}


// =================== FLAPPY BIRD ==================================================================================================

int flappyScore = 0;
int flappyHighScore = 0;

void flappyBird() {
  showButtonControlInfo("Flappy Bird Game");
restart:
  const int birdX = 20;
  float birdY = SCREEN_HEIGHT / 2;
  float velocity = 0;
  float gravity = 0.5;
  float flapStrength = -4;

  const int pipeWidth = 10;
  const int pipeGap = 25;
  const int numPipes = 3;
  const int uiMargin = 12;                // Space for score text
  const int pipeStartX = SCREEN_WIDTH + 20;

  int pipeX[numPipes];
  int pipeGapY[numPipes];

  for (int i = 0; i < numPipes; i++) {
    pipeX[i] = pipeStartX + i * (SCREEN_WIDTH / numPipes);
    pipeGapY[i] = random(uiMargin + 5, SCREEN_HEIGHT - pipeGap - 5);
  }

  flappyScore = 0;

  while (true) {
    if (digitalRead(btnCenter) == LOW) {
      velocity = flapStrength;
      // Play flap sound (short and high-pitched)
      tone(buzzerPin, 1000, 50);  // 1000 Hz for 50 ms
    }

    velocity += gravity;
    birdY += velocity;

    for (int i = 0; i < numPipes; i++) {
      pipeX[i] -= 2;

      if (pipeX[i] < -pipeWidth) {
        pipeX[i] = SCREEN_WIDTH;
        pipeGapY[i] = random(uiMargin + 5, SCREEN_HEIGHT - pipeGap - 5);
        flappyScore++;
        if (flappyScore > flappyHighScore) flappyHighScore = flappyScore;

        // Play score sound (mid-pitch)
        tone(buzzerPin, 600, 100);  // 600 Hz for 100 ms
      }

      // Collision check
      if (birdX + 3 > pipeX[i] && birdX < pipeX[i] + pipeWidth &&
          (birdY < pipeGapY[i] || birdY > pipeGapY[i] + pipeGap)) {
        // Play crash sound (low-pitch)
        tone(buzzerPin, 200, 300);  // 200 Hz for 300 ms
        goto gameOver;
      }
    }

    if (birdY < 0 || birdY > SCREEN_HEIGHT - 1) {
      // Bird hits top or bottom
      tone(buzzerPin, 200, 300);  // Crash sound
      break;
    }

    display.clearDisplay();

    // Draw game border
    display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

    // Draw UI separator
    display.drawLine(0, uiMargin, SCREEN_WIDTH, uiMargin, SSD1306_WHITE);

    // Draw bird
    display.fillRect(birdX, (int)birdY, 4, 4, SSD1306_WHITE);
    display.fillRect(birdX + 3, (int)birdY + 1, 1, 1, SSD1306_BLACK);
    display.drawPixel(birdX + 1, (int)birdY - 1, SSD1306_WHITE);

    // Draw pipes
    for (int i = 0; i < numPipes; i++) {
      // Top pipe
      display.fillRect(pipeX[i], uiMargin + 1, pipeWidth, pipeGapY[i] - uiMargin - 1, SSD1306_WHITE);
      display.fillRect(pipeX[i] - 1, pipeGapY[i] - 2, pipeWidth + 2, 2, SSD1306_WHITE);
      // Bottom pipe
      display.fillRect(pipeX[i], pipeGapY[i] + pipeGap, pipeWidth, SCREEN_HEIGHT - pipeGapY[i] - pipeGap - 1, SSD1306_WHITE);
      display.fillRect(pipeX[i] - 1, pipeGapY[i] + pipeGap, pipeWidth + 2, 2, SSD1306_WHITE);
    }

    // Display scores
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2, 2);
    display.print("S:");
    display.print(flappyScore);
    display.setCursor(SCREEN_WIDTH - 45, 2);
    display.print("H:");
    display.print(flappyHighScore);

    display.display();
    delay(30);
  }

gameOver:
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor((SCREEN_WIDTH - 90) / 2, 5);
  display.print("== GAME OVER ==");

  display.setCursor((SCREEN_WIDTH - 75) / 2, 21);
  display.print("Your Score: ");
  display.print(flappyScore);

  display.setCursor((SCREEN_WIDTH - 75) / 2, 31);
  display.print("High Score: ");
  display.print(flappyHighScore);

  display.setCursor((SCREEN_WIDTH - 120) / 2, 43);
  display.println("Click Center = Menu");

  display.setCursor((SCREEN_WIDTH - 120) / 2, 53);
  display.println("Other Button = Retry");

  display.display();

  while (true) {
    if (digitalRead(btnCenter) == LOW) {
      while (digitalRead(btnCenter) == LOW);  // wait release
      delay(300);
      drawMenu();
      return;
    }
    if (digitalRead(btnUp) == LOW || digitalRead(btnDown) == LOW || digitalRead(btnLeft) == LOW || digitalRead(btnRight) == LOW) {
      while (digitalRead(btnUp) == LOW || digitalRead(btnDown) == LOW || digitalRead(btnLeft) == LOW || digitalRead(btnRight) == LOW);
      delay(300);
      goto restart;
    }
  }
}



// =================== CAR RACE ==================================================================================================

int carScore = 0;
int carHighScore = 0;

void carRacing() {
  showJoystickControlInfo("Car Racing");
restart:
  const int scoreBarHeight = 12;
  const int gameAreaTop = scoreBarHeight + 1;
  const int gameAreaHeight = SCREEN_HEIGHT - gameAreaTop;
  const int gameAreaWidth = SCREEN_WIDTH;
  const int carWidth = 10;
  const int carHeight = 8;

  int playerY = gameAreaTop + gameAreaHeight / 2;
  int speed = 3;
  unsigned long lastSpeedIncrease = millis();

  int obsX = SCREEN_WIDTH;
  int obsY = random(gameAreaTop, SCREEN_HEIGHT - carHeight);
  int obsType = random(0, 3);

  carScore = 0;

  // Engine sound start (optional continuous low tone)
  tone(buzzerPin, 100, 50);  // light engine hum

  while (true) {
    int joyY = analogRead(joyYPin);
    if (joyY < 1000) playerY -= 2;
    if (joyY > 3000) playerY += 2;
    playerY = constrain(playerY, gameAreaTop, SCREEN_HEIGHT - carHeight);

    if (millis() - lastSpeedIncrease > 5000 && speed < 7) {
      speed++;
      lastSpeedIncrease = millis();
    }

    obsX -= speed;
    if (obsX < 0) {
      obsX = SCREEN_WIDTH;
      obsY = random(gameAreaTop, SCREEN_HEIGHT - carHeight);
      obsType = random(0, 3);
      carScore++;
      if (carScore > carHighScore) carHighScore = carScore;

      // **Sound: obstacle passed (short beep)**
      tone(buzzerPin, 800, 100);  // 800Hz tone for 100ms
    }

    if (obsX < carWidth + 3 && abs(obsY - playerY) < carHeight) {
      // **Sound: crash (longer tone)**
      tone(buzzerPin, 300, 500);  // crash tone
      delay(500);  // Wait for tone to finish before breaking
      break;
    }

    display.clearDisplay();

    // Draw score bar
    display.drawRect(0, 0, SCREEN_WIDTH, scoreBarHeight, SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2, 2);
    display.print("S:");
    display.print(carScore);
    display.setCursor(SCREEN_WIDTH / 2, 2);
    display.print("H:");
    display.print(carHighScore);

    // Draw game area border
    display.drawRect(0, gameAreaTop, gameAreaWidth, gameAreaHeight, SSD1306_WHITE);

    // Draw Car
    display.fillRect(2, playerY + 2, carWidth, 4, SSD1306_WHITE);
    display.fillRect(0, playerY, 2, 2, SSD1306_WHITE);
    display.fillRect(0, playerY + 6, 2, 2, SSD1306_WHITE);
    display.fillRect(12, playerY, 2, 2, SSD1306_WHITE);
    display.fillRect(12, playerY + 6, 2, 2, SSD1306_WHITE);

    // Draw obstacle
    if (obsType == 0) {
      display.fillTriangle(obsX + 5, obsY, obsX, obsY + 7, obsX + 10, obsY + 7, SSD1306_WHITE);
    } else if (obsType == 1) {
      display.fillRoundRect(obsX, obsY, carWidth + 4, carHeight + 2, 2, SSD1306_WHITE);
    } else {
      display.fillRect(obsX - 2, obsY + 2, carWidth + 4, 4, SSD1306_WHITE);
    }

    display.display();
    delay(30);
  }

  // Stop any tone when game is over
  noTone(buzzerPin);

  // Game Over screen
  display.clearDisplay();
  display.setTextSize(1);

  int16_t x, y;
  uint16_t w, h;

  display.getTextBounds("== GAME OVER ==", 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 5);
  display.println("== GAME OVER ==");

  String scoreStr = "Your Score: " + String(carScore);
  display.getTextBounds(scoreStr, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 20);
  display.println(scoreStr);

  String highStr = "High Score: " + String(carHighScore);
  display.getTextBounds(highStr, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 30);
  display.println(highStr);

  String msg = "Click Center = Menu";
  display.getTextBounds(msg, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - 120) / 2, 41);
  display.println(msg);

  String msg2 = "Other Button = Retry";
  display.getTextBounds(msg2, 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - 120) / 2, 52);
  display.println(msg2);

  display.display();

  while (true) {
    if (digitalRead(btnCenter) == LOW) {
      while (digitalRead(btnCenter) == LOW);
      delay(300);
      drawMenu();
      return;
    }
    if (digitalRead(btnUp) == LOW || digitalRead(btnDown) == LOW || digitalRead(btnLeft) == LOW || digitalRead(btnRight) == LOW) {
      while (digitalRead(btnUp) == LOW || digitalRead(btnDown) == LOW || digitalRead(btnLeft) == LOW || digitalRead(btnRight) == LOW);
      delay(300);
      goto restart;
    }
  }
}


// =================== ASTEROID SHOOTING ==================================================================================================


int asteroidScore = 0;
int asteroidHighScore = 0;

#define MAX_BULLETS 5
#define MAX_ASTEROIDS 3

struct Bullet {
  int x, y;
  bool active;
};

struct Asteroid {
  int x, y;
  bool active;
};

void asteroidShooting() {
  showButtonControlInfo("Asteroid Shooting");
  int shipX = SCREEN_WIDTH / 2;
  int shipY = SCREEN_HEIGHT - 10;

  Bullet bullets[MAX_BULLETS];
  Asteroid asteroids[MAX_ASTEROIDS];

  // Initialize
  for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = false;
  for (int i = 0; i < MAX_ASTEROIDS; i++) {
    asteroids[i].x = random(0, SCREEN_WIDTH - 24);
    asteroids[i].y = random(-20, 0);
    asteroids[i].active = true;
  }

  asteroidScore = 0;

  while (true) {
    // Move ship
    if (digitalRead(btnLeft) == LOW)  shipX -= 2;
    if (digitalRead(btnRight) == LOW) shipX += 2;
    shipX = constrain(shipX, 4, SCREEN_WIDTH - 24);

    // Fire bullets
    if (digitalRead(btnCenter) == LOW) {
      for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
          bullets[i] = {shipX + 3, shipY - 2, true};
          tone(buzzerPin, 1000, 50);  // Bullet fire sound
          break;
        }
      }
    }

    // Update bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (bullets[i].active) {
        bullets[i].y -= 3;
        if (bullets[i].y < 0) bullets[i].active = false;
      }
    }

    // Update asteroids
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
      if (asteroids[i].active) {
        asteroids[i].y += 2;
        if (asteroids[i].y > SCREEN_HEIGHT) {
          asteroids[i].y = 0;
          asteroids[i].x = random(0, SCREEN_WIDTH - 24);
        }

        // Collision with ship
        if (asteroids[i].y > shipY - 3 &&
            abs(asteroids[i].x - shipX) < 6) {
          tone(buzzerPin, 400, 500);  // Game over sound
          goto gameOver;
        }

        // Collision with bullets
        for (int j = 0; j < MAX_BULLETS; j++) {
          if (bullets[j].active &&
              abs(bullets[j].x - asteroids[i].x) < 5 &&
              abs(bullets[j].y - asteroids[i].y) < 5) {
            bullets[j].active = false;
            asteroids[i].y = 0;
            asteroids[i].x = random(0, SCREEN_WIDTH - 24);
            asteroidScore++;
            if (asteroidScore > asteroidHighScore) asteroidHighScore = asteroidScore;
            tone(buzzerPin, 1500, 100);  // Asteroid hit sound
          }
        }
      }
    }

    // Draw
    display.clearDisplay();
    display.drawRect(0, 0, SCREEN_WIDTH - 20, SCREEN_HEIGHT, SSD1306_WHITE);

    // Spaceship
    display.fillTriangle(shipX + 2, shipY, shipX, shipY + 6, shipX + 4, shipY + 6, SSD1306_WHITE);
    display.fillRect(shipX - 1, shipY + 3, 1, 2, SSD1306_WHITE);
    display.fillRect(shipX + 5, shipY + 3, 1, 2, SSD1306_WHITE);
    display.drawPixel(shipX + 2, shipY + 6, SSD1306_WHITE);

    // Bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (bullets[i].active) {
        display.drawPixel(bullets[i].x, bullets[i].y, SSD1306_WHITE);
      }
    }

    // Asteroids
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
      if (asteroids[i].active) {
        display.fillCircle(asteroids[i].x, asteroids[i].y, 3, SSD1306_WHITE);
        display.drawPixel(asteroids[i].x - 1, asteroids[i].y - 1, SSD1306_BLACK);
      }
    }

    // Refill Icon
    bool empty = true;
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (bullets[i].active) {
        empty = false;
        break;
      }
    }
    if (empty) {
      display.drawLine(SCREEN_WIDTH - 10, SCREEN_HEIGHT / 2 - 2, SCREEN_WIDTH - 10, SCREEN_HEIGHT / 2 + 2, SSD1306_WHITE);
      display.drawLine(SCREEN_WIDTH - 12, SCREEN_HEIGHT / 2, SCREEN_WIDTH - 8, SCREEN_HEIGHT / 2, SSD1306_WHITE);
    }

    // Score Panel
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(SCREEN_WIDTH - 18, 2);
    display.print("S:");
    display.print(asteroidScore);
    display.setCursor(SCREEN_WIDTH - 18, 10);
    display.print("H:");
    display.print(asteroidHighScore);

    display.display();
    delay(30);
  }

gameOver:
  noTone(buzzerPin);  // Stop any lingering sound
  display.clearDisplay();
  display.setTextSize(1);

  // Centered Text
  display.setCursor((SCREEN_WIDTH - 90) / 2, 5);
  display.println("== GAME OVER ==");

  display.setCursor((SCREEN_WIDTH - 80) / 2, 20);
  display.print("Your Score: ");
  display.print(asteroidScore);

  display.setCursor((SCREEN_WIDTH - 80) / 2, 30);
  display.print("High Score: ");
  display.print(asteroidHighScore);

  display.setCursor((SCREEN_WIDTH - 120) / 2, 44);
  display.println("Click Center = Menu");
  display.display();

  display.setCursor((SCREEN_WIDTH - 120) / 2, 54);
  display.println("Other Button = Retry");
  display.display();

  // Wait for any button
  while (true) {
    if (digitalRead(btnCenter) == LOW) {
      while (digitalRead(btnCenter) == LOW);  // wait release
      delay(300);
      drawMenu();
      return;
    }
    if (digitalRead(btnUp) == LOW || digitalRead(btnDown) == LOW || digitalRead(btnLeft) == LOW || digitalRead(btnRight) == LOW) {
      while (digitalRead(btnUp) == LOW || digitalRead(btnDown) == LOW || digitalRead(btnLeft) == LOW || digitalRead(btnRight) == LOW);
      delay(300);
      asteroidShooting();
    }
  }
}


// =================== PONG GAME ==================================================================================================


void pongGame() {
  showJoystickControlInfo("Pong Game");  // Show joystick control screen

  static int highScore = 0;

  int playerY = SCREEN_HEIGHT / 2 - 10;
  int aiY = SCREEN_HEIGHT / 2 - 10;
  float ballX = SCREEN_WIDTH / 2, ballY = SCREEN_HEIGHT / 2;
  float ballDX = -2, ballDY = 1;
  float speedFactor = 1.0;
  int playerScore = 0;
  const int boxWidth = SCREEN_WIDTH - 25;  // Leave space for score display

  // === Game Loop ===
  while (true) {
    // Joystick input for player
    int joyY = analogRead(joyYPin);
    if (joyY < 1000) playerY -= 2;
    if (joyY > 3000) playerY += 2;
    playerY = constrain(playerY, 0, SCREEN_HEIGHT - 20);

    // AI paddle movement
    if (aiY + 10 < ballY) aiY++;
    if (aiY + 10 > ballY) aiY--;
    aiY = constrain(aiY, 0, SCREEN_HEIGHT - 20);

    // Ball movement with capped speed
    float maxSpeed = 4.0;
    speedFactor = min(speedFactor, maxSpeed);
    ballX += ballDX * speedFactor;
    ballY += ballDY * speedFactor;

    // Bounce off top/bottom
    if (ballY <= 0 || ballY >= SCREEN_HEIGHT - 1) {
      ballDY = -ballDY;
      tone(buzzerPin, 1000, 50);  // Play bounce sound
    }

    // Collision with player paddle (with margin)
    if (ballX <= 5 && ballY >= playerY - 2 && ballY <= playerY + 22) {
      ballDX = -ballDX;
      playerScore++;
      if (playerScore > highScore) highScore = playerScore;
      speedFactor += 0.1;
      tone(buzzerPin, 1500, 100);  // Paddle hit sound
    }

    // Collision with AI paddle (with margin)
    if (ballX >= boxWidth - 5 && ballY >= aiY - 2 && ballY <= aiY + 22) {
      ballDX = -ballDX;
      speedFactor += 0.05;
      tone(buzzerPin, 1200, 80);  // AI paddle hit sound
    }

    // Game over if ball goes beyond screen
    if (ballX < 0 || ballX > SCREEN_WIDTH) {
      tone(buzzerPin, 500, 300);  // Game over sound
      break;
    }

    // === Drawing ===
    display.clearDisplay();

    // Game box boundary
    display.drawRect(0, 0, boxWidth, SCREEN_HEIGHT, SSD1306_WHITE);

    // Paddles and ball
    display.fillRect(0, playerY, 3, 20, SSD1306_WHITE);                // Player paddle
    display.fillRect(boxWidth - 3, aiY, 3, 20, SSD1306_WHITE);         // AI paddle
    display.fillCircle(ballX, ballY, 2, SSD1306_WHITE);                // Ball

    // Score and high score
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(boxWidth + 2, 10);  display.println("S:");
    display.setCursor(boxWidth + 14, 10); display.println(playerScore);
    display.setCursor(boxWidth + 2, 25);  display.println("H:");
    display.setCursor(boxWidth + 14, 25); display.println(highScore);

    display.display();
    delay(30);
  }

  // === Game Over Screen ===
  display.clearDisplay();
  display.setTextSize(1);
  String gameOver = "== Game Over ==";
  String scoreStr = "Your Score: " + String(playerScore);
  String highStr = "High Score: " + String(highScore);

  int centerX = SCREEN_WIDTH / 2;
  display.setCursor(centerX - gameOver.length() * 3, 10);
  display.println(gameOver);
  display.setCursor(centerX - scoreStr.length() * 3, 25);
  display.println(scoreStr);
  display.setCursor(centerX - highStr.length() * 3, 40);
  display.println(highStr);
  display.display();

  tone(buzzerPin, 400, 500);  // Long game over tone
  delay(2000);

  drawMenu();  // Return to menu
}
