/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"
#include "copyright.h"
int A[1024];	/* size of physical memory; with code, we'll run out of space!*/

int
main()
{
    int i, j, tmp,n;
	do
	{
	ReadInt(n);
	if(n<=0||n>100)
		PrintString("n>0 &n<=100\n");
	}while(n<=0||n>100);
	for(i=0;i<n;i++)
	{
	ReadInt(A[i]);
	}
	PrintString("Mang nhap vao:");
	for(i=0;i<n;i++)
	{
	PrintInt(A[i]);
	}
    /* then sort! */
    for (i = 0; i < 1023; i++)
        for (j = i; j <(1023-i); j++)
	   if (A[j] > A[j+1]) {	/* out of order -> need to swap ! */
	      tmp = A[j];
	      A[j] = A[j+1];
	      A[j+1] = tmp;
    	   }	/* and then we're done -- should be 0! */
	PrintString("Mang sau khi sap xep:");
	for(i=0;i<n;i++)
	{
	PrintInt(A[i]);
	}
return 0;
}
