#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>
#include <sal.h>
#include <string>
#include <vector>
#include <mutex>
#include <functional>
#include "json.h"

#define TRYODBC(h, ht, x) {\
    RETCODE rc = x;\
    if (rc != SQL_SUCCESS) {\
        HandleDiagnosticRecord (h, ht, rc); \
        if (rc == SQL_ERROR) \
        { \
            fwprintf(stderr, L"Error in " L#x L"\n"); \
            goto Exit;  \
        }  \
    }\
}

typedef struct STR_BINDING {
    SQLSMALLINT         cDisplaySize;           /* size to display  */
    WCHAR* wszBuffer;                           /* display buffer   */
    SQLLEN              indPtr;                 /* size or null     */
    BOOL                fChar;                  /* character col?   */
    struct STR_BINDING* sNext;                  /* linked list      */
} BINDING;

void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
std::string getMessage(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
void AllocateBindings(HSTMT hStmt, SQLSMALLINT cCols, BINDING** ppBinding);

#define DISPLAY_MAX         1000
#define NULL_SIZE           6    // <NULL>
#define SQL_QUERY_SIZE      1000
#define PIPE                L'|'

using SelectQueryResult = std::vector<JSON>;
using SelectQuery = std::pair<std::string, std::function<void(SelectQueryResult)>>;
using NonSelectQueryResult = std::string;
using NonSelectQuery = std::pair<std::string, std::function<void(NonSelectQueryResult)>>;

SelectQueryResult getResults(HSTMT hStmt, SQLSMALLINT cCols);

void db(
    WCHAR* connectionString,
    std::mutex& queryLock,
    std::vector<SelectQuery>& selectQueries,
    std::vector<NonSelectQuery>& nonSelectQueries,
    std::mutex& waiter,
    std::condition_variable& cv,
    bool* connected
);
