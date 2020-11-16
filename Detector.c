
/*
Author: Connor Kanalec
ID: 180871330
Class: Intro to Operating Systems
Assignment #2: Detector
*/

// -------------------------------------------------------------------------------------

// libraries

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// -------------------------------------------------------------------------------------

// function definitions

void load_array(int arr[], int num_cols);
void print_array(int arr[], int num_cols, int newline);
void release_resources(int requests[], int allocations[], int unallocated[], int num_cols);
int find_most_allocations(int flag[], int num_rows, int num_cols, int allocations[num_rows][num_cols]);
int grant_requests(int requests[], int unallocated[], int num_cols);
void display_deadlock(int flag[], int num_processes);

// -------------------------------------------------------------------------------------

// main / testing

int main(int argc, char *argv[]) {
    // scan n and m
    int n, m;
    scanf("%d %d\n", &n, &m);
    // scan requests, allocations, unallocated
    int requests[n][m];
    int allocations[n][m];
    int unallocated[m];
    for (int vector = 0; vector < n; vector++) load_array(requests[vector], m);
    for (int vector = 0; vector < n; vector++) load_array(allocations[vector], m);
    load_array(unallocated, m);
    // vars
    int index = 0;
    int flag[n];
    for (int j = 0; j < n; j++) flag[j] = 0;
    int num_finished = 0;
    int exec_order[n];
    int num_executed = 0;
    int blocked_order[n];
    int num_blocked = 0;
    // detector
    while (num_finished != n) {
        // if process is not done and if we can grant requests
        if (flag[index] == 0 && grant_requests(requests[index], unallocated, m) == 1) {
            // set process to done
            flag[index] = 1;
            exec_order[num_executed] = index + 1;
            num_executed++;
            num_finished++;
            // release resources
            release_resources(requests[index], allocations[index], unallocated, m);
            // since we just released resources we want to start at process 1 (to preserve increasing order)
            if (0 < num_blocked) {
                print_array(blocked_order, num_blocked, 1);
                num_blocked = 0;
            }
            index = 0;
        // there is a deadlock
        } else if (index == n - 1) {
            // print already ran processes
            if (num_executed != 0) {
                print_array(exec_order, num_executed, 1);
                num_executed = 0;
            }
            // if we are NOT continuing a deadlock from last time so we can delele more than 1 process
            if (num_blocked == 0) display_deadlock(flag, n);
            // find process with most allocated resources
            int max_index = find_most_allocations(flag, n, m, allocations);
            // set process to done
            flag[max_index] = 1;
            blocked_order[num_blocked] = max_index + 1;
            num_blocked++;
            num_finished++;
            // release resources
            release_resources(requests[max_index], allocations[max_index], unallocated, m);
            index = 0;
        } else {
            index++;
        }
    }
    // print remaining
    if (0 < num_blocked) print_array(blocked_order, num_blocked, 0);
    else print_array(exec_order, num_executed, 0);
}

// -------------------------------------------------------------------------------------

// functions

// reads from decimal values from keyboard and loads them into an array
void load_array(int arr[], int num_cols) {
    char character = 'a';
    int count = 0;
    while (character != '\n') {
        scanf("%c", &character);
        if (isdigit(character) == 1) {
            arr[count] = character - '0';
            count += 1;
        }
    }
}

// prints the contents of an array
void print_array(int arr[], int num_cols, int newline) {
    for (int i = 0; i < num_cols; i++) {
        if (i == num_cols - 1) printf("%d", arr[i]);
        else printf("%d ", arr[i]);
    }
    if (newline == 1) printf("\n");
}

// updates unallocated resources with newly freed resources
void release_resources(int requests[], int allocations[], int unallocated[], int num_cols) {
    for (int j = 0; j < num_cols; j++) {
        unallocated[j] += allocations[j];
        allocations[j] = 0;
        requests[j] = 0;
    }
}

// find the process with the most allocated resources
int find_most_allocations(int flag[], int num_rows, int num_cols, int allocations[num_rows][num_cols]) {
    int max = -1;
    int max_index = 0;
    for (int i = 0; i < num_rows; i++) {
        if (flag[i] == 0) {
            int total = 0;
            for (int j = 0; j < num_cols; j++) total += allocations[i][j];
            if (max < total) {
                max = total;
                max_index = i;
            }
        }
    }
    return max_index;
}

// check if we can grant requests
int grant_requests(int requests[], int unallocated[], int num_cols) {
    int smaller = 1;
    int i = 0;
    while (i < num_cols && smaller == 1) {
        if (unallocated[i] < requests[i]) smaller = 0;
        i++;
    }
    return smaller;
}

// print processes involved in deadlock
void display_deadlock(int flag[], int num_processes) {
    int deadlock[num_processes];
    int num_deadlock = 0;
    for (int i = 0; i < num_processes; i++) {
        if (flag[i] == 0) {
            deadlock[num_deadlock] = i + 1;
            num_deadlock++;
        }
    }
    print_array(deadlock, num_deadlock, 1);
}