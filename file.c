/* file functions
 * written and developed by Donald_ET3 during August and September of 2025
 */

/* clear program data */
void clear(struct data *d)
{
	struct student_t *student, *next;

	/* default seat values */
	d->am = false;

	/* free the student list */
	for(student = d->students; student != NULL; student = next)
	{
		next = student->next;
		free_student(student);
	}
	d->students = NULL;

	/* default the rule order */
	d->rules[0] = GRADE;
	d->rules[1] = GENDER;
	d->rules[2] = STOP;
}

/* prompt the operator before clearing */
void clearprompt(struct data *d, struct input_struct *is)
{
	printf("Are you sure you want to clear program data?");

	if(yes_or_other(is))
	{
		clear(d);
		puts("program data is cleared");
	}
	else puts("program data is retained");
}

int read_student_section(struct data *d, struct input_struct *is)
{
	int error = 0;
	char *l;
	struct student_t *student, **spot;

	for(spot = &d->students; *spot != NULL; spot = &(*spot)->next);
	student = new_student();

	while(true)
	{
		if((l = input_line(is)) == NULL) return 1;

		if(*l == '\n')
		{
			if((student->firstname == NULL) ||
				(search_students(d->students, student) != NULL)) 
				free_student(student);
			else *spot = student;
			return error;
		}

		if((l = strtok(l, " ")) == NULL) return 1;

		if(!strcmp(l, "next"))
		{
			/* dump the data if there is no name or the student is a duplicate */
			if((student->firstname == NULL) ||
				(search_students(d->students, student) != NULL)) 
				free_student(student);
			else
			{
				*spot = student;
				spot = &student->next;
			}
			student = new_student();
		}
		else if(!strcmp(l, "firstname"))
		{
			if((l = strtok(NULL, "\n")) == NULL) {error = 1; continue;}
			student->firstname = str_cpy(l);
		}
		else if(!strcmp(l, "lastname"))
		{
			if((l = strtok(NULL, "\n")) == NULL) {error = 1; continue;}
			student->lastname = str_cpy(l);
		}
		else if(!strcmp(l, "grade"))
		{
			if((l = strtok(NULL, " ")) == NULL) {error = 1; continue;}
			if(sscanf(l, "%u", &student->grade) != 1) error = 1;
		}
		else if(!strcmp(l, "stop"))
		{
			if((l = strtok(NULL, " ")) == NULL) {error = 1; continue;}
			if(sscanf(l, "%u", &student->stop) != 1) error = 1;
		}
		else if(!strcmp(l, "gender"))
		{
			if((l = strtok(NULL, " ")) == NULL) {error = 1; continue;}
			if(!strcmp(l, "male")) student->gender = MALE;
			else if(!strcmp(l, "female")) student->gender = FEMALE;
			else error = 1;
		}
	}
}

int read_rule_section(struct data *d, struct input_struct *is)
{
	int error = 0, rule;
	char *l;
	unsigned precedence;

	while(true)
	{
		if((l = input_line(is)) == NULL) return 1;
		/* return on empty line */
		if(*l == '\n') return error;
		/* get word before space */
		if((l = strtok(l, " ")) == NULL) return 1;

		/* ordering rules */
		if(!strcmp(l, "am")) {d->am = true; continue;}
		else if(!strcmp(l, "grade")) rule = GRADE;
		else if(!strcmp(l, "gender")) rule = GENDER;
		else if(!strcmp(l, "stop")) rule = STOP;
		else continue;

		if((l = strtok(NULL, " ")) == NULL) {error = 1; continue;}
		if(sscanf(l, "%u", &precedence) != 1) {error = 1; continue;}

		/* there are only three rules */
		if(precedence > 2) {error = 1; continue;}
		d->rules[precedence] = rule;
	}
}

/* read data from a file */
int read_file(struct data *d, struct input_struct *is)
{
	int error = 0;
	char *l;

	if((l = input_line(is)) == NULL) return -1;

	/* check file header */
	if(strcmp(l, "Student Order v1")) {puts("wrong file type"); return -1;}

	while(true)
	{
		if((l = input_line(is)) == NULL) return 1;
		/* get text before the first space, ignore the rest of the line */
		if((l = strtok(l, " ")) == NULL) continue;

		/* proper file termination */
		if(!strcmp(l, "end")) return error;
		/* section headers */
		else if(!strcmp(l, "student")) error |= read_student_section(d, is);
		else if(!strcmp(l, "rule")) error |= read_rule_section(d, is);
		/* skip unrecognized section */
		else while(*l != '\n') if((l = input_line(is)) == NULL) return 1;
	}

	return 1;
}

/* load data from a file */
void load(struct data *d, char *fn)
{
	struct input_struct *is;
	FILE *inf;

	if((inf = fopen(fn, "r")) == NULL) {printf("cannot open \"%s\"\n", fn); return;}

	is = input_init(inf);

	clear(d);

	switch(read_file(d, is))
	{
		case -1: printf("failed to read from \"%s\"\n", fn); break;
		case 0: printf("complete data loaded from \"%s\"\n", fn); break;
		case 1: printf("loaded incomplete file \"%s\"\n", fn);
	}

	input_close(is);

	fclose(inf);
}

/* prompt for filename and call load() */
void loadprompt(struct data *d, struct input_struct *is)
{
	char *fn;

	printf("enter file name to load (leave blank to cancel)\n:");

	fn = input_line(is);

	if(*fn == '\n') puts("file load canceled");
	else load(d, fn);
}

/* write data to a file */
int write_file(struct data *d, FILE *of)
{
	/* write file header */
	if(fputs("Student Order v1\n\n", of) == EOF) return -1;

	/* write student list */
	if(fputs("student\n", of) == EOF) return -2;
	for(struct student_t *student = d->students;
		student != NULL; student = student->next)
	{
		if(fprintf(of, "firstname %s\n", student->firstname) < 0) return -2;
		if(fprintf(of, "lastname %s\n", student->lastname) < 0) return -2;

		if(fprintf(of, "grade %u\n", student->grade) < 0) return -2;
		if(student->stop)
			if(fprintf(of, "stop %u\n", student->stop) < 0) return -2;

		switch(student->gender)
		{
			case UNKNOWN: break;
			case MALE: if(fputs("gender male\n", of) == EOF) return -2; break;
			case FEMALE: if(fputs("gender female\n", of) == EOF) return -2; break;
		}

		if(student->next != NULL)
			if(fputs("next\n", of) == EOF) return -2;
	}
	if(putc('\n', of) == EOF) return -2;

	/* write ordering rules */
	if(fputs("rule\n", of) == EOF) return -2;
	if(d->am) if(fputs("am\n", of) == EOF) return -2;
	for(int i = 0; i < 3; i++)
		switch(d->rules[i])
		{
			case NONE: continue;
			case GRADE: if(fprintf(of, "grade %d\n", i) < 0) return -2; break;
			case GENDER: if(fprintf(of, "gender %d\n", i) < 0) return -2; break;
			case STOP: if(fprintf(of, "stop %d\n", i) < 0) return -2; break;
		}
	if(putc('\n', of) == EOF) return -2;

	/* write proper file termination */
	if(fputs("end\n", of) == EOF) return -2;

	return 0;
}

/* store data in a file */
void store(struct data *d, struct input_struct *is)
{
	char *fn;
	FILE *of;

	printf("enter file name to save to\n:");

	fn = input_line(is);

	if(*fn == '\n') {puts("file write cancelled"); return;}

	if((of = fopen(fn, "w")) == NULL) {printf("cannot open \"%s\"\n", fn); return;}

	switch(write_file(d, of))
	{
		case -2: printf("failed to finish writing to \"%s\"\n", fn); break;
		case -1: printf("failed to write to \"%s\"\n", fn); break;
		case 0: printf("complete data stored in \"%s\"\n", fn);
	}

	fclose(of);
}

