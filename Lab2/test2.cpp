#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
using namespace std;
int main()
{
   int f1[2];
int f2[2];
long sum=0;
   if(pipe(f1)<0)
   {
       cout<<"Pipe 1 creation Failed Sorry!!!";
       exit(1);
   }
   if(pipe(f2)<0)
   {
       cout<<"Pipe 2 creation Failed Sorry!!!";
       exit(1);
   }
   int pid=fork();
   if(pid<0)
   {
       cout<<"Child process failed to create!!!";
       exit(1);
   }
   else if(pid==0)
   {
       int a=0;
       cout<<"Child Process:"<<endl;
       while(a!=-1)
       {
           cout<<"child:";
           cin>>a;
           cout<<" ";
           write(f1[1], &a, 1);
       }
       read(f2[0],&sum,sizeof(sum));
       cout<<" The sum received by child is:"<<sum;
   }
   else
   {
       cout<<"Parent Process"<<endl;
       int var=0;
       do
       {
           sum=sum+var;
           read(f1[0],&var,1);
           cout<<"parent:"<<var<<endl;
       }while(var!=255);//when -1 is sent it is received as 255
       write(f2[1],&sum,sizeof(sum));
   }
}