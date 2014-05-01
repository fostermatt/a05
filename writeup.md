
I started by putting all the variables (counters, bools, array for table storage, etc.) I would need into a struct. This way I would be able to make my function prototypes and definitions much more concise, as well as making the function calls much easier to use. The start of my program intializes the variables of my struct and sets the rest based on command line input. The hash table is initialized by inserting zero at each index.

Once intitialized the program will use the fgets/sscanf combo to read integers from the provided data file.  

If the user has selected linear probing then the linear insert function will hash the input value and attempt to place it, if the spot is full (currerntly contains non-zero) then the index is incremented by one and the attempt is made again. Once the attempts have been table-size times this is considered a failure. The function returns true for successful input and false for failure.  

If the user selected quadratic probing then the quadratic insert function will be used. The quadratic insert function starts by setting i to 0 and index to the hashed input. If the index position is already full then index is incremented by i^2 + 1. Each time the new index position is full i is incremented and a new attempt is made. Once i reaches table-size we know that the value cannot be inserted. The insert function returns true if the insert was successful, and false if it failed.  

After each insert the load factor is checked. If the current load is greater than the max load factor (as entered by the user) a rehash is done. A new array is created and each value from the previous table is hashed and inserted into the new table. The old table is the deleted to free the memory.  
The filestream is then rewound and each value is searched for. Finally all relevant data is output.

The biggest problem I had was the amount of variables that needed to be passed between different functions. I solved the issue by creating a struct with all the variables that I would need.  

I went with my hash function because it seemed to spread data pretty well, and I like the number 13. I decided on my linear probing function because it kept good locality of reference. The cluster can be an issue, but the data sets we're working with aren't very large. My quadratic insert function I used the same we had for the last quiz. This way I had a built in data set to test that I knew the correct output.  

After limited testing I have decided that quadratic probing with a max load factor of .9 is best. A higher load factor increases collisions, but since the data sets are relatively small this doesn't have a noticeable impact on runtime.