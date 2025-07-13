#include "stdio.h"
#include "string.h"
#include "stdlib.h"

struct node {
    int key;
    struct node* left;
    struct node* right;
    struct node* parent;
    int timestamp;

};
typedef struct node node;

struct tree {
    node* root;
};
typedef struct tree tree;

// MARK: criacao
tree* criar() {
    tree* new_tree = malloc(sizeof(tree));
    if(new_tree == NULL)
        return NULL;
    new_tree->root = NULL;
    return new_tree;
}

// MARK: altura
int altura(node* raiz){
    if (raiz == NULL)
        return 0;
    int altuara_esq = altura(raiz->left);
    int altuara_dir = altura(raiz->right);
    if(altuara_esq > altuara_dir)
        return (1 + altuara_esq);
    else
        return (1 + altuara_dir);
}

//MARK: insercao
int inserir(tree* T, int value, int time) {
    node* new_leaf = malloc(sizeof(node));
    if(!new_leaf)
        return -1; // erro de memoria
    new_leaf->key = value;
    new_leaf->timestamp = time;
    new_leaf->left = NULL;
    new_leaf->right = NULL;

    if (T->root == NULL){
        new_leaf->parent = NULL;
        T->root = new_leaf;
        return 1;
    }

    node* p = T->root;
    node* u = NULL;
    while (p != NULL) {
        u = p;
        if(value > p->key){
            p = p->right;
        } else if (value < p->key) {
            p = p->left;
        } else {
            return 0; // elemento igual: nao acrescenta
        }
    }
    new_leaf->parent = u;
    if (value > u->key)
        u->right = new_leaf;
    else
        u->left = new_leaf;
    
    p = u; // guarda u para tirar caso arvore deixe de ser PTCN    
    while(u->parent != NULL){

        u = u->parent;
        if(abs(altura(u->left) - altura(u->right)) > 2){
            // printf("diferenca = %d ", abs(altura(u->left) - altura(u->right)));
            if (value > p->key)
                p->right = NULL;
            else
                p->left = NULL;
            free(new_leaf);
            return 0; // arvore deixa de ser PTCN
        }
    }
    return 1;

}

//MARK: search
node* search(node* raiz, int key) {
    if(raiz == NULL)
        return NULL; // nao achou
    if(raiz->key == key)
        return raiz; //retorna posicao
    
    if(key < raiz->key)
        return search(raiz->left, key);
    else
        return search(raiz->right, key);
}

//MARK: continua ptcn
int continua_ptcn(node* no){
    if(no==NULL)
        return 1;
    // no = no->parent;
    while (no != NULL) {
        // printf("%d", abs((altura(no->left)-1) - (altura(no->right)-1)));
        if(abs((altura(no->left)-1) - (altura(no->right)-1)) > 2)
            return 0;
        no = no->parent;
    }
    return 1;
    
}

node* minimo(node* raiz){
    if(raiz == NULL)
        return NULL;
    if(raiz->left == NULL)
        return raiz;
    else
        return minimo(raiz->left);
}

//MARK: sucessor
node* sucessor(node* raiz){
    if (raiz == NULL){
        return NULL;
    }
    if(raiz->right != NULL)
        return minimo(raiz->right);
    else {
        node* p = raiz->parent;
        node* u = raiz;
        while ((p != NULL)&&(p->right == u)) {
            u = p;
            p = p->parent;
        }
        return p;
    }
}

//MARK: remocao
void remover_no_com_um_filho(tree* T, node* p) {
    if (!p) return;

    node* son = (p->left) ? p->left : p->right;

    if (son)
        son->parent = p->parent;

    if (p->parent) {
        // pai aponta para o filho
        if (p->parent->left == p)
            p->parent->left = son;
        else
            p->parent->right = son;
    } else {
        // p era a raiz
        T->root = son;
    }
}

void remover_folha(tree* T, node* p) {
    if (!T || !p) return;

    // p é a raiz e não tem filhos
    if (p->parent == NULL) {
        T->root = NULL;
        return;
    }

    // Desconecta o no do seu pai
    if (p->parent->left == p)
        p->parent->left = NULL;
    else if (p->parent->right == p)
        p->parent->right = NULL;

}

void recolocar_no_completo(tree* T, node* p) {
    if (!T || !p) return;

    // Se p era a raiz
    if (p->parent == NULL) {
        T->root = p;
        return;
    }

    // Reconecta p ao seu pai
    if (p->key < p->parent->key)
        p->parent->left = p;
    else
        p->parent->right = p;


    if (p->left)
        p->left->parent = p;
    if (p->right)
        p->right->parent = p;
}

int remover(tree* T, int key){

    /*A remoção é como na ABB, sendo que uma remoção de nó com dois filhos sempre usa o sucessor. 
    Além disso, se a remoção fizer com que a árvore deixe de ser PTCN então ela é desfeita 
    e a árvore permanece como estava antes da tentativa de remoção.*/

    if(T->root == NULL) return 0;
    node* p = T->root;
    p = search(p, key);
    
    if(p==NULL) return 1; // nao tem o elemento

    // caso 1: 2 filhos
    if ((p->left != NULL) && (p->right != NULL)) {
    
        node* subs_p = minimo(p->right);
        node* pai = subs_p->parent;


        int old_timestamp = p->timestamp;
        int old_key = p->key;
        
        p->timestamp = subs_p->timestamp;
        p->key = subs_p->key;

        // removendo sucessor
        if ((subs_p->right)||(subs_p->left)){
            remover_no_com_um_filho(T, subs_p);
        } else {
            remover_folha(T, subs_p);
        }

        if(continua_ptcn(subs_p->parent)){
            // printf("altura esquerda: %d, altura direita: %d", altura(T->root->left)-1, altura(T->root->right)-1);
            free(subs_p);
            return 1;
        } else {
            // printf("Eu sou louco");
            recolocar_no_completo(T, subs_p);
            p->timestamp = old_timestamp;
            p->key = old_key;
        }

    } else if ((p->left == NULL) != (p->right == NULL)) { // exatamente um filho
        node* filho = (p->left) ? p->left : p->right;

        // Remove p
        remover_no_com_um_filho(T, p);

        if (continua_ptcn(filho->parent)) {
            free(p);
            return 1;
        } else {
            // desfaz
            recolocar_no_completo(T, p);
            return 0;
        }
    } else if (p->left == NULL && p->right == NULL) {
        remover_folha(T, p);

        if (continua_ptcn(p->parent)) {
            free(p);
            return 1;
        } else {
            recolocar_no_completo(T, p);
            return 0;
        }
    }
}


//MARK: impressao
void impressao(node* raiz) {
    if(raiz==NULL) {
        return;
    }
    impressao(raiz->left);
    printf("%d ", raiz->key);
    impressao(raiz->right);
}

// MARK: limpar nos
int limpar_nos(tree* T, node* raiz){
    if(T->root == NULL)
        return 1; // ja esta vaizia
    if(raiz == NULL){
        return 0;
    } else {
        limpar_nos(T, raiz->left);
        limpar_nos(T, raiz->right);
        free(raiz);
        return 1;
    }
}

//MARK: conta nos
int conta_nos(node* raiz){
    if (raiz == NULL){
        return 0;
    }
    int nos_esquerda = conta_nos(raiz->left);
    int nos_direita = conta_nos(raiz->right);
    
    return 1 + nos_direita + nos_esquerda;

}

//MARK: conta folhas
int conta_folhas(node* raiz){
    if (raiz == NULL)
        return 0;
    if ((raiz->left==NULL)&&(raiz->right==NULL)){
        return 1;
    }
    return conta_folhas(raiz->left) + conta_folhas(raiz->right);

}

int main(void){
    int executa = 1, value, time = 0;
    char comando[10];
    tree* T;
    while (executa) {
        scanf("%s", comando);

        if(!strcmp(comando, "inserir")){

            scanf("%d", &value);
            int result = inserir(T, value, time);
            if(result == -1)
                printf("memoria insuficiente\n");
            time++;
        
        } else if(!strcmp(comando, "remover")) {
            scanf("%d ", &value);
            remover(T, value);
            time++;
        
        } else if(!strcmp(comando, "buscar")) {
            
            scanf("%d", &value);
            node* posicao = search(T->root, value);
            if(posicao == NULL)
                printf("nao ha chave %d\n", value);
            else
                printf("chave %d: inserida no tempo %d\n", value, posicao->timestamp);
        
        } else if(!strcmp(comando, "imprimir")) {
            
            if(T->root == NULL)
                printf("arvore vazia\n");
            else{
                printf("em-ordem: ");
                impressao(T->root);
                printf("\n");
            }

        } else if(!strcmp(comando, "info")){
            
            int height = altura(T->root);
            height = (height>0) ? (height-1) : 0;
            printf("nos: %d, folhas: %d, altura: %d\n", conta_nos(T->root), conta_folhas(T->root), height);
        
        } else if(!strcmp(comando, "criar")) {
        
            if((T == NULL) || (time == 0)){
                T = criar();
            } else {
                limpar_nos(T, T->root);
                T = criar();
            }
            if(T == NULL){
                printf("memoria insuficiente\n");
            }
        
        } else if(!strcmp(comando, "raiz")) {
            if(T->root)
                printf("Raiz: %d\n", T->root->key);
                // printf("Raiz->right: %d. altura: %d\n", T->root->right->key, altura(T->root->right));
                // printf("Raiz->left: %d. altura: %d\n", T->root->left->key, altura(T->root->left));
                // printf("esquerda: %d | direita: %d", search(T->root, 198)->left->key, search(T->root, 198)->right->key);
        } else {
        
            limpar_nos(T, T->root);
            free(T);
            executa = 0;

        }
    }
    
}