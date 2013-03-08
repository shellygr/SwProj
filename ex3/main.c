#include "common.h"

int main() {
	network *net = NULL;
	char *command;
	int exit_status = 0;
	int need_to_recalc_page_rank = TRUE;
	
	if (init_network(&net) == 2) {
		send_perror("malloc");
		exit(EXIT_FAILURE);
	}


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
