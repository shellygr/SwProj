#include "common.h"
#include "structs.h"

char *input_dir, *output_dir, *filename;
double C; /* Ghost weight */

/* 1 on error, 0 when no error */
int main(int argc, char **argv) {
	network *net = NULL;
	FILE *input_file;
	int exit_status = 0;

	/* Assert inputs are good */
	if (check_args(argc, argv) != 0) {
		exit(EXIT_FAILURE);
	}

	/* Init network */
	if (init_network(&net) == 2) {
		send_perror("malloc");
		exit(EXIT_FAILURE);
	}

	/* Open network file */
	*input_file = fopen(filename);
	if (input_file == NULL) {
		send_perror("fopen");
		exit(EXIT_FAILURE);
	}

	/* Running on the input file and dispatching the network-building commands*/
	exit_status = build_network(net);
	if (exit_status != 0) {
		exit(EXIT_FAILURE);
	}



	destroy_net(net);

	return exit_status;
}

int check_args(int argc, char **argv) {
	/* Number of arguments*/
	if (argc != 4) {
		send_error(-3);
		return 1;
	}

	/* Folders end with a '/' */
	input_dir = argv[1];
	output_dir = argv[2];
	if ( (input_dir[strlen(input_dir)-1] != '/') || (output_dir[strlen(output_dir)-1] != '/') ) {
		send_error(-2);
		return 1;
	}

	/* Create filename */
	filename = (char *) malloc(strlen(input_dir) + strlen("network") + 1);
	if (filename == NULL) {
		send_perror("malloc");
		return 2;
	}

	/* Input folder contains 'network' */
	if (stat(filename, NULL) == -1) {
		send_error(-1);
		return 1;
	}

	/* Check 0 <= c <= 1 */
	C = atof(argv[3]); // has safer func with str something, also double and not float
	if ( (C > 1) || (C < 0) ) {
		send_error(0);
		return 1;
	}

	return 0;
}

int build_network(network *net, FILE *input_file) {
	char *command;
	int status;

	while (!feof(input_file)) {
		int action;
		char *params;

		command = get_command(&status);
		if(status == 2){
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

		if ((status = dispatch_command(action, params, net)) == 2) {
			free(command);
			destroy_net(net);
			return 1;
		}

		free(command);
	}

	if (fclose(input_file) == EOF) {
		send_perror("fclose");
		return 1; /* Non fatal */
	}

	return 0;
}
