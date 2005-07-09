

#ifndef GA_DEBUG_H
#define GA_DEBUG_H
/**\file
 * name Debuging Tools
 */

/** This function creates a pause in the program which requires user intervention. */
int pausel();

/** An global variable to store the current debug level in. */
extern int debug_level;

/** An global variable to store the default debug level in. */
extern int default_debug_level;

/**@name Debug levels for fitting library:
*/
//@{

/** Absolutely no debug info. */
const int GAR_NICHTS = 0;

/** Debug the stinking Genetic Algorithm. */
const int GA = 2;

//@}

#endif // GA_DEBUG_H
