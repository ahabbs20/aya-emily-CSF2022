Partners:
Emily Berger - eberge11@jh.edu
Aya Habbas - ahabbas1@jh.edu

Contributions: 

For part (b), you'll use the memory traces as well as your simulator to determine which cache configuration has the best overall effectiveness. 
You should take a variety of properties into account: hit rates, miss penalties, total cache size (including overhead), etc. 
In your README.txt, describe in detail what experiments you ran (and why!), what results you got (and how!), and what, in your opinion, 
is the best cache configuration of them all.

Finally, you will write a brief summary of how you divided up the work between partners and what each person contributed. 

--------------------------------------------------------------------------------------------------------------------------------------
||||||||||||||||||||||||||||||||||||||||||                              |                              |                              |
||||||||||||||||||||||||||||||||||||||||||       Direct mapping         |       Fully Associative      |       N-Way Associative      |
||||||||||||||||||||||||||||||||||||||||||                              |                              |                              |
|----------------------------------------+------------------------------+------------------------------+------------------------------|
|                                        |                              |                              |                              |
|    Write allocate, Write back, lru     |                              |                              |                              |
|                                        |                              |                              |                              |
|----------------------------------------+------------------------------+------------------------------+------------------------------|
|                                        |                              |                              |                              |
|   Write allocate, Write through, lru   |                              |                              |                              |
|                                        |                              |                              |                              |
|----------------------------------------+------------------------------+------------------------------+------------------------------|
|                                        |                              |                              |                              |
| No Write allocate, Write through, lru  |                              |                              |                              |                              |
|                                        |                              |                              |                              |
|----------------------------------------+------------------------------+------------------------------+------------------------------|
|                                        |                              |                              |                              |
|    Write allocate, Write back, fifo    |                              |                              |                              |
|                                        |                              |                              |                              |
|----------------------------------------+------------------------------+------------------------------+------------------------------|
|                                        |                              |                              |                              |
|  Write allocate, Write through, fifo   |                              |                              |                              |
|                                        |                              |                              |                              |
|----------------------------------------+------------------------------+------------------------------+------------------------------|
|                                        |                              |                              |                              |
| No Write allocate, Write through, fifo |                              |                              |                              |                              |
|                                        |                              |                              |                              |
--------------------------------------------------------------------------------------------------------------------------------------
