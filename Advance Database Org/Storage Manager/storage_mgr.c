#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include "storage_mgr.h"
#include "dberror.h"


void initStorageManager (void){

    printf("Storage Manager Initialized!\n");

}

RC createPageFile (char *fileName){
    FILE *pageFile;
    pageFile = fopen(fileName,"w+");   //Creates an empty file for both reading and writing.
    for(int i = 0; i < PAGE_SIZE; i++){      //Creates one page in the file. The size of page is 4096 bytes
        fwrite("\0", 1, 1, pageFile);
    }

    fseek(pageFile,0L,SEEK_END);
    int size=ftell(pageFile);       //Get the size of file.

    return RC_OK;
}

RC openPageFile (char *fileName, SM_FileHandle *fHandle){
    FILE *pageFile;
    pageFile = fopen(fileName,"r+");  //Opens the file.

    if(pageFile == NULL){            //If cannot found the file, return "RC_FILE_NOT_FOUND".
        return RC_FILE_NOT_FOUND;
    }else{
        fseek(pageFile,0L,SEEK_END);
        int fileSize=ftell(pageFile);       //Get the size of file.

        fHandle->fileName = fileName;
        fHandle->totalNumPages = fileSize/PAGE_SIZE;
        fHandle->curPagePos = 0;
        fHandle->mgmtInfo = pageFile;

        return RC_OK;
    }
}


RC closePageFile (SM_FileHandle *fHandle){
    if(fclose(fHandle->mgmtInfo)){
        return RC_FILE_NOT_FOUND;
    } else {
        return RC_OK;
    }

}

RC destroyPageFile (char *fileName){

    extern int errno;
    errno=0;
    int ret = remove(fileName);
    if(ret == 0){
        return RC_OK;
    }
    else{
            perror("opendir: ");
        return RC_FILE_NOT_FOUND;
    }

    //return RC_OK;
}




/* reading blocks from disc */
RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
    FILE *filepage;
    int r;
    //checking If the file has less than pageNum pages, the method should return RC_READ_NON_EXISTING_PAGE.
    if(pageNum>fHandle->totalNumPages)
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
    else
    {
        //opening the file for reading purpose,"r" denotes reading the file.
        filepage = fopen(fHandle->fileName,"r");\
        //if the file is NULL then return FILE_NOT_FOUND
        if(filepage != NULL)
            {
            //setting the position to the current offset, q is the total number of bytes and SEEK_SET sets it to the beginning of the page.
            long g = pageNum*PAGE_SIZE;
            int s = fseek(filepage,g,SEEK_SET); //returns 0 is successful otherwise return some integer value.
            if(s==0)
            {
                //reading the file from 1 byte to the full PAGE_SIZE from the pointer to the memory that is "memPage"
                r = fread(memPage,1,PAGE_SIZE,filepage);
            }
            else
            {
                return RC_READ_NON_EXISTING_PAGE;
            }
            //fread should always return a value exactly similar to the file size
            if(r!=PAGE_SIZE)
            {
                return RC_READ_NON_EXISTING_PAGE;
            }
            else
            {
                //seeting the current page value to the Number of pages
                fHandle->curPagePos = pageNum;
            }
        }
        else{
            return RC_FILE_NOT_FOUND;
        }

    }
    //closing the file
    fclose(filepage);
    return RC_OK;
    }


int getBlockPos (SM_FileHandle *fHandle){
    //returning the current page position which we get from the file handle.
    return fHandle->curPagePos;
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    //By setting pageNum = 0, we try to read the page block.
    if(readBlock(0,fHandle,memPage)==RC_OK)
    {
        return RC_OK;
    }
    else
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    //getting the current page number
    int curpage = getBlockPos(fHandle);
    //checking if the previous page is present or not by reducing curpage value by 1
    if(readBlock((curpage-1),fHandle,memPage) == RC_OK)
    {
        return RC_OK;
    }
    else
    {
        return RC_READ_NON_EXISTING_PAGE;
    }


}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    //getting the current page number
    int curpage = getBlockPos(fHandle);
    //checking if the page is valid or not
    if (readBlock(curpage,fHandle,memPage) == RC_OK)
    {
        return RC_OK;
    }
    else
    {
        return RC_READ_NON_EXISTING_PAGE;
    }

}

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    //getting the current page number
    int curpage = getBlockPos(fHandle);
    //checking if the next page is present or not by incrementing curpage by 1
    if (readBlock((curpage+1),fHandle,memPage) == RC_OK && (curpage+1) <= fHandle->totalNumPages)
    {
        return RC_OK;
    }
    else
    {
        return RC_READ_NON_EXISTING_PAGE;
    }

}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    //getting the current page number
    int curpage = getBlockPos(fHandle);
    if (curpage == fHandle->totalNumPages)
    {
        return readBlock(curpage,fHandle,memPage);
    }
    else
    {
        return RC_READ_NON_EXISTING_PAGE;
    }
}

/* writing blocks to a page file */
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
    FILE *filepage;
    int w;
    //checking If the file has less than pageNum pages, the method should return RC_READ_NON_EXISTING_PAGE.
    if(pageNum>fHandle->totalNumPages)
    {
        return RC_WRITE_FAILED;
    }
    else
    {
        //opening the file for writing purpose,"r+" denotes reading and writing the file.
        filepage = fopen(fHandle->fileName,"r+");
        //if the file is NULL then return FILE_NOT_FOUND
        if(filepage != NULL)
            {
            //setting the position to the current offset, q is the total number of bytes and SEEK_SET sets it to the beginning of the page.
            long g = pageNum*PAGE_SIZE;
            int s = fseek(filepage,g,SEEK_SET); //returns 0 is successful otherwise return some integer value.
            if(s==0)
            {
                //writing the file from 1 byte to the full PAGE_SIZE from the pointer to the memory that is "memPage"
                w = fwrite(memPage,1,PAGE_SIZE,filepage);
            }
            else
            {
                return RC_READ_NON_EXISTING_PAGE;
            }
            //fwrite should always return a value exactly similar to the file size
            if(w!=PAGE_SIZE)
            {
                return RC_READ_NON_EXISTING_PAGE;
            }
            else
            {
                //setting the current page value to the Number of pages
                fHandle->curPagePos = pageNum;
            }
        }
        else
        {
            return RC_WRITE_FAILED;
        }

    }
    //closing the file
    fclose(filepage);
    return RC_OK;
}
/*To write current page to block*/
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int blockPosition = getBlockPos(fHandle);
    int pageNum = blockPosition/PAGE_SIZE-1;
    writeBlock(pageNum, fHandle, memPage);

    return RC_OK;
}

//appends the empty block
RC appendEmptyBlock (SM_FileHandle *fHandle)
{
  fseek(fHandle->mgmtInfo, 0, SEEK_END);        //Sets the file position to the end of file.
    for(int i = 0; i < PAGE_SIZE; i++) {     //Creates a new page in the file. The size of page is 4096 bytes
        fwrite("\0", 1, 1, fHandle->mgmtInfo);
    }

    fseek(fHandle->mgmtInfo, 0L, SEEK_END);
    int fileSize = ftell(fHandle->mgmtInfo);       //Get the size of file.
    fHandle->totalNumPages = fileSize/PAGE_SIZE;
    fHandle->curPagePos = fileSize/PAGE_SIZE;

    return RC_OK;
}

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){
if(fHandle->totalNumPages < numberOfPages) {
	int addPages = numberOfPages - totalNumPages;
        fseek(fHandle->mgmtInfo, 0, SEEK_END);        //Sets the file position to the end of file.
        for(int i = 0; i < addPages*PAGE_SIZE; i++) {      //Creates a new page in the file. The size of page is 4096 bytes
            fwrite("\0", 1, 1, fHandle->mgmtInfo);
        }

        fseek(fHandle->mgmtInfo, 0L, SEEK_END);
        int fileSize = ftell(fHandle->mgmtInfo);       //Get the size of file.
        fHandle->totalNumPages = fileSize/PAGE_SIZE;
        fHandle->curPagePos = fileSize/PAGE_SIZE;
    }

    return RC_OK;
}
