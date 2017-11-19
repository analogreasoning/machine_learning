/*kod do realizacji uczenia maszynowego metoda "hit and near-miss"   */
/*sluzacy akwizycji, agregacji i asocjacji danych na temat obiektow  */


#define MAX 100

/*ogolna postac danych*/
struct attribute
{
	char subject[80];
	char verb[80];
	char object[80];
	char active;
};

/*struktura danych zaw asocjacje rodzaju "moze posiadac*/
struct attribute may[MAX];

/*struktura danych zawierajaca asocjacje rodzaju "musi posiadac"*/
struct attribute must[MAX];

/*indeksy do obu struktur*/
int may_pos = 0;
int must_pos = 0;

/*funkcje uzywane w projekcie*/
void learn();
void display();
void assert_may(char* n, char* v, char* o);
void assert_must(char* n, char* v, char* o);
int find_may(char* n, char*  v, char*  o);
int get_example(char* n, char*  v, char*  o);
int generalize(char* n, char*  v, char*  o);
int restrict(char* ms, char* mv, char* mo);

/*funkcja MAIN - przykladowy interface wejsciowy - do pozniejszej automatyzacji*/
int main()
{
	for (;;)
	{
		printf("\n(L)earn, (D)isplay, or (Q)uit\n");
		switch (tolower(getch()))
		{
		case 'L': learn();
			break;
		case 'D': display();
			break;
		case 'Q': exit();
		}
		printf("/n");
	}
	return 0;
}

/*funkcja uczaca asocjacji*/
void learn()
{
	char sub[80], verb[80], obj[80];
	char msub[80], mverb[80], mobj[80];

	for (;;)
	{
		printf("\nEnter an example\n");
		if (!get_example(sub, verb, obj))
		{
			return;
		}
		if (find_may(sub, verb, obj) == -1)
		{
			assert_may(sub, verb, obj);
			generalize(sub, verb, obj);
		}
		printf("\nEnter a near-miss (CR to skip)\n");
		get_example(msub, mverb, mobj);
		restrict(msub, mverb, mobj);
	}
}

/*umieszczamy wpis w bazie danych "moze posiadac"*/
void assert_may(char* n, char*  v, char* o)
{
	if (may_pos < MAX)
	{
		strcpy(may[may_pos].subject, n);
		strcpy(may[may_pos].verb, v);
		strcpy(may[may_pos].object, o);
		may[may_pos].active = 1;
		may_pos++;
	}
	else
		printf("\nout of memory\n");
}

/*umieszczamy wpis w bazie danych "musi posiadac"*/
void assert_must(char* n, char* v, char* o)
{
	if (must_pos < MAX)
	{
		strcpy(must[must_pos].subject, n);
		strcpy(must[must_pos].verb, v);
		strcpy(must[must_pos].object, o);
		must_pos++;
	}
	else printf("\nout of memory\n");
}

/*znajdujemy wpis w bazie danych "moze posiadac"*/
int find_may(char* n, char* v, char* o)
{
	register int t;

	for (t = 0; t < may_pos; t++)
	{
		if (!strcmp(may[t].subject, n) && !strcmp(may[t].verb, n) && !strcmp(may[t].object, n) && may[t].active)
			return t;
	}
	return -1;
}

/*nakladamy ograniczenia na obiekt np. przenosimy z bazy "moze posiadac" do bazy "musi posiadac" itp ...*/
int restrict(char* ms, char* mv, char* mo)
{
	register int t;
	char temp[4];

	for (t = 0; t < 3; t++)
	{
		if (!strcmp(&mv[4], may[t].verb) && !strcmp(may[t].subject, ms) && !strcmp(may[t].object, mo) && may[t].active)
		{
			assert_must(may[t].subject, may[t].verb, may[t].object);
			may[t].active = 0;
			return;
		}
	}
}

/*utworz postac ogolna dla nowego przykladu danych*/
int generalize(char* n, char* v, char* o)
{
	register int t, i;

	for (t = 0; t < may_pos; t++)
	{
		if (strcmp(may[t].subject, n) && !strcmp(may[t].verb, v) && !strcmp(may[t].object, o) && may[t].active)
		{
			strcat(may[t].subject, " lub ");
			strcat(may[t].subject, n);
		}
	}
	for (t = 0; t < may_pos; t++)
	{
		if (!strcmp(may[t].subject, n) && !strcmp(may[t].verb, v) && strcmp(may[t].object, o) && may[t].active)
		{
			strcat(may[t].object, " lub ");
			strcat(may[t].object, o);
		}
	}
	for (t = 0; t < must_pos; t++)
	{
		if (strcmp(must[t].subject, n) && !strcmp(must[t].verb, v) && !strcmp(must[t].object, o))
		{
			strcat(must[t].subject, " lub ");
			strcat(must[t].subject, n);
			i = find_may(n, v, o);
			may[i].active = 0;
		}
	}
	for (t = 0; t < must_pos; t++)
	{
		if (!strcmp(must[t].subject, n) && !strcmp(must[t].verb, v) && strcmp(must[t].object, o))
		{
			strcat(must[t].object, " lub ");
			strcat(must[t].object, o);
			i = find_may(n, v, o);
			may[i].active = 0;
		}
	}
}

/*wprowadzanie danych wejsciowych*/
/*dane wprowadzamy wedlug schematu jak ponizej*/
/*przedmiot: szescian*/
/*zaleznosc: lezy na*/
/*cecha: bok*/
/*albo np:*/
/*przedmiot: rower*/
/*zaleznosc: ma kola*/
/*cecha: dwa*/
/*...................................*/
int get_example(char* n, char* v, char* o)
{
	//wrocic do tego i zmienic - gets() to nie jest najlepszy pomysl - na testy jest ok, ale nie na wersje finalna
	printf("\nprzedmiot:");
	gets(n);
	if (!*n) return 0;
	printf("\nzaleznosc:");
	gets(v);
	printf("\ncecha:");
	gets(o);
	return 1;
}

void display()
{
	int t;

	printf("\nmoze posiadac:\n");
	for (t = 0; t < may_pos; t++)
	{
		if (may[t].active)
		{
			printf("  %s %s %s \n", may[t].subject, may[t].verb, may[t].object);
		}
	}
	printf("\nmusi posiadac:\n");
	for (t = 0; t < must_pos; t++)
	{
		if (must[t].active)
		{
			printf("  %s %s %s \n", must[t].subject, must[t].verb, must[t].object);
		}
	}
}
