package pl.edu.mimuw.matrix;

public class DoubleMatrixFactory {

  private DoubleMatrixFactory() {
  }
// Rzadka nieregularna
  public static IDoubleMatrix sparse(Shape shape, MatrixCellValue... values){
    return new IrregularSparseMatrix(shape, values);
  }
  //wypełniona tablica n x m
  public static IDoubleMatrix full(double[][] values) {
    return new NormalMatrix(values);
  }

  public static IDoubleMatrix identity(int size) {
    return new IdentityMatrix(size);
  }

  public static IDoubleMatrix diagonal(double... diagonalValues) {
    return new DiagonalMatrix(diagonalValues);
  }

  public static IDoubleMatrix antiDiagonal(double... antiDiagonalValues) {
    return new AntiDiagonalMatrix(antiDiagonalValues);
  }

  //macierz z 1 kolumną
  public static IDoubleMatrix vector(double... values){
    return new Vector(values);
  }

  public static IDoubleMatrix zero(Shape shape) {
    return new ZeroMatrix(shape);
  }

  //Macierz stałą
  public static IDoubleMatrix constant(Shape shape, double val) {
    return new ConstantMatrix(shape, val);
  }
  //Macierz wierszowa
  public static IDoubleMatrix rowsM(int rows, double[] values) {
    return new RowsMatrix(rows, values);
  }
  //Macierz kolumnowa
  public static IDoubleMatrix columnsM(int columns, double[] values) {
    return new ColumnsMatrix(columns, values);
  }

}
