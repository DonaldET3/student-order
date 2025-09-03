/* Student Order
 * version 1
 * written by Donald_ET3 during August of 2025
 */

#include <stdio.h>
/* putc()
 * fputs()
 * printf()
 * fprintf()
 * sscanf()
 * getline()
 * EOF
 */

#include <stdlib.h>
/* free()
 * exit()
 * EXIT_FAILURE
 * EXIT_SUCCESS
 * size_t
 * NULL
 */

#include <string.h>
/* strcmp()
 */

#include <stdbool.h>
/* bool
 * true
 * false
 */

#include <unistd.h>
/* getopt()
 */

#include <ctype.h>
/* tolower()
 */

#include <inttypes.h>
/* uintmax_t
 */

#include "fail.c"
/* includes errno.h
 * fail()
 * failed()
 */

#include "ios.c"
/* struct input_struct
 * input_init()
 * input_close()
 * input_line()
 */

/* gender type */
enum gender_t {UNKNOWN, MALE, FEMALE};

/* rule type */
enum rule_t {NONE, GRADE, GENDER, STOP};

/* student data */
struct student_t {
	char *firstname, *lastname;
	unsigned grade, stop;
	enum gender_t gender;
	struct student_t *next;
};

/* main data structure */
struct data {
	bool am;
	struct student_t *students;
	enum rule_t rules[3];
};

/* help messages */
#include "help.c"

/* student functions */
#include "student.c"

/* rule functions */
#include "rule.c"

/* file functions */
#include "file.c"

/* invalid command line argument */
void invalid(char c)
{
	fprintf(stderr, "argument supplied to -%c is invalid\n", c);
	exit(EXIT_FAILURE);
}

/* default values */
void data_init(struct data *d)
{
	d->am = false;
	d->students = NULL;

	/* default the rule order */
	d->rules[0] = GRADE;
	d->rules[1] = GENDER;
	d->rules[2] = STOP;
}

void loop(struct data *d)
{
	char *s;
	struct input_struct *is;

	is = input_init(stdin);

	/* command loop */
	while(true)
	{
		/* command prompt */
		printf("so> ");
		/* get command */
		s = input_line(is);

		/* core commands */
		if((!strcmp(s, "quit")) || (!strcmp(s, "q"))) break;
		else if((!strcmp(s, "help")) || (!strcmp(s, "h")) ||
			(!strcmp(s, "?"))) corehelp();
		else if((!strcmp(s, "filehelp")) || (!strcmp(s, "fh"))) filehelp();
		else if((!strcmp(s, "studenthelp")) || (!strcmp(s, "sh"))) studenthelp();
		else if((!strcmp(s, "rulehelp")) || (!strcmp(s, "rh"))) rulehelp();
		/* file commands */
		else if((!strcmp(s, "store")) || (!strcmp(s, "save")) ||
			(!strcmp(s, "s"))) store(d, is);
		else if((!strcmp(s, "load")) || (!strcmp(s, "l"))) loadprompt(d, is);
		else if((!strcmp(s, "clear")) || (!strcmp(s, "reset")) ||
			(!strcmp(s, "new"))) clearprompt(d, is);
		/* student commands */
		else if((!strcmp(s, "addstudent")) || (!strcmp(s, "as"))) addstudent(d, is);
		else if((!strcmp(s, "editstudent")) || (!strcmp(s, "es"))) editstudent(d, is);
		else if((!strcmp(s, "removestudent")) || (!strcmp(s, "rs"))) removestudent(d, is);
		else if((!strcmp(s, "liststudents")) || (!strcmp(s, "ls"))) liststudentsprompt(d, is);
		else if((!strcmp(s, "gradecounts")) || (!strcmp(s, "gc"))) gradecounts(d->students);
		/* rule commands */
		else if(!strcmp(s, "ampm")) ampm(d, is);
		else if((!strcmp(s, "gradeprecedence")) || (!strcmp(s, "ap"))) gradeprecedence(d, is);
		else if((!strcmp(s, "genderprecedence")) || (!strcmp(s, "gp"))) genderprecedence(d, is);
		else if((!strcmp(s, "stopprecedence")) || (!strcmp(s, "sp"))) stopprecedence(d, is);
		else if((!strcmp(s, "viewruleorder")) || (!strcmp(s, "vro"))) viewruleorder(d, is);
		else if((!strcmp(s, "vieworder")) || (!strcmp(s, "vo"))) vieworder(d, is);
		/* unrecognized command */
		else puts("unrecognized command");
	}

	input_close(is);
}

int main(int argc, char **argv)
{
	int c;
	char *fn = NULL;
	extern char *optarg;
	extern int opterr, optind, optopt;
	struct data d;

	/* the errno symbol is defined in errno.h */
	errno = 0;

	/* parse the command line */
	while((c = getopt(argc, argv, "hf:")) != -1)
		switch(c)
		{
			case 'h': help(); exit(EXIT_SUCCESS);
			case 'f': fn = optarg; break;
			case '?': exit(EXIT_FAILURE);
		}

	data_init(&d);
	if(fn != NULL) load(&d, fn);
	loop(&d);
	clear(&d);

	return EXIT_SUCCESS;
}

