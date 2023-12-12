#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "stack.h"


/*
 * Define your call struct here.
 */
typedef struct call {
    int id;
    char name[50];
    char reason[200];
} call;



int main(int argc, char const *argv[]) {
	struct queue* call_queue = queue_create();
    struct stack* answered_calls = stack_create();
    int call_id = 1;
    int call_count = 0;
    int answered_count = 0;
    char name[50];
    char reason[100];

    while (1) {
        printf("1. Receive a new call\n");
        printf("2. Answer a call\n");
        printf("3. Current state of the stack - answered calls\n");
        printf("4. Current state of the queue - calls to be answered\n");
        printf("5. Quit\n");
        printf("Choose an option: ");
        int option;
        scanf("%d", &option);
        getchar(); // consume newline

        switch (option) {
            case 1: {
                call* new_call = (call*)malloc(sizeof(call));
                new_call->id = call_id++;

                printf("Enter caller's name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0; // Remove newline
                strncpy(new_call->name, name, sizeof(new_call->name));

                printf("Enter call reason: ");
                fgets(reason, sizeof(reason), stdin);
                reason[strcspn(reason, "\n")] = 0; // Remove newline
                strncpy(new_call->reason, reason, sizeof(new_call->reason));

                queue_enqueue(call_queue, new_call);
                call_count++;
                printf("The call has been successfully added to the queue!\n");
                break;
            }
            case 2: {
                if (queue_isempty(call_queue)) {
                    printf("No more calls need to be answered at the moment!\n");
                } else {
                    call* answered_call = (call*)queue_dequeue(call_queue);
                    stack_push(answered_calls, answered_call);
                    answered_count++;
                    call_count--;
                    printf("The following call has been answered and added to the stack!\n");
                    printf("Call ID: %d\nCaller's name: %s\nCall reason: %s\n", answered_call->id, answered_call->name, answered_call->reason);
                }
                break;
            }
            case 3: {
                printf("Number of calls answered: %d\n", answered_count);
                if (!stack_isempty(answered_calls)) {
                    call* last_call = (call*)stack_top(answered_calls);
                    printf("Details of the last call answered:\n");
                    printf("Call ID: %d\nCaller's name: %s\nCall reason: %s\n", last_call->id, last_call->name, last_call->reason);
                }
                break;
            }
            case 4: {
                printf("Number of calls to be answered: %d\n", call_count);
                if (!queue_isempty(call_queue)) {
                    call* next_call = (call*)queue_front(call_queue);
                    printf("Next call in the queue:\n");
                    printf("Call ID: %d\nCaller's name: %s\nCall reason: %s\n", next_call->id, next_call->name, next_call->reason);
                }
                break;
            }
            case 5: {
                goto cleanup;
            }
            default: {
                printf("Invalid option, please try again.\n");
                break;
            }
        }
    }

cleanup:
    // Free stack memory
    while (!stack_isempty(answered_calls)) {
        call* finished_call = (call*)stack_pop(answered_calls);
        free(finished_call);
    }
    stack_free(answered_calls);

    // Free queue memory
    while (!queue_isempty(call_queue)) {
        call* waiting_call = (call*)queue_dequeue(call_queue);
        free(waiting_call);
    }
    queue_free(call_queue);
  
	return 0;
}
