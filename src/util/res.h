#ifndef _RES_H_
#define _RES_H_

#define STB_IMAGE_IMPLEMENTATION
#include <stddef.h>


// File encoding/decoding
unsigned char* read_image_file(const char *filename, size_t *length); 
unsigned int write_image_file(const char *filename, unsigned char *data, size_t length);
void encode_base_64(const unsigned char *input, size_t length, char *output);
size_t decodeBase64(const char *input, unsigned char *output) ;
char *encode_image_resource(const char *filename);
void destroy_resource(void *res);

// Texture generation
unsigned int generate_texture(const char*file_name);


#endif // _RES_H_