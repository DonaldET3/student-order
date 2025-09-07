/* stop functions
 * written by Donald_ET3 September 2025
 */

/* increment later stop numbers to insert a new stop */
void insertstop(struct student_t *students, struct input_struct *is)
{
	char *s;
	unsigned stop;

	while(true)
	{
		printf("enter stop number to insert (leave empty to cancel)\n:");
		s = input_line(is);
		if(*s == '\n') break;
		if(sscanf(s, "%u", &stop) == 1) break;
		puts("not a valid number");
	}

	for(struct student_t *student = students;
		student != NULL; student = student->next)
		if(student->stop >= stop) ++student->stop;
}

/* decrement later stop numbers to remove a stop */
void removestop(struct student_t *students, struct input_struct *is)
{
	char *s;
	unsigned stop;

	while(true)
	{
		printf("enter stop number to remove (leave empty to cancel)\n:");
		s = input_line(is);
		if(*s == '\n') break;
		if(sscanf(s, "%u", &stop) == 1) break;
		puts("not a valid number");
	}

	for(struct student_t *student = students;
		student != NULL; student = student->next)
		if(student->stop == stop) student->stop = 0;
		else if(student->stop > stop) --student->stop;
}

