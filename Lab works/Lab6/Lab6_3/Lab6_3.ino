const int row[] = {6, 11, A5, 9, 5, A4, 4, A2};

                 //C1 C2 C3 C4  C5 C6  C7  C8 - anodes
const int col[] = {10, 3, 2, 7, A3, 8, 12, 13};

byte Matrix[8][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
  {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0}, // 1
  {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0}, // 2
  {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0}, // 3
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 4
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 5
  {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0}, // 6
  {0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, // 7 
};

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < 8; i++)
  {
    pinMode(row[i], OUTPUT);
    pinMode(col[i], OUTPUT);
  }

  for(int i = 0; i < 8; i++)
    {
      digitalWrite(col[i], LOW);
      digitalWrite(row[i], HIGH);
    }
    Serial.begin(9600);
}

int c = 0;
unsigned long long int last_col_flush = 0;
void loop() {
  for( int i = 0; i < 8; i++)
  {
    int j;
    for(j = c ; j < c + 8 && j < 16; j++)
    {
      digitalWrite(col[j - c], Matrix[i][j]);
    }
    for(; j < c + 8; j++)
    {
      digitalWrite(col[j - c], Matrix[i][j - 16]); 
    }
    digitalWrite(row[i], LOW);
    delay(1);
    digitalWrite(row[i], HIGH);
  }
  if(millis() - last_col_flush >= 100) {
    ++c;
    if(c == 16)
      c = 0;
    last_col_flush = millis();
    Serial.println(c);
  }
}
