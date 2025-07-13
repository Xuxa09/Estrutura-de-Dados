#include "stdio.h"
#include "stdlib.h"
#include "math.h"
double distancia (double x_B, double y_B, double x, double y, double z) {
    double distancia_B_A = 0;
    distancia_B_A = sqrt((x_B-x)*(x_B-x) + (y_B - y)*(y_B - y) + z*z);
    
    return distancia_B_A;
}

int main(void) {
    int n = 0;
    scanf("%d", &n);
    while (n >= 0) {
        // variaveis para guardar as coordenadas do rato e gaviao
        double cord_ratX, cord_ratY, cord_gavX, cord_gavY, cord_gavZ; 
        double cord_buraco_X, cord_buraco_Y; // cord. buraco
        // Le cords rato e gaviao
        scanf("%lf %lf %lf %lf %lf", &cord_ratX, &cord_ratY, &cord_gavX, &cord_gavY, &cord_gavZ); 

        double cord_burado_respostaX = 0, cord_burado_respostaY = 0; // cords. buracos
        int i = 0, procurando = 1; // se procurando verdadeiro programa continua procurando, caso contrário apenas lê
        while ((i < n)) {
            scanf("%lf %lf", &cord_buraco_X, &cord_buraco_Y);
            double diferenca_rat_bur = distancia(cord_buraco_X, cord_buraco_Y, cord_ratX, cord_ratY, 0);
            double diferenca_gav_bur = distancia(cord_buraco_X, cord_buraco_Y, cord_gavX, cord_gavY, cord_gavZ);
            
            if (((2 * diferenca_rat_bur) < diferenca_gav_bur) &&  (procurando)) {
                cord_burado_respostaX = cord_buraco_X;
                cord_burado_respostaY = cord_buraco_Y;
                procurando = 0;
            }
            i++;
        }
        if (!procurando) {
            printf("O rato pode escapar pelo buraco (%.3lf,%.3lf).\n", cord_burado_respostaX, cord_burado_respostaY);
        } else {
            printf("O rato nao pode escapar.\n");
        }
        scanf("%d", &n);
    }
    
}