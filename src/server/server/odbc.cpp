#include "odbc.h"
#include "onExit.h"
#include <condition_variable>
#include "print.h"
#include "logger.h"
#include "session.h"

void db(
    WCHAR* connectionString,
    std::mutex& queryLock,
    std::vector<SelectQuery>& selectQueries,
    std::vector<NonSelectQuery>& nonSelectQueries,
    std::mutex& waiter,
    std::condition_variable& cv,
    bool* connected,
    std::string queryOnConnect,
    std::mutex& initLock
) {
    auto log = defaultFolderLogger(getSession().get("logs").get("server").asString(), "db.txt", true);
    std::stringstream ss;
    ss << std::this_thread::get_id();
    auto tid = ss.str();
    initLock.lock();
    bool didQueryOnConnect = false;
    std::string queryForLog = "";
    std::string infoForLog = "";
    OnExit _([&]() {
        log(tid + " OnExit called, last query was: " + queryForLog + " info = " + infoForLog);
        if (!didQueryOnConnect)
            initLock.unlock();
    });
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

    SQLDriverConnect(hDbc,
        NULL,
        connectionString,
        SQL_NTS,
        NULL,
        0,
        NULL,
        SQL_DRIVER_NOPROMPT);

    //fwprintf(stderr, L"Connected!\n");
    *connected = true;

    TRYODBC(hDbc,
        SQL_HANDLE_DBC,
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt));

    while (*connected) {
        queryForLog = "";
        infoForLog = std::to_string(0);
        SelectQuery sq;
        NonSelectQuery nsq;
        log(tid + " begin of while");
        {
            infoForLog += std::to_string(1);
            std::unique_lock<std::mutex> lock(waiter);
            log(tid + " waiting on cv");
            cv.wait(lock, [&, connected]() {
                std::lock_guard<std::mutex> g(queryLock);
                return selectQueries.size() != 0 || nonSelectQueries.size() != 0 || !*connected || !didQueryOnConnect;
            });
            infoForLog += std::to_string(2);
            log(
                tid +
                " done waiting on cv:"
                " sq = " + std::to_string(selectQueries.size()) +
                ", nsq = " + std::to_string(nonSelectQueries.size()) +
                ", connected = " + std::to_string(*connected) +
                ", didqueryonconect = " + std::to_string(didQueryOnConnect)
            );
            infoForLog += std::to_string(3);
        }
        infoForLog += std::to_string(4);
        if (!*connected)
            return;
        infoForLog += std::to_string(5);
        bool SELECT = false;
        WCHAR wquery[SQL_QUERY_SIZE];
        std::string q;

        infoForLog += std::to_string(6);
        if (!didQueryOnConnect) {
            nsq = NonSelectQuery(queryOnConnect, [](NonSelectQueryResult) {});
            q = queryOnConnect;
            didQueryOnConnect = true;
            initLock.unlock();
            log(tid + " doing query on conect");
            infoForLog += std::to_string(7);
        }
        else {
            infoForLog += std::to_string(8);
            std::lock_guard<std::mutex> g(queryLock);
            if (selectQueries.size() > nonSelectQueries.size()) {
                sq = selectQueries[0];
                SELECT = true;
                selectQueries.erase(selectQueries.begin());
                q = sq.first;
                infoForLog += std::to_string(9);
            }
            else {
                infoForLog += std::to_string(10);
                nsq = nonSelectQueries[0];
                SELECT = false;
                nonSelectQueries.erase(nonSelectQueries.begin());
                q = nsq.first;
            }
        }
        infoForLog += std::to_string(11);
        log(tid + " executing "  + q);
        queryForLog = q;
        for (unsigned i = 0; i < q.length(); ++i)
            wquery[i] = q[i];
        wquery[q.length()] = 0;

        std::shared_ptr<bool> called = std::make_shared<bool>(false);
        OnExit _([&, called, q]() {
            log(tid + " end of while");
            if (!*called)
                throw std::exception(("Callback never called after query: " + q).c_str());
        });

        RETCODE     RetCode;
        SQLSMALLINT sNumResults;
        RetCode = SQLExecDirect(hStmt, wquery, SQL_NTS);

        switch (RetCode) {
            case SQL_SUCCESS_WITH_INFO:
            {
                log(tid + " query info");
                std::string info = getMessage(hStmt, SQL_HANDLE_STMT, RetCode);
                infoForLog = info;
                if (SELECT)
                    throw std::exception((info + " encountered during select: '" + sq.first + "'\n").c_str());
                else {
                    *called = true;
                    nsq.second(info);
                }
                break;
                // fall through
            }
            case SQL_SUCCESS:
            {
                log(tid + " query sucess");
                infoForLog = "Query Success";
                // If this is a row-returning query, display
                // results
                TRYODBC(hStmt,
                    SQL_HANDLE_STMT,
                    SQLNumResultCols(hStmt, &sNumResults));

                if (sNumResults > 0) {
                    // Does not mean that select has to return at least 1 element, I'm not sure what sNumResults stands for
                    // But select queries that return empty go through here still
                    _ASSERT(SELECT);
                    *called = true;
                    sq.second(getResults(hStmt, sNumResults));
                }
                else {
                    _ASSERT(!SELECT);
                    *called = true;
                    nsq.second(NonSelectQueryResult(getMessage(hStmt, SQL_HANDLE_STMT, RetCode)));
                    SQLLEN cRowCount;
                    TRYODBC(hStmt,
                        SQL_HANDLE_STMT,
                        SQLRowCount(hStmt, &cRowCount));
                }
                break;
            }

            case SQL_ERROR:
            {
                log(tid + " query error");
                infoForLog = "query error";
                std::string err = getMessage(hStmt, SQL_HANDLE_STMT, RetCode);
                if (SELECT)
                    throw std::exception((err + " encountered during select: '" + sq.first + "'\n").c_str());
                else {
                    *called = true;
                    std::cout << err << std::endl;
                    nsq.second(err);
                }
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
    *connected = false;
    log(tid + " exiting");
    if (hStmt)
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    if (hDbc) {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    }
    if (hEnv)
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    std::cout << "Database Disconnected\n";
    exit(-100);
}

std::vector<std::string> getTitles(HSTMT hStmt, BINDING* pBinding) {
    WCHAR           wszTitle[DISPLAY_MAX];
    SQLSMALLINT     iCol = 0;
    std::vector<std::string> titles;
    for (; pBinding; pBinding = pBinding->sNext) {
        TRYODBC(hStmt,
            SQL_HANDLE_STMT,
            SQLColAttribute(hStmt,
                ++iCol,
                SQL_DESC_NAME,
                wszTitle,
                sizeof(wszTitle), // Note count of bytes!
                NULL,
                NULL));
        std::wstring ws(wszTitle);
        titles.push_back(std::string(ws.begin(), ws.end()));
    }
    return titles;
Exit:
    throw std::exception("Failed to get titles for query\n");
}

SelectQueryResult getResults(HSTMT hStmt, SQLSMALLINT cCols) {
    BINDING* pFirstBinding, * pThisBinding;
    RETCODE RetCode = SQL_SUCCESS;

    AllocateBindings(hStmt, cCols, &pFirstBinding);
    auto titles = getTitles(hStmt, pFirstBinding);

    bool fNoData = false;
    SelectQueryResult result;
    int column;
    do {
        TRYODBC(hStmt, SQL_HANDLE_STMT, RetCode = SQLFetch(hStmt));

        if (RetCode == SQL_NO_DATA_FOUND)
            fNoData = true;
        else {
            column = 0;
            result.push_back(JSON());

            for (pThisBinding = pFirstBinding; pThisBinding; pThisBinding = pThisBinding->sNext) {
                auto columnName = titles[column];
                if (pThisBinding->indPtr != SQL_NULL_DATA)
                {
                    auto w = pThisBinding->wszBuffer;
                    auto ws = std::wstring(w);
                    auto s = std::string(ws.begin(), ws.end());
                    result.back()[columnName] = '"' + s + '"';
                }
                else
                    result.back()[columnName] = "\"<NULL>\"";
                ++column;
            }
        }
    } while (!fNoData);
Exit:
    while (pFirstBinding)
    {
        pThisBinding = pFirstBinding->sNext;
        free(pFirstBinding->wszBuffer);
        free(pFirstBinding);
        pFirstBinding = pThisBinding;
    }
    return result;
}

void AllocateBindings(HSTMT hStmt, SQLSMALLINT  cCols, BINDING** ppBinding) {
    SQLSMALLINT     iCol;
    BINDING* pThisBinding, * pLastBinding = NULL;
    SQLLEN          cchDisplay, ssType;
    SQLSMALLINT     cchColumnNameLength;

    for (iCol = 1; iCol <= cCols; iCol++)
    {
        pThisBinding = (BINDING*)(malloc(sizeof(BINDING)));
        if (!(pThisBinding))
        {
            std::cout << "Out of memory!\nDatabase FAILED\n";
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
            std::cout << "Out of memory!\nDatabase FAILED\n";
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
    }
    return;
Exit:
    std::cout << "Database FAILED\n";
    exit(-1);
    return;
}

std::string getMessage(SQLHANDLE      hHandle,
    SQLSMALLINT    hType,
    RETCODE        RetCode) {
    SQLSMALLINT iRec = 0;
    SQLINTEGER  iError;
    WCHAR       wszMessage[1000];
    WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];

    std::string msg;

    if (RetCode == SQL_INVALID_HANDLE)
        msg = "Invalid handle\n";
    else {
        while (SQLGetDiagRec(hType,
            hHandle,
            ++iRec,
            wszState,
            &iError,
            wszMessage,
            (SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)),
            (SQLSMALLINT*)NULL) == SQL_SUCCESS)
        {
            //if (wcsncmp(wszState, L"01004", 5))
                //fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);

            std::wstring werr(wszMessage);
            msg += std::string(werr.begin(), werr.end()) + "\n";
        }
    }
    return msg;
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
