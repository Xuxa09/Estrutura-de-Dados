// Roupas são pontos no plano
// pegue 3 pontos e determine um plano no espaço
// Achar a normal do plano
// Pegando todos os planos possíveis, você vai saber todas as formas de dividir as roupas em 2 lotes possíveis
//  -> os pontos que estão sobre o ponto serão testastados para ver em qual lote compensa mais. 
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
double* media_rgb(int roupas[][3], int tam){
    double media_r = 0, media_g = 0, media_b = 0;
    
    for (int i = 0; i < tam; i++) {
        media_r += roupas[i][0];
        media_g += roupas[i][1];
        media_b += roupas[i][2];
    }
    double* medias = (double*)malloc(3 * sizeof(double));
    if (medias == NULL) return NULL;
    medias[0] = media_r/tam;
    medias[1] = media_g/tam;
    medias[2] = media_b/tam;
    
    return medias;
}

double* calculo_plano(int p1[3], int p2[3], int p3[3]) {
    double v1[3], v2[3];
    double *normal = (double*)malloc(3 * sizeof(double));
    if (normal == NULL) {
        return NULL; // Falha na alocação
    }
    // vetor 1
    v1[0] = p2[0] - p1[0];
    v1[1] = p2[1] - p1[1];
    v1[2] = p2[2] - p1[2];

    // vetor 2
    v2[0] = p3[0] - p1[0];
    v2[1] = p3[1] - p1[1];
    v2[2] = p3[2] - p1[2];

    normal[0] = v1[1] * v2[2] - v1[2] * v2[1]; 
    normal[1] = v1[2] * v2[0] - v1[0] * v2[2]; 
    normal[2] = v1[0] * v2[1] - v1[1] * v2[0];

    return normal;

}

int posicao_ponto(int p1[3], int p0[3], double normal[3]) {
    double resultado =( normal[0] * (p1[0] - p0[0]) +
                        normal[1] * (p1[1] - p0[1]) +
                        normal[2] * (p1[2] - p0[2]));

    if (resultado > 0)
        return 1; // frente
    else if (resultado < 0)
        return -1; // atrás
    else
        return 0; 
}

    double quanto_trasfere(int roupas[][3], int tam, int mapa[]) {
        // 1. Contar quantas roupas estão em 'tras' e 'frente'
        int count_tras = 0, count_frente = 0;
        for (int i = 0; i < tam; i++) {
            if (mapa[i] == -1) count_tras++;
            else if (mapa[i] == 1) count_frente++;
        }

        // 2. Alocar e preencher 'tras' e 'frente'
        int (*tras)[3] = malloc(count_tras * sizeof(int[3]));
        int (*frente)[3] = malloc(count_frente * sizeof(int[3]));
        int idx_tras = 0, idx_frente = 0;

        for (int i = 0; i < tam; i++) {
            if (mapa[i] == -1) {
                tras[idx_tras][0] = roupas[i][0];
                tras[idx_tras][1] = roupas[i][1];
                tras[idx_tras][2] = roupas[i][2];
                idx_tras++;
            } else if (mapa[i] == 1) {
                frente[idx_frente][0] = roupas[i][0];
                frente[idx_frente][1] = roupas[i][1];
                frente[idx_frente][2] = roupas[i][2];
                idx_frente++;
            }
        }

        // 3. Calcular médias de 'tras' e 'frente'
        double *media_tras = media_rgb(tras, count_tras);
        double *media_frente = media_rgb(frente, count_frente);

        // 4. Calcular transferências (soma dos quadrados das diferenças)
        double transfer_tras = 0.0, transfer_frente = 0.0;

        for (int i = 0; i < count_tras; i++) {
            transfer_tras += pow(tras[i][0] - media_tras[0], 2) +
                            pow(tras[i][1] - media_tras[1], 2) +
                            pow(tras[i][2] - media_tras[2], 2);
        }

        for (int i = 0; i < count_frente; i++) {
            transfer_frente += pow(frente[i][0] - media_frente[0], 2) +
                            pow(frente[i][1] - media_frente[1], 2) +
                            pow(frente[i][2] - media_frente[2], 2);
        }

        // 5. Liberar memória e retornar a soma
        free(tras);
        free(frente);
        free(media_tras);
        free(media_frente);

        return transfer_tras + transfer_frente;
    }

double calculo_transferencia(int roupas[][3], int tam) {
    double transferencia = INFINITY;
    for (int i = 0; i < tam-2; i++) {
        for (int j = i+1; j < tam-1; j++) {
            for (int k = j+1; k < tam; k++) {
                double* normal = calculo_plano(roupas[i], roupas[j], roupas[k]);
                int posis[tam];
                for (int m = 0; m < tam; m++){
                    posis[m] = posicao_ponto(roupas[m], roupas[i], normal);
                }
                double transferencia_aux;
                // printf("%d %d %d\n", i, j, k);
                // caso 1: roupa i j k no primeiro lote
                posis[i] = -1;
                posis[j] = -1;
                posis[k] = -1;
                transferencia_aux = quanto_trasfere(roupas, tam, posis);
                transferencia = (transferencia_aux < transferencia) ? transferencia_aux : transferencia;
                // printf("%f ", transferencia);
                // caso 2: roupas i j k no segundo lote
                posis[i] = 1;
                posis[j] = 1;
                posis[k] = 1;
                transferencia_aux = quanto_trasfere(roupas, tam, posis);
                transferencia = (transferencia_aux<transferencia) ? transferencia_aux : transferencia;
                // printf("%f ", transferencia);
                // caso 3: roupas i j no primeiro lote e k no segundo
                posis[i] = -1;
                posis[j] = -1;
                posis[k] = 1;
                transferencia_aux = quanto_trasfere(roupas, tam, posis);
                transferencia = (transferencia_aux<transferencia) ? transferencia_aux : transferencia;
                // printf("%f ", transferencia);
                // caso 4: roupas i  no primeiro lote e j, k no segundo
                posis[i] = -1;
                posis[j] = 1;
                posis[k] = 1;
                transferencia_aux = quanto_trasfere(roupas, tam, posis);
                transferencia = (transferencia_aux<transferencia) ? transferencia_aux : transferencia;
                // printf("%f ", transferencia);
                // caso 5: roupas i k no primeiro lote e j no segundo
                posis[i] = -1;
                posis[j] = 1;
                posis[k] = -1;
                transferencia_aux = quanto_trasfere(roupas, tam, posis);
                transferencia = (transferencia_aux<transferencia) ? transferencia_aux : transferencia;
                // printf("%f ", transferencia);
                // caso 7: roupas i no segundo lote, j no primeiro lote, k no primeiro lote
                posis[i] = 1;
                posis[j] = -1;
                posis[k] = -1;
                transferencia_aux = quanto_trasfere(roupas, tam, posis);
                transferencia = (transferencia_aux<transferencia) ? transferencia_aux : transferencia;
                // printf("%f ", transferencia);
                // caso 8: roupas i no segundo lote, j no primeiro lote, k no segundo lote
                posis[i] = 1;
                posis[j] = -1;
                posis[k] = 1;
                transferencia_aux = quanto_trasfere(roupas, tam, posis);
                transferencia = (transferencia_aux<transferencia) ? transferencia_aux : transferencia;
                // printf("%f ", transferencia);
                // caso 9: roupas i no segundo lote, j no segundo lote, k no primeiro lote  
                posis[i] = 1;
                posis[j] = 1;
                posis[k] = -1;
                transferencia_aux = quanto_trasfere(roupas, tam, posis);
                transferencia = (transferencia_aux<transferencia) ? transferencia_aux : transferencia;
                // printf("%f \n", transferencia);
                
                free(normal);
            }
            
        }
        
    }
    return transferencia;
    
}

int main(void) {
    int n_roupas, k_lotes;
    double transferencia = 0;
    scanf("%d %d", &n_roupas, &k_lotes);
    int vet_roupas[n_roupas][3];
    for (int i = 0; i < n_roupas; i++) {
        scanf("%d %d %d", &vet_roupas[i][0], &vet_roupas[i][1], &vet_roupas[i][2]);
    }
    double *media = media_rgb(vet_roupas, n_roupas);
    if(k_lotes == 1) {
        for(int i = 0; i < n_roupas; i++) {
            transferencia += pow((vet_roupas[i][0] - media[0]), 2) + pow((vet_roupas[i][1] - media[1]),2) + pow((vet_roupas[i][2] - media[2]), 2);
        }
    } else {
        if(n_roupas < 3)
            transferencia = 0;
        else {
            transferencia = calculo_transferencia(vet_roupas, n_roupas);
        }
    }
    printf("%.6f\n", transferencia);
    free(media);
    
}