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

    Entry entries[t];



    for (int i = 0; i < t; ++i) {
        unsigned char byte[3];
        Entry entry;

        for (int j = 0; j < 3; ++j) {
            int value;
            scanf("%x", &value);
            byte[j] = (unsigned char) value;
        }

        entry.character = byte[0];
        entry.length = (byte[1] & 0xf0) >> 4;
        entry.encoding = (unsigned int) (byte[1] & 0x0f);
        entry.encoding = (entry.encoding << 8) | (unsigned int) (byte[2]);
        entry.encoding = entry.encoding >> (int) (12 - (int) entry.length);

        entries[i] = entry;
    }

    unsigned long int n;
    for (int j = 0; j < 4; ++j) {
        unsigned int value;

        scanf("%x", &value);

        if (j == 0) {
            n = value;
        } else {
            n = n << 8;
            n |= value;
        }
    }

    if (n > 0) {
        int count = 0, flag = 0, len = 0, j = 7;

        unsigned int value, d;

        scanf("%x", &value);

        while (count < (int)n) {

            if(j < 0){
                scanf("%x", &value);
                j = 7;
            }
            if( len <= 12){
                if(len == 0){
                    d = bitmap_test(value,j--);
                    ++len;
                }else{
                    d  = d << 1;
                    d |= bitmap_test(value,j--);
                    ++len;
                }
            }

            for (int i = 0; i < t; ++i) {
                Entry entry = entries[i];

                if ( len == entry.length && d == entry.encoding) {
                    printf("%c", entry.character);
                    count++;
                    flag = 1;
                    break;
                }
            }
            if (flag) {
                flag = 0;
                d = d & 0x0;
                len = 0;
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
