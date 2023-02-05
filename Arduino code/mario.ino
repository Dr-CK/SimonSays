/*
 * SIMON SAYS - SUPER MARIO THEME SONG
 * Christoffer Karlsson 2019
 * 
 * Plays the Super Mario theme song on the Simon Says game
 */

const int marioNoteLength = 50,
  marioN = 140, marioNReprises = 9,
  marioNotes[marioN] = {NOTE_E5, NOTE_E5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_C3, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_E5, NOTE_GS4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, NOTE_C3, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_E5, NOTE_C6, NOTE_C6, NOTE_C6, NOTE_G3, NOTE_C3, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_E5, NOTE_GS4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, NOTE_C3, NOTE_DS5, NOTE_D5, NOTE_C5, NOTE_G3, NOTE_G3, NOTE_C3, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_G3, NOTE_C3, NOTE_G2, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_E5, NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_F5, NOTE_F5, NOTE_A4, NOTE_B4, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_E5, NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_F5, NOTE_F5, NOTE_A4, NOTE_B4, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_E4, NOTE_E4, NOTE_C4, NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_G4, NOTE_E4, NOTE_G4},
  marioLengths[marioN] = {150,   300,     300,     150,     300,     600,     600,     450,     450,     450,     300,     300,     150,      300,     200,     200,     200,     300,     150,     300,     300,     150,     150,     450,     300,     150,     150,      150,     300,      300,     150,      150,     300,     150,     150,     150,     300,     150,     150,      150,     300,      300,     300,     150,     300,     300,     300,     150,     150,      150,     300,      300,     150,      150,     300,     150,     150,     150,     300,     450,      450,     450,     150,     300,     300,     150,     300,     300,     150,     300,     150,     300,     150,     600,     150,     300,     300,     150,     150,     150,     450,     450,     300,     150,     300,     300,     150,     300,     150,     300,     150,     600,     150,     300,     450,     300,      150,     300,     150,     600,     200,     200,     200,     200,     200,     200,     150,     300,     150,     600,     150,     300,     450,     300,      150,     300,     150,     600,     150,     300,     150,     200,     200,     200,     150,     300,     150,     600,     500,     500,     500,     200,     200,     200,     220,      220,      220,      240,     240,     240},
  marioReprises[marioNReprises] = {24, 65, 92, 24, 24, 128, 128, 92, 7},
  marioGoTo[marioNReprises] =     {7,  24, 0,  7,  92, 92,  65,  0,  92};

void playSuperMarioTheme() {
  //Serial.println("Playing Super Mario Theme...");
  int color = -1, repr = 0;
  for (int i=0; i<marioN; i++) {
    if (repr < marioNReprises && i == marioReprises[repr])
      i = marioGoTo[repr++];
    color = getNewRandomColor(color);
    digitalWrite(ledPin[color], HIGH);
    tone(soundPin, marioNotes[i], marioNoteLength);
    delay(marioLengths[i]);
    digitalWrite(ledPin[color], LOW);
  }
}
