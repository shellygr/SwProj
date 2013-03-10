#include "common.h"
#define ERROR_CODE_m3 "Error: Should have 3 arguments: input folder, output folder, constant C"
#define ERROR_CODE_m2 "Error: Folder name must end with a '/'
#define ERROR_CODE_m1 "Error: input folder must contain a file named 'network'"
#define ERROR_CODE_0 "Error: Constant c must be 0 <= c <= 1"
#define ERROR_CODE_1 "Error: command must have at least one character\n"
#define ERROR_CODE_2 "Error: command length must be less than or equal to 500 characters\n"
#define ERROR_CODE_3 "Error: command not found\n"
#define ERROR_CODE_4 "Error: command %s must have %d  parameters\n"
#define ERROR_CODE_5 "Error: ID parameter is not a non-negative integer\n"
#define ERROR_CODE_6 "Error: cannot add vertex - too many vertices\n"
#define ERROR_CODE_7 "Error: ID number is not in the system\n"
#define ERROR_CODE_8 "Error: edge must be between two different vertices\n"
#define ERROR_CODE_9 "Error: edge is in the graph\n"

#define NUM_OF_PARAMS_FOR_1 1
#define NUM_OF_PARAMS_FOR_2 3
#define NUM_OF_PARAMS_FOR_3 0
#define NUM_OF_PARAMS_FOR_4 0
#define NUM_OF_PARAMS_FOR_5 0

void send_perror(char *func) {
	char error_text[80];
	sprintf(error_text, "Error: standard function %s has failed\n", func);
	perror(error_text);
}

void send_error(int code) {
	char *error_message = NULL;
	if (code <= 0) {
		switch (code) {
			case -1: error_message = ERROR_CODE_m1;
			case -2: error_message = ERROR_CODE_m2;
			case -3: error_message = ERROR_CODE_m3;
			case 0: error_message = ERROR_CODE_0;
		}
		printf("%s", error_message);
	} else {
		switch(code % 10) {
			case 1: error_message = ERROR_CODE_1; break;
			case 2: error_message = ERROR_CODE_2; break;
			case 3: error_message = ERROR_CODE_3; break;
			case 4: error_message = ERROR_CODE_4; break;
			case 5: error_message = ERROR_CODE_5; break;
			case 6: error_message = ERROR_CODE_6; break;
			case 7: error_message = ERROR_CODE_7; break;
			case 8: error_message = ERROR_CODE_8; break;
			case 9: error_message = ERROR_CODE_9; break;
		}
		if (code % 10 != 4) {
			printf("%s", error_message);
		} else { // code-4/10 = command code
			char command_name[11];
			int num_of_params;
			switch ((code-4)/10) {
				case 1: strncpy(command_name, "add_vertex", 11); num_of_params = NUM_OF_PARAMS_FOR_1; break;
				case 2: strncpy(command_name, "add_edge", 9); num_of_params = NUM_OF_PARAMS_FOR_2; break;
				case 3: strncpy(command_name, "print", 6); num_of_params = NUM_OF_PARAMS_FOR_3; break;
				case 4: strncpy(command_name, "page_rank", 10); num_of_params = NUM_OF_PARAMS_FOR_4; break;
				case 5: strncpy(command_name, "quit", 5); num_of_params = NUM_OF_PARAMS_FOR_5; break;
			}
			printf(ERROR_CODE_4, command_name, num_of_params);
		}
	}
}
