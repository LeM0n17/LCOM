#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BIT(n) (1<<(n))

char *byte2bin(uint8_t n, char *binstr) 
{
    // one element per bit (0/1)
    uint8_t binary[8];

    int i = 0;
    for(i = 0; i < 8; i++) {
        binary[i] = n % 2;
        n = n / 2;
    }

    // printing binary array in reverse order
    for (int j = 7, k= 0; j >= 0; j--, k++)
        sprintf(binstr + k, "%d", binary[j]);
        
	return binstr;
}

void print_usage(char *name) {
	printf("Usage: %s <action> <byte> <bit no>\n", name);
	printf("\tWhere: <action> one of 'h'|'l'|'r'|'s'|'t'\n"
		"\t\t <byte> 8-bit integer in hexadecimal\n"
		"\t\t <bit no> of bit to operate on\n");
}

int main(int argc, char *argv[]) {
	char a = *argv[1];	// action: 'h', 'l', 'r', 's', 't'
	unsigned long n = atoi(argv[2]); // value to convert must be smaller than 256
	int bit = atoi(argv[3]); // bit to operate on: must be between 0 an 7
    char binary[9]; // array for binary representation of n,
					//  remember that in C strings are terminated with a 0
    int aux;
    char binar[9];

	// Validate command line arguments
    if (n >= 256 || bit < 0 || bit > 7) {
        printf("Invalid Input!\n");
        return 0;
    } byte2bin(n, binary);

    if (a != 'h' && a != 'l' && a != 'r' && a != 's' && a != 't') {
        printf("Invalid Action!\n");
        return 0;
    }
	
	// Print to stdout the binary representation of n
    printf("Input in binary: %s\n", binary);
	
	// Do what the user asked and print the result
    switch (a) {
    case 'h':
        aux = 1 << (bit);
        printf("'Is High' Test Result = %i\n", (aux & n) > 0);
        break;

    case 'l':
        aux = 1 << (bit);
        printf("'Is Low' Test Result = %i\n", (aux & n) == 0);
        break;
    case 'r':
        aux = 1 << (bit);
        n |= aux;
        printf("Your new number is now = %lu\n", n);
        break;
    case 's':
        aux = ~(1 << (bit));
        n &= aux;
        printf("Your new number is now = %lu\n", n);
        break;
    case 't':
        aux = 1 << (bit);
        if ((aux & n) > 0) n |= aux;
        else n &= (~aux);
        printf("Your new number is now = %lu\n", n);
        break;
    }
	
    return 0;
}

