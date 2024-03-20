#include<stdio.h>
#include<stdlib.h>
struct packet {
int time;
int size;
};
int main () {
int i, n, m, k = 0;
int b_size, b_current, b_outrate;
printf ("Enter the no. of packets: ");
scanf ("%d", &n);
struct packet p[n];
printf ("\nEnter the packets in the order of their arrival time: \n");
for (i = 0; i < n; i++) {
printf ("Enter the time and size: ");
scanf ("%d %d", &p[i].time, &p[i].size);
}
printf ("\nEnter the bucket size: ");
scanf ("%d", &b_size);
printf ("Enter the output rate: ");
scanf ("%d", &b_outrate);
m = p[n - 1].time;
i = 1;
k = 0;
b_current = 0;
while (i <= m || b_current != 0) {
printf("\nAt time %d", i);
if (p[k].time == i) {
if (b_size >= b_current + p[k].size) {
b_current = b_current + p[k].size;
printf("\n%d byte packet is inserted", p[k].size);
k = k + 1;
}
else {
printf("\n%d byte packet is discarded", p[k].size);
k = k + 1;
}
}
if (b_current == 0) {
printf("\nNo packets to transmit");
}
else if (b_current >= b_outrate) {
b_current = b_current - b_outrate;
printf("\n%d bytes transferred", b_outrate);
}
else {
printf("\n%d bytes transferred", b_current);
b_current = 0;
}
printf("\nPackets in the bucket %d byte(s)\n", b_current);
i++;
}
return 0;
}
