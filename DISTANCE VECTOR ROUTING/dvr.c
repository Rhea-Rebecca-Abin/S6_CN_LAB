// Distance Vector Routing
#include<stdio.h>
struct node
{
 unsigned dist[20];
 unsigned from[20];
} rt[10];
int main()
{
 int costmat[20][20];
 int nodes, i, j, k, count=0;
 printf ("\n Distance Vector Routing");
 printf ("\n\nEnter the number of nodes : ");
 scanf ("%d",&nodes); // Enter the nodes
 printf ("\nEnter the cost matrix :\n");
 for (i = 0; i < nodes; i++)
 {
 for (j = 0; j < nodes; j++)
 {
 scanf ("%d", &costmat[i][j]);
 costmat[i][i] = 0;
 rt[i].dist[j] = costmat[i][j]; // initialise the distance equal tocost matrix
 rt[i].from[j] = j;
 }
 }
 do
 {
 count = 0;
 for (i = 0; i < nodes; i++) // We choose arbitary vertex k and wecalculate the direct distance from the node i to k using the cost matrix and add
//the distance from k to node j
 for (j = 0; j < nodes; j++)
 for( k = 0; k < nodes; k++)
 if (rt[i].dist[j] > costmat[i][k] + rt[k].dist[j])
{ //We calculate the minimum distance
 rt[i].dist[j] = rt[i].dist[k] + rt[k].dist[j];
 rt[i].from[j] = k;
 count++;
 }
 } while (count != 0);
 for (i = 0; i < nodes; i++)
 {
 printf ("\n\nFor router %d:\n", i+1);
 for(j = 0; j < nodes; j++)
 {
 printf ("\t\nnode %d via %d distance %d ", j + 1, rt[i].from[j] + 1,
rt[i].dist[j]);
 }
 }}
 /*OUTPUT
 
 Distance Vector Routing

Enter the number of nodes : 3

Enter the cost matrix :
0 1 5
1 0 2
5 2 0


For router 1:
	
node 1 via 1 distance 0 	
node 2 via 2 distance 1 	
node 3 via 2 distance 3 

For router 2:
	
node 1 via 1 distance 1 	
node 2 via 2 distance 0 	
node 3 via 3 distance 2 

For router 3:
	
node 1 via 2 distance 3 	
node 2 via 2 distance 2 	
node 3 via 3 distance 0*/
