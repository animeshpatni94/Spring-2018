Problem Statement


The goal of this assignment is to implement a buffer manager. The buffer manager manages a fixed number of pages in memory that represent pages from a page file managed by the storage manager implemented in assignment 1. The memory pages managed by the buffer manager are called page frames or frames for short. We call the combination of a page file and the page frames storing pages from that file a Buffer Pool. The Buffer manager should be able to handle more than one open buffer pool at the same time. However, there can only be one buffer pool for each page file. Each buffer pool uses one page replacement strategy that is determined when the buffer pool is initialized.


How to run the script


Step 1:    Go to folder containing the code.


Step 2:    Run “make clean” to clean the compiled files, executable file and log 
                files if there is any.


     $ make clean
        rm -f test_assign1_1 test_assign1_2 test_assign2_1 test_assign2_2 *.o *.log


Step 3:    Run “make” command to compile the code.


     $ make
      gcc -c dberror.c
      gcc -c storage_mgr.c
      gcc -c test_assign1_1.c
      gcc -o test_assign1_1 dberror.o storage_mgr.o test_assign1_1.o
      gcc -c test_assign1_2.c
      gcc -o test_assign1_2 dberror.o storage_mgr.o test_assign1_2.o
      gcc -c buffer_mgr_stat.c
      gcc -c buffer_mgr.c
      gcc -c test_assign2_1.c
      gcc -o test_assign2_1 dberror.o storage_mgr.o buffer_mgr_stat.o 
      buffer_mgr.o test_assign2_1.o -lpthread
      gcc -c test_assign2_2.c
      gcc -o test_assign2_2 dberror.o storage_mgr.o buffer_mgr_stat.o
      buffer_mgr.o test_assign2_2.o -lpthread


Step 4:    Run test_assign2_1


     $ ./test_assign2_1


Step 5:   View the result of the test_assign2_1


Step 6:    Run test_assign2_2


     $ ./test_assign2_2


Step 7:   View the result of the test_assign2_2. We also included test cases for 
             storage manager.


Step 8:   If you want to re-run the executable files start from step 4. If you want to
               Re-compile the files, start from step 1.




Function Description


1. findPage:                search the requested page in the Buffer Pool, if found, load the requested page into the BM_pageHandle and return RC_OK else, return error code.
2. extendPage:         Read the requested page from the disk and append it to the tail of the PageList.
3. substitutePage:         Replace the current page with the requested page read from the disk.
4. initPrevious:          Initialize the PageList to store pages in the Buffer Pool
5. initBufferPool:        This function takes a BM_BufferPool instance as an argument and initializes its attributes based on other arguments. It first validates the arguments provided. If any of them is invalid, it returns an appropriate error message. It then initializes the attributes of BM_BufferPool instance. Finally it initializes the frame list with empty frames.
6. shutdownBufferPool: Flush all dirty pages in the buffer pool back to disk. If this operation takes places successfully, then all pages in the buffer pool looks clean. Now we need to free allocated memory blocks inside out. 
7. forceFlushPool: This function writes all the dirty pages to the disk.
8. FIFO:  First In First Out is the most basic page replacement strategy used. FIFO is generally like a queue where the page which comes in first in the buffer pool is in front and that page will be replaced first if the buffer pool is full. Once the page is located, we write the content of the page frame to the page file on disk and then the new page is added to that location.
9. LRU:  Least Recently Used removes the page frame which hasn’t been used for a long time amongst the other page frames in the buffer pool. 
10.  CLOCK:  Clock algorithm  keeps a track of the last added page frame in the buffer pool. Also, we use a clock pointer which is a counter to point the page frames in the buffer pool.
11.  pinPage:  This function reads the page from the page file present on the disk and stores it on the buffer pool. Before storing the page in the buffer pool it checks if the pool has an empty space or not. If the pool is full then a page replacement strategy has to be used to replace a page. Page replacement algorithms are employed to determine which page is to be replaced. That respective page is checked if it is dirty. In case it’s dirty, then the contents of the page frame is written to the page file on the disk.
12.  unpinPage: This function unpins the page using the pageNum. It locates the page, then decrements the fixCount by one which means that the client is no longer using this page.
13.  markDirty:  This function finds the page which needs to be marked dirty. If the page is found then set the dirty bit of the page as True.
14.  forcePage:  This function finds the page which needs to be forced to the disk. If the page is found, it opens the file and write the current content of the page back to the page file on the disk. If page is found and write operation is successful then the process has executed successfully else returns an error message.
15.  getFrameContents: This function returns an array of PageNumbers. The array size is the buffer size,i.e, number of pages. We iterate over all the page frames in the buffer pool to get the pageNum value of the page frames present in the buffer pool.
16.  getDirtyFlags: This function returns an array of bools. We iterate over all the page frames in the buffer pool to get the dirty bit value of the page frames present in the buffer pool.
17.  getFixCounts: This function returns an array of integers. We iterate over all the page frames in the buffer pool to get the fix count value of the page frames present in the buffer pool.
18.  getNumReadIO: This function returns the count of the total number of IO reads performed by the buffer pool, the number of pages read from the disk.
19.  getNumWriteIO:  This function returns the count of the total number of IO writes performed by the buffer pool, the number of pages written to the disk.


Memory Leak


Checked memory leak using valgrind.


Output:


==2286== LEAK SUMMARY:
==2286==        definitely lost: 0 bytes in 0 blocks
==2286==        indirectly lost: 0 bytes in 0 blocks
==2286==          possibly lost: 0 bytes in 0 blocks


Additional Test Cases


We have additional test cases in the file test_assign2_2.c . This file checks for the page replacement strategy namely CLOCK.