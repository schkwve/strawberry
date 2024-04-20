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
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>

#include <config.h>
#include <config/config.h>
#include <system/daemon.h>

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

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key) {
		case 'c': // config
			config_set_config_path(arg);
			break;
		case 30: // set maxmem
			int limit = atoi(arg);
			config_set_mem_limit(limit);
			break;
		case 1: // stop
			// @todo: find and stop a daemon
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
	const char *pid_filepath = "/var/run/strawberryd/strawberryd.pid";
	const char *lock_filepath = "/var/lock/strawberryd/strawberryd.lock";

	// set up available arguments
	struct argp_option options[] = {
		{"config", 'c', 0, 0, "Use a custom config file"},
		{"max-memory", 30, 0, 0, "Set a maximum memory limit (Default: 4096 MB)"},
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

	// try to obtain a lock
	int lockfile = open(lock_filepath, O_RDWR | O_CREAT, 0666);
	if (lockfile == -1) {
		fprintf(stderr, "Failed to open lock file %s: %s\n", lock_filepath, strerror(errno));
		exit(EXIT_FAILURE);
	}

	int lock_result = flock(lockfile, LOCK_EX | LOCK_NB);
	if (lock_result == -1) {
		fprintf(stderr, "Failed to obtain lock file %s: %s\n", lock_filepath, strerror(errno));
		exit(EXIT_FAILURE);
	}

	daemonize();

	// @todo:
	//atexit(cleanup);

	// save a .pid file to /var/run/strawberryd
	// @note: /var/run/strawberryd MUST be owned by the user (including group)
	//        for this program to properly work.
	pid_t pid = getpid();
	FILE *pid_fd = fopen(pid_filepath, "w");
	if (pid_fd == NULL) {
		syslog(LOG_ERR, "couldn't write PID file; the server will have to be killed manually (PID: %ld)\n", (long)pid);
	} else {
		fprintf(pid_fd, "%ld\xa", (long)pid);
		fclose(pid_fd);
	}

	// @todo: implement the server itself
	while (1);

	// cleanup
	closelog();
	if (unlink(pid_filepath) != 0) {
		fprintf(stderr, "couldn't remove PID file\n");
	}

	return EXIT_SUCCESS;
}