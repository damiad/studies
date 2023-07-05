package pl.edu.mimuw.matrix;

import static pl.edu.mimuw.matrix.Shape.matrix;

//Macierz wierszowa, wszyskie wartości w kolumnie są równe.
public class RowsMatrix extends RegularSparseMatrix implements IDoubleMatrix{

    protected final double[] ColumnValues; //Wartości w danym wierszu.

    RowsMatrix(int rows, double[] columnValues){
        this.Shape=matrix(rows, columnValues.length);
        double[] Values= new double[Shape.columns];
        System.arraycopy(columnValues, 0, Values, 0, Shape.columns);
        this.ColumnValues=Values;
    }

    @Override
    public IDoubleMatrix times(double scalar){
        double[] Values= new double[Shape.columns];
        for(int j=0; j<Shape.columns; j++){
            Values[j]=ColumnValues[j]*scalar;
        }
        return new ColumnsMatrix(Shape.rows, Values);
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.plus(other);
        }
        RowsMatrix o = (RowsMatrix) other;
        double[] newValues= new double[Shape.columns];
        for(int i=0; i<Shape.columns; i++){
            newValues[i]=ColumnValues[i]+o.ColumnValues[i];
        }
        return new RowsMatrix(Shape.rows, newValues);
    }

    @Override
    public IDoubleMatrix plus(double scalar){
        double[] Values= new double[Shape.columns];
        for(int j=0; j<Shape.columns; j++){
            Values[j]=ColumnValues[j]+scalar;
        }
        return new ColumnsMatrix(Shape.rows, Values);
    }

    @Override
    public IDoubleMatrix minus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.minus(other);
        }
        RowsMatrix o = (RowsMatrix) other;
        double[] newValues= new double[Shape.columns];
        for(int i=0; i<Shape.columns; i++){
            newValues[i]=ColumnValues[i]-o.ColumnValues[i];
        }
        return new RowsMatrix(Shape.rows, newValues);
    }

    @Override
    public double get(int row, int column){
        Shape.assertInShape(row, column);
        return ColumnValues[column];
    }

    @Override
    public double normOne(){
        double max=0; //Maks z kolumn.
        for(int j=0; j<Shape.columns; j++){
            if(max<Math.abs(ColumnValues[j])) max=Math.abs(ColumnValues[j]);
        }
        return max* Shape.rows;
    }

    @Override
    public double normInfinity(){
        double result=0; //Dowolny wiersz.
        for(int j=0; j<Shape.columns; j++){
            result+=Math.abs(ColumnValues[j]);
        }
        return result;
    }

    @Override
    public double frobeniusNorm(){
        double result=0;
        for(int j=0; j<Shape.columns; j++){
            result+=ColumnValues[j]*ColumnValues[j];
        }
        result*= Shape.rows; //Każdy wiersz taki sam.
        return Math.sqrt(result);
    }

}


