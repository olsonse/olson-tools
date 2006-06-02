// -*- c -*-
// $Id: logger.c,v 1.1.1.1 2005/01/08 04:27:24 olsonse Exp $
/*
 * Partial Copyright 2000-2004 Spencer Eugene Olson --- All Rights Reserved
 * Partial Copyright Neil Winton <neil@winton.org.uk>
 *   Some of these logging routines are modified code from the Zebedee package
 *   from Niel Winton.  These are thus release under GPL.
 *
 * $Log: logger.c,v $
 * Revision 1.1.1.1  2005/01/08 04:27:24  olsonse
 * Initial import
 *
 */

/*********************\
**		     **
**  Message Logging  **
**		     **
\*********************/

#include "logger.h"
#include "fmacros.h"
#define logger_c_rcsid "$Id: logger.c,v 1.1.1.1 2005/01/08 04:27:24 olsonse Exp $"

#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef __CYGWIN__
#  undef WIN32
#endif

#ifdef WIN32
/* I probably don't need all of these includes, but ... */
#  include <windows.h>
#  include <io.h>
#  include <process.h>
#  include <mmsystem.h>

#elif defined(N_PLAT_NLM)
#  error some more work needs to be done before this will compile for NetWare
#else
#  include <syslog.h>

#endif


#ifdef LOG_MUTEX_TYPE
LOG_MUTEX_TYPE * LogFileMutex = NULL;

/* Make the logger routines fully re-entrant.
 * Set the mutex to be used for the logger.
 * This mutex must protect stdio
 * calls as well as time.h calls (like strftime).
 *
 * By setting this to NULL, no mutexes will be used.
 */
void setLogFileMutex( LOG_MUTEX_TYPE * mutex)
{
    LogFileMutex = mutex;
}

#  ifdef WIN32
#    define mutexLock(a)	EnterCriticalSection((a))
#    define mutexUnlock(a)	LeaveCriticalSection((a))
#  elif defined(N_PLAT_NLM)
#  elif defined(HAVE_PTHREADS)
#    define mutexLock(a)	pthread_mutex_lock((a))
#    define mutexUnlock(a)	pthread_mutex_unlock((a))
#  endif

#  define MUTEX_IO LogFileMutex

#else
#  define mutexLock(a)
#  define mutexUnlock(a)
#endif

FILE *LogFileP = NULL;		/* File handle for log file (NULL => stderr) */
LogType_t LogFileType = LOGFILE_LOCAL;	/* Type of log file */
LogLevel_t LogLevel = LLOG_INFO;	/* Message verbosity level */
int TimestampLog = 0;		/* Should messages have timestamps? */
char * ProgramName = NULL;



void setLogProgramName(const char * prog)
{
    if (!prog) {
	free(ProgramName);
	ProgramName = NULL;
	return;
    } // else

    ProgramName = realloc(ProgramName, strlen(prog) + 1);
    memcpy(ProgramName, prog, strlen(prog) + 1);
}

#define Program (ProgramName != NULL ? ProgramName : "some cool program")

/*
** logThreadPid
**
** Return the current process ID
*/

unsigned long
logThreadPid(void)
{
#ifdef WIN32
    return (unsigned long)GetCurrentProcessId();
#elif defined(N_PLAT_NLM)
    return (unsigned long)GetNLMID();
#else
    return (unsigned long)getpid();
#endif
}

/*
** logThreadTid
**
** Return the current thread ID
*/

unsigned long
LogThreadTid(void)
{
#ifdef WIN32
    return (unsigned long)GetCurrentThreadId();
#elif defined(N_PLAT_NLM)
    return (unsigned long)GetThreadID();
#elif defined(HAVE_PTHREADS)
    return (unsigned long)pthread_self();
#else
    return 0;
#endif
}

/*
** timestamp
**
** Generate a time-stamp string
*/

void
timestamp(char *timeBuf, int local)
{
    time_t now;
    struct tm *tmPtr;

    /* localtime()/gmtime are not thread-safe */

    mutexLock(MUTEX_IO);
    time(&now);
    if (local)
    {
	tmPtr = localtime(&now);
    }
    else
    {
	tmPtr = gmtime(&now);
    }
    strftime(timeBuf, TIMESTAMP_SIZE, "%Y-%m-%d-%H:%M:%S", tmPtr);
    mutexUnlock(MUTEX_IO);
}

#if !defined(N_PLAT_NLM)
/*
** logToSystemLog
**
** Write a message to the system logging facility. On Windows it goes to
** the system application event log. Elsewhere is uses syslog().
*/

void
logToSystemLog(LogLevel_t level, char *msg)
{
#ifdef WIN32
    HANDLE  eventHandle;
    char    *strings[2];


    eventHandle = RegisterEventSource(NULL, Program);

    strings[0] = msg;
    strings[1] = NULL;

    if (eventHandle != NULL)
    {
	ReportEvent(eventHandle,	    /* Handle of event source */
		    (level == LLOG_SEVERE ? EVENTLOG_ERROR_TYPE :
		     EVENTLOG_INFORMATION_TYPE),  /* Event type */
		    (WORD)level,	    /* Event category */
		    0,			    /* Event ID */
		    NULL,		    /* User SID */
		    1,			    /* Number of message strings */
		    0,			    /* Bytes of binary data */
		    (const char **)strings, /* Array of message strings */
		    NULL);		    /* No binary data */
	DeregisterEventSource(eventHandle);
    }
#else
    int logLevel;

    /*
    ** Messages at level 0 are errors, 1 is notice, 2 informational
    ** and everything else is classed as debug.
    */

    switch (level)
    {
    case LLOG_SEVERE:
	logLevel = LOG_ERR;
	break;

    case LLOG_WARNING:
	logLevel = LOG_NOTICE;
	break;

    case LLOG_INFO:
	logLevel = LOG_INFO;
	break;

    default:
	logLevel = LOG_DEBUG;
	break;
    }

    syslog(logLevel, msg);
#endif
}
#endif  // !N_PLAT_NLM

#ifndef _predef_vlog
void vlog(LogLevel_t level, int err, const char *fmt, va_list args);
#  define _predef_vlog
#endif

/**
 * log_severe
 * 
 * Log a message with the severe level.
 */

void
log_severe(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vlog(LLOG_SEVERE, 0, fmt, args);
    va_end(args);
}

#if CAN_ALIAS
    /* make available the weak symbols to allow fortran programs to use this.
     * Unfortunately, not all binary formats allow for weak symbols.
     * */
void log_severe_ (const char *fmt, ...) __attribute__ (( alias ("log_severe") )) ;
#else
    /* take care of the lame binary formats. */
void
log_severe_(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vlog(LLOG_SEVERE, 0, fmt, args);
    va_end(args);
}
#endif

/**
 * log_warning
 * 
 * Log a message with the warning level.
 */

void
log_warning(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vlog(LLOG_WARNING, 0, fmt, args);
    va_end(args);
}

/**
 * log_info
 * 
 * Log a message with the info level.
 */

void
log_info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vlog(LLOG_INFO, 0, fmt, args);
    va_end(args);
}

/**
 * log_config
 * 
 * Log a message with the info level.
 */

void
log_config(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vlog(LLOG_CONFIG, 0, fmt, args);
    va_end(args);
}

/**
 * log_fine
 * 
 * Log a message with the fine level.
 */

void
log_fine(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vlog(LLOG_FINE, 0, fmt, args);
    va_end(args);
}

/**
 * log_finer
 * 
 * Log a message with the finer level.
 */

void
log_finer(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vlog(LLOG_FINER, 0, fmt, args);
    va_end(args);
}

/**
 * log_finest
 * 
 * Log a message with the finest level.
 */

void
log_finest(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vlog(LLOG_FINEST, 0, fmt, args);
    va_end(args);
}

/**
 * log
 * 
 * Log a message with the arbitrary level and error.
 */

void
log_error(LogLevel_t level, int err, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vlog(level, err, fmt, args);
    va_end(args);
}

/*
** vlog
**
** Output a message to the current log file if the message verbosity is
** greater than or equal to the specified level. Messages at level 0
** can not be suppressed (unless the log-file type is NULL) and are all
** error messages.
**
** If errno is non-zero then append the matching error text.
*/

void
vlog(LogLevel_t level, int err, const char *fmt, va_list args)
{
    FILE *fp = LogFileP;
    char timeBuf[TIMESTAMP_SIZE];
    char *timePtr = NULL;
    /* hope that there are no longer messages */
    static char msgBuf[2048];


    if (level > LogLevel || LogFileType == LOGFILE_NULL) return;

    if (fp == NULL)
    {
	fp = stderr;
    }

    if (TimestampLog)
    {
	timestamp(timeBuf, 1);
	timePtr = timeBuf;
    }

    /* Ensure we don't get overlapping messages */
    mutexLock(MUTEX_IO);

    /*
    ** The message format is the program name followed by the (low five
    ** digits of) the PID and thread ID then an optional timestamp followed
    ** by an amount of indentation determined by the level. This is
    ** then followed by the supplied message text and arguments and
    ** finally the error message text (if any) associated with the supplied
    ** error number!
    */

    sprintf(msgBuf, "%s(%lu/%lu): %s%s%.*s%s",
	    Program, (logThreadPid() % 100000), (LogThreadTid() % 100000),
	    (timePtr ? timePtr : ""), (timePtr ? ": " : ""),
	    (int)level, "          ",
	    (level>LLOG_WARNING ? "" : ( level == LLOG_WARNING ? "Warning: ":"ERROR: "))
	    );

    vsprintf(msgBuf + strlen(msgBuf), fmt, args);

    if (err)
    {
	sprintf(msgBuf + strlen(msgBuf), ": (%s)", strerror(err));
    }

    switch (LogFileType)
    {
    case LOGFILE_LOCAL:
	fprintf(fp, "%s\n", msgBuf);
	fflush(fp);
	break;

#if !defined(N_PLAT_NLM)
    case LOGFILE_SYSLOG:
	logToSystemLog(level, msgBuf);
	break;
#endif

    default:
	break;
    }

    mutexUnlock(MUTEX_IO);

}


