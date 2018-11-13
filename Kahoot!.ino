#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523


void setup() {
}

void loop() {
  play();//Play the music.
  delay(300);//Pause for a while.
}

// notes in the melody:
int melody[] = {
  NOTE_C3,//4
  NOTE_G4,//8
  NOTE_G4,//8
  NOTE_C4,//4
  NOTE_G4,//8
  NOTE_G4,//8
  NOTE_B3,//8
  NOTE_C4,//8
  NOTE_G4,//8
  NOTE_G4,//8
  0,
  NOTE_G4,//8
  NOTE_G4,//8
  NOTE_F3,//4
  NOTE_F4,//8
  NOTE_F4,//8
  NOTE_F3,//4
  NOTE_F4,//8
  NOTE_F4,//8
  0,
  NOTE_F4,//8
  NOTE_F4,//8
  NOTE_E3,//8
  NOTE_D3,//8
  NOTE_E4,//8
  NOTE_E4,//8

  NOTE_C3,//4
  NOTE_G4,//8
  NOTE_G4,//8
  NOTE_C4,//4
  NOTE_G4,//8
  NOTE_G4,//8
  NOTE_B3,//8
  NOTE_C4,//8
  NOTE_G4,//8
  NOTE_G4,//8
  0,
  NOTE_G4,//8
  NOTE_G4,//8
  NOTE_F3,//4
  NOTE_F4,//8
  NOTE_F4,//8
  NOTE_F3,//4
  NOTE_F4,//8
  NOTE_F4,//8
  0,
  NOTE_F4,//8
  NOTE_F4,//8
  NOTE_E3,//8
  NOTE_D3,//8
  NOTE_E4,//8
  NOTE_E4,//8

  NOTE_E4,//16
  0,//16
  NOTE_E4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  NOTE_C4,//16
  0,//16
  NOTE_C4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  0,//4
  
  NOTE_G4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  0,//4

  NOTE_G4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  NOTE_F3,//4
  NOTE_F4,//16
  0,//16
  NOTE_F4,//16
  0,//16
  NOTE_F3,//4
  NOTE_F4,//16
  0,//16
  NOTE_F4,//16
  0,//16
  0,//4

  NOTE_F4,//16
  0,//16
  NOTE_F4,//16
  0,//16
  NOTE_E3,//8
  NOTE_D3,//8
  NOTE_E4,//16
  0,//16
  NOTE_E4,//16
  0,//16
  
  NOTE_E4,//16
  0,//16
  NOTE_E4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  NOTE_C4,//16
  0,//16
  NOTE_C4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  0,//4
  
  NOTE_G4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  0,//4

  NOTE_G4,//16
  0,//16
  NOTE_G4,//16
  0,//16
  NOTE_F3,//4
  NOTE_F4,//16
  0,//16
  NOTE_F4,//16
  0,//16
  NOTE_F3,//4
  NOTE_F4,//16
  0,//16
  NOTE_F4,//16
  0,//16
  0,//4

  NOTE_F4,//16
  0,//16
  NOTE_F4,//16
  0,//16
  NOTE_E3,//8
  NOTE_D3,//8
  NOTE_E4,//16
  0,//16
  NOTE_E4,//16
  0,//16
};

int noteDurations[] = {
  4, 8, 8, 4, 8, 8, 8, 8, 8, 8,
  4,
  8, 8, 4, 8, 8, 4, 8, 8,
  4,
  8, 8, 8, 8, 8, 8,
  4, 8, 8, 4, 8, 8, 8, 8, 8, 8,
  4,
  8, 8, 4, 8, 8, 4, 8, 8,
  4,
  8, 8, 8, 8, 8, 8,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  4,
  16, 16, 16, 16,
  4,
  16, 16, 16, 16, 4, 16, 16, 16, 16, 4, 16, 16, 16, 16,
  4,
  16, 16, 16, 16, 8, 8, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  4,
  16, 16, 16, 16,
  4,
  16, 16, 16, 16, 4, 16, 16, 16, 16, 4, 16, 16, 16, 16,
  4,
  16, 16, 16, 16, 8, 8, 16, 16, 16, 16,
};

void play()
{
  for (int thisNote = 0; thisNote < 29; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1500 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}
