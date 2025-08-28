/* rule functions
 * written by Donald_ET3 during August of 2025
 */

/* set morning or afternoon stop ordering */
void ampm(struct data *d, struct input_struct *is)
{
	char *s;

	while(true)
	{
		printf("stop order currently set to: ");
		if(d->am) puts("AM");
		else puts("PM");
		printf("Is this an AM route or PM route? [am/pm] (leave empty to cancel)\n:");
		s = input_line(is);
		if(*s == '\n') return;

		/* convert string to lowercase */
		for(size_t i = 0; s[i] != '\0'; i++) s[i] = tolower(s[i]);

		if((!strcmp(s, "am")) || (!strcmp(s, "a")))
			{d->am = true; break;}
		if((!strcmp(s, "pm")) || (!strcmp(s, "p")))
			{d->am = false; break;}
		puts("not a valid entry");
	}

	printf("stop order is now ");
	if(d->am) puts("AM");
	else puts("PM");
}

/* precedence of grade order in seating arrangement */
void gradeprecedence(struct data *d, struct input_struct *is)
{
	char *s;
	unsigned x;

	while(true)
	{
		printf("enter grade order precedence (\"0\" to remove rule)\n:");
		s = input_line(is);
		if(*s == '\n') break;
		if(sscanf(s, "%u", &x) != 1)
		{puts("not a valid number"); continue;}
		if(x > 3) {puts("Number is too large. There are only 3 rules."); continue;}
		for(int i = 0; i < 3; ++i) if(d->rules[i] == GRADE) d->rules[i] = NONE;
		if(x) d->rules[x - 1] = GRADE;
		break;
	}
}

/* precedence of gender in seating arrangement */
void genderprecedence(struct data *d, struct input_struct *is)
{
	char *s;
	unsigned x;

	while(true)
	{
		printf("enter gender order precedence (\"0\" to remove rule)\n:");
		s = input_line(is);
		if(*s == '\n') break;
		if(sscanf(s, "%u", &x) != 1)
		{puts("not a valid number"); continue;}
		if(x > 3) {puts("Number is too large. There are only 3 rules."); continue;}
		for(int i = 0; i < 3; ++i) if(d->rules[i] == GENDER) d->rules[i] = NONE;
		if(x) d->rules[x - 1] = GENDER;
		break;
	}
}

/* precedence of stop order in seating arrangement */
void stopprecedence(struct data *d, struct input_struct *is)
{
	char *s;
	unsigned x;

	while(true)
	{
		printf("enter stop order precedence (\"0\" to remove rule)\n:");
		s = input_line(is);
		if(*s == '\n') break;
		if(sscanf(s, "%u", &x) != 1)
		{puts("not a valid number"); continue;}
		if(x > 3) {puts("Number is too large. There are only 3 rules."); continue;}
		for(int i = 0; i < 3; ++i) if(d->rules[i] == STOP) d->rules[i] = NONE;
		if(x) d->rules[x - 1] = STOP;
		break;
	}
}

/* display student ordering rule configuration */
void viewruleorder(struct data *d, struct input_struct *is)
{
	for(int i = 0; i < 3; ++i)
		if(d->rules[i] != NONE)
		{
			printf("%d: ", i);
			switch(d->rules[i])
			{
				case GRADE: puts("grade"); break;
				case GENDER: puts("gender"); break;
				case STOP: puts("stop"); break;
				default: puts(":scream:");
			}
		}
}

/* output ordered student list */
int print_order(struct data *d, FILE *os)
{
	struct student_t *student;
	unsigned grade = 0, stop = 0, laststop = 0;
	unsigned gradep = 0, stopp = 0, genderp = 0;
	enum gender_t gender = MALE;
	enum rule_t rule;

	for(unsigned i = 0; i < 3; ++i)
	{
		switch(d->rules[i])
		{
			case NONE: continue;
			case GRADE: gradep = i + 1; break;
			case STOP: stopp = i + 1; break;
			case GENDER: genderp = i + 1;
		}
	}

	for(student = d->students; student != NULL; student = student->next)
		if(student->stop > laststop) laststop = student->stop;

	if(d->am) stop = laststop;

	while(true)
	{
		for(student = d->students; student != NULL; student = student->next)
			if((student->grade == grade) || (!gradep))
				if((student->stop == stop) || (!stopp))
					if((student->gender == gender) || (!genderp))
					{
						if(print_student(student, os)) return -1;
						if(putc('\n', os) == EOF) return -1;
					}

		for(int i = 2; i >= 0; --i)
		{
			rule = d->rules[i];
			if(rule == GRADE)
			{
				if(grade == 12)
				{
					if(i == 0) return 0;
					grade = 0;
				}
				else {++grade; break;}
			}
			else if(rule == STOP)
			{
				if(d->am)
				{
					if(stop == 0)
					{
						if(i == 0) return 0;
						stop = laststop;
					}
					else {--stop; break;}
				}
				else
				{
					if(stop == laststop)
					{
						if(i == 0) return 0;
						stop = 0;
					}
					else {++stop; break;}
				}
			}
			else if(rule == GENDER)
			{
				if(gender == FEMALE)
				{
					if(i == 0) return 0;
					gender = MALE;
				}
				else if(gender == MALE)
				{gender = UNKNOWN; break;}
				else if(gender == UNKNOWN)
				{gender = FEMALE; break;}
			}
			else if(i == 0) return 0;
		}
	}
}

/* output ordered student list */
void vieworder(struct data *d, struct input_struct *is)
{
	FILE *os;

	if((os = get_stream(is)) == NULL) return;

	if(print_order(d, os)) puts("cannot write ordered students");

	if(os != stdout) fclose(os);
}

