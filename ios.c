/* I/O and string library
 * written by Donald_ET3 during August of 2025
 */

/* uses fatal error library */

#include <stdio.h>
/* getline()
 */

#include <stdlib.h>
/* malloc()
 * realloc()
 * free()
 * size_t
 * NULL
 */

#include <string.h>
/* strlen()
 * strcpy()
 * strcat()
 * strtok()
 */

#include <ctype.h>
/* tolower()
 */

#ifndef DSTRINGL
#define DSTRINGL

/* input string struct */
struct input_struct {
	/* input stream */
	FILE *stream;
	/* space allocated */
	size_t space;
	/* the string itself */
	char *line;
};

/* initialize input struct */
struct input_struct *input_init(FILE *s)
{
	struct input_struct *is;

	if((is = malloc(sizeof(struct input_struct))) == NULL)
		failed("allocate input struct");
	is->stream = s;
	is->space = 0;
	is->line = NULL;

	return is;
}

/* close (deallocate) input struct */
void input_close(struct input_struct *is)
{
	free(is->line);
	free(is);
}

/* read input line */
char *input_line(struct input_struct *is)
{
	if(getline(&is->line, &is->space, is->stream) == -1)
	{
		if(is->stream == stdin) failed("read input line");
		return NULL;
	}
	strtok(is->line, "\n");
	return is->line;
}

/* get yes or no (other) answer */
bool yes_or_other(struct input_struct *is)
{
	char *s;

	printf(" [yes/no]\n:");

	s = input_line(is);

	/* convert string to lowercase */
	for(size_t i = 0; s[i] != '\0'; i++) s[i] = tolower(s[i]);

	/* check for "yes" */
	if((!strcmp(s, "yes")) || (!strcmp(s, "y")))
		return true;

	/* any other response is a negative */
	return false;
}

FILE* get_stream(struct input_struct *is)
{
	char *s;
	FILE *os;

	while(true)
	{
		printf("Write to file or display on screen? [write/display]\n"
			"(leave empty to cancel)\n:");
		s = input_line(is);
		if(*s == '\n') return NULL;

		/* convert string to lowercase */
		for(size_t i = 0; s[i] != '\0'; i++) s[i] = tolower(s[i]);

		if((!strcmp(s, "write")) || (!strcmp(s, "w")))
		{
			printf("enter file name\n:");
			s = input_line(is);
			if((os = fopen(s, "w")) == NULL)
			{printf("cannot open \"%s\"\n", s); return NULL;}
			return os;
		}
		if((!strcmp(s, "display")) || (!strcmp(s, "d"))) return stdout;
		puts("not a valid entry");
	}
}

/* copy string */
char *str_cpy(char *a)
{
	char *b;

	if((b = malloc(strlen(a) + 1)) == NULL) failed("allocate string copy");
	return strcpy(b, a);
}

/* set string */
char *str_set(char *b, char *a)
{
	if((b = realloc(b, strlen(a) + 1)) == NULL) failed("reallocate string");
	return strcpy(b, a);
}

/* concatenate strings */
char *str_cat(char *a, char *b)
{
	if((a = realloc(a, strlen(a) + strlen(b) + 1)) == NULL)
		failed("allocate concatenated string");
	return strcat(a, b);
}

#endif

