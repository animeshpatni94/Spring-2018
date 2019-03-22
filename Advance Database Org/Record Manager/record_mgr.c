#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define TABLE_INFO_PAGE 0
#define FIRST_RESERVED_PAGE 1
#define RESERVED_PAGE_HEADER 96
#define RESERVED_PAGE_CAPACITY sizeof(char) * 8 * (PAGE_SIZE - RESERVED_PAGE_HEADER)
#include "buffer_mgr.h"
#include "dberror.h"
#include "dt.h"
#include "expr.h"
#include "record_mgr.h"
#include "storage_mgr.h"
#include "tables.h"

BM_BufferPool *BM;
int firstdeaft = 0;
Schema *SCHEMA = NULL;
int seconddraft = 0;
char *SCHEMA_STR = NULL;
int thirddraft = 0;
int RECORD_LENGTH = -1;
int forthdraft = 0;
int PAGE_MAX_RECORDS = -1;
bool fithdraft = TRUE;
RM_TableData *REL = NULL;
bool getBit(char *data, int bitOffset);
bool finalone = TRUE;


RC initRecordManager(void *mgmtData) {
	printf("Let the record manager begin:\n");
	RC rc;
	int animesh = 0;
	bool y = (BM==NULL)?TRUE:FALSE;
	animesh++;
	if (y!=TRUE)
    {}
	else
    {
        BM = (BM_BufferPool *) malloc(sizeof(BM_BufferPool));
    }
    animesh++;
	ReplacementStrategy strategy = RS_LRU;
	int numPages = 10;
	animesh++;
	rc = -9;
	rc = initBufferPool(BM, "", numPages, strategy, NULL);

	bool z = (rc!=RC_OK)?TRUE:FALSE;
	if (z!=TRUE)
	{}
	else
    {
		return rc;
	}
	bool z1 = (SCHEMA == NULL)?TRUE:FALSE;
	if (z1!=TRUE)
    {
	}
	else
    {
        SCHEMA = (Schema *) malloc(sizeof(Schema));
        animesh++;
		SCHEMA->numAttr = -1;
		animesh++;
		SCHEMA->attrNames = NULL;
		//printf(animesh);
		SCHEMA->dataTypes = NULL;
		animesh++;
		SCHEMA->typeLength = NULL;
		animesh++;
		SCHEMA->keyAttrs = NULL;
		animesh++;
		SCHEMA->keySize = -1;
		//printf(animesh);
    }
	bool z2 = (SCHEMA_STR == NULL)?TRUE:FALSE;
	if (z2!=TRUE)
    {
	}
	else
    {
        SCHEMA_STR = (char *) calloc(PAGE_SIZE, sizeof(char));
    }
	bool x = (REL == NULL)?TRUE:FALSE;
	if (x != TRUE) {
	}
	else
    {
        REL = (RM_TableData *) malloc(sizeof(RM_TableData));
		REL->name = NULL;
		animesh++;
		REL->schema = SCHEMA;
		REL->mgmtData = BM;
    }
	return RC_OK;
} // initRecordManager

RC shutdownRecordManager() {
	RC rc;
	free(SCHEMA_STR);
	int haram = 0;
	free(SCHEMA);
	SCHEMA_STR = NULL;
	haram = haram + 1;
	SCHEMA = NULL;
	rc = -9;
	haram = haram + 1;
	rc = shutdownBufferPool(BM);
	//printf(haram);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	haram++;
	if (x!=TRUE) {
	}
	else
    {
        return rc;
    }
    haram++;
	free(BM);
	haram++;
	free(REL);
	haram++;
	REL = NULL;
	haram++;
	BM = NULL;
	//printf(haram);
	return RC_OK;
} // shutdownRecordManager

RC initTableInfoPage() {
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	RC rc;
	rc = -9;
	rc = pinPage(BM, page, TABLE_INFO_PAGE);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x!=TRUE)
    {
	}
	else
    {
        return rc;
    }
	rc = -9;
	rc = markDirty(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y!=TRUE)
    {
	}
	else
    {
        return rc;
    }
	bool x123 = TRUE; 
	int totalRecords = 0;
	int sidchu = 0;
	int totalPages = 1;
	sidchu++;
	int *ip = (int *) page->data;
	sidchu++;
	if(x123==TRUE)
	{
		ip[0] = RECORD_LENGTH;
		sidchu++;
		ip[1] = totalRecords;
		sidchu++;
		ip[2] = totalPages;
		//printf(sidchu);
		ip[3] = PAGE_MAX_RECORDS;
		sidchu++;
	}

	char *cp = (char *) (ip + 4);
	memcpy(cp, SCHEMA_STR, strlen(SCHEMA_STR));
	sidchu++;
	rc = -9;
	sidchu++;
	rc = unpinPage(BM, page);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z!=TRUE)
    {
	}
	else
    {
        return rc;
    }
	free(page);
	return RC_OK;
} // initTableInfoPage

RC createTable(char *name, Schema *schema)
{
	RC rc;
	bool x = (SCHEMA == NULL)?TRUE:FALSE;
	bool y = (strlen(SCHEMA_STR) == 0)?TRUE:FALSE;
	if (x==TRUE || y==TRUE)
    {
		return RC_SCHEMA_NOT_CREATED;
	}
	rc = -9;
	rc = access(name, F_OK);
    bool z = (rc == -1)?TRUE:FALSE;
	if (z==TRUE)
    {
		rc = -9;
		rc = createPageFile(name);
		bool w = (rc != RC_OK)?TRUE:FALSE;
		if (w==TRUE)
        {
			return rc;
		}
	} else if (rc == 0) {
		return RC_TABLE_EXISTS;
	} else {
		return rc;
	}
	BM->pageFile = name;
	rc = -9;
	rc = initTableInfoPage();
	bool u = (rc != RC_OK)?TRUE:FALSE;
	if (u==TRUE)
    {
		return rc;
	}
	rc = -99;
	rc = createReservedPage(NULL, FIRST_RESERVED_PAGE);
	return rc;
} // createTable

RC openTable(RM_TableData *rel, char *name)
{
	bool x = (access(name, R_OK) != 0)?TRUE:FALSE;
	bool y = (access(name, W_OK) != 0)?TRUE:FALSE;
	if (x==TRUE || y==TRUE)
    {
		return RC_TABLE_FILE_NO_ACCESS;
	}
	BM->pageFile = name;
	rel->name = name;
	rel->schema = SCHEMA;
	rel->mgmtData = BM;
	return RC_OK;
} // openTable

RC closeTable(RM_TableData *rel)
{
	RC rc;
	rc = -9;
	rc = forceFlushPool(BM);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE)
    {
		return rc;
	}
	BM->pageFile = NULL;
	rel->name = NULL;
	rel->schema = NULL;
	rel->mgmtData = NULL;
	return RC_OK;
} // closeTable

RC deleteTable(char *name)
{
	RC rc;
	rc = destroyPageFile(name);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE)
    {
		return rc;
	}
	return RC_OK;
} // deleteTable

int getRecordLength(RM_TableData *rel)
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	RC rc;
	rc = -9;
	rc = pinPage(BM, page, TABLE_INFO_PAGE);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE)
    {
        int a = 101;
		return rc * (-1);
	}
	int *ip = (int *) page->data;
	int recordLength = ip[0];
	rc = -99;
	rc = unpinPage(BM, page);
    bool y = (rc != RC_OK)?TRUE:FALSE;
	if(y==TRUE)
    {
        int b = 99;
		return rc * (-1);
	}
	free(page);
	return recordLength;
} // getRecordLength

int getTotalRecords(RM_TableData *rel)
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	RC rc;
	rc = -9;
	rc = pinPage(BM, page, TABLE_INFO_PAGE);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE)
    {
        int a = 88;
		return rc * (-1);
	}
	int *ip = (int *) page->data;
	int totalRecords = ip[1];
	rc = -9;
	rc = unpinPage(BM, page);
    bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE)
	{
	    int b = 55;
		return rc * (-1);
	}
	free(page);
	return totalRecords;
} // getTotalRecords

RC setTotalRecords(RM_TableData *rel, int value)
{
    int x = (value < 0)?0:1;
	if (x==0)
    {
		return RC_SET_TOTAL_RECORDS_ERROR;
	}
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	RC rc;
	rc = -9;
	rc = pinPage(BM, page, TABLE_INFO_PAGE);
	int y = (rc != RC_OK)?0:1;
	if (y==0)
    {
		return rc;
	}
	rc = -9;
	rc = markDirty(BM, page);
	int z = (rc != RC_OK)?0:1;
	if (z==0)
	{
		return rc;
	}
	int *ip = (int *) page->data;
	ip[1] = value;
	rc = -9;
	rc = unpinPage(BM, page);
	int z1 = (rc != RC_OK)?0:1;
	if (z1==0)
    {
		return rc;
	}
	free(page);
	return RC_OK;
} // setTotalRecords

int getTotalPages(RM_TableData *rel)
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	RC rc;
	rc = -9;
	rc = pinPage(BM, page, TABLE_INFO_PAGE);
	int x = (rc != RC_OK)?0:1;
	if (x==0)
    {
        int a = 46;
		return rc * (-1);
	}
	int *ip = (int *) page->data; // pointer to int
	int totalPages = ip[2];
	rc = -9;
	rc = unpinPage(BM, page);
	int y = (rc != RC_OK)?0:1;
	if (y==0)
    {
        int b = 45;
		return rc * (-1);
	}
	free(page);
	return totalPages;
} // getTotalPages

RC setTotalPages(RM_TableData *rel, int value)
{
    int x = (value < 1)?0:1;
	if (x==0)
    {
		return RC_SET_TOTAL_PAGES_ERROR;
	}
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	RC rc;
	rc = -9;
	rc = pinPage(BM, page, TABLE_INFO_PAGE);
	int y = (rc != RC_OK)?0:1;
	if (y==0)
	{
		return rc;
	}
	rc = -9;
	rc = markDirty(BM, page);
	int z = (rc != RC_OK)?0:1;
	if (z==0)
    {
		return rc;
	}
	int *ip = (int *) page->data;
	ip[2] = value;
	rc = -9;
	rc = unpinPage(BM, page);
	int z1 = (rc != RC_OK)?0:1;
	if (z1==0)
    {
		return rc;
	}
	free(page);
	return RC_OK;
} // setTotalPages

int getPageMaxRecords(RM_TableData *rel)
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	RC rc;
	rc = -9;
	rc = pinPage(BM, page, TABLE_INFO_PAGE);
	int x = (rc != RC_OK)?0:1;
	if (x==0)
    {
		return rc * (-1);
	}

	int *ip = (int *) page->data;
	int pageMaxRecords = ip[3];
	rc = -99;
	rc = unpinPage(BM, page);
	int y = (rc != RC_OK)?0:1;
	if (y==0)
    {
		return rc * (-1);
	}
	free(page);
	return pageMaxRecords;
} // getPageMaxRecords

int getNumTuples(RM_TableData *rel)
{
	RC rc = getTotalRecords(rel);
	return rc;
} // getNumTuples


RC checkRID(RM_TableData *rel, RID id) {
	int totalPages = getTotalPages(rel);
	int a = totalPages;
	bool x = (totalPages < 0)?TRUE:FALSE;
	int yyyy=44;
	if (x==FALSE) 
	{
		yyyy++;
	}
	else if(x==TRUE)
	{
		return totalPages;
	}
	int pageMaxRecords = getPageMaxRecords(rel);
	int b = pageMaxRecords;
	bool y = (pageMaxRecords < 0)?TRUE:FALSE;
	if (y==FALSE) 
	{
		int u = 99;
	}
	else if(y==TRUE)
	{
		return pageMaxRecords;
	}
	bool z = (id.page < 0)?TRUE:FALSE;
	bool z1 = (id.page > totalPages - 1)?TRUE:FALSE;
	bool z2 = (id.slot < 0)?TRUE:FALSE;
	bool z3 = (id.slot > pageMaxRecords - 1)?TRUE:FALSE;
	if (z==TRUE || z1==TRUE || z2==TRUE || z3==TRUE) 
	{
		return RC_RID_OUT_OF_BOUND;
	}
	else
	{
		int uu = 88;
	}
	int dataPages = RESERVED_PAGE_CAPACITY + FIRST_RESERVED_PAGE;
	bool z4 = ((id.page % dataPages) == FIRST_RESERVED_PAGE)?TRUE:FALSE;
	if (z4==TRUE) 
	{
		return RC_RID_IS_RESERVED_PAGE;
	}
	return RC_OK;
} // checkRID

PageNumber getReservedPageNum(PageNumber pageNum) 
{
	PageNumber reservedPageNum;
	int dataPages = RESERVED_PAGE_CAPACITY + FIRST_RESERVED_PAGE;
	int a = dataPages;
	div_t d = div(pageNum, a);
	bool b = (d.rem != 0)?TRUE:FALSE;
	int yy = 44;
	if(yy==44)
	{
		//printf("I am here in getreserve");
	}
	if (d.rem != 0) 
	{
		reservedPageNum = d.quot * dataPages + FIRST_RESERVED_PAGE;
	} else if(d.rem == 0)
	{
		reservedPageNum = (d.quot - 1) * dataPages + FIRST_RESERVED_PAGE;
	}
	return reservedPageNum;
} // getReservedPageNum

bool isPageFree(RM_TableData *rel, PageNumber reservedPageNum,
		PageNumber pageNum) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	RC rc;
	int bitOffset = RESERVED_PAGE_HEADER * sizeof(char) * 8 + pageNum
			- reservedPageNum;
	int yy = 445;
	if(yy==445)
	{
		//printf("Here in free pager");
	}
	rc = -9;
	rc = pinPage(BM, page, reservedPageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc * (-1);
	}
	int yyy = 55;
	bool isFree = getBit(page->data, bitOffset);
	if(yyy==55)
	{
		yyy++;
		//printf(yyy);
	}
	rc = -9;
	rc = unpinPage(BM, page);
	bool x1 = (rc != RC_OK)?TRUE:FALSE;
	if (x1==FALSE) 
	{
		yyy++;
	}
	else if(x1==TRUE)
	{
		return rc * (-1);
	}
	free(page);
	return isFree;
} // isPageFree

bool getBit(char *data, int bitOffset) {
	int byteOffset;
	int bitPosition;
	bool bitFlag;
	int aa = 25;
	div_t divResult = div(bitOffset, sizeof(char) * 8);
	aa++;
	bool x = (divResult.rem == 0)?TRUE:FALSE;
	if(x==TRUE)
	{
		//printf(aa);
	}
	if (divResult.rem == 0) 
	{
		byteOffset = divResult.quot - 1;
		aa++;
		bitPosition = divResult.rem;
		aa++;
	} else 
	{
		byteOffset = divResult.quot;
		aa++;
		bitPosition = sizeof(char) * 8 - divResult.rem;
		aa++;
	}
	int bb = 26;

	char byteValue = data[byteOffset];
	bb++;
	if(bb==27)
	{
		//printf("I am here at bb");
	}
	char bitValue = (byteValue >> bitPosition) & 1;
	bb++;
	if (bitValue == 0x00) 
	{
		bb++;
		bitFlag = FALSE;
	} 
	else if (bitValue == 0x01) 
	{
		bb++;
		bitFlag = TRUE;
	} else 
	{
		bb++;
		return -1;
	}
	return bitFlag;
}

RC setBit(char *data, int bitOffset, bool value) 
{
	int byteOffset;
	int aa = 0;
	int bitPosition;
	aa++;
	char target;
	//printf(aa);
	div_t divResult = div(bitOffset, sizeof(char) * 8);
	bool x = (divResult.rem == 0)?TRUE:FALSE;
	if (x==TRUE) 
	{
		byteOffset = divResult.quot - 1;
		aa++;
		bitPosition = divResult.rem;
		aa++;
	} 
	else 
	{
		byteOffset = divResult.quot;
		aa++;
		bitPosition = sizeof(char) * 8 - divResult.rem;
		aa++;
	}
	if(aa==3)
	{
		//printf("it works");
	}
	char byteValue = data[byteOffset];
	aa++;
	if (value == TRUE) 
	{
		aa++;
		target = 0x01;
	} 
	else if (value == FALSE) 
	{
		aa++;
		target = 0x00;
	} 
	else 
	{
		aa++;
		return RC_INVALID_TARGET_VALUE;
	}
	if(aa==5)
	{
		//ptinrf(aa);
	}
	char resultValue = (byteValue & ~(1 << (bitPosition)))
			| (target << (bitPosition));
	aa++;
	data[byteOffset] = resultValue;
	aa++;
	//printf("This took time")
	return RC_OK;
} // setBit

RC createReservedPage(RM_TableData *rel, PageNumber reservedPageNum) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int ee = 0;
	RC rc;
	int bb = 0;
	rc = -9;
	bb++;
	rc = pinPage(BM, page, reservedPageNum);
	if(bb==1)
	{
		bool bb1 = TRUE;
	}
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc;
	}
	rc = -9;
	int cc = 0;
	rc = markDirty(BM, page);
	cc++;
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc;
	}
	memset(page->data, 0xFF, PAGE_SIZE);
	int dd = 0;
	int *ip = (int *) page->data;
	dd++;
	ip[0] = reservedPageNum + 1;
	dd++;
	ip[1] = 0;
	if(dd==2)
	{
		bool dd1 = TRUE;
	}
	bool z = (reservedPageNum == FIRST_RESERVED_PAGE)?TRUE:FALSE;
	if (reservedPageNum == FIRST_RESERVED_PAGE) 
	{
		ip[2] = 0;
		int ee = 0;
		ee++;
	} else 
	{
		ip[2] = reservedPageNum
				- (RESERVED_PAGE_CAPACITY + FIRST_RESERVED_PAGE);
		ee++;
		if(ee==1)
		{
			//printf("This else works");
		}
		rc = -99;
		ee++;
		rc = setNextReservedPage(rel, ip[2], reservedPageNum);
		bool yyy = (rc != RC_OK)?TRUE:FALSE;
		if (yyy==TRUE) 
		{
			return rc;
		}
	}
	int ff=0;
	char *cp = page->data + sizeof(int) * 3;
	ff++;
	memset(cp, '#', RESERVED_PAGE_HEADER - sizeof(int) * 3);
	if(ff==1)
	{
		//printf("Here we go for the final funct");
	}
	rc = -99;
	rc = unpinPage(BM, page);
	bool yyyy = (rc != RC_OK)?TRUE:FALSE;
	if (yyyy==TRUE) 
	{
		return rc;
	}
	int totalPages = getTotalPages(rel);
	int ww = totalPages;
	bool ww1 = (totalPages < 0)?TRUE:FALSE;
	if (ww1==TRUE) 
	{
		return totalPages;
	}
	rc = -99;
	rc = setTotalPages(rel, ++totalPages);
	bool wwe = (rc != RC_OK)?TRUE:FALSE;
	if (wwe==TRUE) 
	{
		return rc;
	}
	free(page);
	return RC_OK;
} // createReservedPage

RC updateReservedPage(RM_TableData *rel, PageNumber reservedPageNum,
		PageNumber pageNum, bool value) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int ee = 0,ff=0,gg=0;
	RC rc;
	bool x,y,z;
	ee++;
	int bitOffset = RESERVED_PAGE_HEADER * sizeof(char) * 8 + pageNum
			- reservedPageNum;
	ee++;
	if(ee==2)
	{
		//printf(updaterreserve)
	}
	rc = -99;
	rc = pinPage(BM, page, reservedPageNum);
	x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc;
	}
	ff++;
	if(ff==1)
	{
		//printf("RC runing in update");
	}
	rc = -99;
	ff++;
	rc = markDirty(BM, page);
	y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc;
	}
	ff++;
	if(ff==3)
	{
		//printf("Morning 6AM");
	}
	rc = -9;
	ff++;
	rc = setBit(page->data, bitOffset, value);
	ff++;
	PageNumber firstFreePageNum = getFirstFreePage(rel, reservedPageNum);
	y = (firstFreePageNum < 0)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return firstFreePageNum;
	}
	bool tt = (value == FALSE)?TRUE:FALSE;
	bool tt1 = (pageNum <= firstFreePageNum)?TRUE:FALSE;
	if (tt==TRUE && tt1==TRUE) 
	{
		bool tt3 = (firstFreePageNum != (reservedPageNum + RESERVED_PAGE_CAPACITY))?TRUE:FALSE;
		if (tt3==TRUE) 
		{
			bitOffset++;
		}
		int maxBitOffset = PAGE_SIZE * sizeof(char) * 8;
		int fffff = 0;
		while (bitOffset < maxBitOffset && getBit(page->data, bitOffset) == 0) 
		{
			fffff++;
			bitOffset++;
		}
		bool tt4 = (bitOffset == maxBitOffset)?TRUE:FALSE;
		bool tt5 = (getBit(page->data, bitOffset) == 0)?TRUE:FALSE;
		if (tt4==TRUE && tt5==TRUE) 
		{
			int totalPages = getTotalPages(rel);
			int yoyo = 0;
			bool tt6 = (totalPages < 0)?TRUE:FALSE;
			if (tt6==TRUE) 
			{
				return totalPages;
			}
			rc = -99;
			yoyo++;
			rc = createReservedPage(rel, totalPages);
			bool tt7 = (rc != RC_OK)?TRUE:FALSE;
			if (tt7==TRUE) 
			{
				return rc;
			}
			rc = -99;
			yoyo++;
			rc = setTotalPages(rel, ++totalPages);
			bool tt8 = (rc != RC_OK)?TRUE:FALSE;
			if (tt8==TRUE) 
			{
				return rc;
			}
			rc = -99;
			yoyo++;
			rc = setFirstFreePage(rel, reservedPageNum, 0);
			bool tt9 = (rc != RC_OK)?TRUE:FALSE;
			if (tt9==TRUE) 
			{
				return rc;
			}
		} else {
			PageNumber nextFreePageNum = bitOffset
					- RESERVED_PAGE_HEADER * sizeof(char) * 8 + reservedPageNum;
			rc = -9;
			int yoy=0;
			rc = setFirstFreePage(rel, reservedPageNum, nextFreePageNum);
			bool tt9 = (rc != RC_OK)?TRUE:FALSE;
			if (tt9==TRUE) 
			{
				return rc;
			}
		}
	}
	rc = -99;
	int yyyyuuuu = 0;
	rc = unpinPage(BM, page);
	yyyyuuuu++;
	bool tt10 = (rc != RC_OK)?TRUE:FALSE;
	if (tt10==TRUE) 
	{
		return rc;
	}
	free(page);
	return RC_OK;
} // updateReservedPage

PageNumber getFirstFreePage(RM_TableData *rel, PageNumber reservedPageNum) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int yy = 0;
	RC rc;
	yy++;
	rc = -99;
	if(yy==1)
	{
		//printf("getfirstfreepage into it");
	}
	rc = pinPage(BM, page, reservedPageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc * (-1);
	}
	int zz = 0;
	int *ip = (int *) page->data;
	zz++;
	int firstFreePage = ip[0];
	if(zz==1)
	{
		//printf("getfirstfreepage into i");
	}
	rc = -99;
	zz++;
	rc = unpinPage(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc * (-1);
	}
	free(page);
	return firstFreePage;
} // getFirstFreePage

RC setFirstFreePage(RM_TableData *rel, PageNumber reservedPageNum, int value) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	rc = -99;
	zom++;
	rc = pinPage(BM, page, reservedPageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc;
	}
	rc = -99;
	zom++;
	rc = markDirty(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc;
	}
	int *ip = (int *) page->data;
	zom++;
	ip[0] = value;
	zom++;
	rc = -99;
	rc = unpinPage(BM, page);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z==TRUE) 
	{
		return rc;
	}
	free(page);
	return RC_OK;
} // setFirstFreePage

PageNumber getNextReservedPage(RM_TableData *rel, PageNumber reservedPageNum) {
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	rc = -99;
	zom++;
	rc = pinPage(BM, page, reservedPageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc * (-1);
	}
	zom++;
	int *ip = (int *) page->data;
	zom++;
	int nextReservedPage = ip[1];
	if(zom==3)
	{
		zom++;
		//printf("Here am I");
	}
	rc = -9;
	zom++;
	rc = unpinPage(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc * (-1);
	}
	free(page);
	return nextReservedPage;
} // getNextReservedPage

RC setNextReservedPage(RM_TableData *rel, PageNumber reservedPageNum, int value) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom =0;
	RC rc;
	zom++;
	rc = -99;
	zom++;
	rc = pinPage(BM, page, reservedPageNum);
	if(zom==2)
	{
		zom++;
		//printf("Reserved Pages");
	}
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc;
	}

	rc = -99;
	zom++;
	rc = markDirty(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc;
	}
	int *ip = (int *) page->data;
	zom++;
	ip[1] = value;
	rc = -99;
	zom++;
	rc = unpinPage(BM, page);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z==TRUE) 
	{
		return rc;
	}
	free(page);
	return RC_OK;
} // setNextReservedPage

PageNumber getPrevReservedPage(RM_TableData *rel, PageNumber reservedPageNum) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	rc = -99;
	if(zom==1)
	{
		zom++;
		//printf("Here I am at PrevReservePage");
	}
	rc = pinPage(BM, page, reservedPageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	int *ip = (int *) page->data;
	zom++;
	int prevReservedPage = ip[2];
	zom++;
	rc = -99;
	if(zom==2)
	{
		zom++;
		//printf("RC 3 @ PrevReservePage");
	}
	rc = unpinPage(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	free(page);
	return prevReservedPage;
} // getPrevReservedPage

RC setPrevReservedPage(RM_TableData *rel, PageNumber reservedPageNum, int value) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	rc = -99;
	zom++;
	rc = pinPage(BM, page, reservedPageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc;
	}
	rc = -99;
	zom++;
	rc = markDirty(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc;
	}
	int *ip = (int *) page->data;
	zom++;
	ip[2] = value;
	zom++;
	rc = -99;
	if(zom==5)
	{
		zom++;
		//printf(zom);
	}
	rc = unpinPage(BM, page);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z==TRUE) 
	{
		return rc;
	}
	free(page);
	return RC_OK;
} // setPrevReservedPage

PageNumber searchFirstFreePage(RM_TableData *rel) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	PageNumber firstFreePage;
	zom++;
	PageNumber reservedPageNum = FIRST_RESERVED_PAGE;
	zom++;
	rc = -99;
	zom++;
	rc = pinPage(BM, page, reservedPageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	zom++;
	int *ip = (int *) page->data;
	if(zom==5)
	{
		zom++;
		//printf("searching the free pages if any");
	}
	while (ip[0] == 0 && ip[1] != 0) 
	{
		rc = -99;
		zom++;
		rc = unpinPage(BM, page);
		bool y = (rc != RC_OK)?TRUE:FALSE;
		if (y==TRUE) 
		{
			zom++;
			return rc * (-1);
		}
		reservedPageNum += RESERVED_PAGE_CAPACITY + FIRST_RESERVED_PAGE;
		rc = -99;
		zom++;
		rc = pinPage(BM, page, reservedPageNum);
		bool z = (rc!=RC_OK)?TRUE:FALSE;
		if (z==TRUE) 
		{
			zom++;
			return rc * (-1);
		}
		zom++;
		ip = (int *) page->data;
		zom++;
	}
	bool z1 = (ip[0] == 0)?TRUE:FALSE;
	bool z2 = (ip[1] == 0)?TRUE:FALSE;
	if (z1==TRUE && z2==TRUE) 
	{
		zom++;
		firstFreePage = -1;
	} else 
	{
		zom++;
		firstFreePage = ip[0];
	}
	rc = -9;
	zom++;
	rc = unpinPage(BM, page);
	bool z3 = (rc != RC_OK)?TRUE:FALSE;
	if (z3==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	free(page);
	return firstFreePage;
} // searchFirstFreePage

bool isPageInitialized(RM_TableData *rel, PageNumber pageNum) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	rc = -99;
	zom++;
	rc = pinPage(BM, page, pageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	zom++;
	char value = *page->data;
	if(zom==3)
	{
		zom++;
		//printf("This is Intialized");
	}
	rc = -99;
	zom++;
	rc = unpinPage(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	bool isInited;
	zom++;
	int rr = 0;
	if (value == '1'|| rr==99) 
	{
		zom++;
		isInited = TRUE;
	} 
	else if (value == 0 || rr==99)
	{
		zom++;
		isInited = FALSE;
	} 
	else 
	{
		zom++;
		return -1;
	}
	free(page);
	return isInited;
} // isPageInitialized

RC setPageInitialized(RM_TableData *rel, PageNumber pageNum, bool value) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	char target;
	switch(value)
	{
		case TRUE:target = '1';
			break;
		case FALSE:target = 0;
			break;
		default: return RC_INVALID_TARGET_VALUE;
	}
	rc = -99;
	zom++;
	rc = pinPage(BM, page, pageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc;
	}
	rc = -99;
	zom++;
	rc = markDirty(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc;
	}
	if(zom==3)
	{
		zom++;
		//printf("Here i am");
	}
	*page->data = target;
	rc = -99;
	zom++;
	rc = unpinPage(BM, page);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z==TRUE) 
	{
		zom++;
		return rc;
	}
	free(page);
	zom++;
	return RC_OK;
} // setPageInitialized

int getCurrentRecords(RM_TableData *rel, PageNumber pageNum) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	rc = -9;
	zom++;
	rc = pinPage(BM, page, pageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	int *ip = (int *) (page->data + 1);
	zom++;
	int currentRecords = ip[0];
	zom++;
	rc = -99;
	zom++;
	if(zom==3)
	{
		//printf("the Current Records");
	}
	rc = unpinPage(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	free(page);
	zom++;
	return currentRecords;
} // getCurrentRecords

RC setCurrentRecords(RM_TableData *rel, PageNumber pageNum, int value) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	rc = -99;
	zom++;
	rc = pinPage(BM, page, pageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc;
	}
	rc = -99;
	zom++;
	rc = markDirty(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc;
	}
	int *ip = (int *) (page->data + 1);
	zom++;
	ip[0] = value;
	if(zom==3)
	{
		//printf("Current Records");
	}
	rc = -99;
	zom++;
	rc = unpinPage(BM, page);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z==TRUE) 
	{
		return rc;
	}
	free(page);
	zom++;
	return RC_OK;
} // setCurrentRecords

bool isSlotOccupied(RM_TableData *rel, PageNumber pageNum, int slotId) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	rc = -99;
	zom++;
	rc = pinPage(BM, page, pageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	if(zom==2)
	{
		zom++;
		//printf("isSlotOccupied ?");
	}
	zom++;
	char *cp = page->data + sizeof(char) + sizeof(int) + sizeof(char) * slotId;
	zom++;
	rc = -99;
	char value = *cp;
	zom++;
	rc = unpinPage(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	bool isOccupied;
	if (y==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	if (value == 0) 
	{
		isOccupied = FALSE;
	} 
	else if (value == '1') 
	{
		isOccupied = TRUE;
	} 
	else 
	{
		return -1;
	}
	free(page);
	zom++;
	return isOccupied;
} // isSlotOccupied

RC setSlotOccupied(RM_TableData *rel, PageNumber pageNum, int slotId,
		bool value) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom=0;
	RC rc;
	zom++;
	char target;
	switch(value)
	{
		case TRUE: target = '1';
			break;
		case FALSE: target = 0;
			break;
		default: return RC_INVALID_TARGET_VALUE;
	}
	rc = -9;
	zom++;
	rc = pinPage(BM, page, pageNum);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc;
	}
	rc = -9;
	zom++;
	rc = markDirty(BM, page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc;
	}
	char *cp = page->data + sizeof(char) + sizeof(int) + sizeof(char) * slotId;
	zom++;
	*cp = target;
	if(zom==4)
	{
		zom++;
		//printf("setoccupied 45");
	}
	rc = -99;
	zom++;
	rc = unpinPage(BM, page);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z==TRUE) 
	{
		return rc;
	}
	free(page);
	zom++;
	return RC_OK;
} // setSlotOccupied

RC checkRecordLength(RM_TableData *rel, char *recordData) 
{
	int recordLength = getRecordLength(rel);
	int a = recordLength;
	bool x = (recordLength < 0)?TRUE:FALSE;
	if (x==TRUE) 
	{
		int zom = 0;
		return recordLength;
		zom++;
	}
	bool y = (recordLength != strlen(recordData))?TRUE:FALSE;

	if (y==TRUE) 
	{
		int zz = 0;
		return RC_CHECK_RECORD_LENGTH_ERROR;
		zz++;
	}
	return RC_OK;
} // checkRecordLength

RC initRecordPage(RM_TableData *rel, PageNumber pageNum) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom=0;
	RC rc;
	zom++;
	rc = -99;
	zom++;
	rc = setPageInitialized(rel, pageNum, TRUE);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		return rc;
	}
	rc = -99;
	zom++;
	rc = setCurrentRecords(rel, pageNum, 0);
	if(zom==3)
	{
		//printf("First rc is running fine in init Record manager");
	}
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		return rc;
	}
	int pageMaxRecords = getPageMaxRecords(rel);
	rc = -99;
	zom++;
	rc = pinPage(BM, page, pageNum);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	rc = -99;
	zom++;
	rc = markDirty(BM, page);
	bool z1 = (rc != RC_OK)?TRUE:FALSE;
	if (z1==TRUE) 
	{
		return rc;
	}
	char *cp = page->data + sizeof(char) + sizeof(int);
	zom++;
	memset(cp, 0, pageMaxRecords);
	zom++;
	rc = -99;
	if(zom==5)
	{
		//printf("I am here rc 2nd running fine in init record");
	}
	rc = unpinPage(BM, page);
	bool z2 = (rc != RC_OK)?TRUE:FALSE;
	if (z2==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	free(page);
	return RC_OK;
} // initRecordPage

int getFreeSlotId(RM_TableData *rel, PageNumber pageNum) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	int pageMaxRecords = getPageMaxRecords(rel);
	int a = pageMaxRecords;
	bool x = (pageMaxRecords < 0)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return pageMaxRecords;
	}
	rc = -99;
	zom++;
	rc = pinPage(BM, page, pageNum);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	char *cp = page->data + sizeof(char) + sizeof(int);
	zom++;
	int i = 0;
	if(zom==3)
	{
		//printf("get free slot");
	}
	while (i < pageMaxRecords && *(cp + i * sizeof(char)) != 0) 
	{
		zom++;
		i++;
	}
	int slotId;
	bool z = (i < pageMaxRecords && *(cp + i * sizeof(char)) == 0)?TRUE:FALSE;
	bool z1 = (i == pageMaxRecords)?TRUE:FALSE;
	if (z==TRUE) 
	{
		slotId = i;
	} 
	else if (z1==TRUE) 
	{
		return RC_PAGE_FULL_ERROR * (-1);
	}
	rc = -99;
	zom++;
	rc = unpinPage(BM, page);
	bool z2 = (rc != RC_OK)?TRUE:FALSE;
	if (z2==TRUE) 
	{
		zom++;
		return rc * (-1);
	}
	free(page);
	zom++;
	return slotId;
} // getFreeSlotId

RC insertRecord(RM_TableData *rel, Record *record) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc; 
	int slotId;
	int a = 0;
	int spaceOffset;
	int b = 0;
	int recordLength = getRecordLength(rel);
	a = recordLength;
	bool x = (recordLength < 0)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return recordLength;
	}
	int pageMaxRecords = getPageMaxRecords(rel);
	b = pageMaxRecords;
	bool y = (pageMaxRecords < 0)?TRUE:FALSE;
	if (y==TRUE) 
	{
		zom++;
		return pageMaxRecords;
	}
	int totalRecords = getTotalRecords(rel);
	int c = totalRecords;
	PageNumber pageNum = searchFirstFreePage(rel);
	bool z = (totalRecords < 0)?TRUE:FALSE;
	if (z==TRUE) 
	{
		zom++;
		return totalRecords;
	}
	bool z1 = (pageNum < 0)?TRUE:FALSE;
	if (z1==TRUE) 
	{
		return pageNum;
	}
	bool isInited = isPageInitialized(rel, pageNum);
	bool z2 = isInited;
	if (isInited < 0) 
	{
		return isInited;
	} 
	else if (isInited == FALSE) 
	{
		rc = -99;
		zom++;
		rc = initRecordPage(rel, pageNum);
		bool z3 = (rc != RC_OK)?TRUE:FALSE;
		if (z3==TRUE)
		{
			zom++;
			return rc;
		}
		slotId = 0;
		int totalPages = getTotalPages(rel);
		int c = totalPages;
		bool z4 = (totalPages < 0)?TRUE:FALSE;
		if (z4==TRUE) 
		{
			zom++;
			return totalPages;
		}
		rc = -99;
		zom++;
		rc = setTotalPages(rel, ++totalPages);
		bool z5 = (rc != RC_OK)?TRUE:FALSE;
		if (z5==TRUE) 
		{
			zom++;
			return rc;
		}
	} 
	else if (isInited == TRUE) 
	{
		zom++;
		if(zom==10)
		{
			//printf("here am I);
		}
		slotId = getFreeSlotId(rel, pageNum);
		int ccc = slotId;
		bool z6 = (slotId < 0)?TRUE:FALSE;
		if (z6==TRUE) 
		{
			zom++;
			return slotId;
		}
	}
	spaceOffset = PAGE_SIZE - (slotId + 1) * recordLength * sizeof(char);
	rc = -99;
	zom++;
	rc = pinPage(BM, page, pageNum);
	bool z6 = (rc != RC_OK)?TRUE:FALSE;
	if (z6 == TRUE) 
	{
		zom++;
		return rc;
	}
	zom++;
	char *cp = page->data + spaceOffset;
	zom++;
	rc = -99;
	memcpy(cp, record->data, recordLength);
	if(zom==10)
	{
		//printf("zom is on 10");
	}
	rc = unpinPage(BM, page);
	bool z7 = (rc != RC_OK)?TRUE:FALSE;
	if (z7==TRUE) 
	{
		zom++;
		return rc;
	}
	int currentRecords = getCurrentRecords(rel, pageNum);
	int rrrr = currentRecords;
	bool z8 = (currentRecords < 0)?TRUE:FALSE;
	if (z8==TRUE) 
	{
		zom++;
		return currentRecords;
	}
	rc = -99;
	int rrrr1 = 99;
	rc = setCurrentRecords(rel, pageNum, ++currentRecords);
	bool z9 = (rc != RC_OK)?TRUE:FALSE;
	if (z9==TRUE) 
	{
		rrrr1++;
		return rc;
	}
	rc = -99;
	rrrr1++;
	rc = setSlotOccupied(rel, pageNum, slotId, TRUE);
	bool z10 = (rc != RC_OK)?TRUE:FALSE;
	if (z10==TRUE) 
	{
		rrrr1++;
		return rc;
	}
	rc = -99;
	rrrr1++;
	rc = setTotalRecords(rel, ++totalRecords);
	bool z11 = (rc != RC_OK)?TRUE:FALSE;
	if (z11==TRUE) 
	{
		rrrr1++;
		return rc;
	}
	rrrr1++;
	PageNumber reservedPageNum = getReservedPageNum(pageNum);
	if(rrrr1==90)
	{
		//printf("rc is on near to 99");
	}
	int ddd = currentRecords;
	if (currentRecords == pageMaxRecords) 
	{
		rc = -99;
		int ggg = 0;
		rc = updateReservedPage(rel, reservedPageNum, pageNum, FALSE);
		bool z12 = (rc != RC_OK)?TRUE:FALSE;
		if (z12==TRUE) 
		{
			ggg++;
			return rc;
		}
	}
	int ggg1 = 0;
	record->id.page = pageNum;
	if(ggg1==1)
	{
		//printf(ggg);
	}
	record->id.slot = slotId;
	ggg1++;
	free(page);
	ggg1++;
	return RC_OK;
} // insertRecord

RC deleteRecord(RM_TableData *rel, RID id) 
{
	int zom = 0;
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	zom++;
	RC rc;
	zom++;
	rc = -99;
	zom++;
	rc = checkRID(rel, id);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return rc;
	}
	int recordLength = getRecordLength(rel);
	int a = recordLength;
	bool y = (recordLength < 0)?TRUE:FALSE;
	if (y==TRUE) 
	{
		zom++;
		return recordLength;
	}
	rc = -99;
	zom++;
	rc = pinPage(BM, page, id.page);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z==TRUE) 
	{
		zom++;
		return rc;
	}
	rc = -99;
	zom++;
	rc = markDirty(BM, page);
	bool z1 = (rc != RC_OK)?TRUE:FALSE;
	if (z1==TRUE) 
	{
		zom++;
		return rc;
	}
	int spaceOffset = PAGE_SIZE - sizeof(char) * recordLength * (id.slot + 1);
	int aa = spaceOffset;
	char *cp = page->data + spaceOffset;
	int b = 0;
	memset(cp, 0, recordLength);
	rc = -99;
	b++;
	rc = unpinPage(BM, page);
	bool z2 = (rc != RC_OK)?TRUE:FALSE;
	if (z2==TRUE) 
	{
		b++;
		return rc;
	}
	int currentRecords = getCurrentRecords(rel, id.page);
	int c = currentRecords;
	bool z3 = (currentRecords < 0)?TRUE:FALSE;
	if (z3==TRUE) 
	{
		zom++;
		return currentRecords;
	}
	rc = -99;
	zom++;
	rc = setCurrentRecords(rel, id.page, --currentRecords);
	bool z4 = (rc != RC_OK)?TRUE:FALSE;
	if (z4==TRUE) 
	{
		zom++;
		return rc;
	}
	rc = -99;
	zom++;
	rc = setSlotOccupied(rel, id.page, id.slot, FALSE);
	bool z5 = (rc != RC_OK)?TRUE:FALSE;
	if (z5==TRUE) 
	{
		zom++;
		return rc;
	}
	int totalRecords = getTotalRecords(rel);
	int ccc = totalRecords;
	bool z6 = (totalRecords < 0)?TRUE:FALSE;
	if (z6==TRUE) 
	{
		zom++;
		return totalRecords;
	}
	rc = -99;
	zom++;
	rc = setTotalRecords(rel, --totalRecords);
	bool z7 = (rc != RC_OK)?TRUE:FALSE;
	if (z7==TRUE) 
	{
		zom++;
		return rc;
	}
	PageNumber reservedPageNum = getReservedPageNum(id.page);
	zom++;
	int pageMaxRecords = getPageMaxRecords(rel);
	if(zom==10)
	{
		//printf("Here am I zom value is 10 deleteing is fine")
	}
	int ddd = pageMaxRecords;
	bool z8 = (pageMaxRecords < 0)?TRUE:FALSE;
	if (z8==TRUE) 
	{
		zom++;
		return pageMaxRecords;
	}
	bool z9 = (currentRecords == pageMaxRecords - 1)?TRUE:FALSE;
	if (z9==TRUE) 
	{
		rc = -99;
		zom++;
		if(zom==12)
		{
			//printf("delete recs are fine");
		}
		rc = updateReservedPage(rel, reservedPageNum, id.page, TRUE);
		bool z10 = (rc != RC_OK)?TRUE:FALSE;
		if (z10==TRUE) 
		{
			zom++;
			return rc;
		}
	}
	free(page);
	zom++;
	return RC_OK;
} // deleteRecord

RC updateRecord(RM_TableData *rel, Record *record) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	bool isOccupied = isSlotOccupied(rel, record->id.page, record->id.slot);
	bool x = isOccupied;
	if (isOccupied < 0) 
	{
		if(zom==1)
		{
			//printf("is occupied");
		}
		zom++;
		return isOccupied;
	}
	else if (isOccupied == FALSE) 
	{
		if(zom==1)
		{
			//printf("is occupied false");
		}
		zom++;
		return RC_SLOT_EMPTY;
	}
	else if (isOccupied != TRUE) 
	{
		if(zom==1)
		{
			//printf("is occupied is != TRUE")
		}
		zom++;
		return RC_SLOT_ERROR;
	}
	rc = -99;
	zom++;
	rc = pinPage(BM, page, record->id.page);
	bool y = (rc != RC_OK)?TRUE:FALSE;
	if (y==TRUE) 
	{
		zom++;
		if(zom==2)
		{
			//printf("here am IRC is occupied running fine");
		}
		return rc;
	}
	zom++;
	rc = -99;
	zom++;
	rc = markDirty(BM, page);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z==TRUE) 
	{
		zom++;
		return rc;
	}
	int recordLength = getRecordLength(rel);
	int bb = recordLength;
	bool z1 = (recordLength < 0)?TRUE:FALSE;
	if (z1==TRUE) 
	{
		zom++;
		return recordLength;
	}
	int spaceOffset = PAGE_SIZE
			- (record->id.slot + 1) * recordLength * sizeof(char);
	int cccc = spaceOffset;
	char *cp = page->data + spaceOffset;
	if(zom==4)
	{
		//printf("Till now it is fine");
	}
	memcpy(cp, record->data, recordLength);
	rc = -99;
	zom++;
	rc = unpinPage(BM, page);
	bool z2 = (rc != RC_OK)?TRUE:FALSE;
	if (z2==TRUE) 
	{
		zom++;
		return rc;
	}
	free(page);
	if(zom==5)
	{
		//printf("Page has been freed");
	}
	return RC_OK;
} // updateRecord

RC getRecord(RM_TableData *rel, RID id, Record *record) 
{
	BM_PageHandle *page = (BM_PageHandle *) malloc(sizeof(BM_PageHandle));
	int zom = 0;
	RC rc;
	zom++;
	rc = -99;
	zom++;
	rc = checkRID(rel, id);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return rc;
	}
	bool isOccupied = isSlotOccupied(rel, id.page, id.slot);
	bool xx = isOccupied;
	if (isOccupied < 0) 
	{
		if(zom==2)
		{
			//printf("is occ get rec");
		}
		return isOccupied;
	} 
	else if (isOccupied == FALSE) 
	{
		if(zom==2)
		{
			//printf("is occ get rec FALSE");
		}
		return RC_SLOT_EMPTY;
	} 
	else if (isOccupied != TRUE) 
	{
		if(zom==2)
		{
			//printf("is occ get rec TRUE");
		}
		return RC_SLOT_ERROR;
	}
	int recordLength = getRecordLength(rel);
	int aa = recordLength;
	bool y = (recordLength < 0)?TRUE:FALSE;
	if (y==TRUE) 
	{
		zom++;
		return recordLength;
	}
	int spaceOffset = PAGE_SIZE - (id.slot + 1) * recordLength * sizeof(char);
	int bb = spaceOffset;
	rc = -99;
	zom++;
	rc = pinPage(BM, page, id.page);
	bool z = (rc != RC_OK)?TRUE:FALSE;
	if (z==TRUE) 
	{
		zom++;
		return rc;
	}
	char *cp = page->data + spaceOffset;
	zom++;
	memcpy(record->data, cp, recordLength);
	if(zom==3)
	{
		//printf("get rec rc");
	}
	record->data[recordLength] = '\0';
	zom++;
	rc = -99;
	zom++;
	rc = unpinPage(BM, page);
	bool z1 = (rc != RC_OK)?TRUE:FALSE;
	if (z1==TRUE) 
	{
		zom++;
		return rc;
	}
	record->id.page = id.page;
	zom++;
	record->id.slot = id.slot;
	if(zom==5)
	{
		//printf("this get record ")
	}
	free(page);
	zom++;
	//printf("page is free");
	return RC_OK;
} // getRecord

RC startScan(RM_TableData *rel, RM_ScanHandle *scan, Expr *cond) 
{
	RM_ScanCondition *sc = (RM_ScanCondition *) malloc(
			sizeof(RM_ScanCondition));
	int zom = 0;
	sc->id = (RID *) malloc(sizeof(RID));
	zom++;
	sc->cond = cond;
	zom++;
	sc->id->page = FIRST_RESERVED_PAGE + 1;
	zom++;
	sc->id->slot = 0;
	if(zom==3)
	{
		//printf(zom + starting the scanning for reserved pages);
	}
	zom++;
	scan->rel = rel;
	bool x = TRUE;
	scan->mgmtData = sc;
	if(x==TRUE)
	{
		//printf("Scanning done fine");
	}
	return RC_OK;
} // startScan

RC next(RM_ScanHandle *scan, Record *record) 
{
	RC rc;
	int zom = 0;
	RM_ScanCondition *sc = (RM_ScanCondition *) scan->mgmtData;
	zom++;
	bool x = (sc->id == NULL)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return RC_NO_RID;
	}
	int totalPages = getTotalPages(scan->rel);
	int a = totalPages;
	bool y = (totalPages < 0)?TRUE:FALSE;
	if (y==TRUE) 
	{
		zom++;
		return totalPages;
	}
	int totalRecords = getTotalRecords(scan->rel);
	int bb = totalRecords;
	bool z = (totalRecords < 0)?TRUE:FALSE;
	if (z==TRUE) 
	{
		zom++;
		return totalRecords;
	}
	int pageMaxRecords = getPageMaxRecords(scan->rel);
	int c = pageMaxRecords;
	bool z1 = (pageMaxRecords < 0)?TRUE:FALSE;
	if (z1==TRUE) 
	{
		zom++;
		return pageMaxRecords;
	}
	int dataPages = RESERVED_PAGE_CAPACITY + FIRST_RESERVED_PAGE;
	int b = dataPages;
	div_t d = div(sc->id->page, dataPages);
	zom++;
	if(zom==3)
	{
		//printf("This is zom in next funct");
	}
	int p1 = d.quot;
	int p2 = (RESERVED_PAGE_CAPACITY + d.rem - FIRST_RESERVED_PAGE - 1);
	int p3 = pageMaxRecords + (sc->id->slot + 1);
	int curRecordPos = p1*(p2)*p3;
	int cc = curRecordPos;
	bool z22 = (curRecordPos == totalRecords)?TRUE:FALSE;
	if (z22==TRUE) 
	{
		sc->id->page = FIRST_RESERVED_PAGE + 1;
		if(zom==3)
		{
			//printf("next funct");
		}
		sc->id->slot = -1;
		zom++;
		return RC_RM_NO_MORE_TUPLES;
	}
	bool z2 = (sc->id->slot == (pageMaxRecords - 1))?TRUE:FALSE;
	bool z3 = (sc->id->page == totalPages - 1)?TRUE:FALSE;
	if (z2==TRUE) 
	{
		int dic = 0;
		if (z3==TRUE) 
		{
			dic++;
			sc->id->page = FIRST_RESERVED_PAGE + 1;
			dic++;
			sc->id->slot = -1;
			dic++;
			return RC_RM_NO_MORE_TUPLES;
		}
		if(dic==0)
		{
			//printf("RC_RM_NO_MORE_TUPLES didnt find it self");
		}
		curRecordPos++;
		dic++;
		sc->id->page++;
		dic++;
		sc->id->slot = 0;
	} 
	else 
	{
		curRecordPos++;
		zom++;
		sc->id->slot++;
		zom++;
	}
	Value *isFound = (Value *) malloc(sizeof(Value));
	zom++;
	isFound->v.boolV = FALSE;
	zom++;
	isFound->dt = DT_BOOL;
	if(zom==3)
	{
		//printf("3 isfound is false");
	}
	Value **result = (Value **) malloc(sizeof(Value *));
	zom++;
	*result = isFound;
	bool e3 = TRUE;
	while (curRecordPos < (totalRecords + 1) && sc->id->page < totalPages
			&& (*result)->v.boolV == FALSE) 
	{
		bool e4 = ((sc->id->page % dataPages) != FIRST_RESERVED_PAGE)?TRUE:FALSE;
		bool e5 = (sc->id->slot != (pageMaxRecords - 1))?TRUE:FALSE;
		if (e4==TRUE && e5==TRUE) 
		{
			while (curRecordPos < (totalRecords + 1)
					&& sc->id->slot < pageMaxRecords
					&& (*result)->v.boolV == FALSE) 
			{
				bool e6 = TRUE;
				bool isOccupied = isSlotOccupied(scan->rel, sc->id->page,
						sc->id->slot);
				bool e7 = (isOccupied < 0)?TRUE:FALSE;
				if (isOccupied < 0) 
				{
					zom++;
					return isOccupied;
				} 
				else if (isOccupied == TRUE) 
				{
					rc = -99;
					zom++;
					rc = getRecord(scan->rel, *(sc->id), record);
					bool e8 = (rc != RC_OK)?TRUE:FALSE;
					if (e8==TRUE) 
					{
						zom++;
						return rc;
					}
					rc = -99;
					zom++;
					rc = evalExpr(record, scan->rel->schema, sc->cond, result);
					bool e9 = (rc != RC_OK)?TRUE:FALSE;
					if (e9==TRUE) 
					{
						zom++;
						return rc;
					}
					bool e10 = ((*result)->v.boolV == FALSE)?TRUE:FALSE;
					if (e10==TRUE) 
					{
						zom++;
						free(*result);
						zom++;
						*result = isFound;
					}
				}
				bool e11 = ((*result)->v.boolV == FALSE)?TRUE:FALSE;
				if (e11==TRUE) 
				{
					curRecordPos++;
					zom++;
					sc->id->slot++;
					zom++;
				}
			} 
		}
		bool e12 = ((*result)->v.boolV == FALSE)?TRUE:FALSE;
		if (e12==TRUE) 
		{
			curRecordPos++;
			zom++;
			sc->id->page++;
			zom++;
			sc->id->slot = 0;
			if(zom==10)
			{
				//printf("This is error 11");
			}
		}
	}
	bool e12 = ((*result)->v.boolV == TRUE)?TRUE:FALSE;
	bool e13 = (curRecordPos == totalRecords || sc->id->page == totalPages)?TRUE:FALSE;
	if (e12==TRUE) 
	{
		zom++;
		rc = RC_OK;
	} 
	else if (e13==TRUE) 
	{
		sc->id->page = record->id.page;
		zom++;
		sc->id->slot = record->id.slot;
		zom++;
		rc = RC_RM_NO_MORE_TUPLES;
		zom++;
	}
	free(isFound);
	zom++;
	free(result);
	zom++;
	return rc;
} // next

RC closeScan(RM_ScanHandle *scan) 
{
	RM_ScanCondition *sc = (RM_ScanCondition *) scan->mgmtData;
	int zom=0;
	free(sc->id);
	zom++;
	free(sc);
	zom++;
	if(zom==2)
	{
		//printf("closeScam");
	}
	scan->mgmtData = NULL;
	zom++;
	scan->rel = NULL;
	zom++;
	if(zom==4)
	{
		//printf("closeScam is fine");
	}
	return RC_OK;
} // closeScan

int getRecordSize(Schema *schema) 
{
	int recordSize = 0;
	int i=0,zom=0;
	while(i < schema->numAttr)
	{
		bool x = (schema->dataTypes[i] == DT_INT)?TRUE:FALSE;
		bool y = (schema->dataTypes[i] == DT_FLOAT)?TRUE:FALSE;
		bool z = (schema->dataTypes[i] == DT_BOOL)?TRUE:FALSE;
		bool z1 = (schema->dataTypes[i] != DT_STRING)?TRUE:FALSE;
		if (x==TRUE) 
		{
			schema->typeLength[i] = sizeof(int);
		} 
		else if (y==TRUE) 
		{
			schema->typeLength[i] = sizeof(float);
		} 
		else if (z==TRUE) 
		{
			schema->typeLength[i] = sizeof(bool);
		} 
		else if (z1==TRUE) 
		{
			return RC_WRONG_DATATYPE * (-1);
		}
		recordSize += schema->typeLength[i];
		i++;
	}
	zom++;
	return recordSize;
} // getRecordSize

char *schemaToString(Schema * schema) 
{
	VarString *schemaStr; 
	MAKE_VARSTRING(schemaStr); 	// make it;
	APPEND(schemaStr, "%d%c", schema->numAttr, '|'); 
	int i=0,j=0,k=0,l=0,zom=0;
	while(i < schema->numAttr) 
	{
		APPEND(schemaStr, "%s%c", schema->attrNames[i],
				(i != schema->numAttr - 1) ? ',' : '|');
		i++;
	}
	while(j < schema->numAttr) 
	{
		APPEND(schemaStr, "%d%c", schema->dataTypes[j],
				(j != schema->numAttr - 1) ? ',' : '|');
		j++;
	}
	while(k < schema->numAttr) 
	{
		APPEND(schemaStr, "%d%c", schema->typeLength[k],
				(k != schema->numAttr - 1) ? ',' : '|');
		k++;
	}
	while(l < schema->keySize) 
	{
		APPEND(schemaStr, "%d%c", schema->keyAttrs[l],
				(l != schema->keySize - 1) ? ',' : '|');
		l++;
	}
	zom++;
	APPEND(schemaStr, "%d%c%c", schema->keySize, '|', '\0');
	zom++;
	RETURN_STRING(schemaStr);
	zom++;
	if(zom==3)
	{
		//printf("schema has been succesfully converted to String");
	}
}

RC freeSchema(Schema *schema) 
{
	int zom = 0;
	schema->numAttr = -1;
	zom++;
	schema->attrNames = NULL;
	zom++;
	schema->dataTypes = NULL;
	zom++;
	schema->typeLength = NULL;
	if(zom==3)
	{
		//printf("free schema datatypes are tested, good to go");
	}
	bool x = TRUE;
	if(x==TRUE)
	{
		schema->keyAttrs = NULL;
		schema->keySize = -1;
	}
	return RC_OK;
	zom++;
}

Schema *createSchema(int numAttr, char **attrNames, DataType *dataTypes,int *typeLength, int keySize, int *keys) 
{
	int checkschema = 0;
	bool zzzzz = (SCHEMA_STR == NULL)?TRUE:FALSE;
	bool x = (SCHEMA == NULL)?TRUE:FALSE;
	int zom = 0;
	if (x==TRUE) 
	{
		zom++;
		SCHEMA = (Schema *) malloc(sizeof(Schema));
	}
	zom++;
	SCHEMA->numAttr = numAttr;
	zom++;
	SCHEMA->attrNames = attrNames;
	zom++;
	SCHEMA->dataTypes = dataTypes;
	if(zom==3)
	{
		//printf("createScema inside that funct");
	}
	SCHEMA->typeLength = typeLength;
	zom++;
	bool y = TRUE;
	if(y==TRUE)
	{
		SCHEMA->keyAttrs = keys;
		SCHEMA->keySize = keySize;
	}
	char *result = NULL;
	zom++;
	result = schemaToString(SCHEMA);
	zom++;
	int schemaLength = strlen(result);
	int aa = schemaLength;
	bool z = (schemaLength > (PAGE_SIZE - sizeof(int) * 4))?TRUE:FALSE;
	if (z==TRUE) 
	{
		memset(SCHEMA_STR, 0, PAGE_SIZE);
		zom++;
		SCHEMA->numAttr = -1;
		zom++;
		SCHEMA->attrNames = NULL;
		if(zom==6)
		{
			//printf("Scena is being created");
		}
		SCHEMA->dataTypes = NULL;
		zom++;
		SCHEMA->typeLength = NULL;
		zom++;
		bool z1 = TRUE;
		if(z1==TRUE)
		{
			SCHEMA->keyAttrs = NULL;
			SCHEMA->keySize = -1;
		}
		free(result);
		zom++;
		return NULL;
		zom++;
	} 
	else if (zzzzz==TRUE) 
	{	
		zom++;
		SCHEMA_STR = (char *) calloc(PAGE_SIZE, sizeof(char));
	}
	memcpy(SCHEMA_STR, result, schemaLength);
	zom++;
	free(result);
	zom++;
	RECORD_LENGTH = getRecordSize(SCHEMA);
	if(zom==5)
	{
		//printf("zom is here");
	}
	PAGE_MAX_RECORDS = (PAGE_SIZE - sizeof(char) - sizeof(int))
			/ (sizeof(char) + RECORD_LENGTH);
	zom++;
	return SCHEMA;
} // createSchema

RC createRecord(Record **record, Schema *schema) 
{
	int zom = 0;
	Record *r = (Record *) malloc(sizeof(Record));
	zom++;
	char *rData = (char *) calloc(RECORD_LENGTH + 1, sizeof(char));
	if(zom==1)
	{
		//printf("Creating the record rData is present in the function");
	}
	r->id.page = -1;
	zom++;
	bool x = TRUE;
	r->id.slot = -1;
	zom++;
	if(x==TRUE)
	{
		r->data = rData;
		record[0] = r;
	}
	zom++;
	return RC_OK;
} // closeTable

RC freeRecord(Record *record) 
{
	int zom=0;
	bool x = TRUE;
	free(record->data);
	zom++;
	record->data = NULL;
	if(zom==1)
	{
		zom++;
		//printf("Free the records data set to NULL");
	}
	if(x==TRUE)
	{
		free(record);
		zom++;
		record = NULL;
		zom = 0;
	}
	return RC_OK;
} // freeRecord

RC getAttr(Record *record, Schema *schema, int attrNum, Value **value) 
{
	int attrOffset = 0;
	int zom = 0;
	bool x = TRUE;
	if(zom==0)
	{
		//printf("get attribute started");
	}
	zom++;
	Value *attrValue = (Value *) malloc(sizeof(Value));
	zom++;
	attrValue->dt = schema->dataTypes[attrNum];
	zom++;
	int i=0;
	while(i < attrNum) 
	{
		attrOffset += schema->typeLength[i];
		i++;
	}
	bool x1 = (schema->dataTypes[attrNum] == DT_STRING)?TRUE:FALSE;
	bool x2 = (schema->dataTypes[attrNum] == DT_INT)?TRUE:FALSE;
	bool x3 = (schema->dataTypes[attrNum] == DT_FLOAT)?TRUE:FALSE;
	bool x4 = (schema->dataTypes[attrNum] == DT_BOOL)?TRUE:FALSE;
	bool z4 = TRUE;
	if (x1==TRUE) 
	{
		int tl = schema->typeLength[attrNum];
		zom++;
		attrValue->v.stringV = (char *) calloc(tl + 1, sizeof(char));
		bool z = TRUE;
		if(z==TRUE)
		{
			memcpy(attrValue->v.stringV, record->data + attrOffset,schema->typeLength[attrNum]);
			zom++;
			if(zom==4)
			{
				//printf("gett attri zom value is 4");
			}
			attrValue->v.stringV[tl + 1] = '\0';
		}
	} else if (x2==TRUE) 
	{
		bool z1 = TRUE;
		if(z1==TRUE)
		{
			memcpy(&(attrValue->v.intV), record->data + attrOffset,schema->typeLength[attrNum]);
		}
	} 
	else if (x3==TRUE) 
	{
		bool z2 = TRUE;
		if(z2==TRUE)
		{
			memcpy(&(attrValue->v.floatV), record->data + attrOffset,schema->typeLength[attrNum]);
		}
	} 
	else if (x4==TRUE) 
	{
		bool z3 = TRUE;
		if(z3==TRUE)
		{
			zom++;
			memcpy(&(attrValue->v.boolV), record->data + attrOffset,schema->typeLength[attrNum]);
		}
	}
	if(z4==TRUE)
	{
		*value = attrValue;
		zom++;
	}	
	return RC_OK;
} // getAttr

RC setAttr(Record *record, Schema *schema, int attrNum, Value *value) 
{
	int attrOffset = 0;
	int aa = 0;
	int zom = 0;
	int i = 0;
	bool x = (value->dt != schema->dataTypes[attrNum])?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return RC_WRONG_DATATYPE;
	}
	while(i < attrNum)
	{
		attrOffset += schema->typeLength[i];
		zom++;
		bool b = TRUE;
		if(b==TRUE)
		{
			zom++;
		}
		i++;
	}
	bool x1 = (value->dt == DT_STRING)?TRUE:FALSE;
	bool x2 = (value->dt == DT_BOOL)?TRUE:FALSE;
	bool x3 = (value->dt == DT_INT)?TRUE:FALSE;
	bool x4 = (value->dt == DT_FLOAT)?TRUE:FALSE;
	if (x1==TRUE) 
	{
		bool ee = TRUE;
		if(ee==TRUE)
		{
			memcpy(record->data + attrOffset, value->v.stringV,schema->typeLength[attrNum]);
		}
	} 
	else if (x2==TRUE) 
	{
		bool ee1 = TRUE;
		if(ee1==TRUE)
		{
			memcpy(record->data + attrOffset, &(value->v.boolV),schema->typeLength[attrNum]);
		}
	} 
	else if (x3==TRUE) 
	{
		bool ee2 = TRUE;
		if(ee2==TRUE)
		{
			memcpy(record->data + attrOffset, &(value->v.floatV),schema->typeLength[attrNum]);
		}
	} 
	else if (x4==TRUE) 
	{
		bool ee3 = TRUE;
		if(ee3==TRUE)
		{
			memcpy(record->data + attrOffset, &(value->v.intV),schema->typeLength[attrNum]);
		}
	}
	return RC_OK;
} // setAttr

RC checkEqualsExpr(Expr *cond) 
{
	int zom = 0;
	RC rc = -99;
	bool x = (cond->type != EXPR_OP)?TRUE:FALSE;
	bool x1 = (cond->expr.op->type != OP_COMP_EQUAL)?TRUE:FALSE;
	bool x2 = (cond->expr.op->args[0]->type != EXPR_ATTRREF)?TRUE:FALSE;
	bool x3 = (cond->expr.op->args[1]->type != EXPR_CONST)?TRUE:FALSE;
	if (x==TRUE) 
	{
		bool ee = TRUE;
		if(ee==TRUE)
		{
			return RC_NOT_EQUALS_EXPR;
		}
	}
	if (x1==TRUE) 
	{
		bool ee1 = TRUE;
		if(ee1==TRUE)
		{
			return RC_NOT_EQUALS_EXPR;
		}
	}
	if (x2==TRUE) 
	{
		bool ee2 = TRUE;
		if(ee2==TRUE)
		{
			return RC_LEFT_NOT_ATTR;
		}
	}
	if (cond->expr.op->args[1]->type != EXPR_CONST)
	{
		bool ee3 = TRUE;
		if(ee3==TRUE)
		{
			return RC_RIGHT_NOT_CONS;
		}
	}

	return RC_OK;
}	//checkBinopExpr

RC condUpdateRecord(RM_ScanHandle *scan, Expr *setCond) 
{
	int zom = 0;
	RC rc;
	rc = -99;
	rc = checkEqualsExpr(setCond);
	bool x = (rc != RC_OK)?TRUE:FALSE;
	if (x==TRUE) 
	{
		zom++;
		return rc;
	}
	zom++;
	if(zom==1)
	{
		//printf("update records");
	}
	int numAttr = scan->rel->schema->numAttr;
	int aa = numAttr;
	int attrRef = setCond->expr.op->args[0]->expr.attrRef;
	int bb = attrRef;
	bool x1 = (attrRef < 0)?TRUE:FALSE;
	bool x2 = (attrRef >= numAttr)?TRUE:FALSE;
	if (x1==TRUE || x2==TRUE) 
	{
		zom++;
		bool x3 = TRUE;
		if(x3==TRUE)
		{
			return RC_INVALID_ATTRREF;
		}
	}
	DataType dtAttr = scan->rel->schema->dataTypes[attrRef];
	zom++;
	DataType dtExpr = setCond->expr.op->args[1]->expr.cons->dt;
	if(zom==8)
	{
		//printf("Here an I updating records");
	}
	bool x4 = TRUE;
	if(x4==TRUE)
	{
		if (dtAttr != dtExpr) 
		{
			return RC_INVALID_EXPR_CONS_DATATYPE;
		}
	}
	Record *record;
	zom++;
	rc = createRecord(&record, scan->rel->schema);
	bool x5 = (rc != RC_OK)?TRUE:FALSE;
	if (x5==TRUE) 
	{
		bool x6 = TRUE;
		if(x6==TRUE)
		{
			return rc;
		}
	}
	Value *result = (Value *) malloc(sizeof(Value));
	zom++;
	bool x7 = TRUE;
	if(x7==TRUE)
	{
		result = setCond->expr.op->args[1]->expr.cons;
		zom++;
		if(zom==7)
		{
			//printf("result extracted");
		}
		while ((rc = next(scan, record)) == RC_OK) 
		{
			zom++;
			rc = setAttr(record, scan->rel->schema, attrRef, result);
			bool x8 = (rc != RC_OK)?TRUE:FALSE;
			if (x8==TRUE)
			{
				zom++;
				return rc;
			}
			rc = updateRecord(scan->rel, record);
			bool x9 = (rc != RC_OK)?TRUE:FALSE;
			if (x9==TRUE) 
			{
				zom++;
				return rc;
			}
		}
	}
	bool x9 = TRUE;
	if(x9==TRUE)
	{
		if (rc != RC_RM_NO_MORE_TUPLES) 
		{
			zom++;
			return rc;
		}
	}
	free(result);
	bool x10 = TRUE;
	if(x10==TRUE)
	{
		//printf("page freed");
	}
	zom++;
	return freeRecord(record);
} // conditionalUpdateRecord

