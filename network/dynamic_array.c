#include "common.h"

int is_power_two(int x) {
	return ((x != 0) && !(x & (x - 1)));
}

dynamic_array* init_array(dynamic_array *arr) {
	*arr = (void **) malloc(sizeof(void *));
	if (arr == NULL) {
		send_perror("malloc");
	}
	return arr;
}

void* get_element(int index, dynamic_array *arr) {
	return (arr[index]);
}

int double_and_add(dynamic_array *arr, int *arr_size, void *element) {
	if (is_power_two(*arr_size)) {
		dynamic_array tmp;
		if ((tmp = realloc(*arr, 2 * (*arr_size) * sizeof(void*))) == NULL) {
			free(arr);
			send_perror("realloc");
			return 2;
		} else {
			*arr = tmp;
		}
	}

	//if (!is_power_two(*arr_size))
	(*arr)[(*arr_size)++] = element;
	printf("array size now %d\n", *arr_size);
	return 0;
}
