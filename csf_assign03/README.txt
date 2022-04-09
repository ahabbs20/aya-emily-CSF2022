Partners:
Emily Berger - eberge11@jh.edu
Aya Habbas - ahabbas1@jh.edu

Contributions: 
    We pair coded everything and wrote the paragraphs together.

Methods:

To conduct our experiment, all our test cases were run on gcc.trace and swim.trace, since they are both very large and contained realistic loads/stores.
From there, we simply found the best combination of the number of sets, number of blocks, and block size for each type of cache (direct 
mapping, fully associative, set associative), essentially dividing 2^20. The hardest part was figuring out what parameters we wanted to minimise, 
as sometimes the lowest miss rate included the highest cycle count. In the end we decided to try and reach a balance between the two, 
though it was somewhat arbitrary, thus, "best size" is used loosely. When the "best" size was found, we then tried all 6 possible combination 
of boolean parameters on both gcc.trace and swim.trace for consistency, noting the Load Miss rate, store miss rate, total miss rate, 
and total cycles. Our results are detailed in the tables below. 

Results:
    Row 1: Load Miss Rate
    Row 2: Store Miss Rate
    Row 3: Miss Rate
    Row 4: Total cycle count

    gcc.trace result table:
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
    
    Swim.trace result table
+----------------------------------------+----------------+-------------------+-------------------+
|               Combination              | Direct Mapping | Fully Associative | M-Way Associative |
|                                        |   262144 1 4   |     1 262144 4    |    1 1024 1024    |
+----------------------------------------+----------------+-------------------+-------------------+
|     Write allocate, write back, lru    |      .0069     |       .0069       |       .0005       |
|                                        |      .2527     |       .2526       |       .0027       |
|                                        |      .0738     |       .0738       |       .0011       |
|                                        |    2,523,519   |     2,517,625     |     8,853,259     |
+----------------------------------------+----------------+-------------------+-------------------+
|   Write allocate, write through, lru   |      .0069     |       .0069       |       .0005       |
|                                        |      .2527     |       .2526       |       .0027       |
|                                        |      .0738     |       .0738       |       .0011       |
|                                        |   10,709,045   |     10,708,448    |     17,023,460    |
+----------------------------------------+----------------+-------------------+-------------------+
|  No write allocate, write through, lru |      .0157     |       .0156       |       .0010       |
|                                        |      .0329     |       .3292       |       .1141       |
|                                        |      .1010     |       .1009       |       .0318       |
|                                        |    8,815,114   |     8,814,817     |     14,053,750    |
+----------------------------------------+----------------+-------------------+-------------------+
|    Write allocate, write back, fifo    |      .0069     |       .0069       |       .0005       |
|                                        |      .2526     |       .2526       |       .0027       |
|                                        |      .0738     |       .0738       |       .0011       |
|                                        |    2,523,519   |     2,517,625     |     8,853,259     |
+----------------------------------------+----------------+-------------------+-------------------+
|   Write allocate, write through, fifo  |      .0069     |       .0068       |       .0005       |
|                                        |      .2527     |       .2526       |       .0027       |
|                                        |      .0738     |       .0738       |       .0011       |
|                                        |   10,709,045   |     10,708,448    |     17,023,460    |
+----------------------------------------+----------------+-------------------+-------------------+
| No write allocate, write through, fifo |      .0157     |       .0156       |       .0001       |
|                                        |      .3293     |       .3292       |       .1141       |
|                                        |      .1010     |       .1001       |       .0318       |
|                                        |    8,815,114   |     8,814,817     |     14,053,750    |
+----------------------------------------+----------------+-------------------+-------------------+


Discussion:

When comparing the results between gcc.trace and swim.trace, we found the results to be consistent across the board (with the caveat that all figures are lower for swim.trace because it is a smaller file). 
From now on we will consider the results overall instead of talking about each file individually since the following result are comparative and therefore true for both files.

Between Direct-Mapping, Fully-Associative, and M-Way Associative caches there are multiple factors to consider when deciding which to use. M-Way associative caches
(as per the above table) leads to a lower miss rate (for loads, stores, and total). Miss rates for Direct Mapping and Fully Associative caches are generally much larger
than the miss rates for M-Way Associative caches (for the gcc.trace file). This is significantly larger and thus is important to keep in mind. Total cycles were found to be optimal in Direct Mapping
or Fully Associative caches (as you can see in the table), with minor differences. Note that when running Fully Associative and Direct mapping caches, we noticed a time delay that is not reflected within 
the cycle count but is indicative of the inherent difference between the two. We saw this especially from the combination of write-allocate write-through. From this point on, we will not note this time 
difference since it is not reflected in the results. Additionally, we found lru and fifo to produce the same results, because they don't determine how frequently the costly operations occur. 
This is why we don't differentiate between them when considering ideal configurations. Additionally, M-way associative cache cycle count is found to be larger than direct mapping or fully associative caches. 
There are a lot of similarities between the results of direct mapping and fully associative, with the M-way associative cache being the one that is significantly different from the others. Clearly, it has a lower 
miss rate and a higher cycle count. We believe this to be the best option because optimizing the miss rate seems to be the most important thing to do, instead of purely optimizing the cycle count. 
M-way caches do this significantly better than the others, and though cycle count is higher we believe the trade-off is worth it. The cycle count difference, while large, when reflected in the time taken 
to run the program, does not lead to an impactful difference in performance for caches of this size. Of course, if the cache were to be significantly larger, or had to endure many more 
loads/stores it would be more necessary to optimise for the time. 

One of the most interesting things to note is that between Direct Mapping and Fully Associative, a given combination of boolean parameters will
produce identical miss rates (load, store, and total), while the cycle count is the only thing that's different, but only marginally. This makes
somewhat sense, of course, as in our simulation tests Direct Mapping and Fully Associative can be considered vectors in two different directions. 
As an aside, our simulation results are not entirely accurate for Direct Mapping and Fully Associative. We do not count the time (cycles) it takes to 
actually iterate through the Fully Associative cache, lending to the cycle counts being incredibly similar. In reality, there is a cost to that search, 
and that cost is the major difference between Direct mapping and Fully associative, one that should be reflected in future simulations of caches. As it 
stands, our results do not fully reflect this inherent difference, and so the "greed" of the fully associative cache is in essence ignored.

With this in mind and as per the table above, we have (somewhat arbitrarily) decided that an M-Way associative with Write-allocate and write-back is an optimal
configuration for this given cache simulation. Across the board, the total cycles for write-back and write-allocate configurations are significantly smaller than 
their counterparts. We expected this since write-back makes stores less costly until eviction. Additionally, the M-way associative caches have significantly smaller
miss rates than any other type of cache. Combined, that means that the less costly write-back operation will occur much more frequently. As noted, the larger cycle count
is not so large as to be very detrimental to the performance of the cache, at such point that it is, we expect it to be a worthwhile tradeoff. 