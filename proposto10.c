#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct node {
    int data;
    struct node* left;
    struct node* right;
};
typedef struct node node;


node* make_tree(char* pre_ordem, char* in_ordem, int* index_pre, int inicio, int fim) {
    if ((inicio > fim) || (inicio < 0)){
        return NULL;
    }
    node* new_node = malloc(sizeof(node));
    // if (inicio == fim){
    //     new_node->data = in_ordem[inicio];
    //     new_node->left = NULL;
    //     new_node->right = NULL;
    //     return new_node;
    // }
    // Encontra o índice do caractere pre_ordem[index_pre] na substring in_ordem[inicio : fim]
    int root_index = -1;
    for (int i = inicio; i <= fim; i++) {
        if (in_ordem[i] == pre_ordem[(*index_pre)]) {
            root_index = i;
            break;
        }
    }
    if (root_index == -1) {
        // Não encontrado, trate o erro conforme necessário
        free(new_node);
        return NULL;
    }

    new_node->data = pre_ordem[(*index_pre)++];

    // printf("%c ", new_node->data);

    new_node->left = make_tree(pre_ordem, in_ordem, index_pre, inicio, root_index-1);
    new_node->right = make_tree(pre_ordem, in_ordem, index_pre, root_index + 1, fim);

    return new_node;
}

// void in_ord(node* root){
//     if (root == NULL)
//         return;
//     in_ord(root->left);
//     printf("%c ", root->data);
//     in_ord(root->right);

// }

void pos_ord(node* root){
    if (root == NULL)
        return;
    pos_ord(root->left);
    pos_ord(root->right);
    printf("%c", root->data);
}



void largura(node* root, int tamanho) {
    if (!root) return;
    node* fila[128];
    int ini = 0, fim = 0;
    fila[fim++] = root;
    while (ini < fim) {
        node* p = fila[ini++];
        printf("%c", p->data);
        if (p->left) fila[fim++] = p->left;
        if (p->right) fila[fim++] = p->right;
    }
}

void clean_tree(node* root){
    if (root == NULL)
        return;
    clean_tree(root->left);
    clean_tree(root->right);
    free(root);
}


int main() {
    char *pre_ordem, *in_ordem;
    int tamanho = 126 - 33 + 1;    
    pre_ordem = malloc(tamanho * sizeof(char));
    in_ordem = malloc(tamanho * sizeof(char));
    
    while (scanf(" %s %s", pre_ordem, in_ordem) == 2) {
        
        tamanho = strlen(pre_ordem);
        int i = 0;
        node* arvore = make_tree(pre_ordem, in_ordem, &i, 0, tamanho-1);
        pos_ord(arvore);
        printf(" ");
        largura(arvore, tamanho);
        printf("\n");
        clean_tree(arvore);        
    }
    free(pre_ordem);
    free(in_ordem);
}



/*
ABCZEF CBAEZF

      A
    B   Z 
  C    E  F

ZADEK ZAEDK

       Z
         A
           D
          E  K


ACBD ABDC

         A
            C
           B
             D
*/