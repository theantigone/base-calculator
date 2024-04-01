//
// Created by Quang on 3/26/2024.
//
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "calc.h"

bin_str binary_str;

void convert_to_binary(short val, bin_str bin) { // convert val to binary str for output
    int i;
    unsigned short unsigned_val = val;

    // Initialize bin to all zeros and null terminator
    for (i = 0; i < 19; i++) {
        bin[i] = '0';

        // Insert space every 4 bits (except the last 4)
        if ((i + 1) % 5 == 0) {
            bin[i] = ' ';
        }
    }
    bin[19] = '\0'; // Null terminator

    // Convert val to binary and store in bin
    i = 0;
    while (unsigned_val > 0) {
        bin[18 - i] = (unsigned_val % 2) ? '1' : '0';
        unsigned_val /= 2;
        i++;
        if ((i + 1) % 5 == 0) {
            i++;
        }
    }
}

unsigned short get_binary_op(bin_str bin) { // convert bin str to short; return value
    return strtol(bin, NULL, 2);
}

short get_operand(char mode) { // read in numeric value in mode; return value
    short val;
    printf("Enter %s value:", mode == 'b' ? "binary" : mode == 'o' ? "octal" : mode == 'h' ? "hex" : "decimal");
    switch (mode) {
        case 'b':
            scanf("%s", binary_str);
            val = get_binary_op(binary_str);
            break;
        case 'o':
            scanf("%ho", &val);
            break;
        case 'h':
            scanf("%hx", &val);
            break;
        default:
            scanf("%hd", &val);
    }
    return val;
}

void add(short *val, char mode) { // call get_operand to get value in mode
    short operand = get_operand(mode);
    short sum = *val + operand;

    // Check for overflow (positive or negative)
    if (operand > 0 && *val > SHRT_MAX - operand) {
        printf("Positive Overflow\n");
    } else if (operand < 0 && *val < SHRT_MAX - operand) {
        printf("Negative Overflow\n");
    }
    *val = sum;
}

void subtract(short *val, char mode) { // similar to add, but subtract
    short operand = get_operand(mode);
    short difference = *val - operand;

    // Check for overflow (negative)
    if ((operand < 0 && *val > SHRT_MAX + operand) || (operand > 0 && *val < SHRT_MAX + operand)) {
        printf("Negative Overflow\n");
    }
    *val = difference;
}

void print_bases(short val, char mode) { // print out base values and str for mode

    // Use convert_to_binary to get the binary string
    convert_to_binary(val, binary_str);

    printf("\n****************************************\n");
    printf("* Base Values:         Input Mode: %s *\n",
           mode == 'b' ? "Bin" : mode == 'o' ? "Oct" : mode == 'h' ? "Hex" : "Dec");
    printf("*   Binary  :  %19s     *\n", binary_str);
    printf("*   Hex     :  %04hX                    *\n", (unsigned short) val); //capital X to capitalize letters
    printf("*   Octal   :  %06ho                  *\n", (unsigned short) val);
    printf("*   Decimal :  %-10hd              *\n", val);
    printf("****************************************\n\n");
}

char print_menu(void) { // print menu; get option until valid; return option
    char option[10];
    while (1) {
        printf("Please select one of the following options:\n\n");
        printf("B  Binary Mode            &  AND with Value           +  Add to Value\n");
        printf("O  Octal Mode             |  OR  with Value           -  Subtract from Value\n");
        printf("H  Hexadecimal Mode       ^  XOR with Value           N  Negate Value\n");
        printf("D  Decimal Mode           ~  Complement Value\n\n");
        printf("C  Clear Value            <  Shift Value Left\n");
        printf("S  Set Value              >  Shift Value Right\n\n");
        printf("Q  Quit\n\n");
        printf("Option:");
        scanf("%16s", option); // Read up to 16 characters

        if (strlen(option) == 1 && strchr("<>&|^-+N~BOHDCSQ", toupper(option[0]))) {
            return toupper(option[0]); // Return uppercase option
        } else {
            printf("Invalid option: %s \n", option);
        }
        scanf("%*[^\n]"); // Clear remaining input
    }
}

int main(void) { // main menu loop; execute option or call
    short value = 0;
    char mode = 'd';
    int shift_value;
    short value_type;

    while (1) {
        print_bases(value, mode);
        char option = print_menu();

        switch (option) {
            case '<':
                printf("Enter number of positions to left shift value: ");
                scanf("%d", &shift_value);
                printf("%d\n", shift_value);
                value <<= shift_value;
                break;
            case '>':
                printf("Enter number of positions to right shift value: ");
                scanf("%d", &shift_value);
                printf("%d\n", shift_value);
                value >>= shift_value;
                break;
            case '&':
                value_type = get_operand(mode);
                value &= value_type;
                break;
            case '|': // or operator
                value_type = get_operand(mode);
                value |= value_type;
                break;
                break;
            case '^': // xor operator
                value_type = get_operand(mode);
                value ^= value_type;
                break;
            case '-': // subtraction, calls on subtract() function
                subtract(&value, mode);
                break;
            case '+': // addition, calls on add() function
                add(&value, mode);
                break;
            case 'N':
                value *= -1;
                break;
            case '~':
                value = ~value;
                break;
            case 'B':
                mode = 'b';
                printf("Mode: Binary\n");
                break;
            case 'O':
                mode = 'o';
                printf("Mode: Octal\n");
                break;
            case 'H':
                mode = 'h';
                printf("Mode: Hexadecimal\n");
                break;
            case 'D':
                mode = 'd';
                printf("Mode: Decimal\n");
                break;
            case 'C':
                value = 0;
                break;
            case 'S':
                value = get_operand(mode);
                break;
            case 'Q':
                return 0;
        }
    }
}
