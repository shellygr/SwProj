#include "common.h"


char* get_command(int *exit_status) {
	char *ret = NULL, *command = NULL;
	int count = 0,i=0;
	int first_non_whitespace = -1;

	if ( (command = (char*)malloc(MAX_COMMAND_LENGTH)) == NULL) {		
		send_perror("malloc");
		*exit_status=2;
		return NULL;
	}

	while (count < MAX_COMMAND_LENGTH) {
		*(command+i) = getchar();
		if(*(command+i)==EOF){
			send_perror("getchar");
			free(command);
			*exit_status=2;
			return NULL;
		}
		
		if ((first_non_whitespace == -1) && (!isspace(*(command+i)))) {
			first_non_whitespace = count;
		}

		if (*(command+i) == '\n') {
			if(i>0&&isspace(*(command+i-1)))
				*(command+i-1) = '\0';
			else
				*(command+i) = '\0';
			break;
		}

		
		if(i==0||!isspace(*(command+i))||!isspace(*(command+i-1)))
			i++;
		
		count++;
	}	


	if (first_non_whitespace == -1) {
		send_error(1);
		free(command);
		return NULL;
	}

	if (count >= MAX_COMMAND_LENGTH) {
		*(command+count-1) = '\0';
		free(command);
		while(getchar()!='\n');
		send_error(2);
		return NULL;
	}

	// Build ret
	if ((ret = malloc(strlen(command)+1-first_non_whitespace)) == NULL) {
		free(command);
		*exit_status=2;
		send_perror("malloc");
		return NULL;
	}
	
	for (count = first_non_whitespace; count < strlen(command); ++count) {
		*(ret+count-first_non_whitespace) = *(command+count);
	}
	*(ret+count-first_non_whitespace) = '\0';
	
	
	free(command);
	return ret;
}

int check_all_whitespace(char *str) {
	int i;
	
	for (i = 0 ; i < strlen(str) ; i++) {
		if (!isspace(*(str+i)))
			return FALSE;
	}
	return TRUE;
}

/*
	Returns the command code defined in commands.c
	0 if error.
 */
int get_action(char *command) {
	int ret = 0;
	
	if (strstr(command, "add_vertex") == command) {
		ret = 1;
	} else if (strstr(command, "add_edge") == command) {
		ret = 2;
	} else if (strncmp(command, "print", 5) == 0) {
		ret = 3;
	} else if (strncmp(command, "page_rank", 9) == 0) {
		ret = 4;
	} else if (strncmp(command, "quit", 4) == 0) {
		ret = 5;
	}

	if (ret == 0)
		send_error(3);
	return ret;
}

char *get_params(char *command, int code) {
	switch (code) {
		case 1: return command+10;
		case 2: return command+8;
		case 3: return command+5;			
		case 4: return command+9;
		case 5: return command+4;
		default: return NULL;
	}
}
