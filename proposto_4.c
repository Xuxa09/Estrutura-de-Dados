// LALLIER PALLU CARDOSO DE FARIA RA: 246639
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int compare_kmers(const void *a, const void *b) {
    // usado para comparar as duas strings
    return strcmp((const char *)a, (const char *)b);
}

void maioresKmers(int n, int k, char *sequencia ){
    // iniciaçizacao do vetor kmers, que tem tamanho n-k, onde ficarão todos kmers de tamanho k possíveis.
    char kmers[n][k+1];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k+1; j++) {
            kmers[i][j] = '\0';
        }
    }
    int numero_sequencias = n-k+1;
    for (int i = 0; i < numero_sequencias; i++) {
        // formando os kmers
        strncpy(kmers[i], sequencia + i, k);
    }
    // ordenando-os
    qsort(kmers, numero_sequencias, (k+1) * sizeof(char), compare_kmers);


    // aqui vamos encontrar os kmers mais frequentes
    int max_count = 1, current_count = 1;
    char max_kmers[n][k+1];
    int num_max_kmers = 0;
    
    strcpy(max_kmers[0], kmers[0]);

    for (int i = 1; i < numero_sequencias; i++) {
        if (strcmp(kmers[i], kmers[i-1]) == 0) {
            current_count++;
            
            if (current_count > max_count) {
                max_count = current_count;
                num_max_kmers = 0;
                strcpy(max_kmers[num_max_kmers], kmers[i]);
                num_max_kmers++;
            } else 
                if (current_count == max_count) {
                    strcpy(max_kmers[num_max_kmers], kmers[i]);
                    num_max_kmers++;
                }
        } else {
            current_count = 1;
            
            if (max_count == 1) {
                strcpy(max_kmers[num_max_kmers], kmers[i]);
                num_max_kmers++;
            }
        }
        
    }
    printf("%d %d: ", k, max_count);
    // caso todos kmers tenham a frequência 1 pega o primeiro da lista ordenada
    if(num_max_kmers == 0)
        printf("%s ", kmers[0]);
    else
        for (int i = 0; i < num_max_kmers; i++) {
            printf("%s ", max_kmers[i]);
        }
    printf("\n");

}

int main(void) {
    
    int n, k=0;
    
    while (scanf("%d", &n) != EOF) {
        char sequencia[n + 1];
        scanf(" %s", sequencia);        
        scanf(" %d", &k);
        maioresKmers(n,k,sequencia);
        
    }

    return 0;
}
