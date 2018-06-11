#include<iostream.h>
#include<fstream.h>
#include<stdio.h>
#include<iomanip.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<dir.h>
#include<dos.h>
#include<bios.h>
#include<graphics.h>

void screen();
void drawdoublelinebox(int,int,int,int);
void drawsinglelinebox(int,int,int,int);
void normal();


#define datafile "pa10.txt"
#define recsize 78
#define max 59
#define m 177
#define bucketsize 3
int hash(char a[]);
fstream file;
char buffer[100];

class parklot
{
public:
  char dv_no[12],v_desc[10],name[12],phone[12],entry_time[7],exit_time[7],slot[8],fair[5];
public:
	void initial();
	void read();
        void pack();
        void unpack();
	int  retrieve(int addr,char k[],int i);
	void datadisp();
	int  remove(int addr,char k[]);
	void modify(int addr ,char k[]);
};

int myatoi(char *k)
{
   int res=0;
   for(int i=0;k[i]!='\0';++i)
   {
      if(k[i]==58)
	 continue;
      res=res*10+k[i]-48;
   }
   //cout<<res;
   return res;
}

void opener(fstream &file,char *fn,int mode)
{
	file.open(fn,mode);
	if(!file)
	{
		cout<<"UNABLE TO OPEN THE FILE\n"<<fn;
		getch();
		exit(1);
	}
}

void parklot::initial()
{
	int i,j;
	file.open(datafile,ios::in);
	if(!file)
	{
		file.open(datafile,ios::out);
		if(!file)
		{
			cout<<"UNABLE TO OPEN THE FILE\n";
			getch();
			exit(1);
		}
		for(i=0;i<max;i++)
		{
			file.seekp(i*recsize*3,ios::beg);
			for(j=0;j<(recsize*3)-2;j++)
			   file<<"#";
			file<<endl;
		}
		cout<<"EMPTY FILE CREATED\n";
	}
	file.close();
	return;
}
void parklot::read()
{
	int addr,len,len1,len2,len3,len4,len5;
	char v_no[12],dv_desc[10],dslot[8];

   LABEL:
	cout<<"\n\t\t\tENTER THE VEHICLE NUMBER: ";
	gets(v_no);
	len=strlen(v_no);
      	addr=hash(v_no);
	if(len!=8||v_no[0]<63||v_no[1]<63||v_no[4]<63||v_no[2]>63 ||v_no[3]>63 ||v_no[5]>63||v_no[6]>63||v_no[7]>63||v_no[8]>63)
	{
		cout<<"\n\t  ENTER THE VALID VEHICLE NUMBER(8 CHARACTERS): ";
		goto LABEL;
	}

	if(retrieve(addr,v_no,0)>0)
	{	
		cout<<"!!*** VEHICLE NUMBER IS ALREADY PRESENT, WE CANNOT ADD IT TO THE HASH FILE ***!!\n";
		goto LABEL;
	}
	file.close();
	strcpy(dv_no,v_no);

      DESC:
	cout<<"\n\t\t\tENTER VEHICLE DESCRIPTION: ";
	gets(dv_desc);
	len1=strlen(dv_desc);
	if(len1!=8||dv_desc[0]>63||dv_desc[1]<63||dv_desc[2]<63||dv_desc[3]<63 ||dv_desc[4]<63 ||dv_desc[5]<63||dv_desc[6]<63||dv_desc[7]<63||dv_desc[8]>63)
	{
		cout<<"\n\t  ENTER THE VALID VEHICLE DESCRIPTION(8 CHARACTERS): ";
		goto DESC;
	}
	file.close();
	strcpy(v_desc,dv_desc);

     NAME: 
	cout<<"\n\t\t\tENTER THE NAME: ";
	gets(name);
	int i=0;
		while(name[i]!='\0')
		{
	   		 if(name[i]<63||i>9)
	    		{
	    			  cout<<"\n\t  ENTER THE VALID NAME(LESS THAN 10 CHARACTERS): ";
	     			 goto NAME;
	   		}
	      		i++;
		}

     PH:
	i=0;
	cout<<"\n\t\t\tENTER THE PHONE: ";
	gets(phone);
	len3=strlen(phone);
	while(phone[i]!='\0')
	{
	    if(phone[i]>63 ||i>9 ||len3!=10)
	    {
	      cout<<"\n\t  ENTER VALID PHONE NUMBER: ";
	      goto PH;
	    }
	      i++;
	}
  /* if(i!=11)
{
     cout<<"\n\t  ENTER VALID PHONE NUMBER: ";
	      goto PH;
	   
}*/
    ENT: 
	cout<<"\n\t\t\tENTER THE ENTRY TIME IN 24 HRS FORMAT: ";
	gets(entry_time);
	len4=strlen(entry_time);
	 i=0;
	while(entry_time[i]!='\0')
	{
	    if(entry_time[i]>63||i>4||entry_time[2]!=58 ||len4!=5)
	    {
	      cout<<"\n\t  ENTER THE VALID ENTRY TIME IN 24 HRS FORMAT: ";
	      goto ENT;
	    }
	      i++;
	}


     EXT:
	cout<<"\n\t\t\tENTER THE EXIT TIME IN 24 HRS FORMAT: ";
	gets(exit_time);
	len5=strlen(exit_time);
	   i=0;
	while(exit_time[i]!='\0')
	{
	    if(exit_time[i]>63||i>4||exit_time[2]!=58 ||len5!=5)
	    {
	      cout<<"\n\t  ENTER THE VALID EXIT TIME IN 24 HRS FORMAT: ";
	      goto EXT;
	    }
	      i++;
	}
	
    SL:
	cout<<"\n\t\t\tENTER THE SLOT: ";
	gets(dslot);
	len2=strlen(dslot);
	if(len2!=5||dslot[0]>63||dslot[1]<63||dslot[2]>63||dslot[3]>63 ||dslot[4]>63||v_no[5]>63)
	{
		cout<<"\n\t  ENTER THE VALID SLOT(5 CHARACTERS): \n";
		goto SL;
	}
	file.close();
	strcpy(slot,dslot);

  /*  FA:
	cout<<"\n\t\t\tENTER THE FARE: ";gets(fair);
	
		i=0;
	while(fair[i]!='\0')
	{
	    if(fair[i]>63||i>4)
	    {
	      cout<<"\n\t  ENTER THE VALID FARE\n";
	      goto FA;
	    }
	      i++;
	}*/

	int val1,val2,rem,a[10],b[10];
	val1=myatoi(entry_time);
	//cout<<val1;
	val2=myatoi(exit_time);
	for(i=0;i<5;i++)
	{
	  rem=val1%10;
	  a[i]=rem;
	  val1=val1/10;
	}
	for( i=0;i<5;i++)
	{
	  rem=val2%10;
	  b[i]=rem;
	  val2=val2/10;
	}
	int l;
	int o,t1,t2 ;
	int h;
	int s;
	int r;
	l=a[3]*10+a[2];
	s=a[1]*10+a[0];
	r=b[3]*10+b[2];
	o=b[1]*10+b[0];
	t1=l*60+s;
	t2=r*60+o;
	if(t2>t1)
		h=(t2-t1)/60+1;
	else  {
	       h=(t1-t2)/60;
	       h=25-h;
	       }
	//h=h*10;
		if(h==0)
		     h=10;
	       else if(h<5)
		       h=h*10;
	    //  else if(h<0)
	      //         h=-(h*10);
	       else  if(h>5 && h<=10)
			h=80;
		else
			h=150;

	     itoa(h,fair,10);
	cout<<" TOTAL FARE: "<<fair<<endl;

	pack();
	return;
}
void parklot::pack()
{
	strcpy(buffer,dv_no);		strcat(buffer,"|");
	strcat(buffer,v_desc);		strcat(buffer,"|");
	strcat(buffer,name);		strcat(buffer,"|");
	strcat(buffer,phone);		strcat(buffer,"|");
	strcat(buffer,entry_time);	strcat(buffer,"|");
	strcat(buffer,exit_time);	strcat(buffer,"|");
	strcat(buffer,slot);		strcat(buffer,"|");
	strcat(buffer,fair);        	strcat(buffer,"|");     
}

int hash(char dv_no[])
{
int i=0,sum=0,c,len;
len=strlen(dv_no);
if(len%2==1)
    len++;
for(i=0;i<len;i+=2)
    sum=(sum+((dv_no[i]+dv_no[i+1])-96));
c=sum%59;
cout<<"\n\t\t    HASH KEY GENERATED IS: "<<c<<endl;
return c;
}

void store(int addr)
{
char dummy[10];
int flag=0,i;
file.open(datafile,ios::in|ios::out);
file.seekg(addr*recsize*bucketsize,ios::beg);
file.getline(dummy,5,'\n');
if(strcmp(dummy,"####")==0 ||strcmp(dummy,"$$$$")==0)
{
	file.seekp(addr*recsize*bucketsize,ios::beg);
	file<<buffer;
	clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int q=0;q<80;q++) cout<<'~';
	for(int w=0;w<80;w++) cout<<'*';
	cout<<"\n\n\t\t   VEHICLE RECORD INSERTED SUCCESSFULLY....!!\n\n";
	for(int e=0;e<80;e++) cout<<'*';
	cout<<"\n";
	for(int r=0;r<80;r++) cout<<'~';
	getch();
	flag=1;
}
file.seekg(addr*recsize*bucketsize+recsize,ios::beg);
file.getline(dummy,5,'\n');
if((strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0) && flag!=1)
{
	file.seekp(addr*recsize*bucketsize+recsize,ios::beg);
	file<<buffer;clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int q=0;q<80;q++) cout<<'~';
	for(int w=0;w<80;w++) cout<<'*';
	cout<<"\n\n\t\t   VEHICLE RECORD INSERTED SUCCESSFULLY....!!\n\n";
	for(int e=0;e<80;e++) cout<<'*';
	cout<<"\n";
	for(int r=0;r<80;r++) cout<<'~';
	getch();
	flag=1;
}
file.seekg(addr*recsize*bucketsize+(2*recsize),ios::beg);
file.getline(dummy,5,'\n');
if((strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)&&flag!=1)
{
	file.seekp(addr*recsize*bucketsize+(2*recsize),ios::beg);
	file<<buffer;
	clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int q=0;q<80;q++) cout<<'~';
	for(int w=0;w<80;w++) cout<<'*';
	cout<<"\n\n\t\t   VEHICLE RECORD INSERTED SUCCESSFULLY....!!\n\n";
	for(int e=0;e<80;e++) cout<<'*';
	cout<<"\n";
	for(int r=0;r<80;r++) cout<<'~';
	getch();
	flag=1;
}
if(flag!=1)
{
for(i=addr+1;i!=addr;i++)
{
	if(i%max==0)
		i=0;
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)
	{
		clrscr();
		textcolor(YELLOW);
		textbackground(BLACK);		
		cout<<"\n\n\n\n";
		for(int t=0;t<80;t++) cout<<'~';
		for(int y=0;y<80;y++) cout<<'*';
		cout<<"\n\n\t\t\tCOLLISION HAS OCCURED....!!\n\n";
		cout<<"\t      HOME ADDRESS IS: "<<addr<<" & ACTUAL ADDRESS IS: "<<i<<"\n";
		file.seekp(i*recsize*bucketsize,ios::beg);
		file<<buffer;
		textcolor(YELLOW);
		textbackground(BLACK);
		cout<<"\n\n\t\t   VEHICLE RECORD INSERTED SUCCESSFULLY....!!\n\n";
		for(int u=0;u<80;u++) cout<<'*';
		cout<<"\n";
		for(int f=0;f<80;f++) cout<<'~';
		getch();
		flag=1;
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)
	{
		clrscr();
		textcolor(YELLOW);
		textbackground(BLACK);	
		cout<<"\n\n\n\n";
		for(int t=0;t<80;t++) cout<<'~';
		for(int y=0;y<80;y++) cout<<'*';
		cout<<"\n\n\t\t\tCOLLISION HAS OCCURED....!!\n\n";
		cout<<"\t      HOME ADDRESS IS: "<<addr<<" & ACTUAL ADDRESS IS: "<<i<<"\n";
		file.seekp(i*recsize*bucketsize+recsize,ios::beg);
		file<<buffer;
		textcolor(YELLOW);
		textbackground(BLACK);
		cout<<"\n\n\t\t   VEHICLE RECORD INSERTED SUCCESSFULLY....!!\n\n";
		for(int u=0;u<80;u++) cout<<'*';
		cout<<"\n";
		for(int f=0;f<80;f++) cout<<'~';
		getch();
		flag=1;
		break;
	}
	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(dummy,5,'\n');
       if(strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)
	{
		clrscr();
		textcolor(YELLOW);
		textbackground(BLACK);	
		cout<<"\n\n\n\n";
		for(int t=0;t<80;t++) cout<<'~';
		for(int y=0;y<80;y++) cout<<'*';
		cout<<"\n\n\t\t\tCOLLISION HAS OCCURED....!!\n\n";
		cout<<"\t      HOME ADDRESS IS: "<<addr<<" & ACTUAL ADDRESS IS: "<<i<<"\n";
		file.seekp(i*recsize*bucketsize+(2*recsize),ios::beg);
		file<<buffer;
		textcolor(YELLOW);
		textbackground(BLACK);
		cout<<"\n\n\t\t   VEHICLE RECORD INSERTED SUCCESSFULLY....!!\n\n";
		for(int u=0;u<80;u++) cout<<'*';
		cout<<"\n";
		for(int f=0;f<80;f++) cout<<'~';
		getch();
		flag=1;
		break;
	}
}
}
if(i==addr && (flag!=1))
{
	clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int o=0;o<80;o++) cout<<'~';
	for(int a=0;a<80;a++) cout<<'*';
	cout<<setw(23)<<" "<<"\n\n\t*** HASH FILE IS FULL, VEHICLE RECORD CANT BE INSERTED ***\n\n";
	for(int s=0;s<80;s++) cout<<'*';
	for(int d=0;d<80;d++) cout<<'~';
	getch();
}
getch();
file.close();
return;
}

void parklot::unpack()
{
	char dummy[100];
	file.getline(v_desc,10,'|');
	file.getline(name,12,'|');
	file.getline(phone,12,'|');
	file.getline(entry_time,7,'|');
	file.getline(exit_time,7,'|');
	file.getline(slot,8,'|');
	file.getline(fair,5,'|');
	file.getline(dummy,100,'#');
}

int parklot::retrieve(int addr,char k[],int l)  //search
{
int found=0,i;
char dummy[10];
i=addr;
file.open(datafile,ios::in);
cout<<setiosflags(ios::left);
//if(l==1)
do
{
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
		break;
    	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dv_no,10,'|');
	if(strcmp(dv_no,k)==0)
	{
		found=1;
		if(l==1)
		{
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);		
	   cout<<endl;   
	   cout<<setw(18)<<" "<<"THE SEARCHED VEHCILE RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<setw(10)<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<setw(9)<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<setw(10)<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<setw(5)<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<setw(5)<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<setw(6)<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<setw(5)<<fair<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";
	  			
	   cout<<"VEHICLE RECORD FOUND...!!"<<endl;		
		}
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(dv_no,10,'|');
	if(strcmp(dv_no,"#########")==0)
	      break;
	if(strcmp(dv_no,k)==0)
	{
		found=1;
		if(l==1)
		{
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);	
	   cout<<endl;   
	   cout<<setw(18)<<" "<<"THE SEARCHED VEHCILE RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<setw(10)<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<setw(9)<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<setw(10)<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<setw(5)<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<setw(5)<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<setw(6)<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<setw(5)<<fair<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";
	  			
	cout<<"VEHICLE RECORD FOUND...!!"<<endl;
		}		
		break;
	}

	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(dv_no,10,'|');
	if(strcmp(dv_no,"#########")==0)
	      break;
	if(strcmp(dv_no,k)==0)
	{
		found=1;
		if(l==1)
		{
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);	
	   cout<<endl;   
	   cout<<setw(18)<<" "<<"THE SEARCHED VEHCILE RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<fair<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";
	  			
	cout<<"VEHICLE RECORD FOUND...!!"<<endl;
		}		
		break;
	}
	else
	{
		i++;
		if(i%max==0)
			i=0;
	}
}while(i!=addr);
if(found==0 && l==1)
{
		textcolor(15);
	      textbackground(YELLOW);
	cout<<setw(23)<<" "<<"\n\n\t!!*** GIVEN VEHICLE RECORD DOESNOT EXISTS IN HASH FILE ***!!\n\n";
	for(int h=0;h<80;h++) cout<<'~';
	for(int j=0;j<80;j++) cout<<'~';
	getch();
}
	/*if(l==0)
	    cout<<found<<endl;*/

return found ;
}

void parklot::datadisp()
{
	file.open(datafile,ios::in);
	int i=0;
	char dummy[10];
	cout<<setiosflags(ios::left);
	while(i<max)
	{
	 if(file.eof())
		break;
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")!=0 && strcmp(dummy,"$$$$")!=0)
	{
		file.seekg(i*recsize*bucketsize,ios::beg);
		file.getline(dv_no,10,'|');
		unpack();
		clrscr();
	      //drawdoublelinebox(17,7,60,30);
	      textcolor(15);
	      textbackground(RED);	
	   cout<<endl<<endl<<endl;
	   cout<<setw(25)<<" "<<"DISPLAYING VEHCILE RECORDS"<<endl<<endl;
	   for(int i=0;i<80;i++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<fair<<endl<<endl;
	   for(int k=0;k<80;k++) cout<<'*';
	   cout<<endl<<endl<<setw(20)<<" ";
	   getch();	

	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")!=0 && strcmp(dummy,"$$$$")!=0)
	{
		 file.seekg(i*recsize*bucketsize+recsize,ios::beg);
		file.getline(dv_no,10,'|');
		unpack();
	     clrscr();
		textcolor(15);
	      textbackground(RED);		
	   cout<<endl<<endl<<endl;
	   cout<<setw(25)<<" "<<"DISPLAYING VEHCILE RECORDS"<<endl<<endl;
	   for(int i=0;i<80;i++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<fair<<endl<<endl;
	   for(int k=0;k<80;k++) cout<<'*';
	   cout<<endl<<endl<<setw(20)<<" ";
	   getch();	


	}
	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")!=0 && strcmp(dummy,"$$$$")!=0)
	{
		file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
		file.getline(dv_no,10,'|');
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(RED);		
	   cout<<endl<<endl<<endl;
	   cout<<setw(25)<<" "<<"DISPLAYING VEHCILE RECORDS"<<endl<<endl;
	   for(int i=0;i<80;i++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<fair<<endl<<endl;
	   for(int k=0;k<80;k++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	   getch();	
	}
	i++;
	}
	clrscr();
	textcolor(15);
	      textbackground(RED);
	cout<<"\n\n\n\n\n\n\n\n\n\t\t=============================================\n"<<endl;	
	cout<<"\t\t     !!*** NO RECORDS TO DISPLAY ***!!"<<endl;
	cout<<"\n\t\t=============================================\n"<<endl;	
	getch();
	file.close();
}



int parklot::remove(int addr,char k[])
{
int found=0,i;
char dummy[10];
i=addr;
int s;

file.open(datafile,ios::in|ios::out);
cout<<setiosflags(ios::left);
do
{
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
		break;
        file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dv_no,10,'|');
	if(strcmp(dv_no,"#########")==0)
	      break;
	if(strcmp(dv_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(1);		
	   cout<<endl<<endl;
	   cout<<setw(26)<<" "<<"VEHCILE RECORD DISPLAYED"<<endl;
	   for(int l=0;l<80;l++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<fair<<endl<<endl;
	   for(int z=0;z<80;z++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	   	
		textcolor(15);
	      textbackground(1);	
		cout<<"VEHICLE RECORD TO BE DELETED IS FOUND"<<endl;
		file.seekp(i*recsize*bucketsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		cout<<setw(25)<<" "<<"  RECORD IS DELETED... !!"<<endl;
		getch();
		file.close();
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(dv_no,10,'|');
		if(strcmp(dv_no,"#########")==0)
		 break;
	if(strcmp(dv_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(1);		
	   cout<<endl<<endl;   
	   cout<<setw(26)<<" "<<"VEHCILE RECORD DISPLAYED"<<endl;
	   for(int l=0;l<80;l++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<fair<<endl<<endl;
	   for(int z=0;z<80;z++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	  
		textcolor(15);
	      textbackground(1);					
	cout<<"VEHICLE RECORD TO BE DELETED IS FOUND"<<endl;
		file.seekp(i*recsize*bucketsize+recsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		cout<<setw(25)<<" "<<"  RECORD IS DELETED... !!"<<endl;
		getch();
		break;
	}

	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(dv_no,10,'|');
	if(strcmp(dv_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(1);			
	   cout<<endl<<endl;
	   cout<<setw(26)<<" "<<"VEHCILE RECORD DISPLAYED"<<endl;
	   for(int l=0;l<80;l++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<fair<<endl<<endl;
	   for(int z=0;z<80;z++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";		

		textcolor(15);
	      textbackground(1);		
		cout<<"VEHICLE RECORD TO BE DELETED IS FOUND"<<endl;
		file.seekp(i*recsize*bucketsize+(2*recsize),ios::beg);
	       for(s=0;s<recsize-2;s++)
		   file.put('$');
		   file.put('\n');
		cout<<setw(25)<<" "<<"  RECORD IS DELETED... !!"<<endl;
		getch();
		break;
	}
	else
	{
		i++;
		if(i%max==0)
			i=0;
	}
}while(i!=addr);
if(found==0)
{
	textcolor(15);
	      textbackground(1);
	cout<<setw(23)<<" "<<"\n\n\t!!*** GIVEN VEHICLE RECORD DOESNOT EXISTS IN HASH FILE ***!!\n\n";
	for(int k=0;k<80;k++) cout<<'^';
	for(int j=0;j<80;j++) cout<<'^';
	getch();
}
return found;
}

void parklot::modify(int addr, char k[])
{
int found=0,i;
char dummy[10];
i=addr;
int s;
int x;

file.open(datafile,ios::in|ios::out);
cout<<setiosflags(ios::left);
do
{
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
		break;
       file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dv_no,10,'|');
	if(strcmp(dv_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(YELLOW);
	      textbackground(0);			
	   cout<<endl;
	   cout<<setw(18)<<" "<<"VEHCILE RECORD TO BE MODIFIED IS DISPLAYED"<<endl<<endl;
	   for(int c=0;c<80;c++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<fair<<endl<<endl;
	   for(int v=0;v<80;v++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	 	
		textcolor(YELLOW);
	      textbackground(0);		
		cout<<"VEHICLE RECORD TO BE MODIFIED IS FOUND"<<endl;		
		getch();
		file.seekp(i*recsize*bucketsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		  cout<<"\t\t\tRECORD DELETED...!!"<<endl;
		  getch();
		   file.close();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n";
	 	for(int b=0;b<80;b++) cout<<'~';
		for(int n=0;n<80;n++) cout<<'*';
		  cout<<"\t\t\tENTER THE NEW(MODIFIED) VEHICLE RECORD: \n";
		  getch();
		read();
		x=hash(dv_no);
		store(x);

		cout<<"\t\t    RECORD IS MODIFIED SUCCESSFULLY...!!\n";
		getch();
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(dv_no,10,'|');
	if(strcmp(dv_no,"#########")==0)
	      break;

	if(strcmp(dv_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(YELLOW);
	      textbackground(0);	
	   cout<<endl;
	   cout<<setw(18)<<" "<<"VEHCILE RECORD TO BE MODIFIED IS DISPLAYED"<<endl<<endl;
	   for(int c=0;c<80;c++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<fair<<endl<<endl;
	   for(int v=0;v<80;v++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	
		 textcolor(YELLOW);
	      textbackground(0);
		cout<<"VEHICLE RECORD TO BE MODIFIED IS FOUND"<<endl;
		getch();
		file.seekp(i*recsize*bucketsize+recsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		cout<<"\t\t\tRECORD DELETED...!!"<<endl;
		  getch();
		   file.close();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n";
	 	for(int b=0;b<80;b++) cout<<'~';
		for(int n=0;n<80;n++) cout<<'*';
		  cout<<"\t\t\tENTER THE NEW(MODIFIED) VEHICLE RECORD: \n";
		  getch();
		read();
		x=hash(dv_no);
		store(x);

		cout<<"\t\t    RECORD IS MODIFIED SUCCESSFULLY...!!\n";
		getch();
		break;
	}

	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(dv_no,10,'|');
	if(strcmp(dv_no,"#########")==0)
	      break;

	if(strcmp(dv_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(YELLOW);
	      textbackground(0);	
	   cout<<endl;
	   cout<<setw(18)<<" "<<"VEHCILE RECORD TO BE MODIFIED IS DISPLAYED"<<endl<<endl;
	   for(int c=0;c<80;c++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE NUMBER      :  "<<dv_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" VEHICLE DESCRIPTION :  "<<v_desc<<endl<<endl;
	   cout<<setw(20)<<" "<<" NAME                :  "<<name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER        :  "<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" ENTRY TIME          :  "<<entry_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" EXIT TIME           :  "<<exit_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" SLOT                :  "<<slot<<endl<<endl;
	   cout<<setw(20)<<" "<<" FARE                :  "<<fair<<endl<<endl;
	   for(int v=0;v<80;v++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	  
		textcolor(YELLOW);
	      textbackground(0);
		cout<<"VEHICLE RECORD TO BE MODIFIED IS FOUND"<<endl;
		getch();
		file.seekp(i*recsize*bucketsize+(2*recsize),ios::beg);
	       for(s=0;s<recsize-2;s++)
		   file.put('$');
		   file.put('\n');
		  cout<<"\t\t\tRECORD DELETED...!!"<<endl;
		  getch();
		   file.close();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n";
	 	for(int b=0;b<80;b++) cout<<'~';
		for(int n=0;n<80;n++) cout<<'*';
		  cout<<"\t\t\tENTER THE NEW(MODIFIED) VEHICLE RECORD: \n";
		  getch();
		read();
		x=hash(dv_no);
		store(x);

		cout<<"\t\t    RECORD IS MODIFIED SUCCESSFULLY...!!\n";
		getch();
		break;
	}
	else
	{
		i++;
		if(i%max==0)
			i=0;
	}
}while(i!=addr);
if(found==0)
{
	textcolor(15);
	      textbackground(1);
	cout<<setw(23)<<" "<<"\n\n\t!!*** GIVEN VEHICLE RECORD DOESNOT EXISTS IN HASH FILE ***!!\n\n";
	for(int k=0;k<80;k++) cout<<'=';
	for(int j=0;j<80;j++) cout<<'^';
	getch();
}
}



/*Douoblebox design*/

void drawdoublelinebox(int x,int y,int len,int bdt)
{
	gotoxy(x,y);
	cprintf("ษ");
		gotoxy(x,y+bdt);
	cprintf("ศ");
	gotoxy(x+len,y);
	cprintf("ป");
	gotoxy(x+len,y+bdt);
	cprintf("ผ");
	for(int i=x+1;i<x+len;i++)
	{
		gotoxy(i,y);
		cprintf("อ");
		gotoxy(i,y+bdt);
		cprintf("อ");
	}
	for(int j=y+1;j<y+bdt;j++)
	{
		gotoxy(x,j);
		cprintf("บ");
		gotoxy(x+len,j);
		cprintf("บ");
			}
}

/*Double box design*/

void drawsinglelinebox(int x,int y,int len,int bdt)
{                                            // (x,y) : co.ord. of 1st tip
	gotoxy(x,y);                         // len : length of box
	cprintf("ฺ");                        // bdt : breadth of box
	gotoxy(x,y+bdt);
	cprintf("ภ");
	gotoxy(x+len,y);
	cprintf("ฟ");
	gotoxy(x+len,y+bdt);
	cprintf("ู");

	for(int i=x+1;i<x+len;i++)
	{
		gotoxy(i,y);
		cprintf("ฤ");
		gotoxy(i,y+bdt);
		cprintf("ฤ");
	}
	for(int j=y+1;j<y+bdt;j++)
	{
		gotoxy(x,j);
		cprintf("ณ");
		gotoxy(x+len,j);
		cprintf("ณ");
	}
}
void normal()  // to obtain normal background
{
	textattr(WHITE); // textattr sets text attributes for text-window functions
	textcolor(WHITE);// textcolor selects a new character color in text mode
	textbackground(BLACK);//textbackground selects a new text background color
	_setcursortype(_NORMALCURSOR); /* sets cursor type to normal underscore type cursor  */
	//return;
}

void screen()
{
 int g=19,h=10;

     textbackground(GREEN);
     gotoxy(24,3);
     textcolor(YELLOW);
     cprintf("        PARKING ALLOTMENT      ");
     gotoxy(24,4);
     textcolor(15);
     cprintf(" อออออออออออออออออออออออออออออ ");
     drawdoublelinebox(17,7,44,14);
     gotoxy(28,7);
     textbackground(RED);
	 textcolor(YELLOW);
	 cprintf("Û  M A I N    M E N U Û" );
     normal();
     textcolor(15);
     gotoxy(g,h++);
     cout<<"	     þ VEHICLE RECORD";
     gotoxy(g,h++);
     cout<<"	     þ QUIT";
     gotoxy(g,h++);
     gotoxy(g,h++);
     gotoxy(g,h++);
     normal();
}


void main()
{
int ch,addr,l,choice, found=0;
char sv_no[10];
parklot p;
p.initial();
clrscr();
do
{
    	screen();
	//cout<<"\n============================"<<endl;
	//cout<<"1.read vehicle details\n2.search vehicle record\n3.display vehicle record\n4.delete vehicle record\n5.modify record\n6.exit\n";
      // cout<<"============================"<<endl;
	cout<<"PLEASE ENTER YOUR CHOICE [1-2] :  ";
	cin>>ch;
	switch(ch)
	{
	case 1:do
	      {
	      clrscr();
	      drawdoublelinebox(17,7,44,14);
	      textcolor(0);
	      textbackground(15);
		  clrscr();
		  textbackground(BLACK);
	      gotoxy(25,3);
	      textcolor(YELLOW);
	      cprintf("        VEHICLE RECORDS       ");
	      gotoxy(25,4);
	      cprintf(" ออออออออออออออออออออออออออออ ");
	      int z=19,q=10;
	      textcolor(YELLOW);
	      drawdoublelinebox(17,7,44,14);
	      gotoxy(28,7);
	      textbackground(RED);
	      textcolor(YELLOW);
	      cprintf("Û  M A I N    M E N U Û" );
	      normal();
	      textcolor(WHITE);
	      gotoxy(z,q++);
	      cout<<"     1.>  READ VEHICLE DETAILS";
	      gotoxy(z,q++);
	      cout<<"     2.>  SEARCH VEHICLE RECORD";
		  gotoxy(z,q++);
	      cout<<"     3.>  DISPLAY VEHICLE DETAILS";
	      gotoxy(z,q++);
	      cout<<"     4.>  DELETE VEHICLE RECORD";
	      gotoxy(z,q++);
		cout<<"     5.>  MODIFY VEHICLE RECORD";
	      gotoxy(z,q++);
	      cout<<"     6.>  QUIT";
	      gotoxy(z,q++);
	      gotoxy(z,q++);
	      gotoxy(z,q++);
	      cout<<"    PLEASE ENTER YOUR CHOICE [1-6]  : ";
	      cin>>choice;
	 switch(choice)
	 {
	case 1:clrscr();
		cout<<"\n";
	 	for(int b=0;b<80;b++) cout<<'~';
		cout<<setw(24)<<" "<<"ENTER THE VEHICLE RECORD DETAILS \n";
		for(int h=0;h<80;h++) cout<<'~';
		cout<<"\n";
		getch();
	 LABEL: p.read();
		addr=hash(p.dv_no);
		store(addr);
		getch();
		file.close();
		break;
	case 2:clrscr();
		textcolor(15);
	      textbackground(YELLOW);
		cout<<"\n\n\n\n";
		cout<<"\t\t\t  SEARCHING VEHICLE RECORD\n\n";
		for(int e=0;e<80;e++) cout<<'~';
		//cout<<"\n";
		for(int w=0;w<80;w++) cout<<'~';
		cout<<"\n";
		cout<<setw(20)<<" "<<"ENTER THE VEHICLE NUMBER TO BE SEARCHED: "<<endl;
		cout<<setw(36)<<" ";
		cin>>sv_no;
		cout<<"\n";
		addr=hash(sv_no);
		p.retrieve(addr,sv_no,1);
		getch();
		file.close();
		break;
	case 3://cout<<"vehicle details:"<<endl;
		textcolor(15);
	      textbackground(RED);	
	       p.datadisp();
	       file.close();
		break;
       case 4://p.del();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n\n\n\n";
		cout<<"\t\t\t  DELETING VEHICLE RECORD\n\n";
		for(int x=0;x<80;x++) cout<<'^';
		//cout<<"\n";
		for(int y=0;y<80;y++) cout<<'^';
		cout<<"\n";
		cout<<setw(20)<<" "<<"ENTER THE VEHICLE NUMBER TO BE DELETED: "<<endl;
		cout<<setw(36)<<" ";
		cin>>sv_no;
		cout<<"\n";
		addr=hash(sv_no);
		p.remove(addr,sv_no);
		getch();
		file.close();
		break;
       case 5:clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n\n\n\n";
		cout<<"\t\t\t  MODIFYING VEHICLE RECORD\n\n";
		for(int u=0;u<80;u++) cout<<'=';
		//cout<<"\n";
		for(int r=0;r<80;r++) cout<<'^';
		cout<<"\n";
		cout<<setw(20)<<" "<<"ENTER THE VEHICLE RECORD TO MODIFY: \n";
	      	cout<<setw(36)<<" ";
		cin>>sv_no;
	      addr=hash(sv_no);
		cout<<"\n";
	      p.modify(addr,sv_no);
		getch();
	     file.close();
	      break; 
	case 6:clrscr();
			 screen();
			 break;
	default:sound(600);delay(500);nosound();
		break;
	}
	}while(choice!=6);   break;

case 2:exit(0);

default: sound(100);delay(500);nosound(); break;
}
}while(ch!=2);

       file.close();
}



