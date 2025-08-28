/* fatal error library
 * written and developed by Donald_ET3 2018-2025
 */

#include <errno.h>
/* errno
 */

#include <stdio.h>
/* putc()
 * fputs()
 * fprintf()
 */

#include <stdlib.h>
/* exit()
 * EXIT_FAILURE
 */

#include <string.h>
/* strerror_l()
 */

#include <locale.h>
/* uselocale()
 * locale_t
 */

#ifndef FAILL
#define FAILL

/* print error message and quit */
void fail(char *message)
{
	/* print error message */
	fputs(message, stderr);
	/* elaborate on the error if possible */
	if(errno) fprintf(stderr, ": %s", strerror_l(errno, uselocale((locale_t)0)));
	putc('\n', stderr);
	exit(EXIT_FAILURE);
}

/* "failed to" <error message> and quit */
void failed(char *message)
{
	/* prepend "failed to" to the error message */
	fputs("failed to ", stderr);
	fail(message);
}

#endif

