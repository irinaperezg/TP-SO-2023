#include <serializacion/buffer.h>

// Funciones publicas
t_buffer *buffer_create(void) 
{
    // Creo y seteo las variables del buffer
    t_buffer *self = malloc(sizeof(*self));
    self->size = 0;
    self->stream = NULL;
    
    return self;
}

t_buffer *buffer_create_copy(t_buffer *bufferToCopy) 
{
    // Creo el nuevo buffer y le copio el contenido del bufferToCopy
    t_buffer *self = malloc(sizeof(*self));
    self->size = bufferToCopy->size;
    self->stream = malloc(self->size);   
    memcpy(self->stream, bufferToCopy->stream, self->size);
    
    return self;
}

void buffer_destroy (t_buffer *self) 
{
    free(self->stream);
    free(self);

    return;
}

void buffer_pack(t_buffer *self, void *streamToAdd, int size) 
{
    // Reservo memoria para alojar el nuevo stream, copio  y actualizo la informacion necesaria
    self->stream = realloc (self->stream, self->size + size);
    memcpy (self->stream + self->size, streamToAdd, size);
    self->size += size;

    return;
}

t_buffer *buffer_unpack(t_buffer *self, void *dest, int size) 
{
    // Chequeo que me hayan pasado un buffer correctamente
    if (self->stream == NULL || self->size == 0) {
        puts("\e[0;31mbuffer_unpack: Error en el desempaquetado del buffer\e[0m");
        exit(-1);
    }
    
    // Desempaqueto la informacion y actualizo el tamano del buffer
    memcpy(dest, self->stream, size);
    self->size -= size;

    // Copio la proxima informacion a desempaquetar al puntero stream
    // Y reduzco la nueva memoria reservada al nuevo tamanio
    memmove(self->stream, self->stream + size, self->size);
    self->stream = realloc(self->stream, self->size);

    return self;
}

void buffer_pack_string(t_buffer *self, char *stringToAdd) 
{
    uint32_t length = strlen(stringToAdd) + 1;
    
    // Empaqueto el tamanio del string
    buffer_pack(self, &length, sizeof(length));
    self->stream = realloc(self->stream, self->size + length);
    // Empaqueto el string
    memcpy(self->stream + self->size, stringToAdd, length);
    self->size += length;

    return;
}

char *buffer_unpack_string(t_buffer *self) 
{
    char *str;
    uint32_t length;
    
    // Desempaqueto el tamanio del string y luego el string
    buffer_unpack(self, &length, sizeof(length));
    str = malloc(length);
    buffer_unpack(self, str, length);
    
    return str;
}