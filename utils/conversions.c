#include "conversions.h"
#include <stdlib.h>
#include "./strings.h"
#include <stdio.h>
#include <math.h>

char symbols[32] = {'!','@','#','$','%','^','&','*','<','>','a',
                  'b','c','d','e','f','g','h','i','j','k','l',
                  'm','n','o','p','q','r','s','t','u','v'};

/**
 * A recursion to create a matching representation in the custom 32 symbols base for the number n.
 * The recursion iterates through the different values required in order to represent the value, starting with the
 * "largest" matching "digit" and up to the "least" matching "digit".
 * For example, for the number 1026, the recursion will run with the values:
 * convert_to_custom_base(1026) --> 1024 + convert_to_custom_base(2) --> 1024 + 2:
 * --> 1024 + 2 = 1 * 32 ^ 2 + 0 * 32 ^ 1 + 2 * 32 ^ 0 = "102" (decimal) = "@!#" (custom).
 * @param n A value that we would like to represent in the custom 32 symbols base.
 * @return A string containing the different characters required in order to represent the received value in the custom
 * base 32.
 */
char * convert_to_custom_base(size_t n) {
    if (n < 32) {
        // Converting current symbol_type to a string and returning it:
        char * symbol = (char *)malloc(1);
        symbol[0] = symbols[n];
        return symbol;
    }
    // Converting current symbol_type to a string:
    char * symbol = (char *)malloc(1);
    symbol[0] = symbols[n % 32];
    // Returning a concatenated string from the next conversion to the custom base 32 and the current one:
    return str_cat_copy(convert_to_custom_base(n / 32), symbol);
}


/**
 * A function in charge of converting a decimal number (long long) to its matching binary 10 bit representation.
 * Heavily assuming that there are at least 11 slots in the received chars array.
 * @param number The number that we would like to convert to 10 bit binary representation.
 * @param value The char array in which we should store the 10 bit binary representation.
 */
void convert_to_10_bit_bin(size_t number, char * value)
{
    int i;
    for (i = 9; i >= 0; i--) {
        // right shift & check the state of the first bit:
        value[9 - i] = (number >> i) & 1 ? '1' : '0';
    }
    value[10] = '\0';
}

/**
 * A function in charge of converting the binary representation of a binary number to an integer value.
 * @param binary A string containing the binary representation of a number in 2s complement.
 * @return
 */
int convert_10_bit_bin_to_decimal(const char *binary)
{
    // Starting from max possible value:
    int power = 2<<8;
    int sum = 0;
    int i;

    for (i = 0; i < 10; i++)
    {
        // "Changing sign":
        if (i == 0 && binary[i] != '0')
            sum = power * -1;
        else
            // Sum + current value char (minus the ascii value of '0') * pow:
            sum += (binary[i] - '0') * power;
        // Dividing power by 2 each time:
        power /= 2;
    }

    return sum;
}

/**
 * COOL THIS SHIT ACTUALLY WORKS LMAO
 * @param n
 * @param base
 * @return
 */
//char testos(int n, int base) {
//    if (n < base) {
//        printf("%c", symbols[n]);
//        return symbols[n];
//    }
//    testos(n / base, base);
//    printf("%c", symbols[n % base]);
//    return symbols[n % base];
//}