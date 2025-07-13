#include <stdio.h>

int main(void) {
    char carac;
    int num, freq;
    while (scanf(" ( %d , %d , %c ) ", &num, &freq, &carac) == 3) {
        int i = 0;

        printf("%4d |", num);
        while (i<freq) {
            printf("%c", carac);
            i++;
        }

        printf(" %d\n", freq);
        
    }
    return 0;
}
