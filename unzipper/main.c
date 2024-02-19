//Name: Anish Gondhi
//This code is designed to unzip a encoded file with special algorithm
//Base approach is to read in the table of encoded bytes/characters
//all input is in hexadecimal notation


#include <stdio.h>

//struct entry is used as a variable type to store the decoding table
typedef struct entry {
    unsigned char character;
    unsigned int length;
    unsigned int encoding;
} Entry;

//fucntion taken from lab3 which returns the ith bit
extern int bitmap_test(unsigned int bitmap, unsigned int i) {
    unsigned int mask = 0x1 << i;
    return (bitmap & mask) != 0;
}


int main() {

    unsigned int t;                                          //no of elements in table

    scanf("%x", &t);

    if (t == 0) {
        t = 256;
    }

    Entry entries[t];                                       //array of type Entry to store the table

    //This loop takes in the input from stdin stream and stores it in the array entries.
    for (int i = 0; i < t; ++i) {
        unsigned char byte[3];
        Entry entry;

        //to read individual bytes from input stream and store it in byte array
        for (int j = 0; j < 3; ++j) {
            int value;
            scanf("%x", &value);
            byte[j] = (unsigned char) value;
        }

        entry.character = byte[0];                         // first byte character in byte[0]
        entry.length = (byte[1] & 0xf0) >> 4;              // extracting first 4 bits from second byte (byte[1])
        entry.encoding = (unsigned int) (byte[1] & 0x0f);  // extracting second half of the second byte
        entry.encoding = (entry.encoding << 8) | (unsigned int) (byte[2]); // adding the last 8 bits to the encoding
        entry.encoding = entry.encoding >> (int) (12 - (int) entry.length); // reducing it to the required length

        entries[i] = entry;
    }

    // This loop takes the four bytes from the input stream entered after the decoding table
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

    // if there is a compressed stream of characters then enter
    // else print the decoding table in specific format
    if (n > 0) {
        // count keeps track of the number of characters printed
        // flag is used to set a condition true where it resets the the variable D, len and flag to 0
        // len is length of bits read from bitstream
        // j is to keep track of loc of bit in the read byte
        int count = 0, flag = 0, len = 0, j = 7;

        unsigned int value, D;

        scanf("%x", &value);

        // loops executes until no of characters printed (count) is equal to n
        while (count < (int) n) {

            // if the index j < 0 read a new byte from bitstream and store it in value
            if (j < 0) {
                scanf("%x", &value);
                j = 7;
            }
            // if length of the encoding (D) is 12, start reading another encoding
            if (len <= 12) {
                if (len == 0) {
                    D = bitmap_test(value, j--);
                    ++len;
                } else {
                    D = D << 1;
                    D |= bitmap_test(value, j--);
                    ++len;
                }
            }

            // This loop checks if the encoding is in the decoding table
            for (int i = 0; i < t; ++i) {
                Entry entry = entries[i];

                if (len == entry.length && D == entry.encoding) {
                    printf("%c", entry.character);
                    count++;
                    flag = 1;
                    break;
                }
            }
            if (flag) {
                flag = 0;
                D = D & 0x0;
                len = 0;
            }
        }

    } else {
        // printing the decoding table
        for (int i = 0; i < t; ++i) {
            printf("0x%2.2x : %d ", entries[i].character, entries[i].length);
            // loop to print the encoding one bit at a time
            for (int j = (int) entries[i].length - 1; j >= 0; --j) {
                printf("%d", bitmap_test(entries[i].encoding, j));
            }
            printf("\n");
        }
    }
    return 0;
}
