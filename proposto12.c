#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct elemento {
    char *cadeia;
    int timestamp;
    int ocupado;
};

typedef struct elemento elemento;

//MARK: criar tabela hash
int isprimo(int num){
    int d = num - 1;
    while (d > 1) {
        if (num % d == 0)
            return 0;
        d--;
    }
    return 1;
}
int calcula_tamanho(int n){
    int tam = (int)n/0.7;
    while (!isprimo(tam)) tam++;
    return tam;
}

elemento *criar(int tam) {
    elemento *tab =  malloc(tam * sizeof(elemento));
    if(!tab)
        return NULL;
    for (int i = 0; i < tam; i++) {
        tab[i].cadeia = NULL;
        tab[i].timestamp = -1;
        tab[i].ocupado = 0; // 0 = vazio
    }
    return tab;
}

//MARK: inserir

// transforma string em numero natural
unsigned long djb2(unsigned char *str) {
  unsigned long hash = 5381;
  int c;
  
  while ((c = *str++))
    hash = ((hash << 5) + hash) ^ c; // hash * 33 XOR c
  
  return hash;
}

int h1(unsigned long k, int tam) {
    return k % tam;
}

int h2(unsigned long k, int tam) {
    return 1 + (k % (tam - 1));
}

int inserir(elemento* tabela, char *cadeia, int tam, int time) {
    unsigned long hash = djb2((unsigned char *)cadeia);
    int pos, i;

    for (i = 0; i < tam; i++) {
        pos = (h1(hash, tam) + i * h2(hash, tam)) % tam;

        if (tabela[pos].ocupado == 0 || tabela[pos].ocupado == 2) {
            // insere
            tabela[pos].cadeia = strdup(cadeia);
            tabela[pos].timestamp = time;
            tabela[pos].ocupado = 1;
            return 1;
        } else if (tabela[pos].ocupado == 1) {
            // ollha se ja existe
            if (strcmp(tabela[pos].cadeia, cadeia) == 0) {
                // se existe, nao faz nada
                return 0;
            }
            // se nao, continua sondando
        }
    }

    return 0;
}

// MARK: search
void buscar(elemento* tabela, const char *cadeia, int tam) {
    unsigned long hash = djb2((unsigned char *)cadeia);
    int pos, i;

    for (i = 0; i < tam; i++) {
        pos = (h1(hash, tam) + i * h2(hash, tam)) % tam;
        if (tabela[pos].ocupado == 1) {
            if (strcmp(tabela[pos].cadeia, cadeia) == 0) {
                printf("[%s] esta na tabela, timestamp %d\n", cadeia, tabela[pos].timestamp);
                return;
            }
            // Continua procurando
        } else if (tabela[pos].ocupado == 0) {
            // Posição vazia = nao achou, pode parar
            printf("[%s] nao esta na tabela\n", cadeia);
            return;
        }
        // Se for ocupado == 2 (removido): continua sondando
    }

    // Se percorreu tudo e nao achou
    printf("[%s] nao esta na tabela\n", cadeia);
}

//MARK: remoção
void remover(elemento* tabela, char *cadeia, int tam) {
    unsigned long hash = djb2((unsigned char *)cadeia);
    int pos, i;

    for (i = 0; i < tam; i++) {
        pos = (h1(hash,tam) + i * h2(hash,tam)) % tam;

        if (tabela[pos].ocupado == 1) {
            if (strcmp(tabela[pos].cadeia, cadeia) == 0) {
                // Encontrou, remove
                free(tabela[pos].cadeia);  // Libera a string
                tabela[pos].cadeia = NULL;
                tabela[pos].ocupado = 2;   // Marca como removido
                tabela[pos].timestamp = -1;
                return;
            }
            // Se nao bate, continua sondando
        } else if (tabela[pos].ocupado == 0) {
            // Posição vazia => não está na tabela
            return;
        }
        // Se ocupado == 2 (removido), continua sondando
    }
    // Se percorreu tudo e não encontrou, não faz nada
}

// MARK: limpeza
void limpar(elemento* tab, int tam){
    if (tab == NULL)
        return;
    for(int i=0; i < tam; i++)
        free(tab[i].cadeia);
    free(tab);
}
//MARK: main
int main(void) {
    char comando = ' ';
    int tam=0, timestamp = 0;
    elemento* tabela = NULL;
    
    // scanf(" %c", &comando);
    char linha[300];
    fgets(linha, sizeof(linha), stdin);
    comando = linha[0];
    while (comando != 'f') {
        int len = strlen(linha);
        if (linha[len-1] == '\n') linha[len-1] = '\0';

        if (comando == 'c'){
            int n = 0;
            n = atoi(linha + 2);
            
            if(timestamp != 0){
                limpar(tabela, tam);
            }
            timestamp = 0;

            // make
            tam = calcula_tamanho(n);
            tabela = criar(tam);
            
        } else if(comando == 'i'){
            char cadeia[250];
            // scanf("%[^\n]", cadeia);
            
            strcpy(cadeia, linha+2);
            // insert
            if (inserir(tabela, cadeia, tam, timestamp)){
                timestamp++;
            }

            // timestamp++;
        } else if(comando == 'r'){
            char cadeia[250];
            // scanf(" ");
            // scanf("%[^\n]", cadeia);
            strcpy(cadeia, linha+2);
            // remove
            remover(tabela, cadeia, tam);
        } else if(comando == 'b'){
            char cadeia[250];
            // scanf(" ");
            // scanf("%[^\n]", cadeia);

            strcpy(cadeia, linha+2);
            // search
            buscar(tabela, cadeia, tam);
        }
        // scanf(" %c", &comando);
        fgets(linha, sizeof(linha), stdin);
        comando = linha[0];    
    }
    limpar(tabela, tam);
    
}