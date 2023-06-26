#include "FastLED.h"
#include "lights.h"
#include "tests.h"
#include "project.h"

CRGB leds[NUM_LEDS+1];

int potPin = A0;       // Analog input pin for potentiometer
int buttonPin = 7;     // Digital input pin for button

int currentPlayer = 1;       // Player 1: X, Player 2: O
int currentSpot = 0;         // Current spot selected by the potentiometer
int currentSpotI = 0;
int availableSpotNumber = 0;

bool bang = false;

int potVal = analogRead(potPin);

// Tic Tac Toe game board
int board[3][3] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

int inside[] = {8,7,6,11,12,13,18,17,16};
int outside[] = {0,1,2,3,4,5,14,15,24,23,22,21,20,19,10,9};
int insideOpen[] = {0,0,0,0,0,0,0,0,0};

void setup()
{
  delay( 3000 ); // power-up safety delay
  Serial.begin(9600); //enables serial output for debugging
  
  //WS2811 on pin 7, color sequence, lights data structure, and number of lights for controller
  FastLED.addLeds<LED_TYPE, LED_PIN, RGB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  FastLED.setBrightness(BRIGHTNESS);  //sets the brightness for the strand of LEDs.  Default is 64, max is 255.
  test_leds();
  leds_init(); 

  pinMode(buttonPin, INPUT_PULLUP);
  resetGame();

}

// Function to read the potentiometer input and select the spot


// Function to reset the game board
void resetGame() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = 0;
    }
  }
  currentSpot = 0;
  currentPlayer = 1;
}
  // Check game over condition (win or draw)
  // ... Add your game over

// Function to check if the game has been won
bool checkWin(int player) {
  // Check rows
  for (int i = 0; i < 3; i++) {
    if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
      Serial.println("Check1");
      return true;
    }
  }

  // Check columns
  for (int i = 0; i < 3; i++) {
    if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
      Serial.println("Check2");
      return true;
    }
  }

  // Check diagonals
  if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
    Serial.println("Check3");
    return true;
  }
  if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
    Serial.println("Check4");
    return true;
  }

  return false;
}

// Function to check if the game ended in a draw
bool checkDraw() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

void loop() {
  readPotentiometer();
  while(digitalRead(buttonPin) == 0){
    bang = true;
  }
  for(int i = 0; i<16; i++){
    if(currentPlayer == 1){
      leds[outside[i]] = CRGB::Red;
    } else{
      leds[outside[i]] = CRGB::Blue;
    }
  }
  buttonStuff();
  
  // Check game over condition (win or draw)
  

  //read potentiometer until change
  //once button clicked and change occurs - change color and drop marker
  //once that is done check wins and draws
  //once that is deemed false change player and color etc.

}

void readPotentiometer() {  
  potVal = analogRead(potPin);
  updateAvailableSpots();
  ledPotentiometer();
}

void updateAvailableSpots() {
  int count = 0;
  int ch = 0;
  for(int a = 0; a<3; a++){
    for(int b = 0; b<3; b++){
    if(board[a][b] == 0){
      insideOpen[count] = inside[(a*3)+b];
      count++;
      ch++;
    } 
  }
  }
  availableSpotNumber = ch;
}

void ledPotentiometer(){
  int magic = 1022/availableSpotNumber;
  for(int i = 0; i<availableSpotNumber; i++){
    if((magic*i <= potVal) && (potVal <= magic*(i+1))){
      if(availableSpotNumber == 1){
      } else if(i == 0 && availableSpotNumber>=2){
        leds[insideOpen[availableSpotNumber-1]] = CRGB::Black;
        leds[insideOpen[i+1]] = CRGB::Black;
        FastLED.delay(1);
      } else if(i == availableSpotNumber-1  && availableSpotNumber>=2){
        leds[insideOpen[0]] = CRGB::Black;
        leds[insideOpen[i-1]] = CRGB::Black;
        FastLED.delay(1);
      } else{
        leds[insideOpen[i-1]] = CRGB::Black;
        leds[insideOpen[i+1]] = CRGB::Black;
        FastLED.delay(1);
      }
      leds[insideOpen[i]] = CRGB::Green;
      FastLED.delay(1);
      currentSpot = insideOpen[i];
      currentSpotI = i;
    }
  }
}

void findCorresponder(int num){
  for(int a = 0; a<3; a++){
    for(int b = 0; b<3; b++){
      if(num == inside[(a*3)+b]){
        board[a][b] = currentPlayer;
      }
    }
  }
}
void buttonStuff(){
  if(bang){
    bang = false;
    if(currentPlayer == 1){
      leds[currentSpot] = CRGB::Red;
      FastLED.delay(1);
    } else{
      leds[currentSpot] = CRGB::Blue;
      FastLED.delay(1);
    }
    for(int a = 0; a<3; a++){
      for(int b = 0; b<3; b++){
        Serial.print(board[a][b]);
      }
      Serial.println();
    }
    findCorresponder(currentSpot);
    for(int a = 0; a<3; a++){
      for(int b = 0; b<3; b++){
        Serial.print(board[a][b]);
      }
      Serial.println();
    }
    if (checkWin(1) || checkWin(2) || checkDraw()) {
      if(checkWin(1)){
        displayWinnerAnimationRed();
      } else if (checkWin(2)){
        displayWinnerAnimationBlue();
      } else{
        displayTieAnimation();
      }
      resetGame();
      test_leds();
      for(int i = 0; i<25; i++){
        leds[i] = CRGB::Black;
      }
      FastLED.delay(1);
    } else{
      if(currentPlayer == 1){
        for(int i = 0; i<16; i++){
          leds[outside[i]] = CRGB::Blue;
          FastLED.delay(100);
        }
        currentPlayer = 2;
      } else{
        for(int i = 0; i<16; i++){
          leds[outside[i]] = CRGB::Red;
          FastLED.delay(100);
        }
        currentPlayer = 1;
      }
    }
}
}

void displayWinnerAnimationRed(){
  for(int i = 0;i<25;i++){
    if(i == 0){
        leds[24] = CRGB::Black;
        leds[i+1] = CRGB::Black;
        FastLED.delay(1);
      } else if(i == 24){
        leds[0] = CRGB::Black;
        leds[insideOpen[i-1]] = CRGB::Black;
        FastLED.delay(1);
      } else{
        leds[i-1] = CRGB::Black;
        leds[i+1] = CRGB::Black;
        FastLED.delay(1);
      }
    leds[i] = CRGB::Red;
    FastLED.delay(150);
  }
  for(int i = 0; i<5; i++){
    for(int i = 0;i<25;i++){
    leds[i] = CRGB::Red;
    }
    FastLED.delay(500);
    for(int i = 0;i<25;i++){
    leds[i] = CRGB::Black;
    }
    FastLED.delay(500);
  }
}

void displayWinnerAnimationBlue(){
  for(int i = 0;i<25;i++){
    if(i == 0){
        leds[24] = CRGB::Black;
        leds[i+1] = CRGB::Black;
        FastLED.delay(1);
      } else if(i == 24){
        leds[0] = CRGB::Black;
        leds[insideOpen[i-1]] = CRGB::Black;
        FastLED.delay(1);
      } else{
        leds[i-1] = CRGB::Black;
        leds[i+1] = CRGB::Black;
        FastLED.delay(1);
      }
    leds[i] = CRGB::Blue;
    FastLED.delay(150);
  }
  for(int i = 0; i<5; i++){
    for(int i = 0;i<25;i++){
    leds[i] = CRGB::Blue;
    }
    FastLED.delay(500);
    for(int i = 0;i<25;i++){
    leds[i] = CRGB::Black;
    }
    FastLED.delay(500);
  }
}

void displayTieAnimation(){
  int cnt = 1;
  for(int i = 0;i<25;i++){
    if(i == 0){
        leds[24] = CRGB::Black;
        leds[i+1] = CRGB::Black;
        FastLED.delay(1);
      } else if(i == 24){
        leds[0] = CRGB::Black;
        leds[insideOpen[i-1]] = CRGB::Black;
        FastLED.delay(1);
      } else{
        leds[i-1] = CRGB::Black;
        leds[i+1] = CRGB::Black;
        FastLED.delay(1);
      }
    if(cnt == 1){
      leds[i] = CRGB::Red;
      FastLED.delay(150);
      cnt = 0;
    } else{
      leds[i] = CRGB::Blue;
      FastLED.delay(150);
      cnt = 1;
    }
    
  }
  for(int i = 0; i<2; i++){
    for(int i = 0;i<25;i++){
    leds[i] = CRGB::Red;
    }
    FastLED.delay(500);
    for(int i = 0;i<25;i++){
    leds[i] = CRGB::Black;
    }
    FastLED.delay(500);
    for(int i = 0;i<25;i++){
    leds[i] = CRGB::Blue;
    }
    FastLED.delay(500);
    for(int i = 0;i<25;i++){
    leds[i] = CRGB::Black;
    }
    FastLED.delay(500);
  }
}
