#include "stdio.h"
#include "stdlib.h"
#include "string.h"


int main(void){
    int n;
    scanf("%d", &n);
    while (n != 0){
        int memorysize = 0;
        char ordem;
        scanf(" %d%cb", &memorysize, &ordem);
        for (int i = 0; i < n; i++) {
            char op[10], nome[10]; // nome e operação
            scanf(" %s %s ", op, nome);
            if(strcmp(op, "inserir")==0){
                // inserir
                int filesize; // tamanho do arquivo se for inserido
                char fileordem;
                scanf(" %d%cb", &filesize, &fileordem);

            } else if(strcmp(op, "remover")==0){
                // remover
                
            }
        }
        scanf(" %d", &n);    
    }
    
    
}