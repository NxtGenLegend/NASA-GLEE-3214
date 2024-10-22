//gaussian elimination with partial pivoting, assuming 2 matrices exist : sensor data and outlier data
 
//for reference: i: index variable for rows, j index variable for columns, k index variable for selecting pivot row,
// l is index variable for eliminating non-zero entries below the pivot
 
// Naiessha Suggestions
// elimination of zeros needs to be looped for all columns, need to handle pivot = 0 case
// and remaining rows that have some elements as zeros maintain their original elements in place of the zeroes

//rows=readings, columns=no of sensors
const int ROWS= 6;
const int COL=11;
 
//temporarily inputted non-accurate sample values (no. of decimal places are accurate)
float finalreadings [6][11]= {
    {1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00000, 9.00000, 100, 0.02},
    {1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00000, 9.00000, 100, 0.02},
    {1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00000, 9.00000, 100, 0.02},
    {1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00000, 9.00000, 100, 0.02},
    {1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00000, 9.00000, 100, 0.02},
    {1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00000, 9.00000, 100, 0.02},
    };
 
float outliersmatrix [6][11]= {
    {7.40, 9.20, 9.30, 33.08, 20.40, 1.07, 6.07, 3.00450, 3.87000, 176, 0.37},
    {7.40, 9.20, 9.30, 33.08, 20.40, 1.07, 6.07, 3.00450, 3.87000, 176, 0.37},
    {7.40, 9.20, 9.30, 33.08, 20.40, 1.07, 6.07, 3.00450, 3.87000, 176, 0.37},
    {7.40, 9.20, 9.30, 33.08, 20.40, 1.07, 6.07, 3.00450, 3.87000, 176, 0.37},
    {7.40, 9.20, 9.30, 33.08, 20.40, 1.07, 6.07, 3.00450, 3.87000, 176, 0.37},
    {7.40, 9.20, 9.30, 33.08, 20.40, 1.07, 6.07, 3.00450, 3.87000, 176, 0.37},
    };
 
void setup () {
    Serial.begin(9600);
}
 
void loop () {
 
int i;
int j;
int k;
int l;
 
// row reduction step 1 : determine pivot and divide rows
for (int i = 0, k = 0; i < ROWS && k < ROWS; i++, k++) {
    float pivot[2] = {finalreadings[i][i], outliersmatrix[i][i]}; //first diagonal element in row is pivot
    for (int j = i; j < COL; j++) {
        finalreadings[i][j] /= pivot[0];
    }
}
 
//obtain row echelon form through factoring
for (int l = 0; l < ROWS; l++) {
  if (l == i)
    continue;
  float factor = finalreadings[l][i]/finalreadings [i][i];
  for (int j = i; j < COL; j++) {
    finalreadings[l][j] -= factor * finalreadings[i][j];
  }
  for (int j= 0; j < ROWS; j++)
  outliersmatrix[i][j] -= factor * outliersmatrix[i][j]; //changed l j to i j
}

//eliminating zero rows
int nonzero_rows=ROWS; {
for (int i=0; i < ROWS; i++) {
  if (finalreadings [i][0]==0)
  nonzero_rows--;
  for (int j=0; j < ROWS; j++) {
    finalreadings[i][j]=finalreadings[i+1][j];
}
}
}
 
// print finalreadings matrix
  for (int i = 0; i < nonzero_rows; i++) {
    for (int j = 0; j < COL; j++) {
      Serial.print(finalreadings[i][j]);
      Serial.print(" ");
    }
  }
 
  // print outliersmatrix matrix
  for (int i = 0; i < nonzero_rows; i++) {
    for (int j = 0; j < COL; j++) {
      Serial.print(outliersmatrix[i][j]);
      Serial.print(" ");
    }
  }
} 


