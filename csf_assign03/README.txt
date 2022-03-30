Partners:
Emily Berger - eberge11@jh.edu
Aya Habbas - ahabbas1@jh.edu

Contributions: 

For part (b), you'll use the memory traces as well as your simulator to determine which cache configuration has the best overall effectiveness. 
You should take a variety of properties into account: hit rates, miss penalties, total cache size (including overhead), etc. 
In your README.txt, describe in detail what experiments you ran (and why!), what results you got (and how!), and what, in your opinion, 
is the best cache configuration of them all.

To conduct our experiment, all our test cases were run on gcc.trace, since it was both very large and contained realistic loads/stores.
From there, we simply found the best combination of the number of sets, number of blocks, and block size. We minimised the number of clock cycles
for each combination and inputted them in the table below. Our table contains the following statistics: Load Hits/Loads, Store Hits/Stores,
 Total Cycle, as well as the type of 

Our 

Load Miss Rate
Store Miss Rate
Miss Rate
Total cycles 



+----------------------------------------+----------------+-------------------+-------------------+
|               Combination              | Direct Mapping | Fully Associative | M-Way Associative |
|                                        |   262144 1 4   |     1 262144 4    |    1 1024 1024    |
+----------------------------------------+----------------+-------------------+-------------------+
|     Write allocate, write back, lru    |      .0139     |       .0139       |       0.0008      |
|                                        |      .1385     |       .1385       |       0.0013      |
|                                        |      .0616     |       .0616       |       0.0010      |
|                                        |    3,682,168   |     3,662,596     |     13,878,361    |
+----------------------------------------+----------------+-------------------+-------------------+
|   Write allocate, write through, lru   |      .0139     |       .0139       |       0.0008      |
|                                        |      .1385     |       .1385       |       0.0013      |
|                                        |      .0616     |       .0616       |       0.0010      |
|                                        |   23,243,847   |     23,241,066    |     33,429,731    |
+----------------------------------------+----------------+-------------------+-------------------+
|  No write allocate, write through, lru |      .0297     |       .0297       |       0.0014      |
|                                        |      .1897     |       .1897       |       0.0505      |
|                                        |      .0909     |       .0909       |       0.0202      |
|                                        |   21,004,228   |     21,002,743    |     31,304,758    |
+----------------------------------------+----------------+-------------------+-------------------+
|    Write allocate, write back, fifo    |      .0139     |       .0139       |       0.0008      |
|                                        |      .1385     |       .1385       |       0.0013      |
|                                        |      .0616     |       .0616       |       0.0010      |
|                                        |    3,682,168   |     3,662,596     |     13,878,361    |
+----------------------------------------+----------------+-------------------+-------------------+
|   Write allocate, write through, fifo  |      .0139     |       .0139       |       0.0008      |
|                                        |      .1385     |       .1385       |       0.0013      |
|                                        |      .0616     |       .0616       |       0.0010      |
|                                        |   23,243,847   |     23,241,066    |     33,429,731    |
+----------------------------------------+----------------+-------------------+-------------------+
| No write allocate, write through, fifo |      .0297     |       .0297       |       0.0014      |
|                                        |      .1897     |       .1897       |       0.0505      |
|                                        |      .0909     |       .0909       |       0.0202      |
|                                        |   21,004,228   |     21,002,743    |     31,304,758    |
+----------------------------------------+----------------+-------------------+-------------------+