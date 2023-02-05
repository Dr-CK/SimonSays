/*
 * SIMON SAYS - COLOR DANCE
 * Christoffer Karlsson 2019
 * 
 * Some functions to do entertaining "color dance" animations
 * on the Simon Says display, kinda like a screen saver.
 * Press any button between animations to return to the game.
 * Leave the animations on for a while for another surprise...
 */

//Perform color dance animations
void performColorDance() {
  //Serial.println("Starting color dance...");

  //Do 15 separate animations
  for (int i=0; i<15; i++){
    switch (random(5)) { //Randomize each time
      case 0: dance0(); break;
      case 1: dance1(); break;
      case 2: dance2(); break;
      case 3: dance3(); break;
      case 4: dance4(); break;
    }
    delay(1000);

    //If any button is pressed, return to the game
    for (int i=0; i<4; i++)
      if (digitalRead(buttonPin[i]) == LOW)
        return;
  }

  //When animations are finished, play the Super Mario theme :)
  if (soundOn) playSuperMarioTheme();

  while (true) {} //Then do nothing
}

//Dance 0: Simple spin
void dance0() {
  //Serial.println("Simple spin");
  for (int j=0; j<16; j++)
    for (int i=0; i<4; i++) {
      digitalWrite(ledPin[i], HIGH);
      delay(100);
      digitalWrite(ledPin[i], LOW);
    }
}

//Dance 1: Fade all colors in and out
void dance1() {
  //Serial.println("All fade");
  int change = 5;
  for (int k=0; k<8; k++) {
    for (int j=0; j<256; j+=change) {
      for (int i=0; i<4; i++) analogWrite(ledPin[i], j);
      delay(20);
    }
    for (int j=255; j>=0; j-=change) {
      for (int i=0; i<4; i++) analogWrite(ledPin[i], j);
      delay(20);
    }
  }
  for (int i=0; i<4; i++) digitalWrite(ledPin[i], LOW);
}

//Dance 2: Spin around fading the colors
void dance2() {
  //Serial.println("Fade spin");
  int change = 8;
  for (int k=0; k<8; k++) {
    for (int i=0; i<4; i++) {
      for (int j=0; j<256; j+=change) {
        analogWrite(ledPin[i], 255-j);
        analogWrite(ledPin[(i+1)%4], j);
        delay(20);
      }
      digitalWrite(ledPin[i], LOW);
    }
  }
  for (int i=0; i<4; i++) digitalWrite(ledPin[i], LOW);
}

//Dance 3: Light up colors pairwise
void dance3() {
  //Serial.println("Cross");
  for (int j=0; j<16; j++)
    for (int i=0; i<2; i++) {
      digitalWrite(ledPin[i], HIGH);
      digitalWrite(ledPin[i+2], HIGH);
      delay(200);
      digitalWrite(ledPin[i], LOW);
      digitalWrite(ledPin[i+2], LOW);
    }
}

//Dance 4: Speeding up random colors
void dance4() {
  //Serial.println("R2D2");
  int color; // = -1;
  for (float d = 500.0f; d > 15.0f; d *= 0.94f) {
    color = getNewRandomColor(color);
    digitalWrite(ledPin[color], HIGH);
    delay(int(d));
    digitalWrite(ledPin[color], LOW);
  }
}

//Dance 5: Another fading color spin
/*void dance5() {
  int change = 8;
  for (int k=0; k<8; k++) {
      for (int j=0; j<256; j+=change) {
        analogWrite(ledPin[0], j);
        delay(10);
      }
      for (int j=0; j<256; j+=change) {
        analogWrite(ledPin[0], 255-j);
        analogWrite(ledPin[1], j);
        analogWrite(ledPin[3], j);
        delay(10);
      }
      for (int j=0; j<256; j+=change) {
        analogWrite(ledPin[1], 255-j);
        analogWrite(ledPin[3], 255-j);
        analogWrite(ledPin[2], j);
        delay(10);
      }
      for (int j=0; j<256; j+=change) {
        analogWrite(ledPin[2], 255-j);
        delay(10);
      }
    }
}*/

//Return a random color which is not the same as the old color
int getNewRandomColor(int oldColor) {
  int color;
  do color = random(4);
  while (color == oldColor);
  return color;
}
