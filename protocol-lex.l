%{
#include "protocol.h"
#include <string.h>
#undef YY_INPUT
#define YY_INPUT(b, r, ms) (r = my_yyinput(b, ms))
extern int verbose;
%}
%option caseless

%%
^#.*\n ; /* Ignore comments: lines begining w/ #. */

add   {yylval.s = strdup(yytext); return ADD; }
del   {yylval.s = strdup(yytext); return DEL; }
reset {yylval.s = strdup(yytext); return RESET; }

neighbor {yylval.s = strdup(yytext); return NEIGHBOR;}
gui      {yylval.s = strdup(yytext); return GUI;}
job      {yylval.s = strdup(yytext); return JOB;}
psgui    {yylval.s = strdup(yytext); return PSGUI; }
database {yylval.s = strdup(yytext); return DATABASE; }
jobdb    {yylval.s = strdup(yytext); return JOBDB; }
imagedb  {yylval.s = strdup(yytext); return IMAGEDB; }
product_gui {yylval.s = strdup(yytext); return PRODUCT_GUI; }

product   {yylval.s = strdup(yytext); return PRODUCT; }
status:   {yylval.s = strdup(yytext); return STATUS; }
from_gui:      {yylval.s = strdup(yytext); return FROM_GUI; }
from_neighbor: {yylval.s = strdup(yytext); return FROM_NEIGHBOR; }

running {yylval.s = strdup(yytext); return RUNNING; }
successful {yylval.s = strdup(yytext); return SUCCESSFUL; }
finished {yylval.s = strdup(yytext); return FINISHED; }
aborted {yylval.s = strdup(yytext); return ABORTED; }
warning {yylval.s = strdup(yytext); return WARNING; }
failed {yylval.s = strdup(yytext); return FAILED; }

"disk" |
"/dev/8mm"[0-9]* |
"/dev/cdrom"[0-9]* { yylval.s = strdup(yytext); return DEVICE;}

[[:alpha:]]+ { yylval.s = strdup(yytext); return ALPHA;}
[[:digit:]]+ { yylval.s = strdup(yytext); return NUMBER; }
[[:alnum:]]+ { yylval.s = strdup(yytext); return ALNUM;}


[-[:alnum:]/_]+"*" |
[-[:alnum:]/_]+ |
".."[-[:alnum:]/_]+"*" | 
".."[-[:alnum:]/_]+ { yylval.s = strdup(yytext); return FILENAME;}

\.|\n { return yytext[0]; }


. ; /* Ignore. */


<<EOF>> {return yytext[0];}
%%

extern char monitorps_buffer[];
extern char *monitorps_bufptr;
extern int monitorps_bufsiz;

int my_yyinput(char *buf, int max_size)
{
  int n;
  n = (monitorps_buffer+monitorps_bufsiz) - monitorps_bufptr;
  if (max_size < n) n = max_size;

  if (verbose) printf("Inside my_yyinput: buffer=<%s>\n", monitorps_buffer);
  if (verbose) printf("Inside my_yyinput: max_size = %d, n=%d,buf=<%s>\n", max_size, n, monitorps_bufptr);
  if (n > 0) {
    memcpy(buf, monitorps_bufptr, n);
    monitorps_bufptr += n;
  }
  return n;
}