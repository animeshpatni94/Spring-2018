#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include "storage_mgr.h"
#include "dberror.h"


void initStorageManager (void) {

    printf("Storage Manager Initialized!\n");

}

RC createPageFile (char *fileName) {
    FILE *pageFile;
    pageFile = fopen(fileName,"w+");   //Creates an empty file for both reading and writing.
    fseek(pageFile, 0, SEEK_SET);        //Sets the file position to the beginning of file.
    for(int i = 0; i < PAGE_SIZE; i++) {     //Creates one page in the file. The size of page is 4096 bytes
        fwrite("\0", 1, 1, pageFile);
    }

    fseek(pageFile, 0L, SEEK_END);

    return RC_OK;
}

RC openPageFile (char *fileName, SM_FileHandle *fHandle) {
    FILE *pageFile;
    pageFile = fopen(fileName,"r+");  //Opens the file.

    if(pageFile == NULL) {           //If cannot found the file, return "RC_FILE_NOT_FOUND".
        return RC_FILE_NOT_FOUND;
    } else {
        fseek(pageFile, 0L, SEEK_END);
        int fileSize = ftell(pageFile);       //Get the size of file.

        fHandle->fileName = fileName;
        fHandle->totalNumPages = fileSize/PAGE_SIZE;
        fHandle->curPagePos = 0;
        fHandle->mgmtInfo = pageFile;

        return RC_OK;
    }
}


RC closePageFile (SM_FileHandle *fHandle) {
    if(fclose(fHandle->mgmtInfo)) {
        return RC_FILE_NOT_FOUND;
    } else {
        return RC_OK;
    }

}

RC destroyPageFile (char *fileName) {

    extern int errno;
    errno=0;
    int ret = remove(fileName);
    if(ret == 0) {
        return RC_OK;
    } else {
        // perror("opendir: ");   //Returns error information
        return RC_FILE_NOT_FOUND;
    }
}


/* reading blocks from disc */
RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {

    fseek(fHandle->mgmtInfo, 0L, SEEK_END);
    int fileSize = ftell(fHandle->mgmtInfo);       //Get the size of file.

    if(fileSize/PAGE_SIZE < pageNum + 1) {
        return RC_READ_NON_EXISTING_PAGE;
    } else {
        fseek(fHandle->mgmtInfo, pageNum*PAGE_SIZE, SEEK_SET);
        fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo);
        fHandle->curPagePos = getBlockPos(fHandle);
        return RC_OK;
    }
}

int getBlockPos (SM_FileHandle *fHandle) {
    return ftell(fHandle->mgmtInfo);
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(0, fHandle, memPage);
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    int currentBlock = getBlockPos(fHandle) / PAGE_SIZE - 1;
    int previousBlock = currentBlock - 1;
    return readBlock(previousBlock, fHandle, memPage);

}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    int blockPosition = getBlockPos(fHandle);
    int pageNum = blockPosition/PAGE_SIZE - 1;
    return readBlock(pageNum, fHandle, memPage);
}

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    int currentBlock = getBlockPos(fHandle) / PAGE_SIZE - 1;
    int nextBlock = currentBlock + 1;
    return readBlock(nextBlock, fHandle, memPage);
}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    int lastPage = fHandle->totalNumPages - 1;
    return readBlock(lastPage, fHandle, memPage);
}

/* writing blocks to a page file */
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    fseek(fHandle->mgmtInfo, pageNum*PAGE_SIZE, SEEK_SET);
    fwrite(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo);
    return RC_OK;

}

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    int blockPosition = getBlockPos(fHandle);
    int pageNum = blockPosition/PAGE_SIZE-1;
    writeBlock(pageNum, fHandle, memPage);

    return RC_OK;

}

RC appendEmptyBlock (SM_FileHandle *fHandle) {
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

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle) {
    if(fHandle->totalNumPages < numberOfPages) {
        fseek(fHandle->mgmtInfo, 0, SEEK_END);        //Sets the file position to the end of file.
        for(int i = 0; i < PAGE_SIZE; i++) {      //Creates a new page in the file. The size of page is 4096 bytes
            fwrite("\0", 1, 1, fHandle->mgmtInfo);
        }

        fseek(fHandle->mgmtInfo, 0L, SEEK_END);
        int fileSize = ftell(fHandle->mgmtInfo);       //Get the size of file.
        fHandle->totalNumPages = fileSize/PAGE_SIZE;
        fHandle->curPagePos = fileSize/PAGE_SIZE;
    }

    return RC_OK;

}

