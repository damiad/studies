# Concurrent Programming 2022/2023
Java Concurrent Programming Assignment

## Description
Sharing economy is a socio-economic trend based on sharing potentially expensive resources with other market participants, allowing for the cost amortization of such resources. One example of this trend is workshops equipped with high-quality specialized workstations. The devices used to create such workstations are often financially out of reach for an average individual. However, assuming that a single individual doesn't need them on a daily basis but only occasionally, sharing these resources becomes economically justified. For instance, multiple individuals with similar needs can share a workshop, which enables access to the specialized workstations. 

Your task is to solve the problem of concurrent coordination of users' access to workstations in such a workshop using the Java programming language. The assignment is for the course "Concurrent Programming" for the academic year 2022/2023. You need to implement the solution in Java according to the specified requirements and using the provided template.

## Specification
In the provided model, a workshop consists of a collection of workstations, each represented by an object of a class that inherits from the `Workplace` class in the provided template. Each workstation has a unique identifier within the workshop, accessible through the `getId` method of the `Workplace` class, and can be used by any user through the `use` method of the `Workplace` class.

Each user of the workshop is represented by a Java thread and has a unique identifier accessible through the `getId` method of the `Thread` class. The set of users wanting to use the workshop can be of arbitrary size and can change dynamically.

The workshop, represented by an object implementing the `Workshop` interface, is responsible for coordinating users' access to the workstations according to the following rules:

### Workshop Interface
```java
public interface Workshop {

    public Workplace enter(WorkplaceId wid);
    
    public Workplace switchTo(WorkplaceId wid);
    
    public void leave();
    
}
```

### Workshop Behavior for a Single User
When a user enters the workshop, they occupy a specific workstation by calling the `enter` method of the `Workshop` interface, which returns an object representing the requested workstation. After entering, the user can start working at the workstation by calling the `use` method on the corresponding object. The user can then either leave the workshop by calling the `leave` method or switch to another workstation by calling the `switchTo` method, which returns an object representing the requested workstation. After switching, the user can start working at the new workstation using the `use` method. Before leaving the workshop, a user can switch workstations any number of times. They can even return to the same workstation immediately after finishing work there. Additionally, a user can appear in the workshop multiple times.

The following terminology is adopted for further explanation:

- A user enters the workshop when the corresponding thread finishes executing the `enter` method and receives an object representing the first requested workstation.
- A user leaves the workshop when the corresponding thread starts executing the `leave` method.
- Between entering and leaving the workshop, the user is present in the workshop.
- A user who has started but not finished the `enter` or `switchTo` calls wants to enter the workshop.
- A user switches workstations when the corresponding thread finishes executing the `switchTo` method and receives an object representing the next requested workstation.
- Between finishing an `enter` or `switchTo` call for a given workstation and starting to execute `leave` or another `switchTo` for another workstation, the user occupies the given workstation.
- During this period, when the `use` method is called on the object representing the workstation, the user starts working there, and they finish work when the method call ends.
- Finally, a user who has started but not finished the `switchTo` call wants to switch workstations.

### Coordination of Users
The workshop coordinates the access of different users to the workstations according to the following rules:

- For safety reasons, a user cannot work at a workstation if another user occupies it, regardless of whether the other user is currently working or not. Your solution must strictly comply with this rule.
- For economic reasons, optimal utilization of workstations is essential. However, it should not lead to starvation of any user. The primary goal is to maximize the usage of workstations, but your solution must guarantee that a user who wants to enter the workshop or switch workstations will do so before 2*N other users who started wanting to enter or switch workstations after them, where N is the number of workstations in the workshop. This requirement is for testing purposes.

## Requirements
Your task is to implement the workshop according to the specified requirements and using the provided template, using the concurrency mechanisms available in Java 11. The code should follow good programming practices. Solutions based on active or semi-active waiting (e.g., using sleep, yield, or other time-based constraints) will not receive any points.

Additional requirements for the assignment are as follows:

- You cannot modify the contents of the `cp2022.base` and `cp2022.demo` packages.
- You can only add classes implementing the solution in the `cp2022.solution` package. You cannot create any sub-packages in this package.
- Your implementation must not create any threads.
- In the `cp2022.solution.WorkshopFactory` class, you need to add the implementation of the `newWorkshop` method. This method will be used to instantiate your implemented workshop. Each call to this method should create a new workshop object. Multiple workshop objects should be able to operate simultaneously. However, you cannot modify the signature of this method, the class name, or its location.
- Invoking the `enter` or `switchTo` methods of your implementation of the `Workshop` interface can return either the original workstation object with the requested identifier or an object of another class inheriting from the `Workplace` class. However, in the latter case, every call to the `use` method must eventually lead to exactly one call to the `use` method on the original workstation object. In other words, the returned workstation object can at most be a decorator of the original object, and safety guarantees apply to the original object.
- You can create your own packages for testing purposes, e.g., `cp2022.tests`, but these packages will be ignored during testing by us. Therefore, your workshop code cannot depend on them.
- In Java source files, you cannot use non-English characters (especially Polish characters).
- Your solution should be submitted as a single file named `ab123456.zip`, where `ab123456` should be replaced with your login from the students.mimuw.edu.pl machine. The file structure should be the same as in the template, with a `cp2022` directory containing the relevant subdirectories (`base`, `demo`, and `solution`), which in turn contain the respective source files (`*.java`).
- Your solution should compile on the students.mimuw.edu.pl machine using the command `javac cp2022/base/*.java cp2022/solution/*.java cp2022/demo/*.java`.
- Your solution should work with the provided demo program, which can be run using the command `java cp2022.demo.TroysWorkshop` without raising any exceptions.