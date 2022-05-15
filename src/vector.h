#ifndef VECTOR_H
#define VECTOR_H
#include "stddef.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"

// if this value is 1 vector_swap will use the vector's memory for the temporary value
// this is faster as it doesn't always need to malloc to swap, but this may call a realloc
// making the vector take more memory.
#define VECTOR_SWAP_TMP_VALUE_IN_VECTOR

#define vector_of(type) (vector_new(5, sizeof(type)))

// dynamically allocated single type vector
struct Vector {
	size_t data_size;
	void* data;
	size_t allocated;
	size_t used;
};

typedef struct Vector Vector;

// makes a new vector with enough memory for INIT_SIZE elements
// of DATA_SIZE bytes each
Vector vector_new(size_t init_size, size_t data_size); 
// copy DATA's value pushing it at the end of VEC
void vector_push(Vector* vec, void* data); 
// removes the last member of VEC, writing its value to DATA, if its not NULL
void vector_pop(Vector* vec, void* data);
// removes the first member of VEC, writing its value to DATA, if its not NULL
void vector_shift(Vector* vec, void* data); 
// copy DATA's value pushing it at the begining of VEC (this moves all members to the right)
void vector_unshift(Vector* vec, void* data); 
// pushes ARRAY data of size LENGTH to the begining of vector VEC
void vector_unshift_array(Vector* vec, void* data, size_t length);
// mostly meant for private usages,
// ensure that the vector VEC has enough allocated memory for size members.
void vector_ensure_allocated(Vector* vec, size_t size); 
// shrink the vector VEC to only have just enough memory for its members
void vector_shrink(Vector* vec); 
// overwrite the value of vector VEC at index INDEX
void vector_replace(Vector* vec, void* data, size_t index); 
// replace COUNT members of VEC from INDEX by DATA
void vector_replace_many(Vector* vec, void* data, size_t index, size_t count);
// copy data from member of index INDEX of vector VEC to data
void vector_copy(Vector* vec, size_t index, void* data);
// copy data from COUNT members from INDEX  of VEC to DATA
void vector_copy_many(Vector* vec, size_t index, size_t count, void* data);
// swap member INDEX of VEC with DATA
void vector_swap(Vector* vec, size_t index, void* data);
// swap COUNTER members from INDEX of VEC with DATA
void vector_swap_many(Vector* vec, size_t index, size_t count, void* data);
// append an array (or any other data type as long as it consists of LENGTH elements of
// VEC->DATA_SIZE bytes each next to each other) to vector VEC
void vector_push_array(Vector* vec, void* data, size_t length); 
// removes the member at index INDEX of vector VEC (this moves all other members after the
// removed one once to the left)
void vector_remove(Vector* vec, size_t index, void* data); 
// returns the pointer to the member at index INDEX of vector VEC
void* vector_get_ptr(Vector* vec, size_t index); 
// push the content of vector OTHER to vector VEC (without consuming OTHER)
void vector_concat(Vector* vec, Vector* other); 
// removes COUNT elements from index INDEX of vector VEC, writing their value to DATA if its not NULL
void vector_remove_many(Vector* vec, size_t index, size_t count, void* data); 
// removes COUNT elements from index INDEX of vector VEC and returning them as a new vector
Vector vector_splice(Vector* vec, size_t index, size_t count); 
// clones the vector VEC
Vector vector_clone(Vector* vec); 
// push the content of vector OTHER to vector VEC, this consumes OTHER
void vector_merge(Vector* vec, Vector* other); 
// frees the vector VEC
void vector_free(Vector* vec); 
// returns true if the vector VEC contains a member at index INDEX
bool vector_has(Vector* vec, size_t index);
// puts member DATA at index INDEX of vector VEC (this moves every member after once to the right)
void vector_insert(Vector* vec, size_t index, void* data);
// puts members in DATA at index INDEX of vector VEC (this moves every member after once to the right)
void vector_insert_array(Vector* vec, size_t index, void* data, size_t length);
// removes COUNT members from the end of vector VEC and writes them to DATA if its not NULL
void vector_pop_many(Vector* vec, size_t count, void* data);
// removes COUNT members from the start of vector VEC and writes them to DATA if its not NULL
void vector_shift_many(Vector* vec, size_t count, void* data);
// convert vector VEC to string CHAR* (heap allocated) by formatting every element with FORMAT
char* vector_to_string(Vector* vec, char* format);
// prints string given bu vector_to_string
void vector_printf(Vector* vec, char* format);
// makes a vector out of an array
Vector vector_from_array(void* data, size_t length, size_t data_size);

#endif
