package pl.edu.mimuw.matrix;

import static pl.edu.mimuw.matrix.Shape.matrix;

//Wartości tylko na głównej przekątnej, reszta 0.
public class DiagonalMatrix extends CrossSparseMatrix implements IDoubleMatrix{

    public DiagonalMatrix(double... crossValues){
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
        DiagonalMatrix o = (DiagonalMatrix) other;
        return new DiagonalMatrix(operations(o.CrossValues, 2));
    }

    @Override
    public IDoubleMatrix times(double scalar){
        double[] Values= new double[Shape.rows]; //=columns
        for(int i=0; i<Shape.rows; i++){
            Values[i]=CrossValues[i]*scalar;
        }
        return new DiagonalMatrix(Values);
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.plus(other);
        }
        DiagonalMatrix o = (DiagonalMatrix) other;
        return new DiagonalMatrix(operations(o.CrossValues, 1));
    }

    @Override
    public IDoubleMatrix minus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.minus(other);
        }
        DiagonalMatrix o = (DiagonalMatrix) other;
        return new DiagonalMatrix(operations(o.CrossValues, -1));
    }

    @Override
    public double get(int row, int column){
        Shape.assertInShape(row, column);
        if(row==column) return CrossValues[row];
        return 0;
    }

    @Override
    public String toString(){
        StringBuilder printf= new StringBuilder("Wymiar: " + Shape.rows + "x" + Shape.columns + "\n");
        for(int i=0; i<Shape.rows; i++){
            if(i<3){
                printf.append("0 ".repeat(i));
            }else{
                printf.append("0 ... 0 ");
            }
            printf.append(CrossValues[i]);
            if(Shape.rows-i<=3){
                printf.append(" 0".repeat(Shape.rows - i - 1));
            }else{
                printf.append(" 0 ... 0");
            }
            printf.append("\n");
        }
        return printf.toString();
    }
    
}
