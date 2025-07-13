#include "stdio.h"
#include "stdlib.h"
#include "math.h"

struct node {
    int data;
    struct node* next;
    struct node* prev;   
};
typedef struct node node;

struct list {
    node* head;
    node* tail;
    int tamanho;
};
typedef struct list list;

list* create_list() {
    list* new_list = calloc(1, sizeof(list));
    if (new_list == NULL) {
        return NULL; // Memory allocation failed
    }
    return new_list;
}

int insert(list* L, int x) {
    if(L == NULL)
        return 0;
    // cria o novo no a ser inserido
    node* new_node = malloc(sizeof(node));
    new_node->data = x;
    // verifica se reserva de memória foi bem sucedida
    if (new_node == NULL) {
        return 0; // Memory allocation failed
    }
    // verifica se a lista está vazia
    if (L->head == NULL) {
        L->head = new_node;
        L->tail = new_node;
        return 1;
    }
    L->tail->next = new_node; // adiciona o novo nó ao final da lista
    new_node->prev = L->tail; // atualiza o ponteiro do novo nó
    L->tail = new_node; // atualiza o ponteiro da cauda
    return 1; // Sucesso
    
}
// refazer a funcao print para que ela ande nos dois sentidos
void print_list(list* L) {
    if (L == NULL) {
        return; // Lista inválida
    }
    node* current = L->head;
    printf("A: { ");
    while (current != L->tail) {
        printf("%d, ", current->data); // imprime o dado do nó atual
        current = current->next; // avança para o próximo nó
    }
    printf("%d }\n", current->data);
}

int libera_nos(list* L){
    node* current_node = L->head;
    node* anterior = NULL;
    while (current_node != L->tail) {
        anterior = current_node;
        current_node = anterior->next;
        free(anterior);
    }
    free(current_node);
    return 1;
}

int libera_memoria(list* L) {
    if (L == NULL){
        return 0;
    }
    libera_nos(L);
    free(L);
    return 1;
}
//MARK: sub_list
int sub_list(list* L, list* sub_list, int inicio, int fim, int passo) {
    if (L == NULL) {
        return 0; // Lista inválida
    }
    node* current = L->head;
    int aux = 0;
    while (aux < inicio) {
        aux++;
        current = current->next; // avança para o próximo nó
    }
    if (passo>0) {
        while (aux <= fim) {
        insert(sub_list, current->data);
        aux = aux + passo;
        current = current->next; // avança para o próximo nó
        }
    }
    else {
        while (aux >= fim) {
            // printf("%d ", current->data); // imprime o dado do nó atual
            insert(sub_list, current->data);
            aux = aux + passo; // passo para trás
            current = current->prev; // avança para o próximo nó
        }
    }
    // printf("\n");
    sub_list->tamanho = abs(fim-inicio) + 1;
    return 1;
}

//MARK: insert_sublist

int insert_sub_list(list* L, list* sub_list, int position) {
    if (L == NULL) {
        return 0; // Lista inválida
    }
    node* current = L->head;
    int aux = 0;
    // insercao no inicio
    if (position == 0){
        sub_list->head->prev = NULL;
        sub_list->tail->next = current;

        current->prev = sub_list->tail;
        L->head = sub_list->head;
        L->tamanho = L->tamanho + sub_list->tamanho;
        return 1;
    }
    // insercao no fim
    if (position == L->tamanho) {
        sub_list->tail->next = NULL;
        sub_list->head->prev = L->tail;
        L->tail->next = sub_list->head;
        L->tail = sub_list->tail;
        L->tamanho = L->tamanho + sub_list->tamanho;
        return 1;
    }
    while (aux < position-1) {
        aux++;
        current = current->next; // avança para o próximo nó
    }
    sub_list->head->prev = current;
    sub_list->tail->next = current->next;
    current->next = sub_list->head;
    sub_list->tail->next->prev = sub_list->tail;
    L->tamanho = L->tamanho + sub_list->tamanho;
    return 1;

}

//MARK: main

int main() {
    list* L = create_list();
    list* L_aux = create_list();
    if (L == NULL) {
        printf("Memory allocation failed\n");
        return 1; // Exit with error
    }
    
    if (L_aux == NULL) {
        printf("Memory allocation failed\n");
        return 1; // Exit with error
    }
    
    int dado_aux = 0, tamanho = 0;
    scanf("A = {%d", &dado_aux);
    tamanho++;
    if (insert(L, dado_aux) == 0) {
        printf("Memory allocation failed\n");
        return 1; // Exit with error
    }
    while (scanf(" , %d ", &dado_aux) == 1) {
        if (insert(L, dado_aux) == 0) {
            return 1; // Exit with error
        }
        tamanho++;
    }
    L->tamanho = tamanho;
    char operacao;
    int inicio, fim, inicio_insercao;
    scanf(" }");
    while (1) {
        scanf(" %c", &operacao);


        if (operacao == 'f') break;
        
        scanf(" A[ %d .. %d ]", &inicio, &fim);

        if (operacao == 's'){
            if (inicio <= fim)
                sub_list(L, L_aux, inicio, fim, 1);
            else
                sub_list(L, L_aux, inicio, fim, -1);
            libera_nos(L); // apaga os laços de L
            // Muda as informações de L para se igualarem a da L_aux
            L->head = L_aux->head;
            L->tail = L_aux->tail;
            L->tamanho = L_aux->tamanho;
            // resset do L_aux
            L_aux->head = NULL;
            L_aux->tail = NULL;
            L_aux->tamanho = 0;
            print_list(L);

        } else if (operacao == '+'){
            scanf(" %d", &inicio_insercao);
            if (inicio <= fim)
                sub_list(L, L_aux, inicio, fim, 1);
            else
                sub_list(L, L_aux, inicio, fim, -1);
            insert_sub_list(L, L_aux, inicio_insercao);
            print_list(L);
        
            // resset do L_aux
            L_aux->head = NULL;
            L_aux->tail = NULL;
            L_aux->tamanho = 0;
        }

    }
    libera_memoria(L);
}