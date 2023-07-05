package pl.edu.mimuw.matrix;

import static pl.edu.mimuw.matrix.Shape.matrix;

//Macierz kolumnowa, wszystkie wartości w wierszu są równe.
public class ColumnsMatrix extends RegularSparseMatrix implements IDoubleMatrix{
    public final double[] RowValues; //wartość w danej kolumnie
    ColumnsMatrix(int columns, double[] rowValues){
        this.Shape=matrix(rowValues.length, columns);
        double[] Values= new double[Shape.rows];
        System.arraycopy(rowValues, 0, Values, 0, Shape.rows);
        this.RowValues=Values;
    }

    @Override
    public IDoubleMatrix times(double scalar){
        double[] Values= new double[Shape.rows];
        for(int i=0; i<Shape.rows; i++){
            Values[i]=RowValues[i]*scalar;
        }
        return new ColumnsMatrix(Shape.columns, Values);
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.plus(other);
        }
        ColumnsMatrix o = (ColumnsMatrix) other;
        double[] newValues= new double[Shape.rows];
        for(int i=0; i<Shape.rows; i++){
            newValues[i]=RowValues[i]+o.RowValues[i];
        }
        return new ColumnsMatrix(Shape.columns, newValues);
    }

    @Override
    public IDoubleMatrix plus(double scalar){
        double[] Values= new double[Shape.rows];
        for(int i=0; i<Shape.rows; i++){
            Values[i]=RowValues[i]+scalar;
        }
        return new ColumnsMatrix(Shape.columns, Values);
    }

    @Override
    public IDoubleMatrix minus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.minus(other);
        }
        ColumnsMatrix o = (ColumnsMatrix) other;
        double[] newValues= new double[Shape.rows];
        for(int i=0; i<Shape.rows; i++){
            newValues[i]=RowValues[i]-o.RowValues[i];
        }
        return new ColumnsMatrix(Shape.columns, newValues);
    }

    @Override
    public double get(int row, int column){
        Shape.assertInShape(row, column);
        return RowValues[row];
    }

    @Override
    public double normOne(){
        double result=0; //Suma w dowolnej kolumnie
        for(int i=0; i<Shape.rows; i++){
            result+=Math.abs(RowValues[i]);
        }
        return result;
    }

    @Override
    public double normInfinity(){
        double max=0; //Max z wierszy x jego długość
        for(int i=0; i<Shape.rows; i++){
            if(max<Math.abs(RowValues[i])) max=Math.abs(RowValues[i]);
        }
        return max* Shape.columns;
    }

    @Override
    public double frobeniusNorm(){
        double result=0;
        for(int i=0; i<Shape.rows; i++){
            result+=RowValues[i]*RowValues[i];
        }
        result*= Shape.columns;
        return Math.sqrt(result);
    }

    @Override
    public String toString(){
        StringBuilder printf= new StringBuilder("Wymiar: " + Shape.rows + "x" + Shape.columns + "\n");
        switch(Shape.columns){
            case 1:
                for(int i=0; i<Shape.rows; i++){
                    printf.append(RowValues[i]).append("\n");
                }
                break;
            case 2:
                for(int i=0; i<Shape.rows; i++){
                    printf.append(RowValues[i]).append(" ").append(RowValues[i]).append("\n");
                }
                break;
            default:
                for(int i=0; i<Shape.rows; i++){
                    printf.append(RowValues[i]).append(" ... ").append(RowValues[i]).append("\n");
                }
        }
        return printf.toString();
    }
}
