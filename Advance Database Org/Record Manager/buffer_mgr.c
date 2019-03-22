#include "buffer_mgr.h"
#include "dberror.h"
#include "storage_mgr.h"
#include "dt.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
SM_FileHandle *F_HANDLE;
int first = 0;
PageNumber *PAGE_NUMS; 
int second = 0;
bool *DIRTY_FLAGS; 
int third = 0;
int *FIX_COUNTS; 
int forth = 0;
int NUM_READ_IOS;
int fifth = 0;
int NUM_WRITE_IOS;
int sixth = 0;
pthread_rwlock_t rwlock; 

RC findPage(BM_BufferPool * const bm, BM_PageHandle * const page,
		PageNumber pageNum) 
{
	int test = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	bool x = (queue->size < 0)?TRUE:FALSE;
	bool y = (queue->size == 0)?TRUE:FALSE;
	if (x==TRUE) 
	{
		test++;
		return RC_PAGELIST_NOT_INITIALIZED;
	} 
	else if (y==TRUE) 
	{
		test++;
		return RC_PAGE_NOT_FOUND;
	}
	test++;
	queue->current = queue->head;
	test++;
	while (queue->current->page->pageNum != pageNum && queue->current != queue->tail) 
	{
		if(test==2)
		{
			//printf("search page begins");
		}
		queue->current = queue->current->next;
		test = 0;
	}
	bool x1 = (queue->current == queue->tail)?TRUE:FALSE;
	bool x2 = (queue->current->page->pageNum != pageNum)?TRUE:FALSE;
	if (x1==TRUE && x2==TRUE) 
	{
		test++;
		return RC_PAGE_NOT_FOUND;
	}
	page->pageNum = queue->current->page->pageNum;
	test++;
	page->data = queue->current->page->data;
	int tt = 0;
	tt=test;
	return RC_PAGE_FOUND;
} 

RC extendPage(BM_BufferPool * const bm, BM_PageHandle * const page,
		PageNumber pageNum) 
{
	int test = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	test++;
	RC rc; 
	if(test==1)
	{
		//printf("unlock the file");
	}
	test++;
	pthread_rwlock_init(&rwlock, NULL);
	test++;
	pthread_rwlock_wrlock(&rwlock);
	rc = -99;
	if(test==3)
	{
		//printf("opening the file");
	}
	rc = openPageFile(bm->pageFile, F_HANDLE);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		test++;
		return rc;
	}
	bool y = (queue->size == 0)?TRUE:FALSE;
	if (y==TRUE) 
	{
		test++;
		queue->head->fixCount = 1;
		if(test==4)
		{
			//printf("queue head is updated");
		}
		queue->head->numWriteIO = 1;
		bool z1 = (F_HANDLE->totalNumPages < pageNum + 1)?TRUE:FALSE;
		if (z1==TRUE) 
		{
			rc = -99;
			int aa = 0;
			int totalPages = F_HANDLE->totalNumPages;
			aa =  totalPages;
			rc = ensureCapacity(pageNum + 1, F_HANDLE);
			aa++;
			NUM_WRITE_IOS += pageNum + 1 - totalPages;
			bool z2 = (rc != RC_OK)?TRUE:FALSE;
			if (z2==TRUE) 
			{
				aa++;
				rc = -99;
				if(aa==totalPages+2)
				{
					//printf("ensurecap throws RC");
				}
				rc = closePageFile(F_HANDLE);
				bool z3 = (rc != RC_OK)?TRUE:FALSE;
				if (z3==TRUE) 
				{
					aa++;
					return rc;
				}
				test++;
				pthread_rwlock_unlock(&rwlock);
				if(test==4)
				{
					//printf("lock the file");
				}
				pthread_rwlock_destroy(&rwlock);
				test++;
				return rc;
			}
		}
		rc = -99;
		test++;
		queue->head->numWriteIO = 0;
		test++;
		queue->head->numReadIO++;
		if(test==6)
		{
			//printf("next part of extend page");
		}
		rc = readBlock(pageNum, F_HANDLE, queue->head->page->data);
		test++;
		NUM_READ_IOS++;
		if(test==7)
		{
			//printf("head = numReadIO--")
		}
		queue->head->numReadIO--;
		bool z3 = (rc != RC_OK)?TRUE:FALSE;
		if (z3==TRUE) 
		{
			test++;
			rc = -99;
			if(test==8)
			{
				//printf("ReadBlock throws an error which is not defined");
			}
			rc = closePageFile(F_HANDLE);
			bool z4 = (rc != RC_OK)?TRUE:FALSE;
			if (z4==TRUE) 
			{
				test++;
				return rc;
			}
			test++;
			pthread_rwlock_unlock(&rwlock);
			if(test==7)
			{
				//printf("lock append");
			}
			pthread_rwlock_destroy(&rwlock);
			test++;
			return rc;
		}
		if(test == 9)
		{
			//printf("setting queue values straight");
		}
		queue->head->page->pageNum = pageNum;
		test++;
		queue->head->dirtyFlag = FALSE;
		test++;
		queue->head->clockFlag = FALSE;
		test--;
	} else {
		if(test == 3)
		{
			//printf("else is the condition fails");
		}
		queue->tail = queue->tail->next;
		int test1 = 0;
		queue->tail->fixCount = 1;
		test1++;
		queue->tail->numWriteIO = 1;
		test1++;
		bool z5 = (F_HANDLE->totalNumPages < pageNum + 1)?TRUE:FALSE;
		if (z5==TRUE) 
		{
			rc = -99;
			int aa1 = 0;
			int totalPages = F_HANDLE->totalNumPages;
			aa1 = totalPages; 
			aa1++;
			rc = ensureCapacity(pageNum + 1, F_HANDLE);
			if(aa1==totalPages+1)
			{
				//printf(ensurecap RC);
			}
			NUM_WRITE_IOS += pageNum + 1 - totalPages;
			bool z6 = (rc != RC_OK)?TRUE:FALSE;
			if (z6==TRUE) 
			{
				test++;
				rc = -99;
				test = 10;
				rc = closePageFile(F_HANDLE);
				bool z7 = (rc != RC_OK)?TRUE:FALSE;
				if (z7==TRUE) 
				{
					test++;
					return rc;
				}
				pthread_rwlock_unlock(&rwlock);
				if(test==10)
				{
					//printf("unlocking the file");
				}
				pthread_rwlock_destroy(&rwlock);
				test++;
				return rc;
			}
		}
		rc = -99;
		test++;
		queue->tail->numWriteIO = 0;
		bool X = TRUE;
		queue->tail->numReadIO++;
		if(X==TRUE)
		{
			//printf("readblock");
		}
		rc = readBlock(pageNum, F_HANDLE, queue->tail->page->data);
		test++;
		NUM_READ_IOS++;
		int gg = NUM_READ_IOS;
		queue->tail->numReadIO--;
		bool z8 = (rc != RC_OK)?TRUE:FALSE;
		if (z8==TRUE) 
		{
			test = 5;
			rc = -99;
			test++;
			rc = closePageFile(F_HANDLE);
			bool z9 = (rc != RC_OK)?TRUE:FALSE;
			if (z9==TRUE) 
			{
				test++;
				return rc;
			}
			test++;
			pthread_rwlock_unlock(&rwlock);
			if(test==6)
			{
				//printf("unlocked");
			}
			pthread_rwlock_destroy(&rwlock);
			test = 0;
			return rc;
		}
		test++;
		queue->tail->page->pageNum = pageNum;
		test++;
		queue->tail->dirtyFlag = FALSE;
		test++;
		queue->tail->clockFlag = FALSE;
		if(test==3)
		{
			//printf("dirty flag and clock is set");
		}
		queue->current = queue->tail;
	}
	int test2=0;
	rc = -99;
	test2++;
	queue->size++;
	test = 50;
	page->data = queue->current->page->data;
	test2 = test;
	page->pageNum = queue->current->page->pageNum;
	if(test2==test)
	{
		//printf("close file");
	}
	rc = closePageFile(F_HANDLE);
	bool z10 = (rc != RC_OK)?TRUE:FALSE;
	if (z10==TRUE) 
	{
		test++;
		return rc;
	}
	pthread_rwlock_unlock(&rwlock);
	if(test==test2+1)
	{
		//printf("end of appending");
	}
	pthread_rwlock_destroy(&rwlock);
	test++;
	return RC_OK;
} 

RC substituePage(BM_BufferPool * const bm, BM_PageHandle * const page,
		PageNumber pageNum) {
	int test = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	int test1 = 0;
	RC rc; 
	rc = -99;
	test++;
	pthread_rwlock_init(&rwlock, NULL);
	if(test==1)
	{
		//printf("lock released replace page");
	}
	pthread_rwlock_wrlock(&rwlock);
	test1++;
	rc = openPageFile(bm->pageFile, F_HANDLE);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		test1++;
		return rc;
	}
	test++;
	queue->current->fixCount = 1;
	if(test==2)
	{
		//printf(fixocunt is updated);
	}
	queue->current->numWriteIO = 1;
	bool x1 = (queue->current->dirtyFlag == TRUE)?TRUE:FALSE;
	if (x1==TRUE) 
	{
		rc = -99;
		test++;
		rc = writeBlock(queue->current->page->pageNum, F_HANDLE,queue->current->page->data);
		if(test==3)
		{
			//printf(writeBlock);
		}
		NUM_WRITE_IOS++;
		bool x2 = (rc != RC_OK)?TRUE:FALSE;
		if (x2==TRUE) 
		{
			test++;
			rc = -99;
			test++;
			rc = closePageFile(F_HANDLE);
			bool x3 = (rc != RC_OK)?TRUE:FALSE;
			if (rc != RC_OK) 
			{
				test++;
				return rc;
			}
			test++;
			pthread_rwlock_unlock(&rwlock);
			if(test==5)
			{
				//printf(return error);
			}
			pthread_rwlock_destroy(&rwlock);
			test++;
			return rc;
		}
		bool yyy =FALSE;
		queue->current->dirtyFlag = FALSE;
		yyy=TRUE;
	}
	bool x4 = (F_HANDLE->totalNumPages < pageNum + 1)?TRUE:FALSE;
	if (x4==TRUE) 
	{
		rc = -99;
		int aa = 0;
		int totalPages = F_HANDLE->totalNumPages;
		aa = totalPages;
		rc = ensureCapacity(pageNum + 1, F_HANDLE);
		aa++;
		NUM_WRITE_IOS += pageNum + 1 - totalPages;
		bool x5 = (rc != RC_OK)?TRUE:FALSE;
		if (x5==TRUE) 
		{
			aa++;
			rc = -99;
			if(aa==totalPages+2)
			{
				//printf("running.....");
			}
			rc = closePageFile(F_HANDLE);
			bool x6 = (rc != RC_OK)?TRUE:FALSE;
			if (x6==TRUE) 
			{
				test++;
				return rc;
			}
			test = 0;
			pthread_rwlock_unlock(&rwlock);
			test++;
			pthread_rwlock_destroy(&rwlock);
			if(test==1)
			{
				//printf("return the code line 434");
			}
			return rc;
		}
	}
	rc = -99;
	test++;
	queue->current->numWriteIO = 0;
	first++;
	queue->current->numReadIO++;
	second++;
	if(first==second)
	{
		//printf("numwrite and num read are updated");
	}
	rc = readBlock(pageNum, F_HANDLE, queue->current->page->data);
	NUM_READ_IOS++;
	test=first+second;
	queue->current->numReadIO--;
	bool x7 = (rc != RC_OK)?TRUE:FALSE;
	if (x7==TRUE) 
	{
		test++;
		rc = -99;
		test++;
		rc = closePageFile(F_HANDLE);
		bool x8 = (rc != RC_OK)?TRUE:FALSE;
		if (x8==TRUE) 
		{
			test++;
			return rc;
		}
		test = 0;
		pthread_rwlock_unlock(&rwlock);
		if(test==0)
		{
			//printf("last one");
		}
		pthread_rwlock_destroy(&rwlock);
		test++;
		return rc;
	}
	rc = -99;
	first++;
	queue->current->page->pageNum = pageNum;
	second++;
	queue->current->clockFlag = FALSE;
	test = first+second;
	page->data = queue->current->page->data;
	test1 = test;
	page->pageNum = queue->current->page->pageNum;
	rc = closePageFile(F_HANDLE);
	bool x9 = (rc != RC_OK)?TRUE:FALSE;
	if (x9==TRUE) 
	{
		return rc;
	}
	first = 0;
	second = 0;
	pthread_rwlock_unlock(&rwlock);
	if(test==test1)
	{
		//printf("The last lock of the funct");
	}
	pthread_rwlock_destroy(&rwlock);
	test = test1 = 0;
	return RC_OK;
}

RC FIFO(BM_BufferPool * const bm, BM_PageHandle * const page,
		PageNumber pageNum) 
{
	RC rc; 
	int test = 0;
	rc = -99;
	PageList *queue = (PageList *) bm->mgmtData;
	int test1 = 0;
	rc = findPage(bm, page, pageNum);
	bool x = (rc == RC_PAGE_FOUND)?TRUE:FALSE;
	bool x1 = (rc != RC_PAGE_NOT_FOUND)?TRUE:FALSE;
	if (x==TRUE) 
	{
		test++;
		queue->current->fixCount++;
		if(test==1)
		{
			//printf("FIFO");
		}
		return rc;
	} 
	else if (x1==TRUE) 
	{
		test1++;
		return rc;
	}
	bool x2 = (queue->size < bm->numPages)?TRUE:FALSE;
	if (x2==TRUE) 
	{
		rc = -99; 
		test++;
		rc = extendPage(bm, page, pageNum);
		if(test!=2)
		{
			//printf("FIFO running.....")
		}
		return rc;
	}
	test1++;
	queue->current = queue->head;
	test++;
	bool X = TRUE;
	while(X==TRUE)
	{
		bool b1 = (queue->current != queue->tail)?TRUE:FALSE;
		bool b2 = (queue->current->fixCount != 0)?TRUE:FALSE;
		bool b3 = (queue->current->numReadIO != 0)?TRUE:FALSE;
		bool b4 = (queue->current->numWriteIO != 0)?TRUE:FALSE;
		if(b1==TRUE && (b2==TRUE || b3==TRUE || b4==TRUE)) 
		{
			test1++;
			queue->current = queue->current->next;
		}
		else
		{
			X=FALSE;
		}
	}
	bool x3 = (queue->current == queue->tail)?TRUE:FALSE;
	bool x4 = (queue->current->fixCount != 0)?TRUE:FALSE;
	bool x5 = (queue->current->numReadIO != 0)?TRUE:FALSE;
	bool x6 = (queue->current->numWriteIO != 0)?TRUE:FALSE;
	if (x3 == TRUE && (x4 == TRUE || x5 == TRUE || x6==TRUE)) 
	{
		test++;
		if(test!=1)
		{
			//printf("No removal Page");
		}
		return RC_NO_REMOVABLE_PAGE;
	}
	test++;
	rc = -99; 
	test1++;
	rc = substituePage(bm, page, pageNum);
	bool x7 = (rc==RC_OK)?TRUE:FALSE;
	bool x8 = (queue->current != queue->tail)?TRUE:FALSE;
	if (x7==TRUE && x8==TRUE) 
	{
		bool x9 = (queue->current == queue->head)?TRUE:FALSE;
		if (x9==TRUE) 
		{
			test++;
			queue->head = queue->head->next;
			test1++;
			queue->current->next->previous = NULL;
			if(test!=2)
			{
				//printf("current==head");
			}
		} 
		else 
		{
			test1++;
			queue->current->previous->next = queue->current->next;
			test++;
			queue->current->next->previous = queue->current->previous;
			if(test1!=2)
			{
				//printf("link is created, running fine");
			}
		}
		int ans = 0;
		queue->current->previous = queue->tail;
		ans++;
		queue->tail->next = queue->current;
		ans++;
		if(ans==2)
		{
			//printf(FIFO implemented);
		}
		queue->current->next = NULL;
		ans++;
		queue->tail = queue->tail->next;
		ans++;
	}
	bool Y = TRUE;
	if(Y==TRUE)
	{
		//printf("returning the RC value.....");
	}
	return rc;
} 

RC LRU(BM_BufferPool * const bm, BM_PageHandle * const page, PageNumber pageNum) 
{
	RC rc; 
	int aa = 0;
	int test =0;
	int test1 = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	aa++;
	rc = -99;
	aa++;
	rc = FIFO(bm, page, pageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	bool x1 = (rc != RC_PAGE_FOUND)?TRUE:FALSE;
	bool x2 = (rc == RC_PAGE_FOUND)?TRUE:FALSE;
	bool x3 = (queue->current != queue->tail)?TRUE:FALSE;
	if (x==TRUE && x1==TRUE) 
	{
		return rc;
	} 
	else if (x2==TRUE && x3==TRUE) 
	{
		bool x4 = (queue->current == queue->head)?TRUE:FALSE;
		if (x4==TRUE) 
		{
			aa++;
			queue->head = queue->head->next;
			aa++;
			if(aa==2)
			{
				//printf("LRU is on...");
			}
			queue->current->next->previous = NULL;
			aa++;
		} 
		else 
		{
			aa++;
			queue->current->previous->next = queue->current->next;
			if(aa==2)
			{
				//printf("previous next to current next");
			}
			aa++;
			queue->current->next->previous = queue->current->previous;
		}
		bool x5 = (queue->size < bm->numPages)?TRUE:FALSE;
		if (x5==TRUE) 
		{
			aa++;
			queue->current->next = queue->tail->next;
			aa++;
			queue->tail->next->previous = queue->current;
		} else 
		{
			if(aa==2)
			{
				//printf("setting next to Null");
			}
			queue->current->next = NULL;
			aa++;
		}
		bool zom = TRUE;
		queue->current->previous = queue->tail;
		if(zom==TRUE)
		{
			queue->tail->next = queue->current;
		}
		queue->tail = queue->tail->next;
		test1=0;
		queue->current = queue->head;
		test = 0;
	}
	return RC_OK;
} 

RC CLOCK(BM_BufferPool * const bm, BM_PageHandle * const page,
		PageNumber pageNum) 
{
	RC rc;
	int aa = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	aa++;
	rc = -99;
	int test = 0;
	rc = findPage(bm, page, pageNum);
	bool x = (rc == RC_PAGE_FOUND)?TRUE:FALSE;
	bool y = (rc != RC_PAGE_NOT_FOUND)?TRUE:FALSE;
	if (x==TRUE) 
	{
		test++;
		queue->current->fixCount++;
		if(test==1)
		{
			//printf("Clock has been initialized");
		}
		queue->current->clockFlag = TRUE;
		aa++;
		return rc;
	} 
	else if (y==TRUE) 
	{
		test++;
		return rc;
	}
	bool x1 = (queue->size < bm->numPages)?TRUE:FALSE;
	if (x1==TRUE) 
	{
		int a = 0;
		rc = -99;
		a++;
		rc = extendPage(bm, page, pageNum);
		bool x2 = (rc == RC_OK)?TRUE:FALSE;
		if (x2==TRUE) 
		{
			bool x3 = (queue->size < bm->numPages)?TRUE:FALSE;
			bool x4 = (queue->size == bm->numPages)?TRUE:FALSE;
			if (x3==TRUE) 
			{
				a++;
				queue->clock = queue->current->next;
			} 
			else if(x4==TRUE) 
			{
				queue->clock = queue->head;
			}
		}
		return rc;
	}
	bool whi = TRUE;
	while(whi==TRUE)
	{
		bool x5 = (queue->clock->numReadIO != 0)?TRUE:FALSE;
		bool x6 = (queue->clock->numWriteIO != 0)?TRUE:FALSE;
		bool x7 = (queue->clock->clockFlag == TRUE)?TRUE:FALSE;
		bool x8 = (queue->clock->fixCount != 0)?TRUE:FALSE;
		if(x5==TRUE || x6==TRUE || x7 == TRUE || x8==TRUE) 
		{
			test++;
			queue->clock->clockFlag = FALSE;
			bool x9 = (queue->clock == queue->tail)?TRUE:FALSE;
			if (x9==TRUE) 
			{
				test++;
				queue->clock = queue->head;
			} 
			else 
			{
				test++;
				queue->clock = queue->clock->next;
			}
		}
		else
		{
			whi = FALSE;
		}
	}
	rc = -99;
	test++;
	queue->current = queue->clock;
	rc = substituePage(bm, page, pageNum);
	bool x10 = (rc == RC_OK)?TRUE:FALSE;
	if (x10==TRUE) 
	{
		test++;
		queue->clock->clockFlag = TRUE;
		bool x11 = (queue->clock == queue->tail)?TRUE:FALSE;
		if (x11==TRUE) 
		{
			test++;
			queue->clock = queue->head;
		} 
		else 
		{
			test++;
			queue->clock = queue->clock->next;
		}
	}
	test++;
	return rc;
} 
void initPageList(BM_BufferPool * const bm) 
{
	int test=0;
	PageList *queue = (PageList *) bm->mgmtData;
	int test1 = 0;
	PageFrame *pf[bm->numPages];
	int i=0;
	while (i < bm->numPages)
	{
		first++;
		pf[i] = (PageFrame *) malloc(sizeof(PageFrame));
		first++;
		pf[i]->page = MAKE_PAGE_HANDLE();
		second++;
		pf[i]->page->data = (char *) malloc(PAGE_SIZE * sizeof(char));
		second++;
		pf[i]->page->pageNum = NO_PAGE;
		third++;
		pf[i]->frameNum = i;
		third++;
		pf[i]->numReadIO = 0;
		forth++;
		pf[i]->numWriteIO = 0;
		forth++;
		pf[i]->fixCount = 0;
		if(first == second)
		{
			//printf("initialization for PageList");
		}
		pf[i]->dirtyFlag = FALSE;
		forth++;
		pf[i]->clockFlag = FALSE;
		third++;
		if(forth==third)
		{
			//printf("dirtyflag and clock flag has been set to False");
		}
		bool x = (i==0)?TRUE:FALSE;
		if (x==TRUE) 
		{
			first++;
			pf[i]->previous = NULL;
			second++;
			pf[i]->next = NULL;
			third++;
		} 
		else 
		{
			first++;
			pf[i - 1]->next = pf[i];
			second++;
			pf[i]->previous = pf[i - 1];
			third++;
			pf[i]->next = NULL;
			forth++;
		}
		i++;
	}
	test++;
	queue->head = pf[0];
	test1++;
	queue->tail = queue->head;
	test++;
	queue->current = queue->head;
	test1++;
	if(test==test1 && first==second)
	{
		//printf(initialization suxxessful);
	}
	queue->clock = queue->head;
	first = second = 0;
	queue->size = 0;
	third = forth = 0;
	test = test1 = 0;;
	return;
} 

RC shutdownBufferPool(BM_BufferPool * const bm) 
{
	int test=0;
	RC rc; 
	PageList *queue = (PageList *) bm->mgmtData;
	first = 100;
	rc = -99;
	test++;
	rc = forceFlushPool(bm);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		test++;
		return rc;
	}
	first--;
	queue->current = queue->tail;
	bool x1 = (bm->numPages == 1)?TRUE:FALSE;
	if (x1==TRUE) 
	{
		first--;
		free(queue->head->page->data);
		if(first == 98)
		{
			//printf("freeing the data values from the queue");
		}
		free(queue->head->page);
		first--;
	} 
	else 
	{
		bool x2 = TRUE;
		while(x2==TRUE)
		{
			bool x3 = (queue->current != queue->head)?TRUE:FALSE;
			if (x3==TRUE)
			{
				first--;
				queue->current = queue->current->previous;
				first--;
				free(queue->current->next->page->data);
				first--;
				free(queue->current->next->page);
				if(first==95)
				{
					//printf("Setting default values");
				}
			}
			else
			{
				x2=FALSE;
			}
		}
		first--;
		free(queue->head->page->data);
		first--;
		free(queue->head->page);
		test = 0;
	}
	bool y2 = TRUE;
	if(y2==TRUE)
	{
		free(queue);
		first--;
		free(F_HANDLE);
		first--;
		free(PAGE_NUMS);
		if(first == 93)
		{
			//printf("shutting down and freeing the global vairable");
		}
		free(DIRTY_FLAGS);
		first--;
		free(FIX_COUNTS);
		first--;
	}
	first = 0;
	return RC_OK;
} 

RC initBufferPool(BM_BufferPool * const bm, const char * const pageFileName,
		const int numPages, ReplacementStrategy strategy, void *stratData) 
{
	printf("Buffer Manager initialized...\n");
	int test = 0;
	bool x = (numPages <= 0)?TRUE:FALSE;
	if (x==TRUE) 
	{
		test++;
		return RC_INVALID_NUMPAGES;
	}
	bool y = TRUE;
	if(y==TRUE)
	{
		test++;
		initStorageManager();
		first++;
		F_HANDLE = (SM_FileHandle *) malloc(sizeof(SM_FileHandle));
		second++;
		PAGE_NUMS = (PageNumber *) malloc(bm->numPages * sizeof(PageNumber));
		third++;
		DIRTY_FLAGS = (bool *) malloc(bm->numPages * sizeof(bool));
		forth++;
		FIX_COUNTS = (int *) malloc(bm->numPages * sizeof(int)); 
		fifth++;
		NUM_READ_IOS = 0;
		first++;
		NUM_WRITE_IOS = 0;
		second++;
		bm->pageFile = (char *) pageFileName;
		third++;
		bm->numPages = numPages;
		forth++;
		bm->strategy = strategy;
		fifth++;
		PageList *queue = (PageList *) malloc(sizeof(PageList));
		if(first==second && third == forth)
		{
			//printf("Buff initialized");
		}
		bm->mgmtData = queue;
		test = 0;
		initPageList(bm);
	}
	first = second = third = forth = fifth = 0;
	return RC_OK;
} 

int uwc = 0;
RC forceFlushPool(BM_BufferPool * const bm) 
{
	int test = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	first = 50;
	int unwritableCount = 0;
	uwc = unwritableCount;
	queue->current = queue->head;
	bool x = TRUE;
	while(x==TRUE)
	{
		bool x1 = (queue->current != queue->tail)?TRUE:FALSE;
		if(x1==TRUE) 
		{
			bool x2 = (queue->current->dirtyFlag == TRUE)?TRUE:FALSE;
			bool x3 = (queue->current->fixCount > 0)?TRUE:FALSE;
			bool x4 = (queue->current->dirtyFlag == TRUE)?TRUE:FALSE;
			bool x5 = (queue->current->fixCount == 0)?TRUE:FALSE;
			if (x2==TRUE && x3==TRUE) 
			{
				first--;
				unwritableCount++;
				uwc = unwritableCount;
			} 
			else if (x4==TRUE && x5==TRUE) 
			{
				first--;
				forcePage(bm, queue->current->page);
				uwc++;
			}
			first--;
			queue->current = queue->current->next;
		}
		else
		{
			x=FALSE;
		}
	}
	bool x6 = (queue->current == queue->tail)?TRUE:FALSE;
	bool x7 = (queue->current->dirtyFlag == TRUE)?TRUE:FALSE;
	bool x8 = (queue->current->fixCount > 0)?TRUE:FALSE;
	bool x9 = (queue->current->dirtyFlag == TRUE)?TRUE:FALSE;
	bool x10 = (queue->current->fixCount == 0)?TRUE:FALSE;
	if (x6==TRUE) 
	{
		if (x7==TRUE && x8==TRUE) 
		{
			first--;
			unwritableCount++;
			uwc = unwritableCount;
		} 
		else if (x9==TRUE && x10==TRUE) 
		{
			first--;
			forcePage(bm, queue->current->page);
			uwc++;
		}
	}
	bool x11 = (unwritableCount != 0)?TRUE:FALSE;
	if (x11==TRUE) 
	{
		first--;
		return RC_FLUSH_POOL_ERROR;
	}
	first = 0;
	uwc = 0;
	return RC_OK;
} 
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
RC markDirty(BM_BufferPool * const bm, BM_PageHandle * const page) 
{
	int test = 0;
	RC rc;
	PageList *queue = (PageList *) bm->mgmtData;
	rc = -99;
	first++;
	rc = findPage(bm, page, page->pageNum);
	bool x = (rc != RC_PAGE_FOUND)?TRUE:FALSE;
	if (x==TRUE) 
	{
		test++;
		return rc;
		first++;
	}
	if(first==1)
	{
		//printf("mark dirty");
	}
	queue->current->dirtyFlag = TRUE;
	first = 0;
	return RC_OK;
} 
RC unpinPage(BM_BufferPool * const bm, BM_PageHandle * const page) 
{
	RC rc;
	first = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	int test = 0;
	rc = -99;
	test++;
	rc = findPage(bm, page, page->pageNum);
	bool x = (rc != RC_PAGE_FOUND)?TRUE:FALSE;
	if (x==TRUE) 
	{
		first++;
		return rc;
		test++;
	}
	if(first==0)
	{
		//printf("Unpining the page by reducing the fux count");
	}
	queue->current->fixCount--;
	first = 0;
	test = 0;
	return RC_OK;
} 
RC forcePage(BM_BufferPool * const bm, BM_PageHandle * const page) 
{
	bool X = TRUE;
	int test = 0;
	first = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	first++;
	RC rc;
	pthread_rwlock_init(&rwlock, NULL);
	first++;
	if(first==2)
	{
		//printf("unlicking th pages");
	}
	pthread_rwlock_wrlock(&rwlock);
	rc = -99;
	test++;
	rc = openPageFile(bm->pageFile, F_HANDLE);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc;
	}
	rc = -99;
	first++;
	queue->current->numWriteIO = 1;
	test++;
	rc = writeBlock(page->pageNum, F_HANDLE, page->data);
	if(first==3)
	{
		//printf("write block has been called from the storage manager");
	}
	NUM_WRITE_IOS++;
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		test++;
		rc = -99;
		if(test==4)
		{
			//printf("CHecking for closePage");
		}
		rc = closePageFile(F_HANDLE);
		bool z = (rc != RC_OK)?TRUE:FALSE;
		if (z==TRUE) 
		{
			test++;
			return rc;
			first++;
		}
		test = first;
		bool x1 = TRUE;
		if(x1==TRUE)
		{
			test++;
			pthread_rwlock_unlock(&rwlock);
			first++;
			pthread_rwlock_destroy(&rwlock);
			if(first==test)
			{
				//printf("return the RC code");
			}
		}
		return rc;
	}
	rc = -99;
	test++;
	queue->current->numWriteIO = 0;
	first++;
	queue->current->dirtyFlag = FALSE;
	rc = closePageFile(F_HANDLE);
	bool ee = (rc != RC_OK)?TRUE:FALSE;
	if (ee==TRUE) 
	{
		first--;
		return rc;
	}
	first--;
	pthread_rwlock_unlock(&rwlock);
	test = 0;
	pthread_rwlock_destroy(&rwlock);
	first = 0;
	return RC_OK;
} 

PageNumber *getFrameContents(BM_BufferPool * const bm) 
{
	int test = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	first++;
	queue->current = queue->head;
	bool X = TRUE;
	int pos = 0;
	int a = pos;
	while(X==TRUE)
	{
		bool x1 = (queue->current != queue->tail)?TRUE:FALSE;
		if (x1==TRUE) 
		{
			a++;
			PAGE_NUMS[pos] = queue->current->page->pageNum;
			test++;
			queue->current = queue->current->next;
			pos++;
		}
		else
		{
			X=FALSE;
		}
	}
	test++;
	PAGE_NUMS[pos++] = queue->current->page->pageNum;
	bool x2 = (pos < bm->numPages)?TRUE:FALSE;
	if (x2==TRUE) 
	{
		int i=pos;
		while(i < bm->numPages) 
		{
			PAGE_NUMS[i] = NO_PAGE;
			i++;
		}
	}
	test++;
	queue->current = queue->tail;
	first = 0;
	test = 0;
	return PAGE_NUMS;
} 
bool *getDirtyFlags(BM_BufferPool * const bm) 
{
	int test = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	first = 0;
	int pos = 0;
	queue->current = queue->head; 
	bool X = TRUE;
	while(X==TRUE)
	{
		bool x1 = (queue->current != queue->tail)?TRUE:FALSE;
		if (x1==TRUE) 
		{
			first++;
			DIRTY_FLAGS[pos] = queue->current->dirtyFlag;
			test++;
			queue->current = queue->current->next;
			pos++;
		}
		else 
		{
			X = FALSE;
		}
	}
	test++;
	first++;
	DIRTY_FLAGS[pos++] = queue->current->dirtyFlag;
	bool y = (pos < bm->numPages)?TRUE:FALSE;
	if (y==TRUE) 
	{
		int i=pos;
		while (i < bm->numPages) 
		{
			first++;
			DIRTY_FLAGS[i] = FALSE;
			i++;
		}
	}
	test++;
	queue->current = queue->tail;
	first = 0;
	return DIRTY_FLAGS;
} 
int *getFixCounts(BM_BufferPool * const bm) 
{
	int test = 0;
	PageList *queue = (PageList *) bm->mgmtData;
	first++;
	queue->current = queue->head;
	test++;
	int pos = 0;
	bool XX = TRUE;
	while(XX==TRUE)
	{
		bool x1 = (queue->current != queue->tail)?TRUE:FALSE;
		if(x1==TRUE) 
		{
			first++;
			FIX_COUNTS[pos] = queue->current->fixCount;
			test++;
			queue->current = queue->current->next;
			if(first==test)
			{
				//printf("Current value is TAIL");
			}
			pos++; 
			test = pos;
		}
		else
		{
			XX=FALSE;
		}
	}
	first++;
	FIX_COUNTS[pos++] = queue->current->fixCount;
	bool x2 = (pos < bm->numPages)?TRUE:FALSE;
	if (x2==TRUE) 
	{
		int i=pos;
		while (i < bm->numPages) 
		{
			first++;
			FIX_COUNTS[i] = 0;
			i++;
		}
	}
	first++;
	queue->current = queue->tail;
	test = 0;
	first = 0;
	return FIX_COUNTS;
}
int getNumReadIO(BM_BufferPool * const bm) 
{
	int r = 0;
	r = NUM_READ_IOS;
	return r;
} 
int getNumWriteIO(BM_BufferPool * const bm) 
{
	int w = 0;
	w = NUM_WRITE_IOS;
	return w;
}
