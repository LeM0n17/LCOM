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

int main(int argc, char *argv[])
{
	char a = argv[1][0];	// action: 'h', 'l', 'r', 's', 't'
	unsigned long n; // value to convert must be smaller than 256
    if(atoi(argv[2]) >= 256 || atoi(argv[2]) < 0){
        printf("Invalid number, must be between 0-255 inclusively\n");
        return 0;
    }
    n = atoi(argv[2]);
	int bit = atoi(argv[3]); // bit to operate on: must be between 0 an 7
    if(bit < 0 || bit > 7){
        printf("Invalid bit, must be between 0-7 inclusively\n");
        return 0;
    }
    char binary[9]; // array for binary representation of n,
					//  remember that in C strings are terminated with a 0


	
	// Do what the user asked and print the result
    #define i BIT(bit)
    switch (a)
    {
    case 'h':
        printf("%s\n", byte2bin(n,binary));
        uint8_t test = i&n;
        printf("%s\n", byte2bin(test, binary));
        if(test > 0){
            printf("The bit %d of the given 8-bit integer (%lu) is high\n", bit, n);
        } else {
            printf("The bit %d of the given 8-bit integer (%lu) is not high\n", bit, n);
        }
        break;
    
    case 'l':
        printf("%s\n", byte2bin(n,binary));
        uint8_t t = i&n;
        printf("%s\n", byte2bin(t, binary));
        if(t == 0){
            printf("The bit %d of the given 8-bit integer (%lu) is low\n", bit, n);
        } else {
            printf("The bit %d of the given 8-bit integer (%lu) is not low\n", bit, n);
        }
        break;

    case 's':
        printf("%s\n", byte2bin(n,binary));
        uint8_t s = i|n;
        printf("%s\n", byte2bin(s,binary));
        break;
    
    default:
        printf("Invalid command, type h for bit high test, l for bit low test, s to set bit to 1, r to reset the bit and t to toggle bit value\n");
        break;
    }
	
    return 0;
}