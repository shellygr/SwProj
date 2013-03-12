#include "common.h"

/* 1 on error, 0 when no error */
int main(int argc, char **argv) {
	network *net = NULL;
	char *command;
	FILE *input_file;
	char *input_dir, *output_dir;
	double C; /* Ghost weight */
	int exit_status = 0;
	/*int need_to_recalc_page_rank = TRUE;*/
	
	/* Assert inputs are good */
	/* Num of arguments*/
	if (argc != 4) {
		send_error(-3);
		exit(EXIT_FAILURE);
	}
	
	/* Folders end with a '/' */
	input_dir = argv[1];
	output_dir = argv[2];
	if ( (input_dir[strlen(input_dir)-1] != '/') || (output_dir[strlen(output_dir)-1] != '/') ) {
		send_error(-2);
		exit(EXIT_FAILURE);
	}
	
	/* Input folder contains 'network' */
	if (stat(input_dir+"network", NULL) == -1) {
		send_error(-1);
		exit(EXIT_FAILURE);
	}
		
	/* Check 0<=c<=1 */
	c = atof(argv[3]); // has safer func with str something, also double and not float
	if ( (c > 1) || (c < 0) ) {
		send_error(0);
		exit(EXIT_FAILURE);
	}
		
	/* Init network */
	if (init_network(&net) == 2) {
		send_perror("malloc");
		exit(EXIT_FAILURE);
	}

	/* Open network file */
	*input_file = fopen(input_dir+"network");
	if (input_file == NULL) {
		send_perror("fopen");
		exit(EXIT_FAILURE);
	}
	
	/* Running on the input file and dispatching the network-building commands*/
	while (TRUE) {
		command = get_command(&exit_status);
		int action;
		char *params;
		if(exit_status==2){
			destroy_net(net);
			exit(EXIT_FAILURE);
		}

		if (command == NULL)
			continue;

		action = get_action(command);

		params = get_params(command, action);

		if (action == 0) { // No valid command
			free(command);
			continue;
		}
		else if (action == 5) { // Quit
			
			if(check_all_whitespace(params)){
				free(command);
				break;	
			}
			else if(!isspace(params[0]))
				send_error(3);
			else
				send_error(54);
			
		} else if ((action == 4) && (need_to_recalc_page_rank == FALSE)) {
			action = 6; // Special action code for printing previous page_rank results without recalculating
		}
		
		if ((exit_status = dispatch_command(action, params, net)) == 2) {
			free(command);
			destroy_net(net);
			exit(EXIT_FAILURE);
		}
		
		if (action == 4) { // Page Rank
			need_to_recalc_page_rank = FALSE;
		} else {
			need_to_recalc_page_rank = TRUE;
		}
		
		free(command);
	}
	
	
	destroy_net(net);

	return exit_status;
}
