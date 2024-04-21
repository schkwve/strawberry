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
	// @todo: move these outside of this function
	//        and create signal handlers
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

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
}

void daemon_stop(void)
{
	// see if a lock is available
	FILE *lockfile = fopen(lock_filepath, "r");
	if (lockfile == NULL) {
		fprintf(stderr, "Failed to open lock file %s: %s\n", lock_filepath, strerror(errno));
		exit(EXIT_FAILURE);
	}

	int lock_result = flock(fileno(lockfile), LOCK_EX | LOCK_NB);
	if (lock_result == 0) {
		// the lock is not locked, throw an error and exit
		fprintf(stderr, "Lock %s is not locked: %s. Is the server still running?\n", lock_filepath, strerror(errno));
		fclose(lockfile);
		exit(EXIT_FAILURE);
	}

	// try to open PID file
	FILE *pid_fd = fopen(pid_filepath, "r");
	pid_t pid;
	if (pid_fd == NULL) {
		fprintf(stderr, "Failed to read PID %s: %s. Check system logs and kill the server manually.\n", pid_filepath, strerror(errno));
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

	kill(pid, SIGTERM);
	exit(EXIT_SUCCESS);
}
