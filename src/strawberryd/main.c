/* SPDX-License-Identifier: GPL-3.0-only or GPL-3.0-or-later */
/*
 * Strawberry is a configurable SMTP server meant to be small, fast
 * and reliable.
 *
 * Copyright (C) 2024 Jozef Nagy <schkwve@gmail.com>.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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