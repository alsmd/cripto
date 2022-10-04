
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char HEXA_CHARS[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

uint8_t* datahex(char* string) {
    size_t slength = strlen(string);
    if((slength % 2) != 0){ // must be even
        printf("Hexadecimal invalido");
       return NULL;
    }
    size_t dlength = slength / 2;
    uint8_t* data = (uint8_t*) calloc(sizeof(char), dlength);
    size_t index = 0;
    while (index < slength) {
        char c = string[index];
        int value = 0;
        if(c >= '0' && c <= '9')
          value = (c - '0');
        else if (c >= 'A' && c <= 'F') 
          value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
          value = (10 + (c - 'a'));
        else {
          free(data);
          std::cout << "INvalid char" << "\n";
          return NULL;
        }
        data[(index/2)] += value << (((index + 1) % 2) * 4);
        index++;
    }
    return data;
}

void print_bytes(void *ptr, int len){
    for (int index = 0; index < len; index++){
        for (int bits = 7; bits >= 0; bits--){
            std::cout << (((char *)ptr)[index] >> bits & 1);
        }
        std::cout << "\n";
    }
}

uint8_t *xor_operation(uint8_t *first, uint8_t * second, int size){
    int index = 0;
    uint8_t *result = (uint8_t *) calloc(sizeof(uint8_t), size);
    uint8_t result_bit = 0;
    while (index < size){
        result_bit = 0;
        // print_bytes((void *)(first + index), 1);
        // print_bytes((void *)(second + index), 1);
        for (int bit = 7; bit >= 0; bit--){
            if (((first[index] >> bit) & 1) != ((second[index] >> bit) & 1)){
                result_bit = (1 << bit) | result_bit;
            }
        }
        // print_bytes(&result_bit, 1);
        // std::cout << "\n\n\n";
        result[index] = result_bit;
        index++;
    }
    return result;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        std::cout << "arc diferente de 3\n";
        return 1;
    }
    
    uint8_t* first_hexa = datahex(argv[1]);
    uint8_t* second_hexa = datahex(argv[2]);
    if (strlen(argv[1]) != strlen(argv[2])){
        std::cout << "hexas de diferentes tamanhos\n";
        return 1;
    }
    if (first_hexa == NULL || second_hexa == NULL){
        std::cout << "null parameters\n";
        return 1;
    }
    int size = strlen(argv[1]) / 2;
    uint8_t *result = xor_operation(first_hexa, second_hexa, size);
    for (int index = 0; index < size; index++){
        // std::cout << result[index];
        std::cout << HEXA_CHARS[result[index] / 16];
        std::cout << HEXA_CHARS[result[index] % 16];
    }
    std::cout << std::endl;
    return 0;
}
