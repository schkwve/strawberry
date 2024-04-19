#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

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

	// close all open file descriptors
	for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
		close(x);
	}

	openlog("strawberryd", LOG_PID, LOG_DAEMON);
	syslog(LOG_INFO, "strawberryd has started");
}
