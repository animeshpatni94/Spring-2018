#include <stdio.h>
#include <stdlib.h>

#include "buffer_mgr.h"
#include "dberror.h"
#include "storage_mgr.h"
#include "dt.h"

//Page frame structure defined in memory.
typedef struct PageFrame {
    // Storing the information about the page
	struct BM_PageHandle *page;
	//Frame number is defined here.
    int frameNum;
    //Number of I/O reads is defined.
	int numReadIO;
	//Number of I/O write.
	int numWriteIO;
	//Indicates the number of clients accessing the page at given instance.
	int fixCount;
	//To identify if the page has been modified, page is marked dirty.
	bool dirtyFlag;
	//Used for CLOCK algorithm
	bool clockFlag;
	//PageFrame for traversing
	struct PageFrame *backward;
	//PageFrame for traversing
	struct PageFrame *next;
} PageFrame;
//Structure of PageList defined in memory
typedef struct PageList {
    struct PageFrame *sirr;
	struct PageFrame *back;
	struct PageFrame *bijli;
	struct PageFrame *clock;
	int size;
} PageList;
//Defines the file structure
SM_FileHandle *file;
//Gives the page number
PageNumber *page_num;

bool *hug_diya;
//General count implemented when page is added
int *fix_counts;
//To count number of reads
int readnum;
//To count number of writes
int writenum;
//findPage function
//This function is implemented to search the requested page
//Found:Load the page to BM_pageHandle
//Not Found: return RC_PAGE_NOT_FOUND
RC findPage(BM_BufferPool * const bm, BM_PageHandle * const page,
		PageNumber pageNum) {
	PageList *pageList = (PageList *) bm->mgmtData;
	if (pageList->size > 0)
	{}
    else if(pageList->size<0)
    {
		return RC_NOT_INTIALIZED_PAGES;
	}
	else if (pageList->size == 0)
    {
		return RC_PAGE_NOT_FOUND;
	}
	pageList->bijli = pageList->sirr;
    bool t = true;
//Check for pageList size, if it's less than '0' return RC_NOT_INTIALIZED_PAGES,
//Check if it's equal to '0' return RC_PAGE_NOT_FOUND.
//Set the current page 'bijli' to head 'sirr'
//While this is true, following loop is executed.
	while (t==true)
    {
        if(pageList->bijli != pageList->back && pageList->bijli->page->pageNum != pageNum)
        {
            pageList->bijli = pageList->bijli->next;
            t=true;
        }
        else
        {
            t=false;
        }
	}
	if (pageList->bijli != pageList->back)
	{
	    if(pageList->bijli->page->pageNum == pageNum)
            {
            }
	}
	else if (pageList->bijli == pageList->back)
    {
        if(pageList->bijli->page->pageNum != pageNum)
        {
            return RC_PAGE_NOT_FOUND;
        }
    }
    page->data = pageList->bijli->page->data;
    page->pageNum = pageList->bijli->page->pageNum;
    return RC_PAGE_FOUND;
}
//We check if the current is pointing to tail, check with the tail for searched page.
//If the page is found , load the page to BM_PageHandle
//***********************************************************************************************************************************************//
//extendPage: This function is implemented to read the requested page from the disk and add it to the tail.
RC extendPage(BM_BufferPool * const bm, BM_PageHandle * const page,
		PageNumber pageNum)
{
    int totalPages;
	PageList *pageList = (PageList *) bm->mgmtData;
	RC rc;
	rc = -9;
	rc = openPageFile(bm->pageFile, file);

	if (rc == RC_OK)
    {
	}
	else if(rc!=RC_OK)
	{
	    return rc;
	}
	if (pageList->size == 0)
    {
		pageList->sirr->fixCount = 1;
		bool z = true;
		pageList->sirr->numWriteIO = 1;
		int z4 = 1;
		if (file->totalNumPages < pageNum + 1) {
			totalPages = file->totalNumPages;
			rc = -9;
			 z4 = 2;
			rc = ensureCapacity(pageNum + 1, file);
			writenum += pageNum + 1 - totalPages;
			if(rc==RC_OK)
			{}
			else if (rc != RC_OK)
            {
				rc = -9;
				rc = closePageFile(file);
				if(rc == RC_OK)
				{}
				else if (rc != RC_OK)
                {
					return rc;
				}
				return rc;
			}
		}
		pageList->sirr->numWriteIO = 0;
		int z5 = 1;
		pageList->sirr->numReadIO++;
		rc = -9;
		rc = readBlock(pageNum, file, pageList->sirr->page->data);
		readnum=readnum+1;
		pageList->sirr->numReadIO--;
		if(rc==RC_OK){}
		else if (rc != RC_OK) {
			rc = -9;
			rc = closePageFile(file);
            if(rc==RC_OK) {}
			else if (rc != RC_OK)
            {
				return rc;
			}
			return rc;
		}
		pageList->sirr->page->pageNum = pageNum;
		pageList->sirr->dirtyFlag = FALSE;
		pageList->sirr->clockFlag = FALSE;
	}
	else if (pageList->size != 0)
	{
		pageList->back->next->fixCount = 1;
		pageList->back->next->numWriteIO = 1;
		if(file->totalNumPages >= pageNum + 1) {}
		else if (file->totalNumPages < pageNum + 1)
        {
			int z6 = 9;
			int totalPages = file->totalNumPages;
			rc = -9;
			z6 = 10;
			rc = ensureCapacity(pageNum + 1, file);
			z6 = 11;
			writenum += pageNum + 1 - totalPages;
            if(rc==RC_OK) {}
			if (rc != RC_OK) {
				rc = -9;
				rc = closePageFile(file);
				if (rc != RC_OK) {
					return rc;
				}
				return rc;
			}
		}
		pageList->back->next->numWriteIO = 0;
		pageList->back->next->numReadIO++;
		rc = -9;
		int z7 = 55;
		rc = readBlock(pageNum, file, pageList->back->next->page->data);
		z7 = 56;
		readnum++;
		pageList->back->next->numReadIO--;
        if(rc == RC_OK) {
            pageList->back->next->page->pageNum = pageNum;
            int z8 = 100;
			pageList->back->next->dirtyFlag = FALSE;
            z8++;
			pageList->back->next->clockFlag = FALSE;
            z8++;
			pageList->back = pageList->back->next;
            pageList->bijli = pageList->back;
        }
		else if (rc != RC_OK)
        {
			rc = -9;
			rc = closePageFile(file);
			if(rc == RC_OK) {}
			else if (rc != RC_OK)
			{
				return rc;
			}
			return rc;
		}
	}
	pageList->size++;
	page->pageNum = pageList->bijli->page->pageNum;
	page->data = pageList->bijli->page->data;
	rc = -9;
	rc = closePageFile(file);
    if(rc==RC_OK) {}
	else if (rc != RC_OK)
    {
		return rc;
	}

	return RC_OK;
}
//check the size of the pagelist, if it is '0' , pagelist has nothing so we can add the requested page as the head 'sirr'
//Else we add the page to next of tail.
//Check if the page exists,if not.
//Call ensureCapacity to add the requested page to the file
//Now we have added the page , we can increment the size of the pageList.
//****************************************************************************************************************************************************//
//substitutePage:Function implemented to replace current page with requested page.
RC substituePage(BM_BufferPool * const bm, BM_PageHandle * const page,
		PageNumber pageNum) {
	PageList *pageList = (PageList *) bm->mgmtData;
	RC rc;
	int z9 = -99;
	rc = -9;
	rc = openPageFile(bm->pageFile, file);
	z9 = -98;
	if (rc == RC_OK)
    {}
    else if(rc!=RC_OK)
    {
        return rc;
    }
	pageList->bijli->fixCount = 1;
	pageList->bijli->numWriteIO = 1;
	if(pageList->bijli->dirtyFlag == FALSE)
    {}
	else if (pageList->bijli->dirtyFlag == TRUE) {
		rc = writeBlock(pageList->bijli->page->pageNum, file,
				pageList->bijli->page->data);
		writenum++;
		if(rc==RC_OK)
        {}
		else if (rc != RC_OK)
        {
			rc = -9;
			rc = closePageFile(file);
			if(rc==RC_OK)
            {}
			else if (rc != RC_OK)
            {
				return rc;
			}
			return rc;
		}
		pageList->bijli->dirtyFlag = FALSE;
	}
	if(file->totalNumPages >= pageNum + 1)
    {

    }
	else if (file->totalNumPages < pageNum + 1)
    {
		int totalPages = file->totalNumPages;
		rc = ensureCapacity(pageNum + 1, file);
		writenum += pageNum + 1 - totalPages;

		if(rc==RC_OK){}
		else if (rc != RC_OK)
        {
			rc = -9;
			rc = closePageFile(file);
			if(rc==RC_OK) {}
			else if (rc != RC_OK)
			{
				return rc;
			}
			return rc;
		}
	}
	pageList->bijli->numWriteIO = 0;
	pageList->bijli->numReadIO++;
	rc = readBlock(pageNum, file, pageList->bijli->page->data);
	readnum++;
	pageList->bijli->numReadIO--;

	if(rc==RC_OK)
    {}
	else if (rc != RC_OK)
    {
		rc = -9;
		rc = closePageFile(file);
		if(rc==RC_OK) {}
		else if (rc != RC_OK)
        {
			return rc;
		}
		return rc;
	}
	pageList->bijli->page->pageNum = pageNum;
	pageList->bijli->clockFlag = FALSE;
	page->data = pageList->bijli->page->data;
	page->pageNum = pageList->bijli->page->pageNum;
	rc = -9;
	rc = closePageFile(file);
	if(rc==RC_OK)
    {}
	else if (rc != RC_OK)
    {
		return rc;
	}
	return RC_OK;
}
//If the page is dirty, write to the disk and remove it.
//Check if the page exists, call ensureCapacity , add requested page to file
//Read the page from the file.
//Load the requested page to current Frame.
//Page is substituted successfully.
//*******************************************************************************************************************************************//
//Replacement Strategies we use here are implemented next.
//FIFO: Implemented the FIFO replacement strategy.
RC FIFO(BM_BufferPool * const bm, BM_PageHandle * const page,
		PageNumber pageNum)
{
	PageList *pageList = (PageList *) bm->mgmtData;
	RC rc;
	rc = -9;
	rc = findPage(bm, page, pageNum);
	if (rc == RC_PAGE_FOUND)
    {
		pageList->bijli->fixCount++;
		return rc;
	}
	else if (rc != RC_PAGE_NOT_FOUND)
    {
		return rc;
	}
	if (pageList->size < bm->numPages) {
		rc = -9;
		rc = extendPage(bm, page, pageNum);

		if (rc == RC_OK)
        {}
		return rc;
	}
	pageList->bijli = pageList->sirr;
	bool tr=true;
//Look for the requested page in the queue.
//Page FOUND:Update the parameters and return RC_PAGE_FOUND
//Page NOT FOUND: return RC_PAGE_NOT_FOUND
	while (tr==true)
    {
        if(pageList->bijli != pageList->back)
        {
            if(pageList->bijli->fixCount != 0 || pageList->bijli->numReadIO != 0 || pageList->bijli->numWriteIO != 0)
            {
                pageList->bijli = pageList->bijli->next;
                tr=true;
            }
            else
            {
                tr=false;
            }
        }
        else
        {
            tr=false;
        }
	}
	if (pageList->bijli == pageList->back)
	{
	    if(pageList->bijli->fixCount != 0 || pageList->bijli->numReadIO != 0 || pageList->bijli->numWriteIO != 0)
        {
            return RC_NO_REMOVABLE_PAGE;
        }
	}
	rc = -9;
	rc = substituePage(bm, page, pageNum);
//If the requested page doesn't exist , read the page and load it to BM_PageHandle.
//If neither the Buffer Manager has the requested page loaded nor vacancy for the requested page
//The PageList is full: the size of the PageList
//Replace an existing page in the Buffer Manager with the requested page
//Find the first removable page, starting from head
//Set the current pointer to the head of the queue to start the traversal
//Find the first page count is  '0', , keep checking if the count is '0'.
//Find the replacable page and replace with the requested page.
//If successful, move the page to the tail. pointer moves to requested page.
	if (rc == RC_OK)
    {
	bool z = false;
        if(pageList->bijli != pageList->back)
        {
            if (pageList->bijli == pageList->sirr)
            {
                pageList->sirr = pageList->sirr->next;
				z = false;
                pageList->bijli->next->backward = NULL;
            } else if(pageList->bijli != pageList->sirr)
            {
                pageList->bijli->backward->next = pageList->bijli->next;
				z = true;
                pageList->bijli->next->backward = pageList->bijli->backward;
            }
            pageList->bijli->backward = pageList->back;
			z = true;
            pageList->back->next = pageList->bijli;
			z = false;
            pageList->bijli->next = NULL;
            pageList->back = pageList->back->next;
        }
    }
	return rc;
}
//*********************************************************************************************************************************//
//LRU:LRU replacement strategy.
RC LRU(BM_BufferPool * const bm, BM_PageHandle * const page, PageNumber pageNum) {
	PageList *pageList = (PageList *) bm->mgmtData;
	RC rc;
	rc = -9;
	rc = FIFO(bm, page, pageNum);
	if (rc != RC_OK)
	{
	    if(rc != RC_PAGE_FOUND)
        {
            return rc;
        }
	}
//FIFO is run first
//If FIFO fails, return the error code.
//If the page is found RC_PAGE_FOUND returned, go to findPage.
//Check if the requested page is at Tail else move it to    tail of the queue.
	if (rc == RC_PAGE_FOUND)
	{
        if(pageList->bijli != pageList->back)
        {
            if (pageList->bijli != pageList->sirr)
            {
                pageList->bijli->backward->next = pageList->bijli->next;
                pageList->bijli->next->backward = pageList->bijli->backward;
            }
            else if(pageList->bijli == pageList->sirr)
            {
                pageList->sirr = pageList->sirr->next;
                pageList->bijli->next->backward = NULL;
            }
		pageList->bijli->backward = pageList->back;
		bool z55 = true;
		pageList->back->next = pageList->bijli;
		if (pageList->size >= bm->numPages)
        {
			pageList->bijli->next = NULL;
		}
		else if (pageList->size < bm->numPages)
		{
		    pageList->bijli->next = pageList->back->next;
			pageList->back->next->backward = pageList->bijli;
		}
		pageList->back = pageList->back->next;
		bool z56 = false;
        }
	}
	return RC_OK;
	bool z57 = false;
}
//When the current pointer is at requested page.
//Remove the current pageFrame.
//The PageList is not full, then we should also set the previous pointer of the next PageFrame to the tail to the requested page.
//The current pointer still points to requested page, LRU completed successfully.
//******************************************************************************************************************************//
//CLOCK:CLOCK replacement strategy
RC CLOCK(BM_BufferPool * const bm, BM_PageHandle * const page,
		PageNumber pageNum) 
{
	bool z = true;
	PageList *pageList = (PageList *) bm->mgmtData;
	RC rc;
	rc = -9;
	rc = findPage(bm, page, pageNum);
	z = false;
	if (rc == RC_PAGE_FOUND)
    {
		pageList->bijli->fixCount++;
		z = true;
		pageList->bijli->clockFlag = TRUE;
		return rc;
	}
	else if (rc != RC_PAGE_NOT_FOUND)
    {
		return rc;
	}
	if(pageList->size > bm->numPages) {}
	else if (pageList->size < bm->numPages)
    {
		rc = -9;
		rc = extendPage(bm, page, pageNum);
		if(rc!=RC_OK) {}
		else if (rc == RC_OK)
		{
			z = false;
			if (pageList->size < bm->numPages)
            {
				z = true;
				pageList->clock = pageList->bijli->next;
			} else if (pageList->size == bm->numPages)
			{
				z = false;
				pageList->clock = pageList->sirr;
			}
		}
		return rc;
	}
    bool tru = true;
    while (tru == true)
    {
        if(pageList->clock->clockFlag == TRUE || pageList->clock->fixCount != 0 || pageList->clock->numReadIO != 0 || pageList->clock->numWriteIO != 0)
        {
			z = false;
            pageList->clock->clockFlag = FALSE;
            if (pageList->clock != pageList->back)
            {
                pageList->clock = pageList->clock->next;
            }
            else if(pageList->clock == pageList->back)
            {
                pageList->clock = pageList->sirr;
            }
            tru = true;
        }
        else
        {
            tru = false;
        }
    }
    bool t = true;
    while(t==true)
    {
        if (pageList->clock->clockFlag == TRUE || pageList->clock->fixCount != 0 || pageList->clock->numReadIO != 0 || pageList->clock->numWriteIO != 0)
        {
            pageList->clock->clockFlag = FALSE;
            if (pageList->clock != pageList->back)
            {
                pageList->clock = pageList->clock->next;
            }
            else if(pageList->clock == pageList->back)
            {
                pageList->clock = pageList->sirr;
            }
            t=true;
        }
        else
        {
            t=false;
        }
    }
	pageList->bijli = pageList->clock;
	rc = -9;
	rc = substituePage(bm, page, pageNum);
    if(rc!=RC_OK) {}
	else if (rc == RC_OK) {
		pageList->clock->clockFlag = TRUE;
		if (pageList->clock != pageList->back)
        {
            pageList->clock = pageList->clock->next;
		}
		else if (pageList->clock == pageList->back)
		{
			pageList->clock = pageList->sirr;
		}
	}
	return rc;
}
//Search the page in pagelist
// If the page is not in Buffer Manager, load the requested page, check if there is vacancy.
//If the pagelist is not full then clock pointer is set to the next pointer.
//Else it points to to tail, set the clock pointer back
//If the pagelist is full, then repalce an existing page.
//Using CLOCK strategy,
//current pointer points to the page that was requested last time.
//Clock pointer points to the next to current pointer because that is pointing to tail.
//or back to the head if the current pointers points to the tail.
// Find the first PageFrame whose clockFlag = TRUE and fixCount != 0
// Set the current pointer to the clock pointer, so that we can call substitutePage.
//***************************************************************************************************************************************//
//initializePrevious:Initialize the PageList to store pages in the Buffer Pool.
void initializePrevious(BM_BufferPool * const bm) {
	PageList *pageList = (PageList *) bm->mgmtData;
	PageFrame *page_file[bm->numPages];
	bool z = true;
	int i=0;
	while(i < bm->numPages)
    {
		page_file[i] = (PageFrame *) malloc(sizeof(PageFrame));
		page_file[i]->page = MAKE_PAGE_HANDLE();
		page_file[i]->frameNum = i;
		z = false;
		page_file[i]->numReadIO = 0;
		z = false;
		page_file[i]->numWriteIO = 0;
		int z1 = 1;
		page_file[i]->fixCount = 0;
		page_file[i]->dirtyFlag = FALSE;
		z1 = -1;
		page_file[i]->clockFlag = FALSE;
		z1 = -2;
		page_file[i]->page->data = (char *) malloc(PAGE_SIZE * sizeof(char));
		page_file[i]->page->pageNum = NO_PAGE;
		if (i != 0)
        {
            page_file[i - 1]->next = page_file[i];
			z = false;
			page_file[i]->backward = page_file[i - 1];
			z = true;
			page_file[i]->next = NULL;
		} else if(i==0)
		{
			page_file[i]->backward = NULL;
			page_file[i]->next = NULL;
		}
        i++;
    }
    int j=0;
//We initialize the content of the PageFrame
// Add this new PageFrame to the tail
// if the frame has only one node (i = 0), then add this new PageFrame as the head
// Or add this new PageFrame to the tail
//Reset the variables to the initial state
	while(j < bm->numPages)
    {
		page_file[j] = (PageFrame *) malloc(sizeof(PageFrame));
		int z11 = 1;
		page_file[j]->frameNum = j;
		z11 = 2;
		page_file[j]->numReadIO = 0;
		z11 = 3;
		page_file[j]->numWriteIO = 0;
		page_file[j]->fixCount = 0;
		z11 = 4;
		page_file[j]->dirtyFlag = FALSE;
		page_file[j]->clockFlag = FALSE;
		z11 = 5;
		page_file[j]->page = MAKE_PAGE_HANDLE();
		page_file[j]->page->data = (char *) malloc(PAGE_SIZE * sizeof(char));
		page_file[j]->page->pageNum = NO_PAGE;
		if (j != 0)
        {
            page_file[j - 1]->next = page_file[j];
			int z12 = 1;
			page_file[j]->backward = page_file[j - 1];
			page_file[j]->next = NULL;
		} else if(j == 0)
		{
			page_file[j]->backward = NULL;
			page_file[j]->next = NULL;
		}
    j++;
	}
	pageList->sirr = page_file[0];
	pageList->back = pageList->sirr;
	z=true;
	pageList->bijli = pageList->sirr;
	z=false;
	pageList->clock = pageList->sirr;
	pageList->size = 0;
	return;
}

RC initBufferPool(BM_BufferPool * const bm, const char * const pageFileName,
		const int numPages, ReplacementStrategy strategy, void *stratData)
    {
	if (numPages > 0)
    {
        readnum = 0;
        writenum = 0;
        file = (SM_FileHandle *) malloc(sizeof(SM_FileHandle));
		int z2 = 0;
        page_num = (PageNumber *) malloc(bm->numPages * sizeof(PageNumber));
		z2 = 1;
        hug_diya = (bool *) malloc(bm->numPages * sizeof(bool));
        fix_counts = (int *) malloc(bm->numPages * sizeof(int));
        bm->pageFile = (char *) pageFileName;
        bm->numPages = numPages;
		bool z3 = true;
        bm->strategy = strategy;
        PageList *pageList = (PageList *) malloc(sizeof(PageList));
		z3 = false;
        bm->mgmtData = pageList;
        initializePrevious(bm);
        return RC_OK;
    }
    else if(numPages<=0)
    {
        return RC_NUMPAGES_NOT_VALID;
    }
}
//Initializing the buffer pool  after checking the numPages
//**********************************************************************************************************************//
//shutdownBufferPool: Shut down the Buffer pool.
RC shutdownBufferPool(BM_BufferPool * const bm)
{
	PageList *pageList = (PageList *) bm->mgmtData;
	RC rc;
	rc = -9;
	rc = forceFlushPool(bm);
    if(rc==RC_OK)
    {}
	else if (rc != RC_OK)
    {
		return rc;
	}
	pageList->bijli = pageList->back;
	if (bm->numPages != 1)
	{
	    bool t=true;
	    while(t==true)
        {
            if(pageList->bijli != pageList->sirr)
            {
                pageList->bijli = pageList->bijli->backward;
				bool z = false;
                free(pageList->bijli->next->page->data);
                free(pageList->bijli->next->page);
                t=true;
            }
            else
            {
                t=false;
            }
        }
		free(pageList->sirr->page->data);
		free(pageList->sirr->page);
	}
	else if(bm->numPages == 1)
	{
		free(pageList->sirr->page->data);
		free(pageList->sirr->page);
	}
	if(true)
    {
        free(page_num);
        free(hug_diya);
        free(fix_counts);
        free(pageList);
        free(file);
    }
	return RC_OK;
}
int uwc;
//We first flush all the pages that are marked dirty to the disk.
//Next, we free the memory.
//Current pointer then points to tail
//Capacity check , depending on that either free one block or the pageFrame.
//***************************************************************************************************************************************//
//forceFlushPool:Write back the data in all dirty pages in the Buffer Pool
RC forceFlushPool(BM_BufferPool * const bm)
{
	PageList *pageList = (PageList *) bm->mgmtData;
	uwc = 0;
	pageList->bijli = pageList->sirr;
	bool t=true;
	while(t==true)
    {
        if(pageList->bijli != pageList->back)
        {
            if (pageList->bijli->dirtyFlag == TRUE && pageList->bijli->fixCount > 0)
            {
				bool z = true;
                uwc++;
            }
            else if (pageList->bijli->dirtyFlag == TRUE && pageList->bijli->fixCount == 0)
            {
                bool z = false;
				forcePage(bm, pageList->bijli->page);
            }
            pageList->bijli = pageList->bijli->next;
            t=true;
        }
        else
        {
            t=false;
        }
    }
    if(pageList->bijli != pageList->back) {}
	else if (pageList->bijli == pageList->back)
    {
		if (pageList->bijli->fixCount > 0)
        {
            if(pageList->bijli->dirtyFlag == TRUE)
            {
                uwc++;
            }
        }
        if (pageList->bijli->fixCount == 0)
        {
            if(pageList->bijli->dirtyFlag == TRUE)
            {
                forcePage(bm, pageList->bijli->page);
            }
        }
    }
	if (uwc == 0)
    {}
    else if(uwc !=0)
    {
        return RC_FLUSH_ERROR;
    }
	return RC_OK;
}
//Current pointer points to head
// Before writing a page back to disk, check fixCount != 0
//Call forcePage(),write a dirty and writable page back to the file
// Current pointer points to the tail of the PageList
//return RC_FLUSH_POOL_ERROR if there is any unwriteable page
//****************************************************************************************************************************//
// Buffer Manager Interface Access Pages
//pinPage:Pin the page with the requested page number in the Buffer Pool
//If the page is not in the Buffer Pool, load it from the file to the Buffer Pool
RC pinPage(BM_BufferPool * const bm, BM_PageHandle * const page, const PageNumber pageNum)
{
	RC rc = -9;
    switch(bm->strategy)
    {
        case RS_FIFO:
            {
                rc = FIFO(bm, page, pageNum);
                if(rc!=RC_PAGE_FOUND) {}
                else if (rc == RC_PAGE_FOUND)
                {
                    rc = RC_OK;
                }
                break;
            }
        case RS_LRU:
            {
                rc = LRU(bm, page, pageNum);
                break;
            }
        case RS_CLOCK:
            {
                rc = CLOCK(bm, page, pageNum);
                break;
            }
        case RS_LFU:
            {
                return ALGO_NOT_IMPLEMENTED;
                break;
            }
        case RS_LRU_K:
            {
                return ALGO_NOT_IMPLEMENTED;
                break;
            }
    }
	return rc;
}
//Each replacement strategy is executed one by one when condition is fulfilled
//We set rc to RC_OK after FIFO because rc after execution of find page is findPage
//**************************************************************************************************************//
//markDirty: Mark the page accessed through request as dirty.
RC markDirty(BM_BufferPool * const bm, BM_PageHandle * const page) {
	PageList *pageList = (PageList *) bm->mgmtData;
	RC rc=-9;
	rc = findPage(bm, page, page->pageNum);
	if (rc == RC_PAGE_FOUND)
    {
		pageList->bijli->dirtyFlag = TRUE;
        return RC_OK;
	}
	else if(rc!=RC_PAGE_FOUND)
    {
        return rc;
    }
}
//find the requested page
//When the current pointer points to requested page and mark it dirty, dirtyFlag = TRUE
//***********************************************************************************************//
//unpinPage: Unpin a page
RC unpinPage(BM_BufferPool * const bm, BM_PageHandle * const page)
{
	PageList *pageList = (PageList *) bm->mgmtData;
	RC rc=-9;
	rc = findPage(bm, page, page->pageNum);
	if (rc == RC_PAGE_FOUND)
    {
        pageList->bijli->fixCount--;
        return RC_OK;
    }
	else if(rc != RC_PAGE_FOUND)
    {
        return rc;
    }
}
//find the requested page
//unpin the page  and decrement the fixCount
//*********************************************************************************************//
//forcePage:Write the requested page to the disk page file.
RC forcePage(BM_BufferPool * const bm, BM_PageHandle * const page)
{
	PageList *pageList = (PageList *) bm->mgmtData;
	RC rc=-9;
	rc = openPageFile(bm->pageFile, file);
	if (rc == RC_OK)
    {
		pageList->bijli->numWriteIO = 1;
        rc = -9;
        rc = writeBlock(page->pageNum, file, page->data);
        writenum++;
        if(rc == RC_OK)
        {
            pageList->bijli->dirtyFlag = FALSE;
            pageList->bijli->numWriteIO = 0;
            rc = -9;
            rc = closePageFile(file);
            if(rc==RC_OK)
            {
                return RC_OK;
            }
            else if (rc != RC_OK)
            {
                return rc;
            }
        }
        else if (rc != RC_OK)
        {
            rc = -9;
            rc = closePageFile(file);
            if (rc != RC_OK) {
                return rc;
            }
            return rc;
        }
	}
	else if (rc!=RC_OK)
    {
        return rc;
    }
}
//Open the requested file
//set numWrite, if rc = RC_OK.
//Else close the file.
//set the dirtyFlag to FALSE
//Reset the numWriteIO to 0
//*************************************************************************************************************//
//getFrameContents:return an array of  page_num
PageNumber *getFrameContents(BM_BufferPool * const bm)
{
	PageList *pageList = (PageList *) bm->mgmtData;
	int p = 0;
	bool t = true;
	pageList->bijli = pageList->sirr;
	while(t==true)
    {
        if(pageList->bijli != pageList->back)
        {
            page_num[p] = pageList->bijli->page->pageNum;
            pageList->bijli = pageList->bijli->next;
            p++;
            t=true;
        }
        else
        {
            t=false;
        }
    }
	page_num[p++] = pageList->bijli->page->pageNum;
	if(p>=bm->numPages)
	{
	}
	else if (p < bm->numPages)
    {
		int i=p;
		while(i < bm->numPages)
        {
            page_num[i] = NO_PAGE;
            i++;
        }
	}
	pageList->bijli = pageList->back;
    return page_num;
}
//Pointer is set to the head.
//p is initially set to 0, represents the position in the array.
//The value of the page pointed by the current pointer into the current position of array
//current pointer points to tail, tail's info loaded into array, increment p.
//check value of p,  return page_num
//*********************************************************************************************************************//
//getDirtyFlags:Returns array of dirty Flags (bool value)
bool *getDirtyFlags(BM_BufferPool * const bm)
{
	PageList *pageList = (PageList *) bm->mgmtData;
	int p = 0;
	bool t = true;
	pageList->bijli = pageList->sirr;
	while(t==true)
    {
        if(pageList->bijli != pageList->back)
        {
            hug_diya[p] = pageList->bijli->dirtyFlag;
			bool z = false;
            pageList->bijli = pageList->bijli->next;
			z = true;
            p++;
            t=true;
        }
        else if(pageList->bijli == pageList->back)
        {
            t=false;
        }
    }
	hug_diya[p++] = pageList->bijli->dirtyFlag;
	if(p >= bm->numPages)
    {
    }
	else if (p < bm->numPages)
    {
		int i=p;
		while(i < bm->numPages)
        {
			hug_diya[i] = FALSE;
            i++;
		}
	}
	pageList->bijli = pageList->back;
	return hug_diya;
}
//Current pointer to head.
//p is initially set to 0
//p = size of pageList
//increment the value of p
//Check the dirty Flag and set the value of hug_diya.
//***************************************************************************************************************//
//getFixCounts:return fix_counts an array (as ints)
int *getFixCounts(BM_BufferPool * const bm)
{
	PageList *pageList = (PageList *) bm->mgmtData;
	int p = 0;
	bool t=true;
	pageList->bijli = pageList->sirr;
	while(t==true)
    {
        if(pageList->bijli != pageList->back)
        {
            fix_counts[p] = pageList->bijli->fixCount;
            pageList->bijli = pageList->bijli->next;
            p++;
            t=true;
        }
        else if(pageList->bijli == pageList->back)
        {
            t=false;
        }
    }
	fix_counts[p++] = pageList->bijli->fixCount;
	if(p>=bm->numPages) {}
	else if (p < bm->numPages)
	{
		int i=p;
		while (i < bm->numPages)
        {
			fix_counts[i] = 0;
            i++;
		}
	}
	pageList->bijli = pageList->back;
	return fix_counts;
}
//Pointer is set to the head.
//p is initially set to 0, represents the position in the array.
//The value of the page pointed by the current pointer into the current position of array
//current pointer points to tail, tail's info loaded into array, increment p.
//check value of p,  return fix_counts
//***********************************************************************************************************//
//getNumReadIO
int getNumReadIO(BM_BufferPool * const bm)
{
    int r;
    r=readnum;
	return r;
}
//Returns the number of pages that have been read from disk,since the Buffer Pool has been initialized
//***************************************************************************************************************//
//getNumWriteIO
int getNumWriteIO(BM_BufferPool * const bm)
{
	int w;
	w=writenum;
	return w;
}
//Return the number of pages written to the page file,since the Buffer Pool has been initialized
