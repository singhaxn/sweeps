//	The precedence table

#ifndef _PRECEDENCE
#define _PRECEDENCE

char oper[]={'^', '*', '/', '+', '-', '#'};
int prec[][6]=
{
	{0,0,0,0,0,1},
	{1,0,0,0,0,1},
	{1,0,0,0,0,1},
	{1,1,1,0,0,1},
	{1,1,1,0,0,1},
	{1,1,1,1,1,1}
};

int indexof(char ch)
{
	int i;
	
	for (i=0; i<6; i++)
		if(oper[i]==ch)
			return i;
			
	return -1;
}

int preceeds(char arr, char tos)
{
	return prec[indexof(tos)][indexof(arr)];
}

#endif
