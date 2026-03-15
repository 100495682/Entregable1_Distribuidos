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

    printf("Cliente 4 arrancando\n");

    for (i = 8; i < 20; i++) {
        snprintf(key, sizeof(key), "k%d", i);

        r = exist(key);
        printf("[C4] exist(%s) = %d\n", key, r);

        if (r == 1) {
            r = get_value(key, out, &n, vout, &pout);
            printf("[C4] get_value(%s) = %d\n", key, r);

            if (i % 3 == 0) {
                r = delete_key(key);
                printf("[C4] delete_key(%s) = %d\n", key, r);
            } else {
                snprintf(txt, sizeof(txt), "c4_mod_%d", i);
                v[0] = 1000.0f + i;
                v[1] = 2000.0f + i;
                p.x = 800 + i;
                p.y = 900 + i;
                p.z = 1000 + i;

                r = modify_value(key, txt, 2, v, p);
                printf("[C4] modify_value(%s) = %d\n", key, r);
            }
        } else {
            snprintf(txt, sizeof(txt), "c4_alta_%d", i);
            v[0] = 1.0f * i;
            v[1] = 2.0f * i;
            v[2] = 3.0f * i;
            p.x = 10 + i;
            p.y = 20 + i;
            p.z = 30 + i;

            r = set_value(key, txt, 3, v, p);
            printf("[C4] set_value(%s) = %d\n", key, r);
        }
    }

    printf("Cliente 4 termina\n");
    return 0;
}