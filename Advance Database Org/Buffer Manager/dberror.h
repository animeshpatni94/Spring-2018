#ifndef DBERROR_H
#define DBERROR_H

#include <stdio.h>

#define PAGE_SIZE 4096


typedef int RC;

#define RC_OK 0
#define RC_FILE_NOT_FOUND 1
#define RC_FILE_HANDLE_NOT_INIT 2
#define RC_WRITE_FILE_FAILED 3
#define RC_NON_EXISTING_PAGE 4
#define RC_READ_NON_EXISTING_PAGE 5

#define RC_SEEK_FILE_POSITION_ERROR 100
#define RC_SEEK_FILE_TAIL_ERROR 101
#define RC_CLOSE_FILE_FAILED 102
#define RC_REMOVE_FILE_FAILED 103
#define RC_ENOUGH_PAGES 104

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

#define RC_NO_REMOVABLE_PAGE 401
#define RC_NOT_INTIALIZED_PAGES 402
#define RC_PAGE_NOT_FOUND 403
#define RC_NUMPAGES_NOT_VALID 404
#define RC_PAGE_FOUND 405
#define RC_FLUSH_ERROR 406
#define ALGO_NOT_IMPLEMENTED 407
#define RC_READ_FILE_FAILED 408

/* holder for error messages */
extern char *RC_message;

/* print a message to standard out describing the error */
extern void printError(RC error);
extern char *errorMessage(RC error);

/* throw message */
#define THROW(rc,message) \
  do {			  \
    RC_message=message;	  \
    return rc;		  \
  } while (0)		  \

/* check the return code and exit if it is an error */
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
