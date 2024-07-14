/*
 * University of Warsaw
 * Concurrent Programming Course 2022/2023
 * Java Assignment
 */
package cp2022.base;


public abstract class Workplace {
    
    private final WorkplaceId id;
    
    protected Workplace(WorkplaceId id) {
        this.id = id;
    }
    
    public final WorkplaceId getId() {
        return this.id;
    }
    
    public abstract void use();
}
