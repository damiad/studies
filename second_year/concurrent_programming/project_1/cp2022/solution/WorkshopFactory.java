/*
 * University of Warsaw
 * Concurrent Programming Course 2022/2023
 * Java Assignment
 *
 * Author: Konrad Iwanicki (iwanicki@mimuw.edu.pl)
 */
package cp2022.solution;

import java.util.*;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Semaphore;

import cp2022.base.Workplace;
import cp2022.base.WorkplaceId;
import cp2022.base.Workshop;


public final class WorkshopFactory {

    public final static class WorkshopI implements Workshop{

        private final LinkedList<Long> threadsBeforeEntering = new LinkedList<>(); //lista watkow PRZED wejsciem do warsztatu
        private final TreeMap<Long, HashSet<Long>> mapCounterThreads = new TreeMap<>(); //warunek 2N, watki
        private final HashMap<Long, Long> mapThreadCounter= new HashMap<>(); //pomocnicze, zeby szybko usuwac z mapCounterThreads
        private long counterTotal = 0; //ile watkow sumarycznie zrobilo enter
        private long counterInside = 0; //ile watkow moze zakonczyc enter (warunek 2N)
        private final long twiceN; //ilosc stanowisk
        private final Semaphore mutex = new Semaphore(1, true);
        private final HashMap<WorkplaceId, FakeWorkplace> fakeWorkplaces; //wrapper na Workplace
        private final HashMap<WorkplaceId, HashSet<Long>> queueToWorkshop; //kolejka osob do warsztatu
        private final HashMap<WorkplaceId, WorkplaceId> cycleFromTo = new HashMap<>(); //mapa cykli skad dokad
        private final HashMap<WorkplaceId, Boolean> isWorkplaceFree = new HashMap<>(); //czy warsztaty zajete
        private final HashMap<Long, Semaphore> privateThreadSemaphore = new HashMap<>(); //kazdy watek ma swoj wlasny semaphore (binarny)
        private final HashMap<Long, WorkplaceId> occupiedWorkshop = new HashMap<>(); //warsztat zajmowany obecnie przez watek
        private final HashMap<WorkplaceId, Long> threadInWorkshop = new HashMap<>(); //watek, ktory czeka w danym warsztacie nim przejdzie do innego
        private final Set<Long> wokeFromCycle = new HashSet<>(); //zawiera Id thread-ow co sa w cyklach

        WorkshopI(Collection<Workplace> workplaces){
            this.queueToWorkshop = new HashMap<>();
            this.fakeWorkplaces = new HashMap<>();
            this.twiceN = 2L * workplaces.size(); //cast na long
            for (Workplace workplace : workplaces) {
                this.fakeWorkplaces.put(workplace.getId(), new FakeWorkplace(workplace.getId(), workplace));
                this.queueToWorkshop.put(workplace.getId(), new HashSet<>());
                this.isWorkplaceFree.put(workplace.getId(), true);
            }
        }

        //wpuszcza do warsztatu lub oznacza jako wolny
        private void freeWorkplace(WorkplaceId prevWorkplaceId){ //mamy mutex
            if(!queueToWorkshop.get(prevWorkplaceId).isEmpty()){ //wpuszczamy dowolnego
                for(Long thread : queueToWorkshop.get((prevWorkplaceId))){
                    queueToWorkshop.get(prevWorkplaceId).remove(thread); //usuwamy czekajacego z kolejki
                    privateThreadSemaphore.remove(thread).release(); //usuwamy i zwalniamy semafor
                    break; //tylko z 1 elementem robimy
                }
            }else{
                isWorkplaceFree.put(prevWorkplaceId, true); //zwalniamy stanowisko
            }
        }

        //dodaje do struktur zapewniajacych 2N
        private void addToMapsCounterThreads(){ //mamy mutex
            long counterAwait = counterTotal + twiceN - 1; //maksymalny licznik counterInside, zanim wykonamy enter lub switch
            if(mapCounterThreads.containsKey(counterAwait)){
                mapCounterThreads.get(counterAwait).add(Thread.currentThread().getId());
            }else{
                mapCounterThreads.put(counterAwait, new HashSet<>());
                mapCounterThreads.get(counterAwait).add(Thread.currentThread().getId());
            }
            mapThreadCounter.put(Thread.currentThread().getId(), counterAwait);
        }

        //blokuje watek, do momentu az ktos go obudzi
        private void freezeThread(){
            try{ //Czekamy na stanowisko lub wpuszczenie do warsztatu
                Semaphore semaphore = new Semaphore(0);
                privateThreadSemaphore.put(Thread.currentThread().getId(), semaphore);
                mutex.release();
                semaphore.acquire(); //czekamy (ktos moze nam wczesniej zwolnic semafor, nie szkodzi)
                mutex.acquire();
            }catch (InterruptedException e) {
                throw new RuntimeException("panic: unexpected thread interruption");
            }
        }

        private void completeEntering(WorkplaceId wid){
            threadInWorkshop.put(wid, Thread.currentThread().getId());
            occupiedWorkshop.put(Thread.currentThread().getId(), wid);
            if(mapThreadCounter.containsKey(Thread.currentThread().getId())){
                long counterThread = mapThreadCounter.remove(Thread.currentThread().getId());
                mapCounterThreads.get(counterThread).remove(Thread.currentThread().getId());
                if(mapCounterThreads.get(counterThread).isEmpty()){
                    mapCounterThreads.remove(counterThread);
                }
            }
            while(!threadsBeforeEntering.isEmpty() && mapCounterThreads.firstKey()  > counterInside) {
                counterInside++;
                privateThreadSemaphore.remove(threadsBeforeEntering.removeFirst()).release(); //usuwamy i wypuszczamy
            }
            mutex.release();
        }

        public Workplace enter(WorkplaceId wid){
            try {
                mutex.acquire();
                counterTotal++;
                addToMapsCounterThreads(); //dodajemy sie do kolejki 2N
                if (!mapCounterThreads.isEmpty() && mapCounterThreads.firstKey()  == counterInside) { //to nie mozemy wejsc, zywotnosc
                        threadsBeforeEntering.addLast(Thread.currentThread().getId()); //czekamy na liscie przed warsztatem
                        freezeThread(); //czekamy na mozliwosc wejscia do jakiegokolwiek warsztatu
                }else{
                    counterInside++; //jak czekalismy, to wpuszczajacy zwiekszy counterInside za nas
                }
                if(isWorkplaceFree.get(wid)){
                    isWorkplaceFree.put(wid, false); //zajmujemy Workshop, reszta wspolna
                }else{
                    queueToWorkshop.get(wid).add(Thread.currentThread().getId()); //dodajemy sie do kolejki i czekamy na odziedziczenie warsztatu!
                    freezeThread(); //czekamy na wolny warsztat
                    queueToWorkshop.get(wid).remove(Thread.currentThread().getId());
                }
                completeEntering(wid);
            }catch (InterruptedException e) {
                throw new RuntimeException("panic: unexpected thread interruption");
            }
            return fakeWorkplaces.get(wid);
        }

        public Workplace switchTo(WorkplaceId wid){
            try{
                mutex.acquire();
                if(wid == occupiedWorkshop.get(Thread.currentThread().getId())){
                    mutex.release();
                    return fakeWorkplaces.get(wid); //zostajemy ok
                }

                WorkplaceId prevWorkplaceId= occupiedWorkshop.get(Thread.currentThread().getId()); //na koniec nadpiszemy
                CountDownLatch protection = new CountDownLatch(1); //tworzymy lock na nasz workshop
                fakeWorkplaces.get(prevWorkplaceId).entryLatch(protection); //i chronimy go przed use

                if(isWorkplaceFree.get(wid)){
                    isWorkplaceFree.put(wid, false); //zajmujemy Workshop
                    freeWorkplace(prevWorkplaceId); //zwalniamy obecne stanowisko
                }else{ //zajete docelowe stanowisko
                    cycleFromTo.put(prevWorkplaceId, wid); //wstawiamy sie do cyklu
                    WorkplaceId end=wid; //sprawdzamy cykl
                    do{
                        if(! cycleFromTo.containsKey(end)) break; //nie ma klucza, nie ma cyklu
                        end = cycleFromTo.get(end); //usuwa i zwraca kolejne WorkplaceId z cyklu
                    }while(end!=prevWorkplaceId);

                    if(end==prevWorkplaceId){ //cykl istnieje
                        end=wid;
                        while(end!=prevWorkplaceId){
                            Long threadID= threadInWorkshop.remove(end); //dowiadujemy sie kto czekal w danym warsztacie
                            wokeFromCycle.add(threadID);  //mowimy, ze obudzil sie w cyklu
                            privateThreadSemaphore.remove(threadID).release(); //zwalnia semaphore konkretnego watku
                            end = cycleFromTo.remove(end); //usuwa i zwraca kolejne WorkplaceId z cyklu
                        }
                        cycleFromTo.remove(prevWorkplaceId); //rowniez siebie usuwamy z cyklu
                    }else{ //nie ma cyklu i nie mozemy wejsc
                        queueToWorkshop.get(wid).add(Thread.currentThread().getId());  //wstawiamy sie do kolejki
                        addToMapsCounterThreads();
                        freezeThread();
                        queueToWorkshop.get(wid).remove(Thread.currentThread().getId()); // mozemy sami, bo nikt nie wejdzie na to stanowisko
                        if(!wokeFromCycle.contains(Thread.currentThread().getId())){
                            cycleFromTo.remove(prevWorkplaceId); //usuwamy siebie z cycleFromTo
                            freeWorkplace(prevWorkplaceId);
                        }else{ //zarzadca cyklu wyrzucil nasz z cycleFromTo
                            wokeFromCycle.remove(Thread.currentThread().getId()); //nie zwalniamy naszego, bo juz obudzony odpowiedni watek
                        }
                        privateThreadSemaphore.remove(Thread.currentThread().getId()); //nie potrzebujemy czekac na semaforze
                    }
                }//mamy otwarty mutex
                fakeWorkplaces.get(wid).previousFreeLatch(protection); //damy znac jak na obecnym da sie pracowac
                completeEntering(wid); //nas na prevStanowisko ktos nadpisze, nie ma problemu

            }catch (InterruptedException e) {
                throw new RuntimeException("panic: unexpected thread interruption");
            }
            return fakeWorkplaces.get(wid);
        }

        public void leave(){
            try{
                mutex.acquire();
                WorkplaceId prevWorkplace = occupiedWorkshop.remove(Thread.currentThread().getId());
                freeWorkplace(prevWorkplace); //wpuszcza z kolejki albo sam ustawia ja na true
                fakeWorkplaces.get(prevWorkplace).entryLatch(new CountDownLatch(0)); //ustawmy tam latch na 0
                occupiedWorkshop.remove(Thread.currentThread().getId());
                mutex.release();
            }catch (InterruptedException e) {
                throw new RuntimeException("panic: unexpected thread interruption");
            }
        }
    }

    public final static Workshop newWorkshop(
            Collection<Workplace> workplaces
    ) {
        return new WorkshopI(workplaces);
    }

}