#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

#include <config/config.h>

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key) {
		case 'c': // config
			// @todo: set config file path
			break;
		case 30: // set maxmem
			int limit = atoi(arg);
			config_set_mem_limit(limit);
			break;
		case 1: // stop
			// @todo: find and stop a daemon
			break;
		default:
			break;
	}

	return 0;
}

int main(int argc, char **argv)
{
	// set up available arguments
	struct argp_option options[] = {
		{"config", 'c', 0, 0, "Use a custom config file"},
		{"max-memory", 30, 0, 0, "Sets a maximum memory limit (Default: 4096 MB)"},
		{"stop", 1, 0, 0, "Stops the Strawberry daemon"},
		{0}
	};

	struct argp argp = {options, parse_opt};
	error_t argpase_error = argp_parse(&argp, argc, argv, 0, 0, 0);
	if (argpase_error != 0) {
		// @todo: log
		return -1;
	}

	return 0;
}