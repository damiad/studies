package pl.edu.mimuw.matrix;

//Macierz stała w tym Zerowa
public class ConstantMatrix extends RegularSparseMatrix implements IDoubleMatrix{
    protected final double CommonValue;
    ConstantMatrix(Shape shape, double commonValue){
        this.Shape=shape;
        this.CommonValue=commonValue;
    }

    @Override
    public IDoubleMatrix times(double scalar){
        return new ConstantMatrix(Shape, CommonValue*scalar);
    }

    @Override
    public IDoubleMatrix plus(double scalar){
        return new ConstantMatrix(Shape, CommonValue+scalar);
    }

    @Override
    public double get(int row, int column){
        Shape.assertInShape(row, column);
        return CommonValue;
    }

    @Override
    public double normOne(){
        return Shape.rows*Math.abs(CommonValue);
    }

    @Override
    public double normInfinity(){ return Shape.columns*Math.abs(CommonValue); }

    @Override
    public double frobeniusNorm(){
        return Math.abs(CommonValue) * Math.sqrt(Shape.rows*Shape.columns);
    }

    @Override
    public String toString(){
        StringBuilder printf= new StringBuilder("Wymiar: " + Shape.rows + "x" + Shape.columns + "\n");
        switch(Shape.columns){
            case 1:
                for(int i=0; i<Shape.rows; i++){
                    printf.append(CommonValue).append("\n");
                }
                break;
            case 2:
                for(int i=0; i<Shape.rows; i++){
                    printf.append(CommonValue).append(" ").append(CommonValue).append("\n");
                }
                break;
            default:
                for(int i=0; i<Shape.rows; i++){
                    printf.append(CommonValue).append(" ... ").append(CommonValue).append("\n");
                }
        }
        return printf.toString();
    }

}
