GROUP-15
=========================
Animesh Patni
Geethanjali Vivekanandan
Junrui Gong
Siddhant Priyadarshi

Assignment Description:
========================
Implement a simple storage manager - a module that is capable of reading blocks from a file on disk into memory and writing blocks from memory to a file on disk.

Project Files:
=========================
C source files:storage_mgr.c,dberror.c,test_assign1_1.c

Header files:storage_mgr.h,dberror.h,test_helper.h 

RUNNING THE SCRIPT
=========================
1) All the files in the given folder need to be compiled.

2) Run storage_mgr.c and test_assign1_1.c 


SOLUTION DESCRIPTION
===========================

1. MANIPULATING PAGE FILES
===========================

This module is to manage the page files so as to facilitate the Read and Write Functions.

createPageFile(...)
--> This function creates a new page file filename.
--> The parameter pageFile creates an empty file for both reading and writing
--> Create one page with 4096 bytes size.

openPageFile(...)
--> This function is to open the file.
--> Contents of SM_FileHandle are assigned, FILE pointer is stored in mgmtInfo.
--> Check for the file, if not present return, "RC_FILE_NOT_FOUND".
--> fseek, to get the size of the file.

closePageFile(...)
--> This function is to close the page file that the File pointer is currently pointing to.
--> Check for the file, if not present return, "RC_FILE_NOT_FOUND".

destroyPageFile(...)
--> This function is to delete the page file.
--> Check for the file, if not present return, "RC_FILE_NOT_FOUND".

2. READING BLOCKS FROM DISC
============================

This module is to implement functions that read pages from file and store in memory.

1.readBlock(...)
--> This function is to read the page given by pageNum into the memory (memPage).
--> First we check for the page nuber and page, return RC_READ_NON_EXISTING_PAGE if page is not found.
--> If the valid pointer is identified, read the page to memory and update the current page position.

2.getBlockPos(...)
--> This function is to return the current page position which we get from the file handle.

3.readFirstBlock(...)
--> This function is to read the first block of the file using pageNum (passing the value '0').
-->RC_OK on success

4.readPreviousBlock(....)
--> This function is to read the first block of the file using pageNum (passing the value curpage - 1).
-->RC_OK on success


3. WRITING BLOCKS TO A PAGE FILE
================================

This module is to implement functions to write blocks from memory to a file from the disk.

1.writeBlock(...)
-->validate the page number
-->SEEK_SET, moves the file pointer to begining of the file.
-->Write the file from memory block using the memory pointer to the file on a disk. 
-->Update the current page position.
-->Close the file.


2.writeCurrentBlock(...) 
-->This function is used to pass current page position as the page number where data needs
   to be written.

3.appendEmptyBlock(...)
--> This function creates a memory block equal to page size and pointer addressing this block.
--> Seek the file write pointer to the end of the file and write to the empty page.
--> Update the total number of pages in the file as a new last page.

4.ensureCapacity(...)
--> This function checks if the file has required number of pages or needs more
--> Check the result, if yes calculate the number of pages required to meet the required limit.
--> Call appendEmptyBlock and append the required number of pages to the file.



