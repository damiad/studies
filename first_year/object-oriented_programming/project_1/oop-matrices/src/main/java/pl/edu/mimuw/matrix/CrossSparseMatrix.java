package pl.edu.mimuw.matrix;

//Macierz rzadka z przekątną (diagonalna bądź antydiagonalna.
public abstract class CrossSparseMatrix extends RegularSparseMatrix implements IDoubleMatrix{
    protected double[] CrossValues; //wartość w danym ixi lub odpowiednio  ixn-1-i przekątnej


    protected double[] operations(double[] o, int sign) { //dodaje bądź odejmuje 2 macierze tych samych klas
        double[] data = new double[Shape.rows];
        if(sign==2){ //sign to 1 dla dodawania lub -1 dla dejmowania, 2 dla mnożenia
            for (int i = 0; i < Shape.rows; i++) {
                data[i] = this.CrossValues[i] * o[i];
            }
        }else {
            for (int i = 0; i < Shape.rows; i++) {
                data[i] = this.CrossValues[i] + sign * o[i];
            }
        }
        return data;
    }

    @Override
    public double normOne(){
        double max=0;
        for(int i=0; i<Shape.rows; i++){
            if(max<Math.abs(CrossValues[i])) max=Math.abs(CrossValues[i]);
        }
        return max;
    }

    @Override
    public double normInfinity(){
        return normOne();
    }

    @Override
    public double frobeniusNorm(){
        int result=0;
        for(int i=0; i<Shape.rows; i++){
            result+= CrossValues[i] * CrossValues[i];
        }
        return Math.sqrt(result);
    }
}
