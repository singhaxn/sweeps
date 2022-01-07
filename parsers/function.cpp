#ifndef _FUNCTION_DECL
#define _FUNCTION_DECL

#include <string.h>
#include <math.h>

double cosec(double);
double sec(double);
double cot(double);
double dabs(double);

struct FunctionNode
{
	char name[6];
	double (*call)(double);
};

struct FunctionNode fTable[]=
{
	{"sin", &sin},
	{"cos", &cos},
	{"tan", &tan},
	{"cosec", &cosec},
	{"sec", &sec},
	{"cot", &cot},
	{"log", &log},
	{"sqrt", &sqrt},
	{"abs", &dabs},
	{"exp", &exp},
	{"asin", &asin},
	{"acos", &acos},
	{"atan", &atan}
};

double cosec(double angle)
{
	return (1.0/sin(angle));
}

double sec(double angle)
{
	return (1.0/cos(angle));
}

double cot(double angle)
{
	return (1.0/tan(angle));
}

double dabs(double value)
{
	return ((double)fabs((float)value));
}

int getFunctionCode(char *fun)
{
	int i, size;
	
	size=sizeof(fTable)/sizeof(struct FunctionNode);
	for (i=0; i<size; i++)
		if (!strcmp(fun, fTable[i].name))
			return i;
	
	return -1;
}

char *getFunction(int index)
{
	if (index<10)
		return fTable[index].name;
	else
		return NULL;
}

#endif
