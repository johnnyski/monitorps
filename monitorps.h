#define MAX_NEIGHBORS 100
#define MAX_GUIS      100
#define STRLEN 500
typedef struct {
  char neighbor[MAX_NEIGHBORS][STRLEN];
  char gui[MAX_GUIS][STRLEN]; /* The gui string (a hostname). */
  char ps_gui[MAX_GUIS][STRLEN]; /* The gui for recieving PS information. */
  char product_gui[MAX_GUIS][STRLEN]; /* The gui for recieving PS information. */
  int n_neighbors;
  int n_guis;
  int n_ps_guis;
  int n_product_guis;
} Lists;

#define JOB_STATUS_FILE "/tmp/monitor.db"
#define PRODUCT_DB_FILE "/tmp/product.db"
#define NEIGHBOR_FILE "/tmp/neighbors.dat"

/* Routines in monitorps.c */
void del_job(char *entry, char *file);
void log_job_status(char *cmd);
void send_stat(char *prog_stat, int nlen, char *gui);
void process_command(char *cmd);
void tell_neighbors(char *cmd);

/* Routines in protocol.y */
void tell_parser_from_who(char *from_who);

void do_gui_cmd(char *cmd, char *type, char *guiport);
void do_neighbor_cmd(char *cmd, char *neighbor, char *host);
void do_psgui_cmd(char *cmd, char *type, char *host);
void do_product_gui_cmd(char *cmd, char *type, char *host);
void do_job_cmd(char *cmd, char *job, char *host,
				char *time1, char *time2,
				char *jobid, char *dev, char *progid);

void do_reset_cmd(char *cmd, char *type, char *hostport, char *host);
void del_file(char *file);
void do_status_cmd(char *status_str);

