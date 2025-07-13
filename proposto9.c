#include "stdio.h"
#include "stdlib.h"
// MARK: Estrutura basica de uma lista encadeada
struct node {
    int data;
    struct node *next;
    int count;
};
typedef struct node node;

struct list {
    node *head;
};

typedef struct list list;

list* create_list() {
    list* new_list = (list*)malloc(sizeof(list));
    if (new_list != NULL) {
        new_list->head = NULL;
    }
    return new_list;
}

void insert_node(list* lst, int value) {
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL) {
        printf("Falhou\n");
        return;
    }
    new_node->data = value;
    new_node->count = 0; 
    new_node->next = NULL; // New node will be the last node

    if (lst->head == NULL) {
        lst->head = new_node; // Insert as the head
    } else {
        node* current = lst->head; 
        while (current->next != NULL) {
            current = current->next; // Traverse to the end
        }
        current->next = new_node; // Insert at the end
    }
}

void remove_node(list* lst, int value) {
    node* current = lst->head;
    node* previous = NULL;

    while (current != NULL && current->data != value) {
        previous = current;
        current = current->next; 
    }

    if (current != NULL) {
        if (previous == NULL) {
            lst->head = current->next; // Remove the head
        } else {
            previous->next = current->next; // Bypass the current
        }
        free(current); // Free the removed node
    }
}

void print_list(list* lst) {
    node* current = lst->head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void free_list(list* lst) {
    node* current = lst->head;
    node* next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    free(lst);
}

void reset_list(list* lst, int n) {
    // Remove all nodes from the existing list, but do not free the list struct itself
    node* current = lst->head;
    while (current != NULL) {
        node* next = current->next;
        free(current);
        current = next;
    }
    lst->head = NULL;

    // Recreate the list with values from 1 to n
    for (int i = 1; i <= n; i++) {
        insert_node(lst, i);
    }
}




// MARK: Algoritimos do proposto: Sequence, Move to Front, Transpose, Count

// sequence find
int sequence_find(list* lst, int value) {
    node* current = lst->head;
    int pos = 1;
    while (current != NULL) {
        if (current->data == value) {
            return pos; // Return number of comparisons (position found)
        }
        current = current->next;
        pos++;
    }
    // If not found, insert at the end and return the index where it was inserted
    insert_node(lst, value);
    return pos-1;
}

// move to front find
int mtf_find(list* lst, int value) {
    node* current = lst->head;
    node* previous = NULL;
    int count = 0;
    while (current != NULL) {
        count++;
        if (current->data == value) {
            // Move to front
            if (previous != NULL) {
                previous->next = current->next; // Bypass current
                current->next = lst->head; // Point to the head
                lst->head = current; // Update head
            }
            return count; // Return number of comparisons (found)
        }
        previous = current;
        current = current->next;
    }
    // If not found, insert at the front and return the index where it was inserted
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL) {
        printf("Falhou\n");
        return -1; // Indicate failure
    }
    
    new_node->data = value;
    new_node->next = lst->head; // Point to the current head
    lst->head = new_node; // Insert at the front

    return count; // Return number of comparisons (not found)
    
}

// transpose find
int transpose_find(list* lst, int value) {
    node* current = lst->head;
    node* previous = NULL;
    node* previous_of_previous = NULL;
    int count = 0;
    while (current != NULL) {
        count++;
        if (current->data == value) {
            // If not at head, swap with previous
            if (previous != NULL) {
                // Properly swap previous and current nodes
                previous->next = current->next;
                current->next = previous;
                if (previous_of_previous != NULL) {
                    previous_of_previous->next = current;
                } else {
                    lst->head = current;
                }
            }
            // print_list(lst); // Print the list after the swap
            
            return count;
        }
        previous_of_previous = previous;
        previous = current;
        current = current->next;
    }
    // Not found: insert at initial position
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL) {
        printf("Falhou\n");
        return -1; // Indicate failure
    }
    new_node->data = value;
    new_node->next = lst->head; // Point to the current head
    lst->head = new_node; // Insert at the front
    // print_list(lst); // Print the list after insertion
    return count;
}



int count_find(list* lst, int value) {
    node* current = lst->head;
    node* previous = NULL;
    int count = 0;

    while (current != NULL) {
        count++;
        if (current->data == value) {
            current->count++; 

            
            if (previous != NULL) {
                previous->next = current->next;
            } else {
                lst->head = current->next;
            }

            
            node* scan = lst->head;
            node* scan_prev = NULL;

            while (scan != NULL && scan->count > current->count) {
                scan_prev = scan;
                scan = scan->next;
            }

            if (scan_prev == NULL) {
            
                current->next = lst->head;
                lst->head = current;
            } else {
                current->next = scan_prev->next;
                scan_prev->next = current;
            }
            return count;
        }

        previous = current;
        current = current->next;
    }


    node* new_node = (node*)malloc(sizeof(node));
    new_node->data = value;
    new_node->count = 1;
    new_node->next = NULL;


    current = lst->head;
    previous = NULL;

    while (current != NULL && current->count > new_node->count) {
        previous = current;
        current = current->next;
    }

    if (previous == NULL) {

        new_node->next = lst->head;
        lst->head = new_node;
    } else {
        new_node->next = current;
        previous->next = new_node;
    }
    return count; 
}


int main(void) {
    // read of inputs
    int n, r;
    scanf("%d", &n);
    scanf("%d", &r);
    int *keys = malloc((r + 1) * sizeof(int));
    if (keys == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    for (int i = 0; i < r; i++) {
        scanf(" %d", &keys[i]);
        // printf("%d ", i);
    }

    // printf("leu");
    list* my_list = create_list();
    if (my_list == NULL) {
        printf("Falhou\n");
        return 1;
    }
    // create the list with the n elements
    for (int i = 1; i <= n; i++) {
        insert_node(my_list, i);
    }
    

    int seq_count = 0;
    int mtf_count = 0;
    int transp_cound = 0;
    int count_count = 0;

    // Sequence find
    for (int i = 0; i < r; i++) {
        int key = keys[i];
        seq_count += sequence_find(my_list, key);
    }
    
    // reset the list for the next test
    reset_list(my_list, n);

    // mtf find
    for (int i = 0; i < r; i++) {
        int key = keys[i];
        mtf_count += mtf_find(my_list, key);

    }

    // reset the list for the next test
    reset_list(my_list, n);

    // transpose find
    for (int i = 0; i < r; i++) {
        int key = keys[i];
        transp_cound += transpose_find(my_list, key);
    }
    // reset the list for the next test
    reset_list(my_list, n);


    // count find
    for (int i = 0; i < r; i++) {
        int key = keys[i];
        count_count += count_find(my_list, key);
    }
    free_list(my_list);
    free(keys);

    printf("Sequencial: %d\n", seq_count);
    printf("MTF: %d\n", mtf_count);
    printf("Transpose: %d\n", transp_cound);
    printf("Count: %d\n", count_count);

    return 0;
}
