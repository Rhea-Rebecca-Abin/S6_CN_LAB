#include <stdio.h>
#include <stdlib.h>

// Structure to represent a packet
struct packet {
    int time; // Arrival time of the packet
    int size; // Size of the packet
};

int main() {
    // Declare variables
    int i, n, m, k = 0;
    int b_size, b_current, b_outrate;

    // Prompt user to input the number of packets
    printf("Enter the number of packets: ");
    scanf("%d", &n);

    // Declare an array of packets
    struct packet p[n];

    // Prompt user to input packets in the order of their arrival time
    printf("\nEnter the packets in the order of their arrival time:\n");
    for (i = 0; i < n; i++) {
        printf("Enter the time and size for packet %d: ", i + 1);
        scanf("%d %d", &p[i].time, &p[i].size);
    }

    // Prompt user to input bucket size and output rate
    printf("\nEnter the bucket size: ");
    scanf("%d", &b_size);
    printf("Enter the output rate: ");
    scanf("%d", &b_outrate);

    // Calculate the maximum time of arrival
    m = p[n - 1].time;

    // Initialize time and bucket current size
    i = 1;
    k = 0;
    b_current = 0;

    // Loop until all packets are transmitted and bucket is empty
    while (i <= m || b_current != 0) {
        // Print current time
        printf("\nAt time %d:", i);

        // Check if a packet arrives at current time
        if (p[k].time == i) {
            // Check if bucket has enough space for the packet
            if (b_size >= b_current + p[k].size) {
                b_current += p[k].size; // Add packet size to bucket
                printf("\n%d byte packet is inserted", p[k].size);
                k++; // Move to the next packet
            } else {
                printf("\n%d byte packet is discarded", p[k].size);
                k++; // Move to the next packet
            }
        }

        // Check if there are packets to transmit
        if (b_current == 0) {
            printf("\nNo packets to transmit");
        } else if (b_current >= b_outrate) {
            // Transmit packets at output rate
            b_current -= b_outrate; // Subtract output rate from bucket
            printf("\n%d bytes transferred", b_outrate);
        } else {
            // Transmit remaining packets in the bucket
            printf("\n%d bytes transferred", b_current);
            b_current = 0; // Empty the bucket
        }

        // Print the current size of packets in the bucket
        printf("\nPackets in the bucket: %d byte(s)\n", b_current);

        // Move to the next time interval
        i++;
    }

    // Exit the program
    return 0;
}

