#ifndef DBERROR_H
#define DBERROR_H

#include <stdio.h>


#define PAGE_SIZE 4096  


typedef int RC; 

#define RC_OK 0 
#define RC_FILE_NOT_FOUND 11 
#define RC_FILE_HANDLE_NOT_INIT 12 
#define RC_WRITE_FILE_FAILED 13 
#define RC_NON_EXISTING_PAGE 14 

#define RC_SEEK_FILE_POSITION_ERROR 100	
#define RC_SEEK_FILE_TAIL_ERROR 101 
#define RC_CLOSE_FILE_FAILED 102 
#define RC_REMOVE_FILE_FAILED 103 
#define RC_ENOUGH_PAGES 104 



#define RC_READ_FILE_FAILED 110 


#define RC_NO_REMOVABLE_PAGE 120 
#define RC_PAGELIST_NOT_INITIALIZED 121 
#define RC_PAGE_NOT_FOUND 122 
#define RC_INVALID_NUMPAGES 123 
#define RC_PAGE_FOUND 124 
#define RC_FLUSH_POOL_ERROR 125 
#define RC_RS_NOT_IMPLEMENTED 126 



#define RC_INVALID_TARGET_VALUE 130 
#define RC_SCHEMA_NOT_CREATED 131 
#define RC_OPEN_TABLE_FAILED 132 
#define RC_WRONG_NEW_RESERVED_PAGE_NUM 133 
#define RC_GET_FIRST_FREE_PAGE_ERROR 134 
#define RC_TABLE_FILE_NO_ACCESS 135
#define RC_GET_IS_OCCUPIED_ERROR 136 
#define RC_CHECK_RECORD_LENGTH_ERROR 137 
#define RC_PAGE_FULL_ERROR 138 
#define RC_SET_TOTAL_PAGES_ERROR 139
#define RC_RID_OUT_OF_BOUND 140
#define RC_RID_IS_RESERVED_PAGE 141
#define RC_SLOT_EMPTY 142
#define RC_SLOT_ERROR 143
#define RC_MEMORY_COPY_ERROR 144
#define RC_WRONG_SCHEMA 145
#define RC_WRONG_DATATYPE 146
#define RC_TABLE_EXISTS 147
#define RC_SET_TOTAL_RECORDS_ERROR 148
#define RC_NO_RID 149
#define RC_NOT_EQUALS_EXPR 150
#define RC_LEFT_NOT_ATTR 151
#define RC_RIGHT_NOT_CONS 152
#define RC_INVALID_ATTRREF 153
#define RC_INVALID_EXPR_CONS_DATATYPE 154


#define RC_RM_COMPARE_VALUE_OF_DIFFERENT_DATATYPE 200
#define RC_RM_EXPR_RESULT_IS_NOT_BOOLEAN 201
#define RC_RM_BOOLEAN_EXPR_ARG_IS_NOT_BOOLEAN 202
#define RC_RM_NO_MORE_TUPLES 203
#define RC_RM_NO_PRINT_FOR_DATATYPE 204
#define RC_RM_UNKOWN_DATATYPE 205

#define RC_IM_KEY_NOT_FOUND 300
#define RC_IM_KEY_ALREADY_EXISTS 301
#define RC_IM_N_TO_LAGE 302
#define RC_IM_NO_MORE_ENTRIES 303
#define RC_READ_NON_EXISTING_PAGE 304
#define ALGO_NOT_IMPLEMENTED 305
#define RC_NUMPAGES_NOT_VALID 306
#define RC_FLUSH_ERROR 307


extern char *RC_message;


extern void printError(RC error);
extern char *errorMessage(RC error);


#define THROW(rc,message) \
  do {			  \
    RC_message=message;	  \
    return rc;		  \
  } while (0)		  \


#define CHECK(code)							\
  do {									\
    int rc_internal = (code);						\
    if (rc_internal != RC_OK)						\
      {									\
	char *message = errorMessage(rc_internal);			\
	printf("[%s-L%i-%s] ERROR: Operation returned error: %s\n",__FILE__, __LINE__, __TIME__, message); \
	free(message);							\
	exit(1);							\
      }									\
  } while(0);

#endif
