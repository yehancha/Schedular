#include <pthread.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <signal.h>
#define max 100

float avg_waiting_time1=0,avg_turnaround_time1=0;//RR
float avg_waiting_time2=0,avg_turnaround_time2=0;//SJF
float percentage_wt=0, percentage_tat=0;
char filename[150];
 
struct process
{  
	char Pname[100];
	int arrival_time;
	int burst_time;
	int finish_time;
	int trunaround_time,rem;
}Strc[max],tempStrc;

void *A(void *qt_ptr)//RR
{
    printf("\n\nRR CPU Scheduling Executing...\n");  
    int rr[20],q,x,k;
    int f,r,n,i,j,tt=0,qt,t,flag,waiting_time=0;
    qt= *((int *)qt_ptr);
    char command[10];    		
    FILE *ptr_file;
    q=0;
    rr[q]=0;

	printf("\nEnter file name : ");
	scanf("%s",&filename);
	ptr_file = fopen(filename, "r");
	if(ptr_file == NULL) 
	{
		printf("Cannot open file.\n");
	} 
	else 
	{
		n=0;
		while (fscanf(ptr_file,"%s %d %d",&Strc[n].Pname,&Strc[n].arrival_time,&Strc[n].burst_time) != EOF)
		{			       
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

    for(f=0;f<n;f++)
    {
        waiting_time=Strc[f].finish_time - Strc[f].burst_time - Strc[f].arrival_time;
        Strc[f].trunaround_time=Strc[f].finish_time - Strc[f].arrival_time;
		avg_turnaround_time1+=Strc[f].trunaround_time;
		avg_waiting_time1+= waiting_time;
    }
    avg_turnaround_time1/=n;
    avg_waiting_time1/=n;
	return NULL;
}



int n;  
struct sjf{  
     char pname[100];	
     int sr;  
     int waitingtime,brusttime,arrivaltime,trunaroundtime;  
     int flag;  
     }arr[100];  
 struct sjf queue[15];  
 int f=-1,r=-1;  
 void enqueue(struct sjf x){  
    if(f==-1 && r==-1)  
    {  
     queue[++f]=x;  
     r++;      
    }  
    else{  
     queue[++r]=x;     
    }  
 }  
 int dequeue(){  
    int x=f;  
    if(f==r)  
        f=r=-1;    
    else  
      f++;  
    return queue[x].sr;  
 }  
void sortQueue(){  
    int i,j;  
    for(i=f;i<=r;i++)  
    {  
     for(j=i+1;j<=r;j++) {  
      if(queue[i].brusttime>queue[j].brusttime)  
      {  
       struct sjf temp= queue[i];  
       queue[i]=queue[j];  
       queue[j]=temp;               
      }           
     }         
    }  
 }  

 void input(){  
	FILE *ptr_file;
	printf("\nEnter file name : ");
	scanf("%s",&filename);
	ptr_file = fopen(filename, "r");
	if(ptr_file == NULL) 
	{
		printf("Cannot open file.\n");
	} 
	else 
	{
		n=0;
		while (fscanf(ptr_file,"%s %d %d",&arr[n].pname,&arr[n].arrivaltime,&arr[n].brusttime) != EOF)
		{		   
          		arr[n].sr=n;   
          		arr[n].flag=0;    
                	n++; 				
    		}
	}  
 }  
 void calculate(){  
    int i,j,btto;  
    arr[0].waitingtime=0;   
    int btt=arr[0].brusttime;  
    arr[0].trunaroundtime=arr[0].brusttime;  
    avg_turnaround_time2=arr[0].brusttime;  
    i=0;  
    do{  
      for(j=1;j<n;j++){  
        if(arr[j].arrivaltime<btt )  
        enqueue(arr[j]);        
      }   
      sortQueue();   
      btto=btt;//Starting time for this process  
      int pid=dequeue();  
      while(arr[pid].flag==1){  
      pid=dequeue();              
      }  
      btt+=arr[pid].brusttime;  
      arr[pid].flag=1;//to show that process is executed  
      arr[pid].waitingtime=btto - arr[pid].arrivaltime;  
      arr[pid].trunaroundtime= arr[pid].brusttime+ arr[pid].waitingtime;      
      avg_waiting_time2+=arr[pid].waitingtime;  
      avg_turnaround_time2+= arr[pid].trunaroundtime;    
      i++;        
      while(f!=-1)  
      dequeue();  
    }while(i<n-1);  
    avg_waiting_time2/=n;  
    avg_turnaround_time2/=n;  
 }  

 
void *B(void* a)//SJF
{
    printf("\nSJF CPU Scheduling Executing...\n");  
    input();  
    calculate();  
    return NULL;
}

void sig_func(int sig)
{
 write(1, "Caught signal 11\n", 17);
 signal(SIGSEGV,sig_func);
}

int main(int argc, char *argv[])
{
char command[10]; 
pthread_t RR_thread;
pthread_t SJF_thread;
int *qt = malloc(sizeof(*qt));
sscanf( argv[1], "%d", qt);

if(pthread_create(&RR_thread, NULL, A, (void *)qt)) {
	fprintf(stderr, "Error creating thread\n");
	return 1;
}
sleep(10);
if(pthread_create(&SJF_thread, NULL, B, NULL)) {
	fprintf(stderr, "Error creating thread\n");
	return 1;
}

if(pthread_join(RR_thread, NULL)) {
	fprintf(stderr, "Error joining thread\n");
	return 2;
}
if(pthread_join(SJF_thread, NULL)) {
	fprintf(stderr, "Error joining thread\n");
	return 2;
}

if(avg_turnaround_time1 >= avg_turnaround_time2)
{
   percentage_tat=((avg_turnaround_time1-avg_turnaround_time2)/avg_turnaround_time2)*100.00;
   printf("\nThe average turnaround time of RR %0.2f% worse as compared to SJF\n",percentage_tat);
}else
{
   percentage_tat=((avg_turnaround_time2-avg_turnaround_time1)/avg_turnaround_time2)*100.00;
   printf("\nThe average turnaround time of RR %0.2f% better as compared to SJF\n",percentage_tat);
}
if(avg_waiting_time1 >= avg_waiting_time2)
{
   percentage_wt=((avg_waiting_time1-avg_waiting_time2)/avg_waiting_time2)*100.00;
   printf("\nThe average waiting time of RR %0.2f% worse as compared to SJF\n",percentage_wt);
}else
{
   percentage_wt=((avg_waiting_time2-avg_waiting_time1)/avg_waiting_time1)*100.00;
   printf("\nThe average waiting time of RR %0.2f% better as compared to SJF\n",percentage_wt);
}

do{
   printf("\nEnter your input: ");
   fgets(command,5,stdin);
   if (strcmp(command, "QUIT") == 0)
   {   	
       exit(1);
   }
   }while(command != "QUIT");

return 0;

}