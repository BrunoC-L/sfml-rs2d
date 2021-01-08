#include "odbc.h"

int db(WCHAR* connectionString, std::vector<Query>& queries, std::mutex& mutex) {
    SQLHENV     hEnv = NULL;
    SQLHDBC     hDbc = NULL;
    SQLHSTMT    hStmt = NULL;

    // Allocate an environment

    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_ERROR)
    {
        fwprintf(stderr, L"Unable to allocate an environment handle\n");
        exit(-1);
    }

    TRYODBC(hEnv,
        SQL_HANDLE_ENV,
        SQLSetEnvAttr(hEnv,
            SQL_ATTR_ODBC_VERSION,
            (SQLPOINTER)SQL_OV_ODBC3,
            0));

    // Allocate a connection
    TRYODBC(hEnv,
        SQL_HANDLE_ENV,
        SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc));

    TRYODBC(hDbc,
        SQL_HANDLE_DBC,
        SQLDriverConnect(hDbc,
            NULL,
            connectionString,
            SQL_NTS,
            NULL,
            0,
            NULL,
            SQL_DRIVER_COMPLETE));

    fwprintf(stderr, L"Connected!\n");

    TRYODBC(hDbc,
        SQL_HANDLE_DBC,
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt));

    // std::wprintf(L"Enter SQL commands, type (control)Z to exit\n"); // SQL COMMAND > ");

    // Loop to get input and execute queries

    while (true) {
        if (queries.size() == 0)
            continue;
        mutex.lock();
        Query query = queries[0];
        queries.erase(queries.begin());
        mutex.unlock();
        RETCODE     RetCode;
        SQLSMALLINT sNumResults;

        WCHAR wquery[SQL_QUERY_SIZE];
        for (int i = 0; i < query.first.length(); ++i)
            wquery[i] = query.first[i];
        wquery[query.first.length()] = 0;
        RetCode = SQLExecDirect(hStmt, wquery, SQL_NTS);

        switch (RetCode)
        {
        case SQL_SUCCESS_WITH_INFO:
        {
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, RetCode);
            // fall through
        }
        case SQL_SUCCESS:
        {
            // If this is a row-returning query, display
            // results
            TRYODBC(hStmt,
                SQL_HANDLE_STMT,
                SQLNumResultCols(hStmt, &sNumResults));

            if (sNumResults > 0)
            {
                query.second(getResults(hStmt, sNumResults));
            }
            else
            {
                SQLLEN cRowCount;

                TRYODBC(hStmt,
                    SQL_HANDLE_STMT,
                    SQLRowCount(hStmt, &cRowCount));

                //if (cRowCount >= 0)
                //{
                //    std::wprintf(L"%Id %s affected\n",
                //        cRowCount,
                //        cRowCount == 1 ? L"row" : L"rows");
                //}
            }
            break;
        }

        case SQL_ERROR:
        {
            HandleDiagnosticRecord(hStmt, SQL_HANDLE_STMT, RetCode);
            break;
        }

        default:
            fwprintf(stderr, L"Unexpected return code %hd!\n", RetCode);

        }
        TRYODBC(hStmt,
            SQL_HANDLE_STMT,
            SQLFreeStmt(hStmt, SQL_CLOSE));
    }

Exit:

    // Free ODBC handles and exit

    if (hStmt)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    if (hDbc)
    {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    }

    if (hEnv)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }

    std::wprintf(L"\nDisconnected.");

    return 0;

}

QueryResult getResults(HSTMT       hStmt,
    SQLSMALLINT cCols)
{
    BINDING* pFirstBinding, * pThisBinding;
    SQLSMALLINT     cDisplaySize;
    RETCODE         RetCode = SQL_SUCCESS;
    int             iCount = 0;

    // Allocate memory for each column

    AllocateBindings(hStmt, cCols, &pFirstBinding, &cDisplaySize);

    // Set the display mode and write the titles

    // Fetch the data

    bool fNoData = false;
    QueryResult result;
    do {
        TRYODBC(hStmt, SQL_HANDLE_STMT, RetCode = SQLFetch(hStmt));

        if (RetCode == SQL_NO_DATA_FOUND)
            fNoData = true;
        else {
            result.push_back({});

            for (pThisBinding = pFirstBinding;
                pThisBinding;
                pThisBinding = pThisBinding->sNext)
            {
                if (pThisBinding->indPtr != SQL_NULL_DATA)
                {
                    auto w = pThisBinding->wszBuffer;
                    auto ws = std::wstring(w);
                    auto s = std::string(ws.begin(), ws.end());
                    result.back().push_back(s);
                    //std::wprintf(pThisBinding->fChar ? DISPLAY_FORMAT_C : DISPLAY_FORMAT,
                    //    PIPE,
                    //    pThisBinding->cDisplaySize,
                    //    pThisBinding->cDisplaySize,
                    //    pThisBinding->wszBuffer);
                }
                else
                {
                    result.back().push_back("<NULL>");
                    //std::wprintf(DISPLAY_FORMAT_C,
                    //    PIPE,
                    //    pThisBinding->cDisplaySize,
                    //    pThisBinding->cDisplaySize,
                    //    L"<NULL>");
                }
            }
            //std::wprintf(L" %c\n", PIPE);
        }
    } while (!fNoData);

    //SetConsole(cDisplaySize + 2, TRUE);
    //std::wprintf(L"%*.*s", cDisplaySize + 2, cDisplaySize + 2, L" ");
    //SetConsole(cDisplaySize + 2, FALSE);
    //std::wprintf(L"\n");

Exit:
    // Clean up the allocated buffers

    while (pFirstBinding)
    {
        pThisBinding = pFirstBinding->sNext;
        free(pFirstBinding->wszBuffer);
        free(pFirstBinding);
        pFirstBinding = pThisBinding;
    }
    return result;
}

void AllocateBindings(HSTMT         hStmt,
    SQLSMALLINT   cCols,
    BINDING** ppBinding,
    SQLSMALLINT* pDisplay)
{
    SQLSMALLINT     iCol;
    BINDING* pThisBinding, * pLastBinding = NULL;
    SQLLEN          cchDisplay, ssType;
    SQLSMALLINT     cchColumnNameLength;

    *pDisplay = 0;

    for (iCol = 1; iCol <= cCols; iCol++)
    {
        pThisBinding = (BINDING*)(malloc(sizeof(BINDING)));
        if (!(pThisBinding))
        {
            fwprintf(stderr, L"Out of memory!\n");
            exit(-100);
        }

        if (iCol == 1)
        {
            *ppBinding = pThisBinding;
        }
        else
        {
            pLastBinding->sNext = pThisBinding;
        }
        pLastBinding = pThisBinding;

        // Figure out the display length of the column (we will
        // bind to char since we are only displaying data, in general
        // you should bind to the appropriate C type if you are going
        // to manipulate data since it is much faster...)

        TRYODBC(hStmt,
            SQL_HANDLE_STMT,
            SQLColAttribute(hStmt,
                iCol,
                SQL_DESC_DISPLAY_SIZE,
                NULL,
                0,
                NULL,
                &cchDisplay));

        // Figure out if this is a character or numeric column; this is
        // used to determine if we want to display the data left- or right-
        // aligned.

        // SQL_DESC_CONCISE_TYPE maps to the 1.x SQL_COLUMN_TYPE.
        // This is what you must use if you want to work
        // against a 2.x driver.

        TRYODBC(hStmt,
            SQL_HANDLE_STMT,
            SQLColAttribute(hStmt,
                iCol,
                SQL_DESC_CONCISE_TYPE,
                NULL,
                0,
                NULL,
                &ssType));

        pThisBinding->fChar = (ssType == SQL_CHAR ||
            ssType == SQL_VARCHAR ||
            ssType == SQL_LONGVARCHAR);

        pThisBinding->sNext = NULL;

        // Arbitrary limit on display size
        if (cchDisplay > DISPLAY_MAX)
            cchDisplay = DISPLAY_MAX;

        // Allocate a buffer big enough to hold the text representation
        // of the data.  Add one character for the null terminator

        pThisBinding->wszBuffer = (WCHAR*)malloc((cchDisplay + 1) * sizeof(WCHAR));

        if (!(pThisBinding->wszBuffer))
        {
            fwprintf(stderr, L"Out of memory!\n");
            exit(-100);
        }

        // Map this buffer to the driver's buffer.   At Fetch time,
        // the driver will fill in this data.  Note that the size is
        // count of bytes (for Unicode).  All ODBC functions that take
        // SQLPOINTER use count of bytes; all functions that take only
        // strings use count of characters.

        TRYODBC(hStmt,
            SQL_HANDLE_STMT,
            SQLBindCol(hStmt,
                iCol,
                SQL_C_TCHAR,
                (SQLPOINTER)pThisBinding->wszBuffer,
                (cchDisplay + 1) * sizeof(WCHAR),
                &pThisBinding->indPtr));


        // Now set the display size that we will use to display
        // the data.   Figure out the length of the column name

        TRYODBC(hStmt,
            SQL_HANDLE_STMT,
            SQLColAttribute(hStmt,
                iCol,
                SQL_DESC_NAME,
                NULL,
                0,
                &cchColumnNameLength,
                NULL));

        pThisBinding->cDisplaySize = max((SQLSMALLINT)cchDisplay, cchColumnNameLength);
        if (pThisBinding->cDisplaySize < NULL_SIZE)
            pThisBinding->cDisplaySize = NULL_SIZE;

        *pDisplay += pThisBinding->cDisplaySize + DISPLAY_FORMAT_EXTRA;

    }

    return;

Exit:

    exit(-1);

    return;
}

void HandleDiagnosticRecord(SQLHANDLE      hHandle,
    SQLSMALLINT    hType,
    RETCODE        RetCode)
{
    SQLSMALLINT iRec = 0;
    SQLINTEGER  iError;
    WCHAR       wszMessage[1000];
    WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];


    if (RetCode == SQL_INVALID_HANDLE) {
        fwprintf(stderr, L"Invalid handle!\n");
        return;
    }

    while (SQLGetDiagRec(hType,
        hHandle,
        ++iRec,
        wszState,
        &iError,
        wszMessage,
        (SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)),
        (SQLSMALLINT*)NULL) == SQL_SUCCESS)
    {
        // Hide data truncated..
        if (wcsncmp(wszState, L"01004", 5))
            fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
    }
}
