#include "stdint.h"
#include "vector.h"

Vector vector_new(size_t init_size, size_t data_size) {
	Vector res = {};
	res.data_size = data_size;
	res.allocated = init_size;
	res.used = 0;
	res.data = malloc((init_size >= 1 ? init_size : 1) * data_size);
	return res;
}
void vector_ensure_allocated(Vector* vec, size_t size) {
	if(vec->allocated < size)
		vec->data = realloc(vec->data, vec->data_size * (vec->allocated * 2 >= size ? (vec->allocated *= 2) : (vec->allocated = size)));
}
void vector_clear(Vector* vec) {
  vec->used = 0;
}
void* vector_get_ptr(Vector* vec, size_t index) {
	return vec->data + index * vec->data_size;
}
void vector_push(Vector* vec, void* data) {
	vector_insert_array(vec, vec->used, data, 1);
}
void vector_unshift(Vector* vec, void* data) {
	vector_insert_array(vec, 0, data, 1);
}
void vector_insert(Vector* vec, size_t index, void* data) {
	vector_insert_array(vec, index, data, 1);
}
void vector_push_array(Vector* vec, void* data, size_t length) {
	vector_insert_array(vec, vec->used, data, length);
}
void vector_unshift_array(Vector* vec, void* data, size_t length) {
	vector_insert_array(vec, 0, data, length);
}
void vector_insert_array(Vector* vec, size_t index, void* data, size_t length) {
	if(length == 0) return;
	size_t i = index > vec->used ? vec->used : index;
	vector_ensure_allocated(vec, vec->used + length);
	if(i < vec->used) memmove(vector_get_ptr(vec, index + length), vector_get_ptr(vec, i), (vec->used - i) * vec->data_size);
	vec->used += length;
	memcpy(vector_get_ptr(vec, index), data, length * vec->data_size);
}
void vector_pop(Vector* vec, void* data) {
	vector_remove_many(vec, vec->used - 1, 1, data);
}
void vector_shift(Vector* vec, void* data) {
	vector_remove_many(vec, 0, 1, data);
}
void vector_remove(Vector* vec, size_t index, void* data) {
	vector_remove_many(vec, index, 1, data);
}
void vector_pop_many(Vector* vec, size_t count, void* data) {
	vector_remove_many(vec, vec->used - (count > vec->used ? vec->used : count), count, data);
}
void vector_shift_many(Vector* vec, size_t count, void* data) {
	vector_remove_many(vec, 0, count, data);
}
void vector_remove_many(Vector* vec, size_t index, size_t count, void* data) {
	if(count == 0) return;
	// clamp values to avoid out-of-range
	size_t i = index >= vec->used ? vec->used - 1 : index;
	size_t c = i + count > vec->used ? vec->used - i : count;
	if(data != NULL) memcpy(data, vector_get_ptr(vec, i), c * vec->data_size);
	if(vec->used - i - c != 0) memmove(vector_get_ptr(vec, i), vector_get_ptr(vec, i + c), (vec->used - i - c) * vec->data_size);
	vec->used -= c;
}
Vector vector_splice(Vector* vec, size_t index, size_t count) {
	size_t i = index >= vec->used ? vec->used - 1 : index;
	size_t c = i + count >= vec->used ? vec->used - i : count;
	Vector res = vector_new(c, vec->data_size);
	res.used = count;
	vector_remove_many(vec, i, c, res.data);
	return res;
}
void vector_shrink(Vector* vec) {
	vec->data = realloc(vec->data, (vec->allocated = vec->used) * vec->data_size);
}
void vector_replace(Vector* vec, void* data, size_t index) {
	vector_replace_many(vec, data, index, 1);
}
void vector_replace_many(Vector* vec, void* data, size_t index, size_t count) {
	size_t i = index >= vec->used ? vec->used - 1 : index;
	size_t c = i + count >= vec->used ? vec->used - i : count;
	memcpy(vector_get_ptr(vec, i), data, c * vec->data_size);
}
void vector_copy(Vector* vec, size_t index, void* data) {
	vector_copy_many(vec, index, 1, data);
}
void vector_copy_many(Vector* vec, size_t index, size_t count, void* data) {
	size_t i = index >= vec->used ? vec->used - 1 : index;
	size_t c = i + count >= vec->used ? vec->used - i : count;
	memcpy(data, vector_get_ptr(vec, i), c * vec->data_size);
}
void vector_swap(Vector* vec, size_t index, void* data) {
	vector_swap_many(vec, index, 1, data);
}
void vector_swap_many(Vector *vec, size_t index, size_t count, void* data) {
	size_t i = index >= vec->used ? vec->used - 1 : index;
	size_t c = i + count >= vec->used ? vec->used - i : count;
#ifdef VECTOR_SWAP_TMP_VALUE_IN_VECTOR
	vector_push_array(vec, data, c); // copy data to end of vec
	vector_copy_many(vec, i, c, data); // copy slice of vec to data
	vector_pop_many(vec, c, vector_get_ptr(vec, i)); // copy end of vec to slice of vec
#else
	void* tmp = malloc(c * vec->data_size);
	memcpy(tmp, data, c * vec->data_size); // copy data to tmp
	vector_copy_many(vec, i, c, data); // copy slice of vec to data
	vector_replace_many(vec, tmp, i, c); // copy tmp to slice of vec
	free(tmp);
#endif
}
void vector_concat(Vector* vec, Vector* other) {
	vector_push_array(vec, other->data, other->used);
}
Vector vector_clone(Vector* vec) {
	Vector res = vector_new(vec->allocated, vec->data_size);
	vector_concat(&res, vec);
	return res;
}
void vector_merge(Vector* vec, Vector* other) {
	vector_concat(vec, other);
	vector_free(other);
}
void vector_free(Vector* vec) {
	free(vec->data);
	vec->allocated = 0;
	vec->used = 0;
}
bool vector_has(Vector* vec, size_t index) {
	return index < vec->used;
}
Vector vector_from_array(void* data, size_t length, size_t data_size) {
	Vector res = vector_new(length, data_size);
	vector_push_array(&res, data, length);
	return res;
}
