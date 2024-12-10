%{
#include <stdio.h>
#include "ports.h"
#include "monitorps.h"

static char from_who[100];
extern Lists lists;
extern int verbose;
%}

/* Here are some commands that monitorps understands.  The '>' and '<'
 * are merely delimeters, here, and are not part of the protocol syntax.
 * They usually represent variable information for the field specified.
 * Other items should be taken literally.  If there is any question,
 * look at the grammer. :-)
 *
 * add neighbor <x> (x is a hostname [w/o domain?]
 * del neighbor <x>
 * add gui <x.n>   (n is a port number)
 * del gui <x.n>   (n is a port number)
 * del job <host> <time1> <time2> <tapeid> <device> <program>
 *           Where:
 *              <host>           is a hostname.
 *              <time1>, <time2> is UTC seconds from time().
 *              <tapeid>         is a string.
 *              <device>         is a device (/dev/8mm1, CD, disk).
 *              <program>        is any string that identifies a program.
 *                               Ex, level_1, 2A-54, dd, etc.
 * add product_gui <x.n>
 *
 * add psgui <x.n>  (to monitor ps on another computer).
 * del psgui <x.n>
 * reset gui <x.n> [host]
 *        Where <host> is a specific host to request a reset from.
 *
 * del database jobdb
 * del database imagedb
 *
 * status: <job#> <time> <tapeid> <device> <program> <file> <status>
 *     Where:
 *        job#     - Integer.  Sequence number.  The job number.
 *        time     - Integer.  Unix time().  Time in seconds since Jan 1, 1970.
 *        tapeid   - String. Tape volume identifier.  
 *        device   - String. disk, /dev/8mm1, etc.
 *        program  - String. Any program identifier.  Product or program.
 *        file     - String. Any file identifier.  File or filename.
 *        status   - String. One of: running, successful, failed, warning,
 *                                   finished, or aborted.
 *
 * product <prod name> <tapeid> <job#> <giffile>
 *	  Where: <prod name> is: 1C-51, 2A-52i, ..., 2A-55.
 *            <file>      is: a GIF image.
 *
 */
/*-----------------------------------------------------------------------*/
/* There are many simple rules present in this grammer.  They are
 * there for simplifying the semantics for some rules.  See the rule
 * for 'statement' which begins w/ 'STATUS NUMBER'.  In other words, 
 * the semantic meaning should be obvious, and if the tokenizer is
 * wrong, well...
 *
 * It is hoped that a grammer will make maintanence easier.  I've already
 * seen some of the benifits.  But, a small grammer change can be hard
 * to debug :-|.
 *
 * This grammer is for the recieving of messages by monitorps and does
 * not address the sender, typically a GUI, protocol at all.  Messages
 * recieved by GUI programs are handled separately.
 *
 * It did not seem feasible to use existing HTTPD software as I needed
 * a database for status messages and one for images and the lookup or
 * insertion needed to be fast.  Additionally, communication to the GUI
 * happens asyncronously and initiated by monitorps.  For example, whenever
 * a status message arrives, echo it to all neighbors and echo it to all
 * GUI's.
 *
 * GUI COMMUNICATION
 * -----------------
 * The first thing that should happen when a GUI is ready to communicate
 * with monitorps is that the GUI should send the message:
 *       add gui HOST.PORT    (HOST is a IP hostname, and PORT is a TCP/IP
 *                             port number that the GUI listens to.
 * This communication happens on port 5000 as that is the port that
 * monitorps listens to.  The gui can prepend 'from_gui:' to each
 * messages it sends to monitorps, but, this is not required.
 *
 * Once a GUI connection is established, monitorps responds to the GUI
 * on what ever port (PORT) with:
 *       time: nnnnnnnn  (nnnnnnnn is a unix time of type time_t).
 * You can use the time to compute a time difference between the computer
 * with the GUI and the computer with monitorps.
 *
 * Then, the GUI should send:
 *       reset gui HOST.PORT
 * This command tells monitorps that the GUI is ready to recieve all the
 * job status messages stored in the database (/tmp/monitor.db).  This
 * could be a lot of data so be ready.  The GUI may need to buffer this;
 * 'gms', a gui, did.
 *
 * After that, any status messages that monitorps recieves is automatically
 * echoed to all GUI's that have connected to it.  The status message
 * pushed to the GUI and it looks like:
 *       status: NUM TIME tapeid device product file message
 * For example:
 *       status: 10 882385257 950731 /dev/null 1C-51 ../950731.010 successful
 *
 *
 * NEIGHBOR COMMUNICATION
 * ----------------------
 * Neighbors are just other computers on the network that run monitorps.
 *
 * Everything that the GUI sends to monitorps is echoed to all
 * neighbors using port 5000.  The list of neighbors is maintained,
 * usually, in /tmp/neighbors.dat (it is hard coded or configurable
 * when building monitorps; see monitorps.h.
 *
 * Everything that a neighbor sends to monitorps is echoed to all
 * neighbors, except to neighbor that is the sender.  There may be
 * some uneccessary communication, but, it's hardly noticable.
 *
 * The GUI should be the process that informs monitorps of new or
 * deleted neighbors.  That syntax is:
 *       add neighbor HOST
 *       del neighbor HOST
 *
 * ADDITIONAL INFORMATION
 * ---------------------
 * See README for additional information.
 */

/* General tokens. */
%token GUI JOB PSGUI HOST TIME DEVICE PROGRAMID NAME_NUM
%token ADD DEL RESET
%token ALPHA NUMBER ALNUM FILENAME
%token NEIGHBOR END_OF_FILE

/* Symbolic for hard coded filenames: JOBDB = /tmp/monitor.db
 *                                  IMAGEDB = /tmp/product.db
 */
%token DATABASE JOBDB IMAGEDB
%token PRODUCT_GUI

/* Protocol prefixes. Ex, 'from_gui:', 'from_neighbor:'. */
%token STATUS PRODUCT FROM_GUI FROM_NEIGHBOR FROM_JOB

/* send_status status messages.  Usually, the last field. */
%token RUNNING SUCCESSFUL FINISHED ABORTED WARNING FAILED

%union {
  char *s;
}

%type <s> valid from_anyone validstmt statement

%%

valid : from_anyone validstmt
{
  char cmd[1000];
  if (verbose && $<s>1) printf("$1=<%s>\n", $<s>1);
  else if (verbose) printf("$1=<NULL>\n");
  if (verbose) printf("RULE --> valid: from_anyone validstmt\n");
  if (verbose && $<s>2) printf("$2=<%s>\n", $<s>2);
  else if (verbose) printf("$2=<NULL>\n");

  sprintf(cmd,"from_neighbor: %s", $<s>2);
  if ($<s>1 != NULL) {
	if (strcasecmp($<s>1, "neighbor") != 0) {
	  tell_neighbors(cmd);
	}
  } else {
	if (strncasecmp($<s>2, "status:", 7))
	  tell_neighbors(cmd); /* from_who is unknown, assume gui. */
  }
  YYACCEPT;
}

validstmt : statement '\n'
{
 if (verbose) printf("VALIDn: %s\n", $<s>1);
 sprintf($<s>$,"%s", $<s>1);
}
          | statement
{
  if (verbose) printf("VALID : %s\n", $<s>1);
  sprintf($<s>$,"%s", $<s>1);
}

statement : command NEIGHBOR host
{
  do_neighbor_cmd($<s>1, $<s>2, $<s>3);
  sprintf($<s>$,"%s %s %s", $<s>1, $<s>2, $<s>3);
}
          | command GUI hostport
{
  do_gui_cmd($<s>1,$<s>2,$<s>3);
  sprintf($<s>$,"%s %s %s", $<s>1, $<s>2, $<s>3);
}
          | STATUS NUMBER time jobid device programid string statstring
{
  sprintf($<s>$,"%s %s %s %s %s %s %s %s", $<s>1, $<s>2, $<s>3, $<s>4,
		  $<s>5, $<s>6, $<s>7, $<s>8);
  do_status_cmd($<s>$);
}
          | command PSGUI hostport
{
  do_psgui_cmd($<s>1, $<s>2, $<s>3);
  sprintf($<s>$,"%s %s %s", $<s>1, $<s>2, $<s>3);
}
          | command PRODUCT_GUI hostport
{
  do_product_gui_cmd($<s>1, $<s>2, $<s>3);
  sprintf($<s>$,"%s %s %s", $<s>1, $<s>2, $<s>3);
}

          | RESET GUI hostport host
{
  do_reset_cmd($<s>1, $<s>2, $<s>3, $<s>4);
  sprintf($<s>$,"%s %s %s %s", $<s>1, $<s>2, $<s>3, $<s>4);
}

          | RESET GUI hostport
{
  do_reset_cmd($<s>1, $<s>2, $<s>3, NULL);
  sprintf($<s>$,"%s %s %s", $<s>1, $<s>2, $<s>3);
}

          | DEL JOB host time time jobid device programid
{
  do_job_cmd($<s>1, $<s>2, $<s>3, $<s>4, $<s>5, $<s>6, $<s>7, $<s>8);
  sprintf($<s>$,"%s %s %s %s %s %s %s %s", $<s>1, $<s>2, $<s>3, $<s>4,
		  $<s>5, $<s>6, $<s>7, $<s>8);
}  
          ;

statement : DEL DATABASE JOBDB
{
  del_file(JOB_STATUS_FILE);
  sprintf($<s>$,"%s %s %s", $<s>1, $<s>2, $<s>3);
}

          | DEL DATABASE IMAGEDB
{
  del_file(PRODUCT_DB_FILE);
  sprintf($<s>$,"%s %s %s", $<s>1, $<s>2, $<s>3);
}


statement : error '\n' {yyerrok; YYACCEPT;}
;

from_anyone :
  FROM_NEIGHBOR
{
  tell_parser_from_who("neighbor");
  sprintf($<s>$,"neighbor");
}
| FROM_GUI
{
  tell_parser_from_who("gui");
  sprintf($<s>$,"gui");
}
| /* Empty */
{
  tell_parser_from_who("unknown");
  $<s>$ = NULL;
}
            ;

string : filename
       | host

filename : filename '.' string
{
  sprintf($<s>$,"%s.%s", $<s>1, $<s>3);
  if (verbose) printf("___filename__ = <%s>\n", $<s>$);
}
         | FILENAME
{
  if (verbose) printf("__filename2__ = <%s>\n", $<s>$);
}

statstring: RUNNING
          | SUCCESSFUL
          | FINISHED
          | ABORTED
          | WARNING
          | FAILED

command : ADD {if(verbose)printf("___ADD___ = <%s>\n", $<s>$);}
        | DEL {if(verbose)printf("___DEL___ = <%s>\n", $<s>$);}

hostport : host '.' port
{
  sprintf($<s>$,"%s.%s", $<s>1, $<s>3);
  if (verbose) printf("__hostport__ = <%s>\n", $<s>$);
}

device : string
       | DEVICE

host : ALNUM
     | NUMBER
     | ALPHA


port : NUMBER

time : NUMBER

jobid : string

programid : string

;

%%

extern FILE *yyin;

yyerror(char *s)
{
  if (verbose) fprintf(stderr, "Monitorps ERROR: %s on token <%s>\n", s, yylval);
}

void tell_parser_from_who(char *who)
{
  strcpy(from_who, who);
  if (verbose) printf("---<CONNECTION FROM %s>---\n", from_who);
}

/*
 * IMPORTANT: The data structure Lists is global and shared
 *            by the parent and child processes.  But, only
 *            the parent can set it.  Therefore, do not modify
 *            the Lists structures in child processes because
 *            the change will be lost when returning to the parent.
 *            This explains why the fork() is called from within 
 *            each routine -- at that point the Lists are already
 *            set.
 */


/******************************************************************/
/*                                                                */
/*                        do_gui_cmd                              */
/*                                                                */
/******************************************************************/

void do_gui_cmd(char *cmd, char *type, char *guiport)
{
  if (verbose) printf("do_gui_cmd: <%s> <%s> <%s>\n", cmd, type, guiport);
  if (strcasecmp(cmd, "add") == 0) {
	if (verbose) printf("ADD: %s %s %s\n", cmd, type, guiport);
	if (add_member(guiport, lists.gui, &lists.n_guis) == 0) return;
	if (fork() == 0) {
	  report_time_to_gui(guiport);
	  exit(0);
	}


  } else if (strcasecmp(cmd, "del") == 0) {
	if (verbose) printf("DEL: %s %s %s\n", cmd, type, guiport);
	del_member(guiport, lists.gui, &lists.n_guis);
	del_member(guiport, lists.ps_gui, &lists.n_ps_guis);
  }
}	

/******************************************************************/
/*                                                                */
/*                       do_neighbor_cmd                          */
/*                                                                */
/******************************************************************/

void do_neighbor_cmd(char *cmd, char *neighbor, char *host)
{
  if (strcasecmp(cmd, "add") == 0) {
	if (verbose)printf("NEIGHBOR+ADD: %s %s %s\n", cmd, neighbor, host);
	if (add_member(host, lists.neighbor, &lists.n_neighbors) == 0) return;
	if (fork() == 0) {
	  /* --- CHILD --- */
	  write_new_neighbor_file(lists.neighbor, lists.n_neighbors, NEIGHBOR_FILE);
	  if (strcasecmp(from_who, "neighbor") != 0) { /* Special when not a neighbor. */
		/* Kinda assuming that this cmd came from a GUI.
         * Formally, the protocol would like the 'from_gui:' start string
         * in the command, but, it is not required -- from_who is 'unknown'.
		 */
		if (verbose) printf("NEIGHBOR+ADD: from_who = <%s>\n", from_who);
		tell_neighbor_the_guis(host);
		tell_neighbor_the_neighbors(host);
	  }
	  exit(0);
	} /* --- End CHILD --- */

  } else if (strcasecmp(cmd, "del") == 0) {
	if (verbose) printf("NEIGHBOR+DEL: %s %s %s\n", cmd, neighbor, host);
	del_member(host, lists.neighbor, &lists.n_neighbors);
	if (fork() == 0) {
	  write_new_neighbor_file(lists.neighbor, lists.n_neighbors, NEIGHBOR_FILE);
	  exit(0);
	}

  }

}


/******************************************************************/
/*                                                                */
/*                       do_psgui_cmd                             */
/*                                                                */
/******************************************************************/

void do_psgui_cmd(char *cmd, char *type, char *host)
{
  add_member(host, lists.ps_gui, &lists.n_ps_guis);
}

/******************************************************************/
/*                                                                */
/*                     do_product_gui_cmd                         */
/*                                                                */
/******************************************************************/

void do_product_gui_cmd(char *cmd, char *type, char *host)
{
  if (strcasecmp(cmd, "add") == 0) {
	if (add_member(host, lists.product_gui, &lists.n_product_guis) == 0) return;
	if (fork() == 0) {
	  /* Now send all 'LAST' products. */
	  /* INEFFICIENT --- I should only send it to one gui, the one added. */
	  send_image_to_gui("LAST 1B-51", lists.product_gui, lists.n_product_guis);
	  send_image_to_gui("LAST 1C-51", lists.product_gui, lists.n_product_guis);
	  send_image_to_gui("LAST 2A-53", lists.product_gui, lists.n_product_guis);
	  send_image_to_gui("LAST 2A-54", lists.product_gui, lists.n_product_guis);
	  exit(0);
	}
  } else if (strcasecmp(cmd, "del") == 0) {
	del_member(host, lists.product_gui, &lists.n_product_guis);
  }

}

/******************************************************************/
/*                                                                */
/*                        do_job_cmd                              */
/*                                                                */
/******************************************************************/

void do_job_cmd(char *cmd, char *job, char *host,
				char *time1, char *time2,
				char *jobid, char *dev, char *progid)

{
  char myhostname[STRLEN];
  char jobspec[500];

  if (strcasecmp(cmd, "del") != 0) return; /* Invalid command for JOB */

  sprintf(jobspec, "%s %s %s %s %s %s",
		  host, time1, time2, jobid, dev, progid);
  if (fork() == 0) {
	gethostname(myhostname, sizeof(myhostname));
	if (strcasecmp(myhostname, host)) exit(0);
	del_job(jobspec, JOB_STATUS_FILE);
	
	sprintf(jobspec, "%s %s %s %s %s %s %s %s",
			cmd, job, host, time1, time2, jobid, dev, progid);
	tell_guis_a_command(jobspec);
	exit(0);
  }
}
	

/******************************************************************/
/*                                                                */
/*                        do_reset_cmd                            */
/*                                                                */
/******************************************************************/

void do_reset_cmd(char *cmd, char *type, char *hostport, char *host)
{
  char myhostname[STRLEN];
  int do_reset;

  if (verbose)printf("do_reset_cmd: <%s> <%s> <%s> <%s>\n", cmd, type, hostport, host);
/* NON-NULL 4-th argument means, only answer the
 * reset request if this hostname matches it.
 * Otherwise, everyone can answer it.
 */
  if (fork() == 0) {
	gethostname(myhostname, sizeof(myhostname));
	if (verbose) printf("myhostname = <<%s>>\n", myhostname);
	if (host == NULL) do_reset = 1;
	else if (strstr(myhostname, host)) do_reset = 1;
	else do_reset = 0;
	
	if (do_reset) {
	  if (verbose) printf("host == hostname. <<%s>>\n", host);
	  report_job_status_to_gui(hostport, JOB_STATUS_FILE);
	} else {
	  if (verbose) printf("Ignoring: host != hostname. <<%s>>\n", host);
	}
	exit(0);
  }
}


/******************************************************************/
/*                                                                */
/*                          del_file                              */
/*                                                                */
/******************************************************************/

void del_file(char *file)
{
  char cmd[100];
  sprintf(cmd, "rm %s", file);
  system(cmd);
}

/******************************************************************/
/*                                                                */
/*                        do_status_cmd                           */
/*                                                                */
/******************************************************************/

void do_status_cmd(char *cmd)
{
  int j;
  if (verbose) printf("STATUS_STR: <%s>\n", cmd);

  if (verbose) printf("log_job_status(%s);\n", cmd);
  log_job_status(cmd);
  
  /* Tell all the GUIs */
  for (j=0; j < lists.n_guis; j++) {
	if (verbose) printf("Sending to %s: %s\n", lists.gui[j], cmd);
	send_stat(cmd, strlen(cmd), (char *)lists.gui[j]);
  }


}
