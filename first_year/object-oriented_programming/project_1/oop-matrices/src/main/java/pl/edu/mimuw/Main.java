package pl.edu.mimuw;

import pl.edu.mimuw.matrix.*;


import static pl.edu.mimuw.matrix.DoubleMatrixFactory.*;
import static pl.edu.mimuw.matrix.MatrixCellValue.cell;
import static pl.edu.mimuw.matrix.Shape.matrix;
//SDK to <17> Openjdk-18
public class Main {

  public static void main(String[] args) {

    System.out.println("MATRIX PRINT TEST");


    final IDoubleMatrix SPARSE = sparse(matrix(10, 10),
            cell(0, 4, 1),
            cell(2, 2, 2),
            cell(4, 7, 3),
            cell(6, 9, 4),
            cell(8, 0, 5),
            cell(9, 4, 6)
    ); System.out.println("SPARSE:  "); System.out.println(SPARSE);

    double[][] temp = new double[10][10];
    for(int i=0; i<10; i++){
      for(int j=0; j<10; j++){
        temp[i][j]=10*i+j;
      }
    }
    final IDoubleMatrix FULL = full(temp);
    System.out.println("FULL:  "); System.out.println(FULL);

    final IDoubleMatrix IDENTITY = identity(10);
    System.out.println("IDENTITY:  "); System.out.println(IDENTITY);

    final IDoubleMatrix DIAGONAL = diagonal(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    System.out.println("DIAGONAL:  "); System.out.println(DIAGONAL);

    final IDoubleMatrix ANTIDIAGONAL = antiDiagonal(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    System.out.println("ANTIDIAGONAL:  "); System.out.println(ANTIDIAGONAL);

    final IDoubleMatrix VECTOR = vector(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    System.out.println("VECTOR:  "); System.out.println(VECTOR);

    final IDoubleMatrix ZERO = zero(matrix(10, 10));
    System.out.println("ZERO:  "); System.out.println(ZERO);

    final IDoubleMatrix CONSTANT = constant(matrix(10, 10),7);
    System.out.println("CONSTANT:  "); System.out.println(CONSTANT);

    final IDoubleMatrix ROWS_MATRIX = rowsM(10, temp[4]); //Stworzona w lini 25
    System.out.println("ROWS_MATRIX:  "); System.out.println(ROWS_MATRIX);

    final IDoubleMatrix COLUMNS_MATRIX = columnsM(10, temp[7]); //Stworzona w lini 25
    System.out.println("COLUMNS_MATRIX:  "); System.out.println(COLUMNS_MATRIX);

  }
}
