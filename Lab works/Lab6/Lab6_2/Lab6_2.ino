const int matrix_row[] = {6, 11, A5, 9, 5, A4, 4, A2};
const int matrix_col[] = {10, 3, 2, 7, A3, 8, 12, 13};
const int matrix_size = 8;

const int joyButton_xPin = A1;
const int joyButton_yPin = A0;

const int joyButton_xMinTreshold = 400;
const int joyButton_xMaxTreshold = 600;
const int joyButton_yMinTreshold = 400;
const int joyButton_yMaxTreshold = 600;

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < matrix_size; ++i) {
    pinMode(matrix_row[i], OUTPUT);
    pinMode(matrix_col[i], OUTPUT);
  }

  for(int i = 0; i < matrix_size; ++i) {
    digitalWrite(matrix_row[i], HIGH);
    digitalWrite(matrix_col[i], LOW);
  }

   changeCell(0, 0, true);

   Serial.begin(9600);
}

void changeCell(int row_num, int col_num, bool state) {
  if(row_num >= matrix_size || col_num >= matrix_size)
    return;
  digitalWrite(matrix_row[row_num], !state);
  digitalWrite(matrix_col[col_num], state);
}

int current_row = 0;
int prev_row = 0;
bool changed_row = false;
int current_col = 0;
int prev_col = 0;
bool changed_col = false;

void loop() {
  // put your main code here, to run repeatedly:
  int joyButton_xValue = analogRead(joyButton_xPin);
  //Serial.println(joyButton_xValue);
  if(changed_col == false && joyButton_xValue < joyButton_xMinTreshold) {
    --current_col;
    if(current_col < 0)
      current_col = matrix_size - 1;
    changed_col = true;
  }
  if(changed_col == false && joyButton_xValue > joyButton_xMaxTreshold ) {
    ++current_col;
    if(current_col == matrix_size)
      current_col = 0;
    changed_col = true;
  }
  if(changed_col == true && joyButton_xValue >= joyButton_xMinTreshold && joyButton_xValue <= joyButton_xMaxTreshold)
    changed_col = false;
    
  int joyButton_yValue = analogRead(joyButton_yPin);
  if(changed_row == false && joyButton_yValue < joyButton_yMinTreshold) {
    --current_row;
    if(current_row < 0)
      current_row = matrix_size - 1;
    changed_row = true;
  }
  if(changed_row == false && joyButton_yValue > joyButton_yMaxTreshold ) {
    ++current_row;
    if(current_row == matrix_size)
      current_row = 0;
    changed_row = true;
  }
  if(changed_row == true && joyButton_yValue >= joyButton_yMinTreshold && joyButton_yValue <= joyButton_yMaxTreshold)
    changed_row = false;
  if(prev_row != current_row || prev_col != current_col) {
   Serial.println('(' + String(current_row) + ", " + String(current_col) + ')');
   changeCell(prev_row, prev_col, false);
   changeCell(current_row, current_col, true);
   prev_row = current_row;
   prev_col = current_col;
  }
}
