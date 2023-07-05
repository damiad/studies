package pl.edu.mimuw.matrix;

import static pl.edu.mimuw.matrix.Shape.matrix;

//Przekątna (ale nie główna), reszta to zera.
public class AntiDiagonalMatrix extends CrossSparseMatrix implements IDoubleMatrix{

    AntiDiagonalMatrix(double... crossValues){
        this.Shape=matrix(crossValues.length, crossValues.length);
        double[] Values= new double[Shape.rows]; //=columns
        System.arraycopy(crossValues, 0, Values, 0, Shape.rows);
        this.CrossValues=Values;
    }

    @Override
    public IDoubleMatrix times(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.times(other);
        }
        AntiDiagonalMatrix o = (AntiDiagonalMatrix) other;
        return new AntiDiagonalMatrix(operations(o.CrossValues, 2));
    }

    @Override
    public IDoubleMatrix times(double scalar){
        double[] Values= new double[Shape.rows]; //=columns
        for(int i=0; i<Shape.rows; i++){
            Values[i]=CrossValues[i]*scalar;
        }
        return new AntiDiagonalMatrix(Values);
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.plus(other);
        }
        AntiDiagonalMatrix o = (AntiDiagonalMatrix) other;
        return new AntiDiagonalMatrix(operations(o.CrossValues, 1));
    }

    @Override
    public IDoubleMatrix minus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.minus(other);
        }
        AntiDiagonalMatrix o = (AntiDiagonalMatrix) other;
        return new AntiDiagonalMatrix(operations(o.CrossValues, -1));
    }

    @Override
    public double get(int row, int column){
        Shape.assertInShape(row, column);
        if(row+column+1==Shape.rows) return CrossValues[row];
        return 0;
    }

    @Override
    public String toString(){
        StringBuilder printf= new StringBuilder("Wymiar: " + Shape.rows + "x" + Shape.columns + "\n");
        for(int i=0; i<Shape.rows; i++){
            if(Shape.rows-i<=3){
                printf.append("0 ".repeat(Shape.rows - i - 1));
            }else{
                printf.append("0 ... 0 ");
            }
            printf.append(CrossValues[i]);
            if(i<3){
                printf.append(" 0".repeat(i));
            }else{
                printf.append(" 0 ... 0");
            }
            printf.append("\n");
        }
        return printf.toString();
    }
}
