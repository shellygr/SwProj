#include "common.h"

/**
 #define COMMAND_CODE_1 add_vertex
 #define COMMAND_CODE_2 add_edge
 #define COMMAND_CODE_3 print
 #define COMMAND_CODE_4 page_rank
 #define COMMAND_CODE_5 quit

 #define NUM_OF_PARAMS_FOR_1 1
 #define NUM_OF_PARAMS_FOR_2 2
 #define NUM_OF_PARAMS_FOR_3 0
 #define NUM_OF_PARAMS_FOR_4 0
 #define NUM_OF_PARAMS_FOR_5 0
 **/

int check_one_param(char *param) {
	if (strpbrk(param, WHITESPACES) != NULL) {
		return FALSE;
	}
	return TRUE;
}

int check_and_get_three_params(char *params, char **first_param,
		char **second_param, char **weight) {
	char *next_token = strtok(params, WHITESPACES);
	int count = 0;

	while (next_token != NULL) {
		count++;
		if (count == 1) {
			*first_param = next_token;

		} else if (count == 2) {
			*second_param = next_token;

		} else if (count == 3)
			*weight = next_token;
		next_token = strtok(NULL, WHITESPACES);

	}

	return (count == 3 ? TRUE : FALSE);
}

int check_string_is_non_negative_integer(char *str) {
	int i;

	for (i = 0; i < strlen(str); i++) {
		if (!isdigit(*(str + i)))
			return FALSE;
	}

	if (atoi(str) < 0 || strlen(str) > 10)
		return FALSE; //INT_MAX=2147483647 has 10 digits.atoi(x)<0 for x>int_max
	else if ((strncmp(str, "2147483647", 10) != 0) && (atoi(str) == 2147483647))
		return FALSE;

	return TRUE;
}

int check_string_is_double(char *str) {
	int i;
	int flag = 0;
	char *tmp;
	double weight;

	for (i = 0; i < strlen(str); i++) {
		if (!isdigit(*(str + i))) {
			if (flag == 1) {
				return FALSE;
			}
			if (*(str + i) == '.') {
				flag = 1;
			} else {
				return FALSE;
			}
		}
	}
	weight = strtod(str, &tmp);
	if (weight == HUGE_VAL || strlen(tmp) > 0  || weight > 1 || weight < 0) {
		return FALSE;
	}

	return TRUE;
}

/*
int check_not_max_num_of_vertices(network *net) {
	return (net->num_of_vertices >= MAX_NUM_OF_VERTICES ? FALSE : TRUE);
}
 */

int is_legal_id(int id, network *net) {
	return (id >= net->num_of_vertices ? FALSE : TRUE);
}

int add_vertex(char *name, network *net) {
	vertex *vrtx = init_vertex(name, net->num_of_vertices);
	if (vrtx == NULL) {
		send_perror("malloc");
		return 2;
	}

	return add_vertex_to_network(&net, vrtx);

	//	printf("%s",net->vertices[0].name);
}

int add_edge(int first_id, int second_id, double weight, network *net) {
	vertex *src_vrtx = get_vertex(first_id, net);
	vertex *dest_vrtx = get_vertex(second_id, net);
	edge *e = NULL;

	init_edge(&e, first_id, second_id, weight);

	if ((add_edge_to_vertex(&src_vrtx, &dest_vrtx, e) == 2))
		return 2;

	return 0;
}

int dispatch_add_vertex(char *params, network *net) {
	if (params[0] == '\0')
		send_error(14);
	else if (!isspace(params[0]))
		send_error(3);
	else if (check_one_param(params + 1) == FALSE) {
		send_error(14);
		//} else if (check_not_max_num_of_vertices(net) == FALSE) {
		//	send_error(6);
	} else {
		return add_vertex(params + 1, net);
	}

	return 1;
}

int dispatch_add_edge(char *params, network *net) {
	char *first_param = NULL, *second_param = NULL, *third_param;
	if (params[0] == '\0')
		send_error(24);
	else if (!isspace(params[0]))
		send_error(3);
	else if (check_and_get_three_params(params + 1, &first_param, &second_param,
			&third_param) == FALSE) {
		send_error(24);
		return 1;
	} else if ((check_string_is_non_negative_integer(first_param) == FALSE)
			|| (check_string_is_non_negative_integer(second_param) == FALSE)) {
		send_error(5);
		return 1;
	} else if (!check_string_is_double(third_param)) {
		send_error(-4);
		return 1;
	} else {
		int first_id = atoi(first_param);
		int second_id = atoi(second_param);
		double weight = strtod(third_param, NULL);
		if ((is_legal_id(first_id, net) == FALSE)
				|| (is_legal_id(second_id, net) == FALSE)) {
			send_error(7);
			return 1;
		} else if (first_id == second_id) {
			send_error(8);
			return 1;
		} else {
			return add_edge(first_id, second_id, weight, net);
		}
	}

	return 1;
}

int dispatch_command(int code, char *params, network *net) {
	switch (code) {
	case 1:
		return dispatch_add_vertex(params, net);
	case 2:
		return dispatch_add_edge(params, net);
		/*case 3:
		 dispatch_print(params,net); return 0;
		 case 4:
		 dispatch_pr(params,net, TRUE); return 0;
		 case 6:
		 dispatch_pr(params,net, FALSE); return 0;
		 */
	}

	return 1;
}

