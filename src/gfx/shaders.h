#ifndef _SHADERS_H_
#define _SHADERS_H_



char* get_shader_error(void);    
unsigned int create_shader_program(const char *vs, const char* fs);
void destroy_shader_program(unsigned int sp);

#endif //_SHADERS_H_