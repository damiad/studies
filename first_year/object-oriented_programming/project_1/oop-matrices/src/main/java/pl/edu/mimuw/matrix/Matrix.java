package pl.edu.mimuw.matrix;
//Każda Macierz
public abstract class Matrix implements IDoubleMatrix{
    protected Shape Shape;

    @Override
    public IDoubleMatrix times(IDoubleMatrix other){
        assert(this.Shape.canMultiply(other.shape()));
        double[][] result= new double [Shape.rows][other.shape().columns];
        for(int i=0; i< Shape.rows; i++){
            for(int j=0; j< other.shape().columns; j++){
                double sum =0; //klasyczne mnożenie macierzy
                for(int k=0; k<Shape.columns; k++){
                    sum+= get(i, k) * other.get(k, j);
                }
                result[i][j]= sum;
            }
        }
        return new NormalMatrix(result);
    }

    @Override
    public IDoubleMatrix times(double scalar){
        double[][] data=this.data();
        for(int i=0; i<Shape.rows; i++){
            for(int j=0; j<Shape.columns; j++){
                data[i][j]*=scalar;
            }
        }
        return new NormalMatrix(data);
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        double[][] data=this.data();
        for(int i=0; i<Shape.rows; i++){
            for(int j=0; j<Shape.columns; j++){
                data[i][j]+=other.get(i, j);
            }
        }
        return new NormalMatrix(data);
    }

    @Override
    public IDoubleMatrix plus(double scalar){
        double[][] data=new double [Shape.rows][Shape.columns];
        for(int i=0; i<Shape.rows; i++){
            for(int j=0; j<Shape.columns; j++){
                data[i][j]=get(i,j)+scalar;
            }
        }
        return new NormalMatrix(data);
    }

    @Override
    public IDoubleMatrix minus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        double[][] data=this.data();
        for(int i=0; i<Shape.rows; i++){
            for(int j=0; j<Shape.columns; j++){
                data[i][j]-=other.get(i, j);
            }
        }
        return new NormalMatrix(data);
    }

    @Override
    public IDoubleMatrix minus(double scalar) {
        return plus(-scalar);
    }

    @Override
    public double[][] data(){
        double[][] data= new double[Shape.rows][Shape.columns];
        for(int i=0; i<Shape.rows; i++){
            for(int j=0; j<Shape.columns; j++){
                data[i][j]=get(i,j);
            }
        }
        return data;
    }

    @Override
    public Shape shape(){
        return this.Shape;
    }

    @Override
    public String toString(){
        StringBuilder printf= new StringBuilder("Wymiar: " + Shape.rows + "x" + Shape.columns + "\n");
        for(int i=0; i<Shape.rows; i++){
            for(int j=0; j<Shape.columns-1; j++){
                printf.append(get(i, j)).append(" ");
            }
            printf.append(get(i, Shape.columns - 1)).append("\n");
        }
        return printf.toString();
    }
}
