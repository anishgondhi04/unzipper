//Name: Anish Gondhi


#include <stdio.h>
#include <stdlib.h>

typedef struct entry {
    unsigned char character;
    unsigned int length;
    unsigned int encoding;
} Entry;

extern int bitmap_test(unsigned int bitmap, unsigned int i) {
    unsigned int mask = 0x1 << i;
    return (bitmap & mask) != 0;
}


int main() {

    unsigned int t;

    scanf("%x", &t);

    if (t == 0) {
        t = 256;
    }

    Entry *entries = calloc(t, sizeof(Entry));

    unsigned char byte[4];

    for (int i = 0; i < t; ++i) {
        for (int j = 0; j < 3; ++j) {
            int value;
            scanf("%x", &value);
            byte[j] = (unsigned char) value;
        }
        entries[i].character = byte[0];
        entries[i].length = (byte[1] & 0xf0) >> 4;
        entries[i].encoding = (unsigned int) (byte[1] & 0x0f);
        entries[i].encoding = (entries[i].encoding << 8) | (unsigned int) (byte[2]);
        entries[i].encoding = entries[i].encoding >> (int) (12 - (int) entries[i].length);
    }

    unsigned long int n;
    for (int j = 0; j < 4; ++j) {
        int value;
        scanf("%x", &value);
        if (j == 0) {
            n = value;
        } else {
            n = n << 8;
            n |= value;
        }
    }

    if (n > 0) {
        int count = (int) n;
        int value;
        unsigned int d;
        int flag = 0;
        int j = 7;

        scanf("%x", &value);
        d = (value >> j--) & 1;

        while (1) {
            for (int i = 0; i < t; ++i) {
                if (d == entries[i].encoding) {
                    printf("%c", entries[i].character);
                    flag = 1;
                    count--;
                    break;
                }
            }
            if (flag == 1) {
                d = (value >> j--) & 1;
            } else {
                d = (d << 1) | (value > j--);
            }
            if (count > 0) {
                if (j < 0) {
                    j = 7;
                    scanf("%x", &value);
                }
            } else {
                break;
            }
        }

    } else {
        for (int i = 0; i < t; ++i) {
            printf("0x%2.2x : %d ", entries[i].character, entries[i].length);
            for (int j = (int) entries[i].length - 1; j >= 0; --j) {
                printf("%d", bitmap_test(entries[i].encoding, j));
            }
            printf("\n");
        }
    }

    return 0;
}
