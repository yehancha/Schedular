#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#define max 100

struct process
{  
	char Pname[100];
    int arrival_time;
	int burst_time;
	int finish_time;
	int trunaround_time,rem;
}Strc[max],tempStrc;

void main(int argc, char *argv[])
{
	int rr[20],q,x,k;
    int f,r,n,i,j,tt=0,qt,t,flag,waiting_time=0;
    float avg_waiting_time=0,avg_turnaround_time=0;
    char command[10];  
	char filename[150];	
    int value1,value2;
    FILE *ptr_file;
	
	tt=0;
    q=0;
    rr[q]=0;

	sscanf( argv[1], "%d", &qt);
	printf("Enter file name : ");
	scanf("%s",&filename);
	ptr_file = fopen(filename, "r");
	if(ptr_file == NULL) 
	{
		printf("Cannot open file.\n");
	} 
	else 
	{
		n=0;
		while (fscanf(ptr_file,"%s %d %d",&Strc[n].Pname,&value1,&value2) != EOF)
		{			 
              		Strc[n].arrival_time =value1;       
					Strc[n].burst_time =value2;         
                	n++; 				
    		}
	}

    for(i=0;i<n;i++)
    {
        for(j=i+1;j<n;j++)
        {
            if(Strc[i].arrival_time>Strc[j].arrival_time)
            {
                tempStrc=Strc[i];
                Strc[i]=Strc[j];
                Strc[j]=tempStrc;
            }
        }
    }

    for(i=0;i<n;i++)
    {
        Strc[i].rem=Strc[i].burst_time;
        Strc[i].finish_time=0;
    }

    do
    {
        for(j=0;j<n;j++)
        if(tt>=Strc[j].arrival_time)
        {
            x=0;
            for(k=0;k<=q;k++)
            if(rr[k]==j)
            x++;
            if(x==0)
            {
                q++;
                rr[q]=j;
            }
        }
        if(q==0)
        i=0;
        if(Strc[i].rem==0)
        i++;
        if(i>q)
        i=(i-1)%q;
        if(i<=q)
        {
            if(Strc[i].rem>0)
            {
                if(Strc[i].rem<qt)
                {
                    tt+=Strc[i].rem;
                    Strc[i].rem=0;
                }
                else
                {
                    tt+=qt;
                    Strc[i].rem-=qt;
                }
                Strc[i].finish_time=tt;
            }
            i++;
        }
        flag=0;
        for(j=0;j<n;j++)
        if(Strc[j].rem>0)
        flag++;
    }while(flag!=0);

    printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    printf("\n     Process\t| Arrival time\t  |Burst time |");
    printf("\n--------------------------------------------------");
    for(f=0;f<n;f++)
    {
        waiting_time=Strc[f].finish_time - Strc[f].burst_time - Strc[f].arrival_time;
        Strc[f].trunaround_time=Strc[f].finish_time - Strc[f].arrival_time;
		avg_turnaround_time+=Strc[f].trunaround_time;
		avg_waiting_time+= waiting_time;
        printf("\n\t%s\t\t%d\t\t%d",Strc[f].Pname,Strc[f].arrival_time,Strc[f].burst_time);
    }
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++");	
    avg_turnaround_time/=n;
    avg_waiting_time/=n;
    printf("\n\nAverage trunaround_time = %f",avg_turnaround_time);
    printf("\nAverage waiting time = %f",avg_waiting_time);

    do{
    printf("\nEnter your input: ");
    fgets(command,5,stdin);
    if(strcmp(command, "QUIT") == 0)
    {
       exit(1);
     }
    }while(command != "QUIT");
  
 }