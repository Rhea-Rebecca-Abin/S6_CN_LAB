#include<stdio.h>

void main(){

int in,out,n,bsize,bucket=0;
printf("\nEnter  no of packets that are incoming ");
scanf("%d",&n);
printf("\nEnter the bucket size");
scanf("%d",&bsize);
printf("\nEnter the outgoing rate :");
scanf("%d",&out);

while(n!=0){
  
  printf("\nEnter the incoming packet size:");
  scanf("%d",&in);
  printf("\nIncoming packet size :%d",in);
  if (in<=(bsize-bucket)){
         bucket+=in;
         printf("\nBucket status : %d out of %d ",bucket,bsize);
         }
   else{
         printf("\nDropped packets %d ",in-(bsize-bucket));
         bucket=bsize;
         printf("\nBucket status : %d out of %d ",bucket,bsize);
         
         }
   printf("\nOutgoing packets %d",out);
   
   bucket -=out;
   if (bucket < 0) bucket = 0;
    printf("\nBucket status : %d out of %d ",bucket,bsize);
   n--;
   }}
   
/*OUTPUT
Enter  no of packets that are incoming 5

Enter the bucket size3

Enter the outgoing rate :2

Enter the incoming packet size:5

Incoming packet size :5
Dropped packets 2 
Bucket status : 3 out of 3 
Outgoing packets 2
Bucket status : 1 out of 3 
Enter the incoming packet size:4

Incoming packet size :4
Dropped packets 2 
Bucket status : 3 out of 3 
Outgoing packets 2
Bucket status : 1 out of 3 
Enter the incoming packet size:3

Incoming packet size :3
Dropped packets 1 
Bucket status : 3 out of 3 
Outgoing packets 2
Bucket status : 1 out of 3 
Enter the incoming packet size:2

Incoming packet size :2
Bucket status : 3 out of 3 
Outgoing packets 2
Bucket status : 1 out of 3 
Enter the incoming packet size:1

Incoming packet size :1
Bucket status : 2 out of 3 
Outgoing packets 2
Bucket status : 0 out of 3*/ 

/*
Initialize Variables:

in: Incoming packet size.
out: Outgoing packet rate.
n: Number of packets.
bsize: Bucket size.
bucket: Current bucket level (initially 0).
Input Values:

Prompt and read the number of incoming packets (n).
Prompt and read the bucket size (bsize).
Prompt and read the outgoing packet rate (out).
Process Each Packet:

Loop while there are packets (n is not 0):
Prompt and read the incoming packet size (in).
Print the size of the incoming packet.
Check if the incoming packet fits into the bucket:
If in is less than or equal to the remaining space in the bucket (bsize - bucket):
Add the incoming packet size to the bucket.
Print the current bucket status.
Else:
Print the number of dropped packets (in - (bsize - bucket)).
Set the bucket to its maximum capacity (bsize).
Print the current bucket status.
Print the outgoing packet rate.
Subtract the outgoing packet rate from the bucket.
Ensure the bucket does not go below 0. If it does, set it to 0.
Print the current bucket status.
Decrease the packet count (n).
End of Algorithm

/
