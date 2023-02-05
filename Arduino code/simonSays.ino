/*
 * SIMON SAYS
 * * * * * * * * * * * * * * * *
 * Christoffer Karlsson 2019
 * 
 * "Simon" game clone
 * See README for more info
 */

// -- Include -- //
#include <pitches.h>
#include <EEPROM.h>

// -- Constants -- //
//Pins                    R  G  Y  B
const int buttonPin[4] = {2, 4, 7, 8},
             ledPin[4] = {5, 6, 9, 10},
             soundPin = 12,
  //Sounds
  sound[4] = {NOTE_E4, NOTE_A4, NOTE_CS5, NOTE_E5}, //Sound of each button when pressed
  failSound = NOTE_C2, //Played upon failing
  successSound[4] = {NOTE_A6, NOTE_CS7, NOTE_E7, NOTE_A7}, //Played as progress indicators throughout the game
  fanfareSound[4] = {NOTE_A5, NOTE_CS6, NOTE_E6, NOTE_A6}, //Played when setting a new highscore
  //Max number of fails in easy mode
  easyMaxFails = 3, 
  //Max number in sequence
  NMAX = 99, 
  //Delay times
  waitDelay = 1500, updateDelay = 50,
  maxInactive = 60000/updateDelay, //Show color dance etc after 60 sec
  //Game modes
  NORMAL = 0, REVERSE = 1, MULTI = 2, //, EASY = 3;
  //Address to save data in EEPROM
  addressEEPROM = 0; 
//Factor to decrease time in the game sequence - increases the difficulty throughout the game
const float speedFactor = 0.93f;

// -- Variables -- //
int nSeq, //Current length of sequence
  iSeq, //Current position in sequence
  sequence[NMAX], //Current sequence
  showDelay, //Time for showing colow
  pauseDelay, //Time between colors
  nFails, //Current number of fails
  nMaxFails, //Allowable number of fails
  gameMode, //Current game mode
  inactive, //Inactive counter
  highscore; //Current highscore

bool soundOn, //If sound should be played
  easyMode, //If currently in easy mode
  freeplay, //If currently in free play mode
  buttonPressed[4] = {false, false, false, false}; //Currently pressed buttons

// -- Setup -- //
void setup() {
  //Serial.begin(9600);
  
  for (int i=0; i<4; i++) { //Enable LED & button pins
    pinMode(ledPin[i], OUTPUT);
    pinMode(buttonPin[i], INPUT_PULLUP);
  }

  //Set game mode by checking if any button is pressed during startup
  soundOn = digitalRead(buttonPin[2]) == HIGH; //Sound on/off
  if (digitalRead(buttonPin[0]) == LOW) //Reverse mode
    gameMode = REVERSE;
  else if (digitalRead(buttonPin[3]) == LOW) //Multiplayer mode
    gameMode = MULTI;
  else //Normal mode
    gameMode = NORMAL;
  easyMode = digitalRead(buttonPin[1]) == LOW; //Easy mode

  checkResetHighscore(); //Check if highscore should be reset
  readHighscoreFromEEPROM(); //Load highscore from EEPROM
  
  if (soundOn) pinMode(soundPin, OUTPUT); //Enable sound pin

  //Serial.print("Game mode: "); Serial.println(gameMode);
  //Serial.print("Sound on: "); Serial.println(soundOn);
  //Serial.print("Easy mode: "); Serial.println(easyMode);
    
  randomSeed(analogRead(0)); //Init random numbers (don't connect A0!)
  
  inactive = 0; //Initialize inactive counter
  
  newGame(); //Start a new game immediately

  //If multiplayer mode, we delay input by showing a simple color animation first
  //so that the button can be released before starting inputting the sequence
  if (gameMode == MULTI) showSimpleColorSpin();
}

// -- Loop -- //
void loop() {
  checkButtons(); //Check button presses and act accordingly
  delay(updateDelay);
}

//Show a simple animation
void showSimpleColorSpin() {
  for (int i=0; i<4; i++) {
    digitalWrite(ledPin[i], HIGH);
    delay(500);
    digitalWrite(ledPin[i], LOW);
  }
}

//Check if any button is pressed or released, and take appropriate action
void checkButtons() {
  //First check if any previously pressed button is still held down
  for (int i=0; i<4; i++)
    if (buttonPressed[i] && digitalRead(buttonPin[i]) == LOW)
      return; //If so, do nothing

  //Check for newly pressed buttons
  for (int i=0; i<4; i++)
    //Button released
    if (buttonPressed[i]) { // && digitalRead(buttonPin[i]) == HIGH) {
      //We don't have to check for HIGH state since it would otherwise have
      //exited in the above loop, therefore it must be released now
      buttonPressed[i] = false;
      showColor(i, false); //Turn off LED & sound
      //Serial.print("Release "); Serial.println(i);
      if (!freeplay) tryNext(i); //Compare the released button to the sequence...
    }
    //Button pressed
    else if (!buttonPressed[i] && digitalRead(buttonPin[i]) == LOW) {
      buttonPressed[i] = true;
      showColor(i, true); //Show that color
      inactive = 0; //Reset inactivity counter
      //Serial.print("Press "); Serial.println(i);
    } 

  //If nothing has happened for a while
  if (++inactive > maxInactive) {
    inactive = 0;
    performColorDance(); //Start "color dance" animation show
  }
}

//Show a color or turn off a color (LED and sound)
void showColor(int color, bool on) {
  digitalWrite(ledPin[color], on);
  if (on) playSound(sound[color]);
  else noTone(soundPin);
}

//Play fail sound (low A)
void playfailSound() {
  playSound(failSound, 100);
  delay(120);
  playSound(failSound, 100);
  delay(120);
}

//Play success sound (high A major chord)
void playSuccessSound() {
  delay(200); //Short delay makes it less weird
  
  int j = 0, duration = 100;

  //Play arpeggio chord depending on current length of sequence
  for (int i=0; i<nSeq; i+=8) {
    if (i == nSeq-8) duration = 300; //Long last note
    playSound(successSound[j], duration);
    delay(120);
    if (++j == 4) j = 0; //Loop through
  }
}

//Play a sound with frequency indefinitely
//Does nothing if in silent mode
void playSound(int frequency) {
  if (soundOn) tone(soundPin, frequency);
}

//Play a sound with freqency for duration
//Does nothing if in silent mode
// >> Call delay separately! <<
void playSound(int frequency, int duration) {
  if (soundOn) tone(soundPin, frequency, duration);
}

//Try the next color in the sequence
void tryNext(int color) {
  //If in multiplayer mode and the end is reached: add the new color to the sequence
  if (gameMode == MULTI && iSeq == nSeq) {
    delay(waitDelay);
    addToSeq(color);
  }
  else if (color == sequence[iSeq]) { // -- Color is correct -- //
    //Serial.println("Correct!");
    //Iterate sequence counter
    if (gameMode == REVERSE) iSeq--;
    else iSeq++;
    //If this was the end of the sequence
    if ((gameMode != MULTI && iSeq == nSeq)
      || (gameMode == REVERSE && iSeq == -1)) {
        //Play success sound every eighth color
        if (isMultipleOfEight(nSeq))
          playSuccessSound();
        //Serial.println("Next step...");
        delay(waitDelay);
        addToSeq(); //Add another color and show sequence again
    }
  } else { // -- Color is wrong -- //
    //Serial.println("Wrong!");
    playfailSound();
    delay(waitDelay);
    //Start new game if failed maximum amount of times
    if (gameMode != MULTI && ++nFails == nMaxFails) {
      updateHighscore();
      newGame();
    }
    else { //If in easy mode we get another chance
      //Serial.print(nFails); Serial.println(" fails");
      iSeq = (gameMode == REVERSE) ? nSeq-1 : 0;
      showSequence();
    }
  }
}

//Returns whether number is a multiple of eight
//by checking the three least significant bits
bool isMultipleOfEight(int number) {
//  return (bitRead(number, 0) == 0 &&
//    bitRead(number, 1) == 0 &&
//    bitRead(number, 2) == 0);
  return (number & 0b00000111) == 0;
}

//Turn off all LEDs and the buzzer
void resetLedsAndSound() {
  for (int i=0; i<4; i++)
    digitalWrite(ledPin[i], LOW);
  noTone(soundPin);
}

//Start a new game
void newGame() {
  //Serial.println("New game!");
  showDelay = 800; //Init values for show speed
  pauseDelay = 200;
  nSeq = 0;
  nFails = 0;
  //Allow more fails in easy mode, otherwise fail immediately
  nMaxFails = easyMode ? easyMaxFails : 1;
  if (gameMode != MULTI) addToSeq(); //In multiplayer mode: add first in sequence
}

//Add a new random color to the sequence and show it
void addToSeq() {
  addToSeq(random(4));
}

//Add a new specified color to the sequence and show it
void addToSeq(int color) {
  //Start at beginning, or end if reverse game
  iSeq = (gameMode == REVERSE) ? nSeq : 0;
  //Don't add new color if NMAX reached (shouldn't happen...)
  if (nSeq < NMAX) {
    sequence[nSeq++] = color;
    showDelay *= speedFactor; //Speed up
    pauseDelay *= speedFactor;
  }
  //else Serial.println("Reached maximum number in sequence!!");
  
  //Serial.print("Sequence length: ");
  //Serial.println(nSeq);
  if (!freeplay) showSequence(); //Show the new sequence
}

//Show the correct sequence
void showSequence() {
  //Serial.print("Showing sequence... ");
  resetLedsAndSound();
  for (int i=0; i<nSeq; i++) {
    delay(pauseDelay);
    showColor(sequence[i], true);
    delay(showDelay);
    showColor(sequence[i], false);
  }
  //Serial.println("Done!");
}

//Update the highscore if a new record is set
void updateHighscore() {
  /*Serial.print("nSeq=");
  Serial.println(nSeq);
  Serial.print("highscore=");
  Serial.println(highscore);*/
  if (nSeq > highscore) {
    //Serial.println("Updating!");
    highscore = nSeq;
    writeHighscoreToEEPROM();
    playFanfare(); //If new highscore: Play a fanfare!
  }
}

//Write the highscore to EEPROM
void writeHighscoreToEEPROM() {
  //Separate highscores stored for easy and normal mode
  EEPROM.update(addressEEPROM + easyMode ? 1 : 0, highscore);
}

//Read the highscore from EEPROM
void readHighscoreFromEEPROM() {
  highscore = EEPROM.read(addressEEPROM + easyMode ? 1 : 0);
  //At first startup: initialize highscore to 1
  if (highscore > NMAX) highscore = 1;
  //Serial.print("highscore=");
  //Serial.println(highscore);
}

//Play a fanfare when setting a new highscore (High A major chord)
void playFanfare() {
  playSound(fanfareSound[0], 280);
  delay(300);
  playSound(fanfareSound[0], 80);
  delay(100);
  for (int i=0; i<4; i++) { //Also light up the LEDs in sequence
    digitalWrite(ledPin[i], HIGH);
    playSound(fanfareSound[i], 380);
    delay(400);
  }
  for (int i=0; i<4; i++) digitalWrite(ledPin[i], LOW);
  delay(waitDelay);
}

//Check if the highscore should be reset or if free play mode should be started
void checkResetHighscore() {
  //First check if all buttons are pressed simultaneously
  for (int i=0; i<4; i++)
    if (digitalRead(buttonPin[i]) == HIGH)
      return;

  //Normal mode
  gameMode = NORMAL;
  easyMode = false;
  soundOn = true;

  //Turn on all LEDs
  for (int i=0; i<4; i++)
    digitalWrite(ledPin[i], HIGH);

  //Play sound and turn off LEDs in sequence
  for (int i=3; i>=0; i--) {
    tone(soundPin, NOTE_A4, 100);
    delay(1000);
    digitalWrite(ledPin[i], LOW);
  }

  //If some button has been released, start free play mode
  for (int i=0; i<4; i++)
    if (digitalRead(buttonPin[i]) == HIGH) {
      freeplay = true;
      return;
    }

  //If all buttons are still pressed, reset the high score
  freeplay = false;
  resetEEPROMHighscore();

  //Acknowledgement sound
  tone(soundPin, NOTE_A6, 300);
  delay(200);
}

//Reset highscore (both normal and easy mode)
void resetEEPROMHighscore() {
  EEPROM.update(addressEEPROM, 1);
  EEPROM.update(addressEEPROM+1, 1);
}
