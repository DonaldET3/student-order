/* student functions
 * written by Donald_ET3 during August of 2025
 */

struct list_t {
	struct student_t *student;
	struct list_t *next;
};

/* allocate student and initialize variables */
struct student_t* new_student()
{
	struct student_t *student;

	if((student = malloc(sizeof(struct student_t))) == NULL)
		failed("allocate student");
	student->firstname = NULL;
	student->lastname = NULL;
	student->grade = 0; student->stop = 0;
	student->gender = UNKNOWN;
	student->next = NULL;

	return student;
}

/* search student list for a match */
struct student_t* search_students(struct student_t *students, struct student_t *query)
{
	if(query->firstname == NULL) return NULL;

	for(struct student_t *candidate = students;
		candidate != NULL; candidate = candidate->next)
		if(!strcmp(candidate->firstname, query->firstname))
			if(candidate->grade == query->grade)
				if(candidate->stop == query->stop)
					if(candidate->gender == query->gender)
						if(candidate->lastname != NULL)
						{
							if(query->lastname != NULL)
								if(!strcmp(candidate->lastname, query->lastname))
									return candidate;
						}
						else return candidate;

	return NULL;
}

/* output student information to a stream */
int print_student(struct student_t *student, FILE *os)
{
	if(fprintf(os, "first name: %s\n", student->firstname) < 0) return -1;

	if(student->lastname != NULL)
		if(fprintf(os, "last name: %s\n", student->lastname) < 0) return -1;

	if(fprintf(os, "grade: %u  ", student->grade) < 0) return -1;

	if(student->stop)
		if(fprintf(os, "stop: %u  ", student->stop) < 0) return -1;

	if(student->gender != UNKNOWN)
	{
		if(fprintf(os, "gender: ") < 0) return -1;
		if(student->gender == MALE) if(fprintf(os, "male") < 0) return -1;
		if(student->gender == FEMALE) if(fprintf(os, "female") < 0) return -1;
	}

	if(putc('\n', os) == EOF) return -1;

	return 0;
}

/* duplicate student information */
struct student_t *copy_student(struct student_t *copy, struct student_t *student)
{
	copy->firstname = str_set(copy->firstname, student->firstname);
	copy->lastname = str_set(copy->lastname, student->lastname);
	copy->grade = student->grade;
	copy->stop = student->stop;
	copy->gender = student->gender;
	copy->next = student->next;

	return copy;
}

/* free the space allocated to a student */
void free_student(struct student_t *student)
{
	free(student->firstname);
	free(student->lastname);
	free(student);
}

/* add a student to the data set */
void addstudent(struct data *d, struct input_struct *is)
{
	char *s;
	struct student_t *student;

	student = new_student();

	printf("enter student first name (leave empty to cancel adding student)\n:");
	s = input_line(is);
	if(*s == '\n') {free(student); return;}
	student->firstname = str_cpy(s);

	printf("enter student last name (leave empty to skip)\n:");
	s = input_line(is);
	if(*s != '\n') student->lastname = str_cpy(s);

	while(true)
	{
		printf("enter student grade (leave empty to cancel adding student)\n:");
		s = input_line(is);
		if(*s == '\n')
		{
			free_student(student);
			return;
		}
		if(sscanf(s, "%u", &student->grade) == 1)
		{
			if(student->grade > 12)
			{
				puts("student grade is out of range (0-12)");
				student->grade = 0;
			}
			else break;
		}
		else puts("not a valid number");
	}

	while(true)
	{
		printf("enter student stop number (leave empty to skip)\n:");
		s = input_line(is);
		if(*s == '\n') break;
		if(sscanf(s, "%u", &student->stop) == 1) break;
		puts("not a valid number");
	}

	while(true)
	{
		printf("enter student gender [m/f] (leave empty to skip)\n:");
		s = input_line(is);
		if(*s == '\n') break;

		/* convert string to lowercase */
		for(size_t i = 0; s[i] != '\0'; i++) s[i] = tolower(s[i]);

		if((!strcmp(s, "male")) || (!strcmp(s, "m")))
			{student->gender = MALE; break;}
		if((!strcmp(s, "female")) || (!strcmp(s, "f")))
			{student->gender = FEMALE; break;}
		puts("not a valid entry");
	}

	if(search_students(d->students, student) != NULL)
	{
		puts("This student is essentially identical to an existing student.\n"
			"student addition cancelled");
		free_student(student);
		return;
	}

	student->next = d->students;
	d->students = student;

	puts("new student added");
	print_student(student, stdout);
}

struct student_t* find_student(struct student_t *list, struct input_struct *is)
{
	char *s;
	unsigned x;
	enum gender_t g;
	struct student_t *student;
	struct list_t *candidates = NULL, *c, **spot;

	while(true)
	{
		printf("enter first name of student to search for (leave empty to cancel)\n:");
		s = input_line(is);
		if(*s == '\n') {puts("cancelled"); return NULL;}
		for(student = list; student != NULL; student = student->next)
			if(!strcmp(s, student->firstname))
			{
				if((c = malloc(sizeof(struct list_t))) == NULL)
					failed("allocate candidate");
				c->student = student;
				c->next = candidates;
				candidates = c;
			}
		if(candidates == NULL) puts("no such student");
		else if(candidates->next == NULL)
		{
			puts("student match");
			free(candidates);
			return student;
		}
		else break;
	}

	while(true)
	{
		printf("enter student last name (leave empty to skip)\n:");
		s = input_line(is);
		if(*s == '\n') break;
		for(c = candidates; c != NULL; c = c->next)
			if(!strcmp(s, c->student->lastname)) break;
		if(c != NULL)
		{
			for(spot = &candidates; *spot != NULL; spot = &(*spot)->next)
				if(strcmp(s, (*spot)->student->lastname))
				{
					c = (*spot)->next;
					free(*spot);
					*spot = c;
				}
			if(candidates->next == NULL)
			{
				puts("student match");
				student = candidates->student;
				free(candidates);
				return student;
			}
			else break;
		}
		puts("no such student");
	}

	while(true)
	{
		printf("enter student grade (leave empty to cancel)\n:");
		s = input_line(is);
		if(*s == '\n') return NULL;
		if(sscanf(s, "%u", &x) != 1) puts("not a valid number");
		else
		{
			for(c = candidates; c != NULL; c = c->next)
				if(c->student->grade == x) break;
			if(c != NULL)
			{
				for(spot = &candidates; *spot != NULL; spot = &(*spot)->next)
					if((*spot)->student->grade != x)
					{
						c = (*spot)->next;
						free(*spot);
						*spot = c;
					}
				if(candidates->next == NULL)
				{
					puts("student match");
					student = candidates->student;
					free(candidates);
					return student;
				}
				else break;
			}
			puts("no such student");
		}
	}

	while(true)
	{
		printf("enter student stop number (leave empty to skip)\n:");
		s = input_line(is);
		if(*s == '\n') break;
		if(sscanf(s, "%u", &x) != 1) puts("not a valid number");
		else
		{
			for(c = candidates; c != NULL; c = c->next)
				if(c->student->stop == x) break;
			if(c != NULL)
			{
				for(spot = &candidates; *spot != NULL; spot = &(*spot)->next)
					if((*spot)->student->stop != x)
					{
						c = (*spot)->next;
						free(*spot);
						*spot = c;
					}
				if(candidates->next == NULL)
				{
					puts("student match");
					student = candidates->student;
					free(candidates);
					return student;
				}
				else break;
			}
			puts("no such student");
		}
	}

	while(true)
	{
		printf("enter student gender [unknown/male/female] (leave empty to cancel)\n:");
		s = input_line(is);
		if(*s == '\n') return NULL;

		/* convert string to lowercase */
		for(size_t i = 0; s[i] != '\0'; i++) s[i] = tolower(s[i]);

		if((!strcmp(s, "unknown")) || (!strcmp(s, "u"))) g = UNKNOWN;
		else if((!strcmp(s, "male")) || (!strcmp(s, "m"))) g = MALE;
		else if((!strcmp(s, "female")) || (!strcmp(s, "f"))) g = FEMALE;
		else {puts("not a valid entry"); continue;}

		for(c = candidates; c != NULL; c = c->next)
			if(c->student->gender == g) break;
		if(c != NULL)
		{
			for(spot = &candidates; *spot != NULL; spot = &(*spot)->next)
				if((*spot)->student->gender != g)
				{
					c = (*spot)->next;
					free(*spot);
					*spot = c;
				}
			student = candidates->student;
			if(candidates->next == NULL) puts("student match");
			else puts("multiple students matched (bug); picking the first one");
			while(candidates != NULL)
			{
				c = candidates->next;
				free(candidates);
				candidates = c;
			}
			return student;
		}
		puts("no such student");
	}
}

/* edit student information */
void editstudent(struct data *d, struct input_struct *is)
{
	unsigned x;
	char *s;
	struct student_t *student, *edit;

	if((student = find_student(d->students, is)) == NULL) return;
	puts("editing student");
	print_student(student, stdout);
	edit = copy_student(new_student(), student);

	printf("current first name: %s\n", edit->firstname);
	printf("enter student first name (leave empty to keep)\n:");
	s = input_line(is);
	if(*s != '\n') edit->firstname = str_set(edit->firstname, s);

	printf("current last name: %s\n", edit->lastname);
	printf("enter student last name (leave empty to keep)\n:");
	s = input_line(is);
	if(*s != '\n') edit->lastname = str_set(edit->lastname, s);

	while(true)
	{
		printf("current student grade: %u\n", edit->grade);
		printf("enter student grade (leave empty to keep)\n:");
		s = input_line(is);
		if(*s == '\n') break;
		if(sscanf(s, "%u", &x) == 1)
		{
			if(x > 12) puts("student grade is out of range (0-12)");
			else {edit->grade = x; break;}
		}
		else puts("not a valid number");
	}

	while(true)
	{
		printf("current student stop number: %u\n", edit->stop);
		printf("enter student stop number (leave empty to keep)\n:");
		s = input_line(is);
		if(*s == '\n') break;
		if(sscanf(s, "%u", &edit->stop) == 1) break;
		puts("not a valid number");
	}

	while(true)
	{
		printf("current student gender: ");
		switch(edit->gender)
		{
			case UNKNOWN: puts("unknown"); break;
			case MALE: puts("male"); break;
			case FEMALE: puts("female");
		}
		printf("enter student gender [unknown/male/female] (leave empty to keep)\n:");
		s = input_line(is);
		if(*s == '\n') break;

		/* convert string to lowercase */
		for(size_t i = 0; s[i] != '\0'; i++) s[i] = tolower(s[i]);

		if((!strcmp(s, "unknown")) || (!strcmp(s, "u")))
			{edit->gender = UNKNOWN; break;}
		if((!strcmp(s, "male")) || (!strcmp(s, "m")))
			{edit->gender = MALE; break;}
		if((!strcmp(s, "female")) || (!strcmp(s, "f")))
			{edit->gender = FEMALE; break;}
		puts("not a valid entry");
	}

	if(search_students(d->students, edit) != NULL)
		puts("This student is essentially identical to an existing student.\n"
			"student edit cancelled");
	else
	{
		copy_student(student, edit);
		puts("student edited");
		print_student(student, stdout);
	}

	free_student(edit);
}

/* remove a student from the data set */
void removestudent(struct data *d, struct input_struct *is)
{
	char *s;
	struct student_t *match, **spot;

	if((match = find_student(d->students, is)) == NULL) return;
	print_student(match, stdout);

	printf("Are you sure you want to remove this student?");

	if(yes_or_other(is))
	{
		for(spot = &d->students; *spot != match; spot = &(*spot)->next);
		*spot = match->next;
		free_student(match);
		puts("student removed");
	}
	else puts("student retained");
}

int liststudents(struct data *d, FILE *os)
{
	if(fputs("student list\n", os) == EOF) return -1;

	for(struct student_t *student = d->students;
		student != NULL; student = student->next)
	{
		if(print_student(student, os)) return -1;

		if(student->next != NULL) if(putc('\n', os) == EOF) return -1;
	}

	return 0;
}

/* display a list of students */
void liststudentsprompt(struct data *d, struct input_struct *is)
{
	FILE *os;

	if((os = get_stream(is)) == NULL) return;

	if(liststudents(d, os)) puts("could not write data");

	if(os != stdout) fclose(os);
}

