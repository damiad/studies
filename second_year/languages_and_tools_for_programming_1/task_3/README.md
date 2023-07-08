# Moneybag Class

The task is to implement the Moneybag class representing moneybags in the Frankish state. A moneybag stores three types of coins: livres, solidi, and denarii. The number of coins of each type ranges from 0 to 2^64 - 1. The following functionalities should be possible:

- Creating a moneybag based on the number of each type of coin. For example, Moneybag(1, 2, 3) should create a moneybag with 1 livre, 2 solidi, and 3 denarii. However, it should not be possible to create an object without specifying the number of coins.
- Creating a moneybag by copying another moneybag.
- Reading the number of coins for each type using the appropriate methods: livre_number(), solidus_number(), and denarius_number().
- Performing addition and subtraction operations on the contents of moneybags, as well as multiplying the contents of a moneybag by a non-negative integer.
- Outputting a string representation of the moneybag's contents to the output stream.
- Comparing the contents of moneybags using the natural partial order defined on moneybags.
- Using initialized during compilation and constant objects Denarius, Livre, and Solidus, representing individual coins.
- Casting a moneybag to bool, indicating whether the moneybag is non-empty (contains at least one coin).

If an arithmetic operation would result in a value outside the allowable range, an std::out_of_range exception should be thrown with an appropriate message. However, the contents of the moneybag on which the operation is performed should not change.

The Moneybag class should provide the coin_number_t numeric type, representing the number of coins.

Additionally, the task requires implementing the Value class, representing the value of a moneybag in denarii. The following functionalities should be possible:

- Creating a value based on a moneybag or a non-negative integer.
- Copying a value.
- Comparing values using the natural linear order defined on values.
- Comparing values with non-negative integers.
- Casting a value to a string object representing the value in decimal notation.

As many methods and functions as possible should be computable at compile time.

Operations on moneybags should not use types representing numbers with a greater range than necessary to represent the number of coins.

Inheritance should not be used.