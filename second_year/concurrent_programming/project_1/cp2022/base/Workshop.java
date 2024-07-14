/*
 * University of Warsaw
 * Concurrent Programming Course 2022/2023
 * Java Assignment
 */
package cp2022.base;


public interface Workshop {

    public Workplace enter(WorkplaceId wid);
    
    public Workplace switchTo(WorkplaceId wid);
    
    public void leave();
    
}
