#include<bits/stdc++.h> 
#define MAX_TREE_HT 100
#include<math.h>
using namespace std;

struct table
{
      int data;
      int freq;
      int a[100];
      int huffcode[100];
      int huffdec;
      int bin_size;     
};

//structure info to store the compression factors
struct info
{
	int pad;
	float ratio;
	int length;
	int maxcodelen;
	float compression_ratio;
	float compression_factor;
	float compression_percentage;
};

 
int decoder(int final_output[30000],int fin_count,struct table input[300],int pad,int output[30000],int bin[240000])
{
	int x=0;
	int i1;
	for(i1=0;i1<fin_count;i1++)
	{
		int r=final_output[i1];
		int m1=0;
		while(r>0)
		{
		int m=r%2;
		bin[x++]=m;
		m1++;
		r=r/2;
		}
		while(m1<8)
		{
		bin[x++]=0;
		m1++;
		}
	}
	
	int p=0;
	int p1=1;
	int sum=0;
	int i;
	for(i=0;i<x;i++)
	{
	sum=sum+(p1*bin[i]);
	p1=p1*2;
	int sum1=sum+(p1);
	int j;
		for(j=0;j<256;j++)
		{
		if(sum1==input[j].huffdec)
		{
		output[p++]=input[j].data;
		p1=1;
		sum=0;
		}
		}
	}
  return p;

}


int main()
{
	FILE *fp1;
	
	fp1=fopen("/media/naseem/New Volume/Firu/Studies/NonAcads/projects/inp_project/INP-FTP/receiver_output.txt", "w");
	
	  int binary[240000];
	int output[30000];
	int out_count=3;
	out_count=decoder(final_output,fin_count,input,info1.pad,output,binary);
		
int fl,i;
fl=n;

int fmax=-1,a,b,fsign,j;
int fout[fl-1];
j=1;

for(i=2;i<fl;i++)
{
   if(output[i]>fmax)
   fmax=output[i];
}

a=output[0]<<4;
b=output[1];
fout[0]=a^b;
float fy=log(fmax)/log(2);
int r1=fy;
if((r1-fy)!=0)
fsign=fy;
int fx=pow(2,fsign);

for(i=2;i<fl;i++)
{
   if(output[i]>=fx)
   fout[j++]=fx-output[i];
   else
   fout[j++]=output[i];
}

for(i=1;i<(sizeof(fout)/4);i++)
fout[i]=fout[i]+fout[i-1];

for(i=0;i<(sizeof(fout)/4);i++)
fprintf(fp1,"%d\n",fout[i]);

	//calculating prd 
	float prd,qs;
	long long int prd_num=0;
	long long int prd_den=0;
	int i6;
	for(i6=0;i6<n-1;i6++)
	{
    prd_num+=(fout[i6]-in[i6])*(fout[i6]-in[i6]);        
    prd_den+=(in[i6])*(in[i6]);
    }
    prd=((double)prd_num)/prd_den;
    prd=sqrt(prd);
    printf("\n\nPRD IS %f \n",prd);
	return 0;
}

