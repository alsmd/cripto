#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char PADDING_CHAR = '=';


char base64Characters[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 
    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };
// BITS ATIVOS COMEÇANDO DO MENOS SIGNIFICATIVO PARA O MAIS SIGNIFICATIVO
const int ACTIVE_BITS_1 = 1;
const int ACTIVE_BITS_2 = 3;
const int ACTIVE_BITS_3 = 7;
const int ACTIVE_BITS_4 = 15;
const int ACTIVE_BITS_5 = 31;
const int ACTIVE_BITS_6 = 63;
const int ACTIVE_BITS_7 = 127;
const int ACTIVE_BITS_8 = 255;

void print_bytes(void *ptr, int len){
    for (int index = 0; index < len; index++){
        for (int bits = 7; bits >= 0; bits--){
            std::cout << (((char *)ptr)[index] >> bits & 1);
        }
        std::cout << "\n";
    }
}
/**
 * @brief encode a char * of 3 characters and returns a std::string base64 encoded with 4 characters
 * 
 * @param chunk 
 * @return std::string 
 */
std::string encode_chunk(const char *chunk){
    std::string encoded_chunk("0000");
    if (strlen(chunk) == 0)
        return std::string("====");
    encoded_chunk[0] = chunk[0] >> 2;
    if (strlen(chunk) < 2){
        encoded_chunk[1] = (chunk[0] << 4 & ACTIVE_BITS_6) | (0);
        encoded_chunk[2] = '=';
        encoded_chunk[3] = '=';
    }
    else{
        encoded_chunk[1] = (chunk[0] << 4 & ACTIVE_BITS_6) | (chunk[1] >> 4);
        if (strlen(chunk) < 3){
            encoded_chunk[2] = (chunk[1] << 2 & ACTIVE_BITS_6) | (0);
            encoded_chunk[3] = '=';
        }else{
            encoded_chunk[2] = (chunk[1] << 2 & ACTIVE_BITS_6) | chunk[2] >> 6;
            encoded_chunk[3] = chunk[2] & ACTIVE_BITS_6;
        }
    }
    // print_bytes((void *)encoded_chunk.c_str(), 4);
    return encoded_chunk;
}


/**
 * @brief Convert a string to base64 and returns a new string encoded
 * @param string 
 * @return base64 string encoded in base 64 
 */
std::string str_to_base64(std::string message){
    std::string encoded;
    for (int index = 0; index < message.size(); index += 3){
        encoded += encode_chunk(&(message.c_str()[index]));
    }
    return encoded;
}

uint8_t* datahex(char* string) {
    size_t slength = strlen(string);
    if((slength % 2) != 0){ // must be even
        printf("Hexadecimal invalido");
       return NULL;
    }
    size_t dlength = slength / 2;
    uint8_t* data = (uint8_t*) calloc(sizeof(char), dlength + 1);
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
          return NULL;
        }
        data[(index/2)] += value << (((index + 1) % 2) * 4);
        index++;
    }
    return data;
}

char base64Char(char character){
    return base64Characters[character];
}

int main(int argc, char *argv[]){
    if (argc < 2)
        return 1;
    std::string message;
    for (int index = 1; index < argc; index++){
        message += argv[index];
        if (index + 1 < argc)
            message += ' ';
    }
    message = (char *) datahex((char *)message.c_str());
    std::string encoded = str_to_base64(message);
    
    printf("Plain text numbers: \n");
    for (int index = 0; index < message.length(); index++){
        printf("%c", message[index]);
    }
    printf("\n\n");
    printf("Encoded text numbers: \n");
    
    for (int index = 0; index < encoded.length(); index++){
        printf("%c", base64Char(encoded[index]));
    }
    printf("\n");
    return 0;
}
