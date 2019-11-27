const int matrix_row[] = {6, 11, A5, 9, 5, A4, 4, A2};
const int matrix_col[] = {10, 3, 2, 7, A3, 8, 12, 13};
const int matrix_size = 8;
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
}

void changeCell(int row_num, int col_num, bool state) {
  if(row_num >= matrix_size || col_num >= matrix_size)
    return;
  digitalWrite(matrix_row[row_num], !state);
  digitalWrite(matrix_col[col_num], state);
}

int current_row = 0;
int current_col = 0;
int delay_milliseconds = 10;
void loop() {
  // put your main code here, to run repeatedly:
  changeCell(current_row, current_col, true);
  delay(delay_milliseconds);
  changeCell(current_row, current_col, false);
  delay(delay_milliseconds);
  ++current_col;
  if(current_col == matrix_size) {
    current_col = 0;
    ++current_row;
    if(current_row == matrix_size)
      current_row = 0;
  }
}
