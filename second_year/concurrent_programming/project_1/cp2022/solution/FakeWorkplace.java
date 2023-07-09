package cp2022.solution;
import cp2022.base.Workplace;
import cp2022.base.WorkplaceId;

import java.util.concurrent.CountDownLatch;

public class FakeWorkplace extends Workplace {

    private Workplace originalWorkplace;
    private CountDownLatch checkEntry = new CountDownLatch(0); //sprawdza czy mozna wejsc
    private CountDownLatch freePreviousWorkplace = new CountDownLatch(1); //zwalnia poprzednie (o ile bylo)

    FakeWorkplace(WorkplaceId workplaceId, Workplace workplace){
        super(workplaceId);
        originalWorkplace = workplace;
    }

    public void entryLatch(CountDownLatch newLatch){
        this.checkEntry = newLatch;
    }

    public void previousFreeLatch(CountDownLatch newLatch){
        this.freePreviousWorkplace = newLatch;
    }

    public void use(){
        freePreviousWorkplace.countDown();
        try{
            checkEntry.await();
        }catch (InterruptedException e) {
            throw new RuntimeException("panic: unexpected thread interruption");
        }
        originalWorkplace.use();
    }
}