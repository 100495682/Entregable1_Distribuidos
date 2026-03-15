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

    printf("Cliente 1 arrancando\n");

    for (i = 0; i < 10; i++) {
        snprintf(key, sizeof(key), "k%d", i);

        r = exist(key);
        printf("[C1] exist(%s) = %d\n", key, r);

        if (r == 0) {
            snprintf(txt, sizeof(txt), "c1_inicial_%d", i);
            v[0] = i + 0.1f;
            v[1] = i + 0.2f;
            v[2] = i + 0.3f;
            p.x = i;
            p.y = i + 10;
            p.z = i + 20;

            r = set_value(key, txt, 3, v, p);
            printf("[C1] set_value(%s) = %d\n", key, r);
        }

        r = get_value(key, out, &n, vout, &pout);
        printf("[C1] get_value(%s) = %d\n", key, r);

        if (i % 3 == 0) {
            snprintf(txt, sizeof(txt), "c1_mod_%d", i);
            v[0] = i * 2.0f;
            v[1] = i * 2.0f + 1.0f;
            p.x = 100 + i;
            p.y = 200 + i;
            p.z = 300 + i;

            r = modify_value(key, txt, 2, v, p);
            printf("[C1] modify_value(%s) = %d\n", key, r);
        }
    }

    printf("Cliente 1 termina\n");
    return 0;
}