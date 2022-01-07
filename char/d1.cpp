#include<stdio.h>
#include<conio.h>
void  main()
{
	FILE *fp1, *fp2;
	unsigned char ch1, ch2, ch3;
	int i,  x1, y1=480;
	int x2, flag=0;
	fp1=fopen("c:\\a.bmp","r");
	fp2=fopen("c:\\char\\a.txt","wt");
	clrscr();
	/* initialize graphics mode */
	for(x1=0;x1<54;x1++)
	  fgetc(fp1);
	for(x2=0;!feof(fp1);x2++)
	{
		i=0;
		ch1=fgetc(fp1);
		if(ch1>=125)
			i=1;
		ch2=fgetc(fp1);
		if(ch2>=125)
			i=1;
		ch3=fgetc(fp1);
		if(ch3>=125)
			i=1;
		if(x2==639)
		{
			fgetc(fp1);
			fgetc(fp1);
			fgetc(fp1);
			y1--;
			x2=0;
			i=1;
			flag=0;
		}
		if(i==0 && flag ==0)
		{
			fprintf(fp2," %f \n",x2/640.0);
			flag = 1;
		}
		else if(flag && i==1)
		{
			fprintf(fp2," %f %f ",x2/640.0, y1/640.0);
			flag=0;
		}
		i=0;
	}
	getch();
}