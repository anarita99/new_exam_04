#include <ctype.h>
#include <stdio.h>

int nbr(char *s, int *i);
int product(char *s, int *i);
int sum(char *s, int *i);

int nbr(char *s, int *i)
{
	int a = 0;
	if(isdigit(s[*i]))
	{
		a = s[*i] - '0';
		(*i)++;
	}
	while(s[*i] == '(')
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
	while(s[*i] == '*')
	{
		(*i)++;
		a *= nbr(s, i);
	}
	return a;
}

int sum(char *s, int *i)
{
	int a = product(s, i);
	while(s[*i] == '+')
	{
		(*i)++;
		a += product(s, i);
	}
	return a;
}

int parse(char *s)
{
	int opened = 0;
	if(!s[0])
		return(printf("Unexpected end of input\n"), 0);
	if(s[0] == '*' || s[0] == '+' || s[0] == ')')
		return(printf("Unexpected token '%c'\n", s[0]), 0);
	for(int i = 0; s[i]; i++)
	{
		if(isdigit(s[i]) && (isdigit(s[i + 1]) || s[i + 1] == '('))
			return(printf("Unexpected token '%c'\n", s[i + 1]), 0);
		if(s[i] == '+' || s[i] == '*')
		{
			if(s[i] == s[i + 1] || s[i + 1] == ')')
				return(printf("Unexpected token '%c'\n", s[i + 1]), 0);
			if(!s[i + 1])
				return(printf("Unexpected end of input\n"), 0);
		}
		if(s[i] == '(')
		{
			opened++;
			if(s[i + 1] == ')' || s[i + 1] == '+' || s[i + 1] == '*')
				return(printf("Unexpected token '%c'\n", s[i + 1]), 0);
			if(!s[i + 1])
				return(printf("Unexpected end of input\n"), 0);
		}
		if(s[i] == ')')
		{
			opened--;
			if(opened < 0)
				return(printf("Unexpected token ')'\n"), 0);
			if(isdigit(s[i + 1]) || s[i + 1] == '(')
				return(printf("Unexpected token '%c'\n", s[i + 1]), 0);
		}
	}
	if(opened > 0)
		return(printf("Unexpected end of input\n"), 0);
	return 1;
}

int main(int ac, char **av)
{
	if(ac != 2)
		return 1;
	if(!parse(av[1]))
		return 1;
	int i = 0;
	printf("%d\n", sum(av[1], &i));
	return 0;
}