#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int sum(char *s, int *i);
int product(char *s, int *i);

int nbr(char *s, int *i)
{
	int a = 0;
	if (isdigit(s[*i]))
	{
		a = s[*i] - '0';
		(*i)++;
		return a;
	}
	while (s[*i] == '(')
	{
		(*i)++;
		a = sum(s, i);
		(*i)++;
	}
	return a;
}

int product(char *s, int *i)
{
	int a = nbr(s, i);
	while (s[*i] == '*')
	{
		(*i)++;
		a *= nbr(s, i);
	}
	return a;
}

int sum(char *s, int *i)
{
	int a = product(s, i);
	while (s[*i] == '+')
	{
		(*i)++;
		a += product(s, i);
	}
	return a;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	int i = 0;
	printf("%d\n", sum(argv[1], &i));
}