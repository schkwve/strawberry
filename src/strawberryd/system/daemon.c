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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include <config/config.h>
#include <system/daemon.h>

void daemonize(void)
{
	pid_t pid;

	// fork step one
	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Failed to daemonize (fork() returned %s)!\n", strerrorname_np((int)pid));
		exit(EXIT_FAILURE);
	}

	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	// child becomes the session leader
	int status = setsid();
	if (status < 0) {
		fprintf(stderr, "Failed to daemonize (setsid() returned %s)!\n", strerrorname_np(status));
		exit(EXIT_FAILURE);
	}

	// set up signal handlers
	signal(SIGCHLD, SIG_IGN);

	// fork step two
	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Failed to daemonize (fork() returned %s)!\n", strerrorname_np((int)pid));
		exit(EXIT_FAILURE);
	}

	if (pid > 0) {
		fprintf(stdout, "-- Started daemon with PID: %u\n", pid);
		exit(EXIT_SUCCESS);
	}

	// set up a default state
	umask(0);
	chdir("/");

	// close all file descriptors
	// redirect stdin/stdout/stderr to /dev/null
	for (int i = sysconf(_SC_OPEN_MAX); i > 3; i--) {
		close(i);
	}

	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w+", stdout);
	freopen("/dev/null", "w+", stderr);

	// save a lockfile file to /var/lock/strawberryd
	// @note: /var/lock/strawberryd MUST be owned by the user (including group)
	//        for this program to properly work.
	int lock_fd = open(lock_filepath, O_RDWR | O_CREAT, 0640);
	if (lock_fd < 0) {
		syslog(LOG_ERR, "couldn't write PID file; the server will have to be killed manually (PID: %ld)\n", (long)pid);
	} else {
		if (lockf(lock_fd, F_TLOCK, 0) < 0) {
			exit(EXIT_FAILURE);
		}

		char buf[8] = {0};
		sprintf(buf, "%ld\n", (long)getpid());
		write(lock_fd, buf, strlen(buf));
	}
}

void daemon_stop(void)
{
	// try to open PID file
	FILE *pid_fd = fopen(lock_filepath, "r");
	pid_t pid;
	if (pid_fd == NULL) {
		fprintf(stderr, "Failed to read PID from %s: %s. Check system logs and kill the server manually.\n", lock_filepath, strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		// read PID

		// 2**22 is the maximum assignable PID
		// which equals to around 4 milion (7 digits)
		// plus one for newline
		char buffer[7];

		// @note: is there a better way to do this?
		fgets(&buffer, 7, pid_fd);
		pid = atoi(buffer);
		fclose(pid_fd);
	}

	kill(pid, SIGINT);
	exit(EXIT_SUCCESS);
}
