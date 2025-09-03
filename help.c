/* help messages
 * written by Donald_ET3 during August of 2025
 */

/* command line help */
void help()
{
	char *message = "Seat Chart\n"
	"version 1\n\n"
	"A program for arranging seating assignments on a bus\n"
	"based on several different factors.\n\n"
	"options\n"
	"h: help message\n"
	"f: load data from a file\n";
	fputs(message, stderr);
}

/* core help */
void corehelp()
{
	char *message = "Seat Chart\n"
	"version 1\n"
	"core commands\n"
	"quit, q: quit the program\n"
	"help, h, ?: help message\n"
	"filehelp, fh: list file commands\n"
	"studenthelp, sh: list student commands\n"
	"rulehelp, rh: list ordering rule commands";
	puts(message);
}

/* file commands */
void filehelp()
{
	char *message = "store, save, s: store data in a file\n"
	"load, l: load data from a file\n"
	"clear, reset, new: clear program data";
	puts(message);
}

/* student commands */
void studenthelp()
{
	char *message = "addstudent, as: add a student to the data set\n"
	"editstudent, es: edit student information\n"
	"removestudent, rs: remove a student from the data set\n"
	"liststudents, ls: display a list of students\n"
	"gradecounts, gc: display the number of students in each grade";
	puts(message);
}

/* rule commands */
void rulehelp()
{
	char *message = "ampm: set morning or afternoon stop ordering\n"
	"gradeprecedence, ap: precedence of grade order in seating arrangement\n"
	"genderprecedence, gp: precedence of gender in seating assignment\n"
	"stopprecedence, sp: precedence of stop order in seating arrangement\n"
	"viewruleorder, vro: display student ordering rule configuraton\n"
	"vieworder, vo: output ordered student list";
	puts(message);
}

