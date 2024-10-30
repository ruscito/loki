#include "tools/res.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stb/stb_image.h>
#include <glad/glad.h>

// Base64 encoding table
static const char base_64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";


static const int base_64_decode_table[256] = {
    ['A'] = 0,  ['B'] = 1,  ['C'] = 2,  ['D'] = 3,
    ['E'] = 4,  ['F'] = 5,  ['G'] = 6,  ['H'] = 7,
    ['I'] = 8,  ['J'] = 9,  ['K'] = 10, ['L'] = 11,
    ['M'] = 12, ['N'] = 13, ['O'] = 14, ['P'] = 15,
    ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
    ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23,
    ['Y'] = 24, ['Z'] = 25, ['a'] = 26, ['b'] = 27,
    ['c'] = 28, ['d'] = 29, ['e'] = 30, ['f'] = 31,
    ['g'] = 32, ['h'] = 33, ['i'] = 34, ['j'] = 35,
    ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39,
    ['o'] = 40, ['p'] = 41, ['q'] = 42, ['r'] = 43,
    ['s'] = 44, ['t'] = 45, ['u'] = 46, ['v'] = 47,
    ['w'] = 48, ['x'] = 49, ['y'] = 50, ['z'] = 51,
    ['0'] = 52, ['1'] = 53, ['2'] = 54, ['3'] = 55,
    ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59,
    ['8'] = 60, ['9'] = 61, ['+'] = 62, ['/'] = 63
};


void encode_base_64(const unsigned char *input, size_t length, char *output)
{
    size_t i, j;
    for (i = 0, j = 0; i < length;) {
        uint32_t octet_a = i < length ? input[i++] : 0;
        uint32_t octet_b = i < length ? input[i++] : 0;
        uint32_t octet_c = i < length ? input[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        output[j++] = base_64_chars[(triple >> 3 * 6) & 0x3F];
        output[j++] = base_64_chars[(triple >> 2 * 6) & 0x3F];
        output[j++] = (i > length + 1) ? '=' : base_64_chars[(triple >> 1 * 6) & 0x3F];
        output[j++] = (i > length) ? '=' : base_64_chars[(triple >> 0 * 6) & 0x3F];
    }
    output[j] = '\0'; // Null-terminate the string
}


size_t decode_base_64(const char *input, unsigned char *output) {
    size_t input_length = strlen(input);
    size_t output_length = 0;

    for (size_t i = 0; i < input_length;) {
        uint32_t octet_a = (i < input_length) ? base_64_decode_table[(unsigned char)input[i++]] : 0;
        uint32_t octet_b = (i < input_length) ? base_64_decode_table[(unsigned char)input[i++]] : 0;
        uint32_t octet_c = (i < input_length) ? base_64_decode_table[(unsigned char)input[i++]] : 0;
        uint32_t octet_d = (i < input_length) ? base_64_decode_table[(unsigned char)input[i++]] : 0;

        if (octet_a == 0 && (i > input_length || input[i - 1] != '=')) break;

        uint32_t triple = (octet_a << 18) + (octet_b << 12) + (octet_c << 6) + octet_d;

        if (output_length < (output_length + 3)) {
            output[output_length++] = (triple >> 16) & 0xFF;
        }
        if (octet_c != 64) {
            output[output_length++] = (triple >> 8) & 0xFF;
        }
        if (octet_d != 64) {
            output[output_length++] = triple & 0xFF;
        }
    }
    return output_length;
}

unsigned char* read_image_file(const char *filename, size_t *length)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = malloc(*length);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *length, file);
    fclose(file);
    return buffer;    
}

unsigned int write_image_file(const char *filename, unsigned char *data, size_t length) 
{
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return 0;
    }
    fwrite(data, 1, length, file);
    fclose(file);
    return 1;
}


char *encode_image_resource(const char *filename)
{
    size_t image_size;
    unsigned char *image_data = read_image_file(filename, &image_size);
    
    if (!image_data) {
        return NULL;
    }    

    size_t base_64_len = 4 * ((image_size + 2) /3) + 1;
    char *base_64_str = malloc(base_64_len);
    if ( !base_64_str ){
        return NULL;
    }

    encode_base_64(image_data, image_size, base_64_str);

    return base_64_str;
}

void destroy_resource(void *res)
{
    free(res);
}

unsigned int generate_texture(const char*file_name)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file_name, &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        return 0;
    }
    stbi_image_free(data);
    return texture;
}