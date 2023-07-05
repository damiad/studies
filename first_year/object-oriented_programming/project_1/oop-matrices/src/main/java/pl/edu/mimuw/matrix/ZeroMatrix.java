package pl.edu.mimuw.matrix;
//Macierz zerowa, łatwe operacje.
public class ZeroMatrix extends RegularSparseMatrix implements IDoubleMatrix{

    public ZeroMatrix(Shape shape){
        this.Shape=shape;
    }

    @Override
    public IDoubleMatrix times(IDoubleMatrix other){
        return new ZeroMatrix(Shape);
    }

    @Override
    public IDoubleMatrix times(double scalar){
        return new ZeroMatrix(Shape);
    }

    @Override
    public IDoubleMatrix plus(double scalar){
        return new ConstantMatrix(this.Shape, scalar);
    }

    @Override
    public double get(int row, int column){
        Shape.assertInShape(row, column);
        return 0;
    }

    @Override
    public double normOne(){
        return 0;
    }

    @Override
    public double normInfinity(){
        return 0;
    }

    @Override
    public double frobeniusNorm(){
        return 0;
    }

    @Override
    public String toString(){
        StringBuilder printf= new StringBuilder("Wymiar: " + Shape.rows + "x" + Shape.columns + "\n");
        switch (Shape.columns) {
            case 1:
                printf.append("0\n".repeat(Shape.rows));
                break;
            case 2:
                printf.append("0 0\n".repeat(Shape.rows));
                break;
            default:
                printf.append("0 ... 0\n".repeat(Shape.rows));
        }
        return printf.toString();
    }
}
