#include <stdio.h>
#include <unistd.h>
#include "claves.h"

int main(void) {
    int i, r, n;
    char key[256];
    char txt[256];
    char out[256];
    float v[32];
    float vout[32];
    struct Paquete p, pout;

    printf("Cliente 2 arrancando\n");

    for (i = 5; i < 15; i++) {
        snprintf(key, sizeof(key), "k%d", i);

        r = get_value(key, out, &n, vout, &pout);
        printf("[C2] get_value(%s) = %d\n", key, r);

        if (r == -1) {
            snprintf(txt, sizeof(txt), "c2_nuevo_%d", i);
            v[0] = i + 5.0f;
            v[1] = i + 6.0f;
            v[2] = i + 7.0f;
            p.x = i + 30;
            p.y = i + 40;
            p.z = i + 50;

            r = set_value(key, txt, 3, v, p);
            printf("[C2] set_value(%s) = %d\n", key, r);
        } else {
            if (i % 2 == 0) {
                snprintf(txt, sizeof(txt), "c2_cambio_%d", i);
                v[0] = i * 3.0f;
                v[1] = i * 3.0f + 1.0f;
                p.x = i + 300;
                p.y = i + 400;
                p.z = i + 500;

                r = modify_value(key, txt, 2, v, p);
                printf("[C2] modify_value(%s) = %d\n", key, r);
            }
        }

        r = exist(key);
        printf("[C2] exist(%s) = %d\n", key, r);
    }

    printf("Cliente 2 termina\n");
    return 0;
}