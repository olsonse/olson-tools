// -*- c -*-
// $Id: logger.h,v 1.2 2005/05/12 04:27:32 olsonse Exp $
/*
 * Partial Copyright 2000-2004 Spencer Eugene Olson --- All Rights Reserved
 * Partial Copyright Neil Winton <neil@winton.org.uk>
 *   Some of these logging routines are modified code from the Zebedee package
 *   from Niel Winton.  These are thus release under GPL.
 *
 * $Log: logger.h,v $
 * Revision 1.2  2005/05/12 04:27:32  olsonse
 * Fixed to for Intel 8.1 compilers.
 * Found (using intel compiler) and fixed an array overflow in BField::potential.
 * Didn't find it earlier because the array is on the stack for the function.
 *
 * Added fmacros.h file to simplify mixing fortran code with others.
 * Added alias function names for Fortran interoperability.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:24  olsonse
 * Initial import
 *
 */

/** \file
 * A collection of handy logging routines.
 *
 * Partial Copyright 2000-2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * Partial Copyright Neil Winton <neil@winton.org.uk>
 *
 *   Some of these logging routines are modified code from the Zebedee package
 *   from Niel Winton.  These are thus released under GPL:
 *
 * This program is free software; you can redistribute it and/or  
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either version 2  
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
namespace olson_tools { namespace logger {
extern "C" {
#endif 

#define logger_h_rcsid "$Id: logger.h,v 1.2 2005/05/12 04:27:32 olsonse Exp $"

#include <stdio.h>

#define STR(x) #x
#define XSTR(x) STR(x)

/** Macro to produce a c-string constant of the source file and line number. */
#define FILE_LINE __FILE__"("XSTR(__LINE__)"):  "

/** Log message destinations.
 * These enumerated type values are used to indicate the destination of
 * log messages.
*/

typedef enum
{
    LOGFILE_NULL,
#if !defined(N_PLAT_NLM)
    LOGFILE_SYSLOG,
#endif
    LOGFILE_LOCAL
}
LogType_t;

/** The various log levels.
 * I really like the new java Logger routines, so ...
 * */
typedef enum
{
    LLOG_SEVERE		= 0,
    LLOG_WARNING	= 1,
    LLOG_INFO		= 2,
    LLOG_CONFIG		= 3,
    LLOG_FINE		= 4,
    LLOG_FINER		= 5,
    LLOG_FINEST		= 6
}
LogLevel_t;


/** Log a message with specified log level and errno.
 * @param level The log level (LogLevel_t) of the message.
 * @param err Errno value to be translated (0 == no errno).
 * @param fmt A printf type format.
 * @param ... All of the arguments for the format.
 * @see LogLevel_t
 * */
void log_error(LogLevel_t level, int err, const char *fmt, ...);

/** Log a severe message.
 * @param fmt A printf type format.
 * @param ... All of the arguments for the format.
 * @see LogLevel_t
 * */
void log_severe(const char *fmt, ...);

/** Log a warning message.
 * @param fmt A printf type format.
 * @param ... All of the arguments for the format.
 * @see LogLevel_t
 * */
void log_warning(const char *fmt, ...);

/** Log an info message.
 * @param fmt A printf type format.
 * @param ... All of the arguments for the format.
 * @see LogLevel_t
 * */
void log_info(const char *fmt, ...);

/** Log a config message.
 * @param fmt A printf type format.
 * @param ... All of the arguments for the format.
 * @see LogLevel_t
 * */
void log_config(const char *fmt, ...);

/** Log a fine message.
 * @param fmt A printf type format.
 * @param ... All of the arguments for the format.
 * @see LogLevel_t
 * */
void log_fine(const char *fmt, ...);

/** Log a finer message.
 * @param fmt A printf type format.
 * @param ... All of the arguments for the format.
 * @see LogLevel_t
 * */
void log_finer(const char *fmt, ...);

/** Log a finest message.
 * @param fmt A printf type format.
 * @param ... All of the arguments for the format.
 * @see LogLevel_t
 * */
void log_finest(const char *fmt, ...);

#ifdef va_list
void vlog(LogLevel_t level, int err, const char *fmt, va_list args);
#  define _predef_vlog
#endif

/** Set the program name to be used in log messages.
 * @param prog The name of the program that will be prepended to all log
 * messages.
 * */
void setLogProgramName(const char * prog);

/** Explicitly log a message to the syslog sub-system. */
void logToSystemLog(LogLevel_t level, char *msg);

/** The size of the char array that must be given to timestamp.
 * @see timestamp.
 * */
#define TIMESTAMP_SIZE	20	/* Size of YYYY-dd-mm-HH:MM:SS timestamp */

/** Generates a timestamp.
 * @param timeBuf A char array of at least TIMESTAMP_SIZE length.
 * @param local Integer boolean (0|1) of whether to to local time or gmtime.
 * @see TIMESTAMP_SIZE.
 * */
void timestamp(char *timeBuf, int local);

#ifdef WIN32
#  include <windows.h>
#  define LOG_MUTEX_TYPE CRITICAL_SECTION
#elif defined(N_PLAT_NLM)
#elif defined(USE_PTHREAD)
/*  This is probably alread included, but oh well. */
#  include <pthread.h>
#  define LOG_MUTEX_TYPE pthread_mutex_t
#endif


#ifdef LOG_MUTEX_TYPE
/* Make the logger routines fully re-entrant.
 * Set the mutex to be used for the logger.
 * This mutex must protect stdio
 * calls as well as time.h calls (like strftime).
 *
 * By setting this to NULL, no mutexes will be used.
 */
void setLogFileMutex( LOG_MUTEX_TYPE * mutex);
#endif

/** Integer value to indicate whether messages have timestamps. */
extern int TimestampLog;

/** File handle for log file (NULL => stderr). */
extern FILE *LogFileP;

/** Type of log file.
 * @see LogType_t
 */
extern LogType_t LogFileType;

/** Message verbosity level.
 * All messages with level > LogLevel will be discarded.
 * */
extern LogLevel_t LogLevel;

/** Set log file.
 * @param logfile
 *    The name of the file to set log output to.  There are three special
 *    values: <br>
 *        NULL          : do not log any messages. <br>
 *        ":syslog:"    : send messages to syslog. <br>
 *        ":stderr:"    : send messages to stderr. <br>
 */
void setLogFile(const char * logfile);

#ifdef __cplusplus
}
}} /*namespace olson_tools::logger */
#endif 

#endif // LOGGER_H

