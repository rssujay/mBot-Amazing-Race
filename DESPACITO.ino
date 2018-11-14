#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_E4  330

void setup() {
}

void loop() {
  play();
  delay(300);
}

int melody[] = {NOTE_D4,NOTE_CS4,NOTE_B3,NOTE_FS3,NOTE_FS3,NOTE_FS3,NOTE_FS3,NOTE_FS3,NOTE_FS3,NOTE_B3,
                NOTE_B3,NOTE_B3,NOTE_B3,NOTE_A3,NOTE_B3,NOTE_G3,NOTE_G3,NOTE_G3,NOTE_G3,NOTE_G3,
                NOTE_B3,NOTE_B3,NOTE_B3,NOTE_B3,NOTE_B3,NOTE_CS4,NOTE_D4,NOTE_A3,NOTE_A3,NOTE_A3,
                NOTE_A3,NOTE_A3,NOTE_A3,NOTE_D4,NOTE_D4,NOTE_D4,NOTE_D4,NOTE_E4,NOTE_E4,NOTE_CS4};

int noteDurations[] = {2,2,4,4,8,8,8,8,8,8,8,8,4,8,4,3,8,8,8,8,8,8,8,8,4,8,4,3,8,8,8,8,8,8,8,8,4,8,4,3};

void play() {
  for (int thisNote = 0; thisNote <= 41; thisNote++) {
    int noteDuration = 1100/noteDurations[thisNote];
    tone(8, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}
