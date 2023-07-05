package pl.edu.mimuw.matrix;

import static pl.edu.mimuw.matrix.Shape.matrix;

//Macierz NIE -rzadka (zwyczajna, bez optymalizacji)
public class NormalMatrix extends Matrix implements IDoubleMatrix {

    protected final double[][] Values;

    public NormalMatrix(double[][] values) {
        assert(values!=null);
        assert(values.length!=0);
        this.Shape = matrix(values.length, values[0].length);
        for (double[] value : values) {
            assert (value.length == Shape.columns); //czy wiersze równej długości
        }
        double [][] val = new double[Shape.rows][Shape.columns];
        for(int i=0; i<Shape.rows; i++){
            System.arraycopy(values[i], 0, val[i], 0, Shape.columns);
        }
        this.Values = val;
    }

    @Override
    public double get(int row, int column) {
        Shape.assertInShape(row, column);
        return Values[row][column];
    }

    @Override
    public double normOne() {
        double max = 0;
        for (int j = 0; j < Shape.columns; j++) {
            double temp = 0;
            for (int i = 0; i < Shape.rows; i++) {
                temp += Math.abs(Values[i][j]);
            }
            if (temp > max) max = temp;
        }
        return max;
    }

    @Override
    public double normInfinity() {
        double max = 0;
        for (int i = 0; i < Shape.rows; i++) {
            double temp = 0;
            for (int j = 0; j < Shape.columns; j++) {
                temp += Math.abs(Values[i][j]);
            }
            if (temp > max) max = temp;
        }
        return max;
    }

    @Override
    public double frobeniusNorm() {
        double val = 0;
        for (int i = 0; i < Shape.rows; i++) {
            for (int j = 0; j < Shape.columns; j++) {
                val += Values[i][j] * Values[i][j];
            }
        }
        return Math.sqrt(val);
    }
}
