package pl.edu.mimuw.matrix;


import static pl.edu.mimuw.matrix.Shape.matrix;

//Macierz identycznościowa
public class IdentityMatrix extends RegularSparseMatrix implements IDoubleMatrix{

    public IdentityMatrix(int n){
        this.Shape=matrix(n,n);
    }

    @Override
    public IDoubleMatrix times(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.times(other);
        }
        return new IdentityMatrix(Shape.rows);
    }

    @Override
    public IDoubleMatrix times(double scalar){
        double[] Values= new double[Shape.rows];
        for(int i=0; i<Shape.rows; i++){
            Values[i]=scalar;
        }
        return new DiagonalMatrix(Values);
    }

    @Override
    public IDoubleMatrix minus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.minus(other);
        }
        return new ZeroMatrix(Shape);
    }

    @Override
    public double get(int row, int column){
        Shape.assertInShape(row, column);
        if(row==column) return 1;
        return 0;
    }

    @Override
    public double normOne(){
        return 1;
    }

    @Override
    public double normInfinity(){
        return 1;
    }

    @Override
    public double frobeniusNorm(){
        return Math.sqrt(this.Shape.rows);
    }

    @Override
    public String toString(){
        StringBuilder printf= new StringBuilder("Wymiar: " + Shape.rows + "x" + Shape.columns + "\n");
        for(int i=1; i<=Shape.rows; i++){
            if(i<=3){
                printf.append("0 ".repeat(Math.max(0, i - 1)));
            }else{
                printf.append("0 ... 0 ");
            }
            printf.append("1");
            if(Shape.rows-i<3){
                printf.append(" 0".repeat(Math.max(0, Shape.rows - i)));
            }else{
                printf.append(" 0 ... 0");
            }
            printf.append("\n");
        }
        return printf.toString();
    }
}
