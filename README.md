# Bitcoin-Mining-Simulation-Multithread
_A Multithreaded Program For A Bitcoin Mining Simulation._ 

**Bitcoin** is a digital currency which has gained popularity in the last five years. To mine Bitcoins, _miners_ try to solve hard mathematical problems by trying random solutions and the first one who finds the solution earns some bitcoins. 
## How It Works
* Each thread will be a **_miner_**.  
* Miners will use a `Hash function` to calculate a hard problem. Hash functions are known to be one way functions such
 that given a hash function `f(x)`, its computationally feasible to compute `f(x)` but it’s infeasible to find  `f^-1(x)`.
 * The _hash function_ (given in the `source.cpp` file) takes a transaction and calculates the **_hash_** value for that transaction. 
 
 * If this hash value is less than a threshold, i.e., its first n bits are `0`, it will be accepted as a solution. 
 * If a miner finds a desired solution, it gains a **bitcoin**. Moreover, as mentioned above, the winner miner will append the corresponding transaction to the chain of transactions, which will be in a **_linked-list_** form.

## Program Flow
* At the beginning, user will be prompted 3 inputs:
    * Difficulty of mining: between 1-10 (this will decide how hard is the mining problem). 
    * File name from which the transaction IDs are going to be read
    * Number of miners (number of threads created) 
* Miner threads are created of desired amount. After creating all the threads, main thread sets a flag to let miners start. 
* Miners first calculates the threshold. Threshold will be an unsigned integer for which first n left-most bits are 0 where n is the difficulty.
    * For example, if the difficulty is 4, the threshold will be 0000 1000 0000 0000 0000 0000 0000 0000. Hence if the difficulty increases, it will be harder to find a smaller number less than the threshold. 
* If the hash is less than the _**threshold**_, corresponding thread will add that transaction to the chain. The thread finding the solution will increment its private bitcoin count by one. Also, it will increment the shared integer to notify other threads that a solution is found. 
* Format of the _**input text:**_
    * The first line of the file will be the number of entries in the file. 
    * The rest i.e. transaction ID's are put into an array of unsigned int which will be shared among threads by the main thread.   
        
## The Folders and Files

`source.cpp:` File contains the implementation of the Bitcoin Mining Simulation.  
`input.txt:` An sample of transaction ID sample text file. 

## Examples

![Example](https://doc-04-28-docs.googleusercontent.com/docs/securesc/ha0ro937gcuc7l7deffksulhg5h7mbp1/me0fo23blpjd1b4ka35lvkb91659q93f/1542232800000/03612584247773642441/*/14oyTk4MakuXzYlgjNKf5C3pnXBsXn6rk)
