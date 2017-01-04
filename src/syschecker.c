
# Matthieu PERRIN - License : CC-BY-SA 3.0
# THIS IS DISTRIBUTED "AS IS" WITHOUT ANY WARRANTY.

#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <signal.h>
#include<string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include "syschecker_utils.h"

static void skeleton_daemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/tmp");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    {
        close (x);
    }

    /* Open the log file */
    openlog ("syschecker", LOG_PID, LOG_DAEMON);
}

void init_var()
{

	strcpy(config, "syschecker.cfg");
	strcpy(cmd, "syschecker.cmd");

	strcpy(ip_poller, "POLLER");
	strcpy(hostname, "hostname");
	strcpy(conduite_host, "conduite");
	strcpy(path_libs, "/usr/local/nagios/libexec");
	strcpy(bin_nsca, "/usr/local/nagios/bin/send_nsca");
	strcpy(nsca_cfg, "usr/local/nagios/etc/send_nsca.cfg");

	minute=1;
	test_mode=0;
	debug_mode=0;

}

int main(int argc, char** argv)
{
	init_var();
	
	if ( argc > 1 )
	opt(argc, argv);

	get_config();
		
	if ( ( test_mode ) || (debug_mode) )
	{
		if (debug_mode)
		minute = min_now();
		process_command();
	}
	else
	{
	
		skeleton_daemon();

		syslog (LOG_NOTICE, "syschecker daemon started.");
			
		while (1)
		{
		
			minute = min_now();

			process_command();
			
			sleep(60);
			
			//break;
		}

		syslog (LOG_NOTICE, "syschecker daemon terminated.");
		closelog();

		return EXIT_SUCCESS;
	}
	
    exit(0);
}
