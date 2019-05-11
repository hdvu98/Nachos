#include "syscall.h"
#include "copyright.h"
void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
void bubbleSort(int arr[], int n) 
{ 
   int i, j; 
   for (i = 0; i < n-1; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
           if (arr[j] > arr[j+1]) 
              swap(&arr[j], &arr[j+1]); 
} 

int main()
{
	int arr[100];
	int n,i;
	PrintString("Nhap vao so phan tu cua mang (N>0 & N<=100) N=");
	do{
	n=ReadInt();
	PrintInt(n);
	PrintChar('\n');
	if(n<=0||n>100)
		PrintString("Chi Nhap N thuoc doan [1,100].\nN=");
	}while(n<=0||n>100);
	for(i=0;i<n;i++)
	{
		PrintString("A[");
		PrintInt(i);
		PrintString("]=");
		arr[i]=ReadInt();
	}
	PrintString("Mang INPUT:\n");
	for(i=0;i<n;i++)
	{
		PrintInt(arr[i]);
				PrintChar('\t');
	}
	PrintChar('\n');
	bubbleSort(arr,n);
	PrintString("Mang OUTPUT:\n");
	for(i=0;i<n;i++)
	{
		PrintInt(arr[i]);
		PrintChar('\t');
	}
	PrintChar('\n');
	return 0;
}
