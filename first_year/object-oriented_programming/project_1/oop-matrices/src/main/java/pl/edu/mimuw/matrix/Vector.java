package pl.edu.mimuw.matrix;

import static pl.edu.mimuw.matrix.Shape.vector;

//Macierz z 1 kolumną
public class Vector extends RegularSparseMatrix implements IDoubleMatrix{

    protected final double[] Values;

    public Vector(double... values){
        this.Shape=vector(values.length);
        double[] Val= new double[values.length];
        System.arraycopy(values, 0, Val, 0, values.length);
        this.Values=Val;
    }

    @Override
    public IDoubleMatrix times(double scalar){
        double[] val = new double[Shape.rows];
        for(int i=0; i<Shape.rows; i++){
            val[i]=Values[i]*scalar;
        }
        return new Vector(val);
    }

    @Override
    public IDoubleMatrix plus(double scalar){
        double[] val = new double[Shape.rows];
        for(int i=0; i<Shape.rows; i++){
            val[i]=Values[i]+scalar;
        }
        return new Vector(val);
    }

    @Override
    public double get(int row, int column){
        Shape.assertInShape(row, column);
        return Values[row];
    }

    @Override
    public double normOne(){
        double result=0;
        for (int i = 0; i < Shape.rows; i++) {
            result+=Math.abs(Values[i]);
        }
        return result;
    }

    @Override
    public double normInfinity(){
        double result=0;
        for (int i = 0; i < Shape.rows; i++) {
            if(result < Math.abs(Values[i]))
                result=Math.abs(Values[i]);
        }
        return result;
    }

    @Override
    public double frobeniusNorm(){
        double val=0;
        for(int i=0; i<Shape.rows; i++) {
            val += Values[i] * Values[i];
        }
        return Math.sqrt(val);
    }

    @Override
    public String toString(){
        StringBuilder printf= new StringBuilder("Wymiar: " + Shape.rows + "x" + Shape.columns + "\n");
        for(int i=0; i<Shape.rows; i++){
            printf.append(Values[i]).append("\n");
        }
        return printf.toString();
    }
}
