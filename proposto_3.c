#include <stdio.h>
#include <math.h>

void atualiza_r(int *s, int tam_s, int *R, int tam_R, int r, int posicao, int valor) {
    int aux = s[posicao];
    s[posicao] = valor;
    int index_R = posicao/r;
    R[index_R] += s[posicao] - aux;

}
int somar(int *s, int tam_s, int *R, int tam_R, int r, int limite_inicial, int limite_final) {
    int soma = 0, i = limite_inicial;
    while (i % r != 0) {
        soma += s[i];
        if (i == limite_final){
            return soma;
        }
        i++;
    }
    int index_R = i/r;
    while ((index_R * r + r-1)<= limite_final) {
        soma += R[index_R];
        index_R++;
    }
    i = (index_R-1)*r + r;
    while (i <= limite_final) {
        soma += s[i];
        i++;
    }
    return soma;
    
}
int main(void) {
    int n, lim_inf, lim_sup;
    char op;
    scanf("%d", &n);
    int s[n];

    for (int i = 0; i < n; i++) {
        scanf("%d", &s[i]);
    }
    int r = sqrt(n);
    r = r*r<n ? r+1 : r;
    int R[r], aux_inf = 0, aux_sup = 0, pos=0;
    for (int i = 0; i < r; i++) {
        aux_sup = aux_sup + r;
        aux_sup > n ? aux_sup = n : aux_sup;
        R[pos] = 0;
        for (int j = aux_inf; j < aux_sup; j++) {
            R[pos] += s[j];
        }
        aux_inf = aux_sup;
        pos++;
    }
    if (n > aux_sup) {
        R[pos] = 0;
        for (int i = aux_sup; i < n; i++) {
            R[pos] += s[i];
        }
        pos++;
    }
    int tam_R = pos;
    
    while (scanf(" %c %d %d", &op, &lim_inf, &lim_sup) == 3) {
        if(op == 's'){
            int result_sum = somar(s, n, R, tam_R, r, lim_inf, lim_sup);
            printf("%d\n", result_sum);
        } else {
            atualiza_r(s, n, R, tam_R, r, lim_inf, lim_sup);
        }
    }      
    
}