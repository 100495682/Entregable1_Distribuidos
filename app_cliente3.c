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

    printf("Cliente 3 arrancando\n");

    for (i = 0; i < 12; i++) {
        snprintf(key, sizeof(key), "k%d", i);

        if (i % 4 == 1) {
            r = delete_key(key);
            printf("[C3] delete_key(%s) = %d\n", key, r);
        } else {
            r = exist(key);
            printf("[C3] exist(%s) = %d\n", key, r);

            if (r == 1) {
                r = get_value(key, out, &n, vout, &pout);
                printf("[C3] get_value(%s) = %d\n", key, r);
            } else {
                snprintf(txt, sizeof(txt), "c3_crea_%d", i);
                v[0] = 50.0f + i;
                v[1] = 60.0f + i;
                v[2] = 70.0f + i;
                p.x = 500 + i;
                p.y = 600 + i;
                p.z = 700 + i;

                r = set_value(key, txt, 3, v, p);
                printf("[C3] set_value(%s) = %d\n", key, r);
            }
        }

    }

    printf("Cliente 3 termina\n");
    return 0;
}