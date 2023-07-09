/*
 * University of Warsaw
 * Concurrent Programming Course 2022/2023
 * Java Assignment
 *
 * Author: Konrad Iwanicki (iwanicki@mimuw.edu.pl)
 */
package cp2022.demo;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;

import cp2022.base.Workplace;
import cp2022.base.WorkplaceId;
import cp2022.base.Workshop;
import cp2022.solution.WorkshopFactory;


public class TroysWorkshop {
    
    private static class TroysWorkplaceId extends WorkplaceId {
        private final String name;
        public TroysWorkplaceId(String name) {
            this.name = name;
        }
        @Override
        public int compareTo(WorkplaceId other) {
            if (!(other instanceof TroysWorkplaceId)) {
                throw new RuntimeException("Incomparable workplace types!");
            }
            return this.name.compareTo(((TroysWorkplaceId)other).name);
        }
        public String getName() {
            return this.name;
        }
    }
    
    private static class TroysWorkplace extends Workplace {
        private final static long MIN_USE_TIME_IN_MS = 10;
        private final static long MAX_USE_TIME_IN_MS = 100;
        public TroysWorkplace(TroysWorkplaceId id) {
            super(id);
        }
        @Override
        public void use() {
            Thread dyiManiac = Thread.currentThread();
            System.out.println(dyiManiac.getName() + " starts using " + this.getFullName());
            try {
                Thread.sleep(ThreadLocalRandom.current().nextLong(MIN_USE_TIME_IN_MS, MAX_USE_TIME_IN_MS));
            } catch (InterruptedException e) {
                throw new RuntimeException("panic: unexpected thread interruption");
            } finally {
                System.out.println(dyiManiac.getName() + " stops using " + this.getFullName());
            }
        }
        public String getFullName() {
            return ((TroysWorkplaceId)this.getId()).getName();
        }
    }
    
    private static class DiyFan implements Runnable {
        private final Workshop workshop;
        private final List<TroysWorkplaceId> neededWorkplaces;
        private final int numIterations;
        public DiyFan(
                Workshop workshop,
                List<TroysWorkplaceId> neededWorkplaces,
                int numIterations
        ) {
            this.workshop = workshop;
            this.neededWorkplaces = neededWorkplaces;
            this.numIterations = numIterations;
        }
        @Override
        public void run() {
            boolean entered = false;
            String myName = Thread.currentThread().getName();
            for (int i = 0; i < this.numIterations; ++i) {
                for (TroysWorkplaceId wpt : this.neededWorkplaces) {
                    Workplace workplace = null;
                    if (entered) {
                        System.out.println(myName + " tries to switch its workplace to " + wpt.getName());
                        workplace = this.workshop.switchTo(wpt);
                    } else {
                        System.out.println(myName + " tries to enter the workshop and occupy " + wpt.getName());
                        workplace = this.workshop.enter(wpt);
                        entered = true;
                    }
                    System.out.println(myName + " now occupies " + wpt.getName());
                    workplace.use();
                }
                if (entered) {
                    System.out.println(myName + " leaves the workshop");
                    this.workshop.leave();
                    entered = false;
                }
            }
        }
    }
    
    public static void main(String[] args) {
        // Create the workshop.
        TroysWorkplaceId drillingId = new TroysWorkplaceId("the drilling station");
        TroysWorkplaceId machiningId = new TroysWorkplaceId("the machining station");
        TroysWorkplaceId weldingId = new TroysWorkplaceId("the welding station");        
        TroysWorkplaceId quenchingId = new TroysWorkplaceId("the quenching station");
        TroysWorkplaceId paintingId = new TroysWorkplaceId("the painting station");
        Collection<Workplace> workplaces = new ArrayList<Workplace>(4);
        workplaces.add(new TroysWorkplace(drillingId));
        workplaces.add(new TroysWorkplace(machiningId));
        workplaces.add(new TroysWorkplace(weldingId));
        workplaces.add(new TroysWorkplace(quenchingId));
        workplaces.add(new TroysWorkplace(paintingId));
        Workshop workshop = WorkshopFactory.newWorkshop(workplaces);
        // Create the DIY fans.
        Thread alice =
                new Thread(
                        new DiyFan(
                                workshop,
                                Arrays.asList(new TroysWorkplaceId[] {machiningId, drillingId, paintingId}),
                                2
                        ),
                        "Alice"
                );
        Thread bob =
                new Thread(
                        new DiyFan(
                                workshop,
                                Arrays.asList(new TroysWorkplaceId[] {quenchingId, drillingId, paintingId}),
                                1
                        ),
                        "Bob"
                );
        Thread charlie =
                new Thread(
                        new DiyFan(
                                workshop,
                                Arrays.asList(new TroysWorkplaceId[] {drillingId, weldingId, machiningId, paintingId}),
                                3
                        ),
                        "Charlie"
                );
        // Run everything.
        List<Thread> diyFans = Arrays.asList(new Thread[] {alice, bob, charlie});
        for (Thread diyFan : diyFans) {
            diyFan.start();
        }
        for (Thread diyFan : diyFans) {
            try {
                diyFan.join();                
            } catch (InterruptedException e) {
                throw new RuntimeException("panic: unexpected thread interruption");
            }
        }

    }
    
}
