Assignment 3 : Accomplish a record manager of a database.


1.Implementation:
1. Open Terminal and go to the directory containing the code.
2. Run ‘make clean’ to clean the compiled,executable files and log files if any.


dhcp54:Final1 siddhant$ make clean
rm -f test_assign1_1 test_assign1_2 test_assign2_1 test_assign2_2 test_assign3_1 test_assign3_2 test_assign3_3 test_expr *.o *.log


1. Run ‘make’ command to compile the code.


dhcp54:Final1 siddhant$ make
gcc -w -c storage_mgr.c
gcc -w -c dberror.c
gcc -w -c test_assign1_1.c
gcc -w -o test_assign1_1 storage_mgr.o dberror.o test_assign1_1.o
gcc -w -c test_assign1_2.c
gcc -w -o test_assign1_2 storage_mgr.o dberror.o test_assign1_2.o
gcc -w -c buffer_mgr_stat.c
gcc -w -c buffer_mgr.c
gcc -w -c test_assign2_1.c
gcc -w -o test_assign2_1 storage_mgr.o buffer_mgr_stat.o buffer_mgr.o dberror.o test_assign2_1.o -lpthread -std=gnu99 -I.
gcc -w -c test_assign2_2.c
gcc -w -o test_assign2_2 storage_mgr.o buffer_mgr_stat.o buffer_mgr.o dberror.o test_assign2_2.o -lpthread -std=gnu99 -I.
gcc -w -c expr.c
gcc -w -c record_mgr.c
gcc -w -c rm_serializer.c
gcc -w -c test_assign3_1.c
gcc -w -o test_assign3_1 buffer_mgr.o buffer_mgr_stat.o dberror.o expr.o record_mgr.o rm_serializer.o storage_mgr.o test_assign3_1.o -lpthread -std=gnu99 -I.
gcc -w -c test_assign3_2.c
gcc -w -o test_assign3_2 buffer_mgr.o buffer_mgr_stat.o dberror.o expr.o record_mgr.o rm_serializer.o storage_mgr.o test_assign3_2.o -lpthread -std=gnu99 -I.
gcc -w -c test_assign3_3.c
gcc -w -o test_assign3_3 buffer_mgr.o buffer_mgr_stat.o dberror.o expr.o record_mgr.o rm_serializer.o storage_mgr.o test_assign3_3.o -lpthread -std=gnu99 -I.
gcc -w -c test_expr.c
gcc -w -o test_expr buffer_mgr.o buffer_mgr_stat.o dberror.o expr.o record_mgr.o rm_serializer.o storage_mgr.o test_expr.o -lpthread -std=gnu99 -I.
dhcp54:Final1 siddhant$ 




                
                
1. Run test_expr using ‘./test_expr’.
2. Run test_assign3_1 using ‘./test_assign3_1’.
3. Run test_assign3_2 using ‘./test_assign3_2’.
4. Run test_assign3_3 using ‘./test_assign3_3’.
5. If you want to re-run the executable files start from Step 3.
6. If you want to re-compile the executable file start from Step 2.






2.Function Description:


        Table and Manager functions:
1. initRecordManager(): Initializes the record manager with an entry to the buffer manager.
2. shutdownRecordManager(): Shutdown the record manager.
3. createTable(): Creates table file with the name and schema specified, if it doesn’t exist initialize the table information page and create the first reserved page.
4. initTableInfoPage(): Initialize the table information page.
5. openTable(): Opens the table with the given name.
6. closeTable(): Closes the table with the given name.
7. deleteTable(): Deletes the table with the given name.
8. getNumTuples(): Returns the total number of records or tuples stored in the table.


        Handling records in table function:


1. insertRecord(): Insert a record in the table.
2. deleteRecord(): Delete a record from the table.
3. updateRecord(): Update a record of the table.
4. getRecord(): Retrieve a record from the table specified and RID specified.If not found return an error code.


        Scanning functions:


1. startScan(): For a given table manager and expression initializes a new scan manager.
2. next(): Obtain the next valid record in the table.
3. closeScan(): Close the scan manager.


        Dealing with schemas:


1. getRecordSize(): Get the size of each record for a given schema.
2. createSchema(): Creates a new schema with the given values.
3. freeSchema(): Unset a schema.


        Dealing with records and attribute values:


1. createRecord(): Creates a record which is empty for the schema specified.
2. freeRecord(): Removes a record.
3. getAttr(): Returns the value of the attrNumth attribute of the give record of the given schema and stores it at the first spot in an array of values.
4. setAttr(): Updates the value of the attrNumth attribute for a given record to value specified.


        The above functions are given in the record_mgr.h file. Additional functions are also implemented.


3. Additional Notes:


1. The log files for the three test cases have created which contains the output for the three test cases. The file names being test_assign3_1.log, test_assign3_2.log and test_assign3_3.log. 
2. Test_assign3_2 takes few seconds to run.
3. Memory leak has been checked using valgrind. There is zero memory leak. The output of memory leak is attached below.


                ==7994== LEAK SUMMARY:
==7994==    definitely lost: 0 bytes in 0 blocks
==7994==    indirectly lost: 0 bytes in 0 blocks
==7994==      possibly lost: 0 bytes in 0 blocks