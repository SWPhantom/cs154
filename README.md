##CS154- Computer Architecture
* Project 1: Single-cycle CPU simulation
  * Description: https://www.cs.ucsb.edu/~franklin/154/projects/Project1Simulator.html
* Project 2: Pipelining
  * Description: https://www.cs.ucsb.edu/~franklin/154/projects/Project2/
* Project 3: Caches
  * Description: https://www.cs.ucsb.edu/~franklin/154/projects/Project3/
* Project 4: Branch Prediction
 * Description: https://www.cs.ucsb.edu/~franklin/154/projects/Project4/

All of these projects need to be run with an input in this format:

    -1073086449
    -1073020903
    -2056626152
    -2056624088
    -2052427736
    -2052360152
    -2058649590
    -2056615926
    -2058711030
These integers represent assembly instructions:

    addi $10, $0, 15
    addi $11, $0, 25
    sub $12, $11, $10
    and $13, $11, $10
    and $14, $13, $10
    and $15, $13, $11
    sgt $16, $10, $11
    sgt $17, $11, $10
    sgt $18, $10, $10
