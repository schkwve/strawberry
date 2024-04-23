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

#define _GNU_SOURCE
#include <argp.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>

#include <config.h>
#include <config/config.h>
#include <system/daemon.h>

static bool should_daemonize = true;
static bool is_running = false;

static void show_version(void)
{
	fprintf(stdout, "%s\n", PACKAGE_STRING);
}

static void show_license(void)
{
	fprintf(stdout, "Copyright (c) 2024 Jozef Nagy.\n");
	fprintf(stdout, "This is free software.  You may redistribute copies of it under the terms of\n");
	fprintf(stdout, "the GNU General Public License <https://www.gnu.org/licenses/gpl.html>.\n");
	fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
}

static void signal_handler(int sig)
{
	switch (sig) {
		case SIGINT:
			// reset sigint handler
			signal(SIGINT, SIG_DFL);
			is_running = false;
			break;
		case SIGHUP:
			syslog(LOG_INFO, "reloading configuration file");
			break;
		case SIGCHLD:
			syslog(LOG_INFO, "received SIGCHLD");
			break;
		default:
			break;
	}
}

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key) {
		case 'c': // config
			config_set_config_path(arg);
			break;
		case 25: // do not daemonize
			should_daemonize = false;
			break;
		case 30: // set maxmem
			int limit = atoi(arg);
			config_set_mem_limit(limit);
			break;
		case 1: // stop
			daemon_stop();
			break;
		case 'v': // version
			show_version();
			show_license();
			exit(EXIT_SUCCESS);
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
		{"max-memory", 30, 0, 0, "Set a maximum memory limit (Default: 4096 MB)"},
		{"no-daemon", 25, 0, 0, "Do not daemonize the server"},
		{"stop", 1, 0, 0, "Stop the Strawberry daemon"},
		{"version", 'v', 0, 0, "Output version information and exit"},
		{0}
	};

	// parse commandline arguments
	struct argp argp = {options, parse_opt};
	error_t argparse_error = argp_parse(&argp, argc, argv, 0, 0, 0);
	if (argparse_error != 0) {
		fprintf(stderr, "Failed to parse commandline arguments: %s\n", strerrorname_np((int)argparse_error));
		return EXIT_FAILURE;
	}

	if (should_daemonize) {
		daemonize();
	}

	openlog("strawberryd", LOG_PID | LOG_CONS, LOG_DAEMON);
	syslog(LOG_INFO, "strawberryd has started");

	// set up signal handlers
	signal(SIGINT, signal_handler);
	signal(SIGHUP, signal_handler);

	// @todo: implement the server itself
	is_running = true;
	while (is_running) {
		sleep(10);
	}

	syslog(LOG_INFO, "strawberryd is shutting down");

	if (unlink(lock_filepath) != 0) {
		syslog(LOG_ERR, "couldn't remove lockfile; a new daemon will not run until it is removed manually\n");
	}

	closelog();

	return EXIT_SUCCESS;
}
