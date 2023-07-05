package pl.edu.mimuw.matrix;
import java.util.Arrays;
import java.util.Vector;


import static pl.edu.mimuw.matrix.MatrixCellValue.cell;
import static pl.edu.mimuw.matrix.Shape.matrix;


//Nieregularna macierz rzadka
public class IrregularSparseMatrix extends SparseMatrix implements IDoubleMatrix{

    protected final MatrixCellValue[] MatrixCells; //Wartości rzadkie (posortowane wiersz, kolumna)

    IrregularSparseMatrix(Shape shape, MatrixCellValue... values) {
        for (MatrixCellValue value : values) {
            shape.assertInShape(value.row, value.column); //czy Cells w zakresie macierzy
        }
        this.Shape = shape;
        MatrixCellValue[] ValuesCopy= new MatrixCellValue[values.length]; //=columns
        System.arraycopy(values, 0, ValuesCopy, 0, values.length);
        Arrays.sort(ValuesCopy,
                (a, b) -> {
                    if (a.row == b.row) {
                        assert (a.column != b.column);
                        return a.column - b.column;
                    }
                    return a.row - b.row;
                }
        );
        this.MatrixCells = ValuesCopy; //Posortowane wierszowo wartości rzadkie.
    }

    @Override //popraw
    public IDoubleMatrix times(IDoubleMatrix other){
        assert(this.Shape.canMultiply(other.shape()));
        if(other.getClass()!= getClass()){
            return super.times(other);
        }
        IrregularSparseMatrix o = (IrregularSparseMatrix) other;
        MatrixCellValue[] CellsColumnOrder=Arrays.copyOf(o.MatrixCells, o.MatrixCells.length);
        Arrays.sort(CellsColumnOrder,
                (a, b) -> {
                    if (a.column == b.column) {
                        return a.row - b.row;
                    }
                    return a.column - b.column;
                }
        ); //Sortujemy kolumnowo
        Vector<MatrixCellValue> vec = new Vector<>();

        int i=0;
        while(i<this.MatrixCells.length) {
            int row = MatrixCells[i].row;
            int row_count = 0;
            while (i+row_count<MatrixCells.length
                    && MatrixCells[i + row_count].row == row) row_count++; //zliczamy długość wiersza

            int j = 0;
            while (j < CellsColumnOrder.length) {
                int column = CellsColumnOrder[j].column;
                int column_count = 0;
                while (j+column_count<CellsColumnOrder.length
                        && CellsColumnOrder[j + column_count].column == column) column_count++; //długość kolumny
                double sum = 0;
                for (int p = i; p < i + row_count; p++) {
                    for (int q = j; q < j + column_count; q++) {
                        if (MatrixCells[p].column == CellsColumnOrder[q].row)
                            sum += MatrixCells[p].value * CellsColumnOrder[q].value;
                    }
                }
                if (sum != 0) vec.add(cell(MatrixCells[i].row, CellsColumnOrder[j].column, sum));
                j += column_count;
            }
            i += row_count;
        }
        if(vec.size()==0) return new ZeroMatrix(matrix(Shape.rows,other.shape().columns));

        MatrixCellValue[] ValuesCopy= new MatrixCellValue[vec.size()];
        for(int k=0; k<vec.size(); k++){
            ValuesCopy[k]=vec.elementAt(k);
        }

        return new IrregularSparseMatrix(matrix(Shape.rows,other.shape().columns), ValuesCopy);
    }

    @Override
    public IDoubleMatrix times(double scalar){
        MatrixCellValue[] newCells = new MatrixCellValue[MatrixCells.length];
        for(int i=0; i< MatrixCells.length; i++){
            newCells[i]= cell(MatrixCells[i].row, MatrixCells[i].column, MatrixCells[i].value*scalar);
        }
        return new IrregularSparseMatrix(Shape, newCells);
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.plus(other);
        }
        IrregularSparseMatrix o = (IrregularSparseMatrix) other;
        MatrixCellValue[] newValues= new MatrixCellValue[MatrixCells.length + o.MatrixCells.length];
        int k=0; //Ile newCells
        int i=0; int j=0; //Które z kolei sparse Cell
        while(i<this.MatrixCells.length && j<o.MatrixCells.length){
            if(this.MatrixCells[i].row<o.MatrixCells[j].row){
                newValues[k++]=this.MatrixCells[i++];
            }else if(this.MatrixCells[i].row>o.MatrixCells[j].row){
                newValues[k++]=o.MatrixCells[j++];
            }else{ //ten sam wiersz
                if(this.MatrixCells[i].column<o.MatrixCells[j].column){
                    newValues[k++]=this.MatrixCells[i++];
                }else if(this.MatrixCells[i].column>o.MatrixCells[j].column){
                    newValues[k++]=o.MatrixCells[j++];
                }else{ //równe wiersze i kolumny
                    if(this.MatrixCells[i].value==-o.MatrixCells[j].value){
                        i++; j++; //wyrazy się niwelują
                    }else{
                        newValues[k++]=cell(this.MatrixCells[i].row, this.MatrixCells[i].column,
                                this.MatrixCells[i++].value+o.MatrixCells[j++].value);
                    }
                }
            }
        }
        while(j<o.MatrixCells.length){
            newValues[k++]=o.MatrixCells[j++];
        }
        while(i<this.MatrixCells.length){
            newValues[k++]=this.MatrixCells[i++];
        } //przepisujemy pozostałe albo z this albo o

        MatrixCellValue[] newCells= new MatrixCellValue[k];
        System.arraycopy(newValues, 0, newCells, 0, k);
        return new IrregularSparseMatrix(Shape, newCells);
    }

    @Override
    public IDoubleMatrix minus(IDoubleMatrix other){
        assert(this.Shape.equals(other.shape()));
        if(other.getClass()!= getClass()){
            return super.minus(other);
        }
        IrregularSparseMatrix o = (IrregularSparseMatrix) other;
        MatrixCellValue[] newValues= new MatrixCellValue[MatrixCells.length + o.MatrixCells.length];
        int k=0; //Ile newCells
        int i=0; int j=0; //Które z kolei sparse Cell
        while(i<this.MatrixCells.length && j<o.MatrixCells.length){
            if(this.MatrixCells[i].row<o.MatrixCells[j].row){
                newValues[k++]=this.MatrixCells[i++];
            }else if(this.MatrixCells[i].row>o.MatrixCells[j].row){
                newValues[k++]=cell(o.MatrixCells[j].row, o.MatrixCells[j].column,
                        -o.MatrixCells[j++].value);
            }else{ //ten sam wiersz
                if(this.MatrixCells[i].column<o.MatrixCells[j].column){
                    newValues[k++]=this.MatrixCells[i++];
                }else if(this.MatrixCells[i].column>o.MatrixCells[j].column){
                    newValues[k++]=cell(o.MatrixCells[j].row, o.MatrixCells[j].column,
                            -o.MatrixCells[j++].value);
                }else{ //równe wiersze i kolumny
                    if(this.MatrixCells[i].value==o.MatrixCells[j].value){
                        i++; j++; //wyrazy się niwelują
                    }else{
                        newValues[k++]=cell(this.MatrixCells[i].row, this.MatrixCells[i].column,
                                this.MatrixCells[i++].value-o.MatrixCells[j++].value);
                    }
                }
            }
        }
        while(j<o.MatrixCells.length){
            newValues[k++]=cell(o.MatrixCells[j].row, o.MatrixCells[j].column,
                    -o.MatrixCells[j++].value);
        }
        while(i<this.MatrixCells.length){
            newValues[k++]=this.MatrixCells[i++];
        } //przepisujemy pozostałe albo z this albo o

        MatrixCellValue[] newCells= new MatrixCellValue[k];
        System.arraycopy(newValues, 0, newCells, 0, k);
        return new IrregularSparseMatrix(Shape, newCells);
    }

    @Override
    public double get(int row, int column){
        Shape.assertInShape(row, column);
        int i=0;
        while (i<MatrixCells.length && MatrixCells[i].row<row) i++;
        while (i<MatrixCells.length && MatrixCells[i].row==row && MatrixCells[i].column<column) i++;
        if (i<MatrixCells.length && MatrixCells[i].row==row && MatrixCells[i].column==column)
            return MatrixCells[i].value; //if po wyjściu z while, żeby nie używać twice if
        return 0; //Nie ma takiej komórki niezerowej
    }

    @Override
    public double[][] data(){
        double[][] data= new double[Shape.rows][Shape.columns];
        for(int i=0; i<Shape.rows; i++){
            for(int j=0; j<Shape.columns; j++){
                data[i][j]=0;
            }
        }
        for (MatrixCellValue matrixCell : MatrixCells) {
            data[matrixCell.row][matrixCell.column] = matrixCell.value;
        }//działa szybciej niż get
        return data;
    }

    @Override
    public double normOne(){
        MatrixCellValue[] CellsColumnOrder=Arrays.copyOf(MatrixCells, MatrixCells.length);
        Arrays.sort(CellsColumnOrder,
                (a, b) -> {
                    if (a.column == b.column) {
                        return a.row - b.row;
                    }
                    return a.column - b.column;
                }
        ); //Sortujemy kolumnowo
        double max=0;
        double temp=0;
        int j=0;
        for (MatrixCellValue matrixCellValue : CellsColumnOrder) {
            if (matrixCellValue.column != j) {
                if (temp > max) max = temp;
                temp = 0;
                j = matrixCellValue.column;
            }
            temp += Math.abs(matrixCellValue.value);
        }
        if(temp>max) max=temp;
        return max;
    }

    @Override
    public double normInfinity(){
        double max=0;
        double temp=0;
        int i=0;
        for (MatrixCellValue matrixCell : MatrixCells) {
            if (matrixCell.row != i) {
                if (temp > max) max = temp;
                temp = 0;
                i = matrixCell.row;
            }
            temp += Math.abs(matrixCell.value);
        }
        if(temp>max) max=temp; // po wyjściu z for
        return max;
    }

    @Override
    public double frobeniusNorm(){
        double result =0;
        for (MatrixCellValue matrixCell : MatrixCells) {
            result += matrixCell.value * matrixCell.value;
        }
        return Math.sqrt(result);
    }

    @Override
    public String toString() {
        StringBuilder printf = new StringBuilder("Wymiar: " + Shape.rows + "x" + Shape.columns + "\n");

        int k = 0; //Które z kolei MatrixCells

        for (int i = 0; i < Shape.rows; i++) {
            int prev_j = -1;
            while (k < MatrixCells.length && MatrixCells[k].row == i) {
                int diff = MatrixCells[k].column - prev_j;
                if (diff > 3) printf.append("0 ... 0 ");
                else {
                    printf.append("0 ".repeat(Math.max(0, diff - 1)));
                }
                printf.append(MatrixCells[k].value).append(" ");
                prev_j = MatrixCells[k].column;
                k++;
            }
            int diff = Shape.columns - 1 - prev_j;
            if (diff >= 3) printf.append("0 ... 0");
            else {
                printf.append("0 ".repeat(Math.max(0, diff)));
            }
            printf.append("\n");
        }
        return printf.toString();
    }

}
