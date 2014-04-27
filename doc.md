###Assignment 5 - Documentation###
#####What the program does#####  
This program will read integers from a data file and attempt to insert them into a hash table, resizing the table as needed. Once the integers have all been inserted the table will be searched for all integers in the data file. The table will then be displayed on screen as well as accompanying data points (final table size, insertion failures, etc.)

#####Limitations#####   
This program will only accept integers; floats and doubles may not be used. Negative integers should sort properly but '0' cannot be sorted. The amount of integers that can be sorted is only limited by system memory as the program can resize the table as needed. 

#####Operation#####  
Command line operation execution as follows:  

	$ ./<executable.name> <data.file>  <probing option> <load factor>

*data file* - must have non-zero integers seperated by white space.  
*probing option* - enter '0' for linear probing and '1' for quadratic probing.  
*load factor* - enter floating point value between 0 and 1.  

#####Output#####  
Upon completion of execution these items will be output; full hash table, integers read, max load factor, table size, number of elements in the table, final load factor, insertions performed, insertion collisions, insertion failures, integers to find, number found, search collisions, and search failures.