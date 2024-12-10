
/*  A Bison parser, made from protocol.y
 by  GNU Bison version 1.27
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	GUI	257
#define	JOB	258
#define	PSGUI	259
#define	HOST	260
#define	TIME	261
#define	DEVICE	262
#define	PROGRAMID	263
#define	NAME_NUM	264
#define	ADD	265
#define	DEL	266
#define	RESET	267
#define	ALPHA	268
#define	NUMBER	269
#define	ALNUM	270
#define	FILENAME	271
#define	NEIGHBOR	272
#define	END_OF_FILE	273
#define	DATABASE	274
#define	JOBDB	275
#define	IMAGEDB	276
#define	PRODUCT_GUI	277
#define	STATUS	278
#define	PRODUCT	279
#define	FROM_GUI	280
#define	FROM_NEIGHBOR	281
#define	FROM_JOB	282
#define	RUNNING	283
#define	SUCCESSFUL	284
#define	FINISHED	285
#define	ABORTED	286
#define	WARNING	287
#define	FAILED	288

#line 1 "protocol.y"

#include <stdio.h>
#include "ports.h"
#include "monitorps.h"

static char from_who[100];
extern Lists lists;
extern int verbose;

#line 149 "protocol.y"
typedef union {
  char *s;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		67
#define	YYFLAG		-32768
#define	YYNTBASE	37

#define YYTRANSLATE(x) ((unsigned)(x) <= 288 ? yytranslate[x] : 52)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    35,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,    36,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     3,     6,     8,    12,    16,    25,    29,    33,    38,
    42,    51,    55,    59,    62,    64,    66,    67,    69,    71,
    75,    77,    79,    81,    83,    85,    87,    89,    91,    93,
    97,    99,   101,   103,   105,   107,   109,   111,   113
};

static const short yyrhs[] = {    40,
    38,     0,    39,    35,     0,    39,     0,    44,    18,    47,
     0,    44,     3,    45,     0,    24,    15,    49,    50,    46,
    51,    41,    43,     0,    44,     5,    45,     0,    44,    23,
    45,     0,    13,     3,    45,    47,     0,    13,     3,    45,
     0,    12,     4,    47,    49,    49,    50,    46,    51,     0,
    12,    20,    21,     0,    12,    20,    22,     0,     1,    35,
     0,    27,     0,    26,     0,     0,    42,     0,    47,     0,
    42,    36,    41,     0,    17,     0,    29,     0,    30,     0,
    31,     0,    32,     0,    33,     0,    34,     0,    11,     0,
    12,     0,    47,    36,    48,     0,    41,     0,     8,     0,
    16,     0,    15,     0,    14,     0,    15,     0,    15,     0,
    41,     0,    41,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   157,   178,   183,   189,   194,   199,   205,   210,   216,   222,
   228,   236,   242,   249,   252,   258,   263,   270,   271,   273,
   278,   283,   284,   285,   286,   287,   288,   290,   291,   293,
   299,   300,   302,   303,   304,   307,   309,   311,   313
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","GUI","JOB",
"PSGUI","HOST","TIME","DEVICE","PROGRAMID","NAME_NUM","ADD","DEL","RESET","ALPHA",
"NUMBER","ALNUM","FILENAME","NEIGHBOR","END_OF_FILE","DATABASE","JOBDB","IMAGEDB",
"PRODUCT_GUI","STATUS","PRODUCT","FROM_GUI","FROM_NEIGHBOR","FROM_JOB","RUNNING",
"SUCCESSFUL","FINISHED","ABORTED","WARNING","FAILED","'\\n'","'.'","valid","validstmt",
"statement","from_anyone","string","filename","statstring","command","hostport",
"device","host","port","time","jobid","programid", NULL
};
#endif

static const short yyr1[] = {     0,
    37,    38,    38,    39,    39,    39,    39,    39,    39,    39,
    39,    39,    39,    39,    40,    40,    40,    41,    41,    42,
    42,    43,    43,    43,    43,    43,    43,    44,    44,    45,
    46,    46,    47,    47,    47,    48,    49,    50,    51
};

static const short yyr2[] = {     0,
     2,     2,     1,     3,     3,     8,     3,     3,     4,     3,
     8,     3,     3,     2,     1,     1,     0,     1,     1,     3,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
     1,     1,     1,     1,     1,     1,     1,     1,     1
};

static const short yydefact[] = {    17,
    16,    15,     0,     0,    28,    29,     0,     0,     1,     3,
     0,    14,     0,     0,     0,     0,     2,     0,     0,     0,
     0,    35,    34,    33,     0,    12,    13,    10,     0,    37,
     0,     5,     7,     4,     8,     0,     9,     0,    21,    38,
    18,    19,     0,     0,    36,    30,     0,    32,    31,     0,
     0,    20,    39,     0,     0,     0,    11,    22,    23,    24,
    25,    26,    27,     6,     0,     0,     0
};

static const short yydefgoto[] = {    65,
     9,    10,     3,    40,    41,    64,    11,    28,    50,    42,
    46,    31,    43,    54
};

static const short yypact[] = {    -8,
-32768,-32768,    -1,   -26,-32768,     0,    33,    26,-32768,    10,
    -2,-32768,    18,     4,    18,    29,-32768,    18,    18,    18,
    18,-32768,-32768,-32768,    29,-32768,-32768,    18,    20,-32768,
    -9,-32768,-32768,-32768,-32768,    29,-32768,    42,-32768,-32768,
    22,-32768,    32,    -9,-32768,-32768,    -9,-32768,-32768,    -9,
    32,-32768,-32768,    -9,    -9,    21,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,    59,    60,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,   -12,-32768,-32768,-32768,    -4,    11,     9,
-32768,   -23,    17,     8
};


#define	YYLAST		63


static const short yytable[] = {     4,
    18,    36,    19,    13,    22,    23,    24,    39,    12,     5,
     6,     7,    44,    32,    33,    20,    35,     1,     2,    14,
    21,    25,     8,    29,    26,    27,    29,    29,    34,    29,
    49,    22,    23,    24,    52,    15,    37,    53,    49,    48,
    16,    56,    53,    30,    17,    22,    23,    24,    39,    58,
    59,    60,    61,    62,    63,    38,    45,    47,    66,    67,
    51,    55,    57
};

static const short yycheck[] = {     1,
     3,    25,     5,     4,    14,    15,    16,    17,    35,    11,
    12,    13,    36,    18,    19,    18,    21,    26,    27,    20,
    23,    13,    24,    15,    21,    22,    18,    19,    20,    21,
    43,    14,    15,    16,    47,     3,    28,    50,    51,     8,
    15,    54,    55,    15,    35,    14,    15,    16,    17,    29,
    30,    31,    32,    33,    34,    36,    15,    36,     0,     0,
    44,    51,    55
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison.simple"
/* This file comes from bison-1.27.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 216 "/usr/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 158 "protocol.y"
{
  char cmd[1000];
  if (verbose && yyvsp[-1].s) printf("$1=<%s>\n", yyvsp[-1].s);
  else if (verbose) printf("$1=<NULL>\n");
  if (verbose) printf("RULE --> valid: from_anyone validstmt\n");
  if (verbose && yyvsp[0].s) printf("$2=<%s>\n", yyvsp[0].s);
  else if (verbose) printf("$2=<NULL>\n");

  sprintf(cmd,"from_neighbor: %s", yyvsp[0].s);
  if (yyvsp[-1].s != NULL) {
	if (strcasecmp(yyvsp[-1].s, "neighbor") != 0) {
	  tell_neighbors(cmd);
	}
  } else {
	if (strncasecmp(yyvsp[0].s, "status:", 7))
	  tell_neighbors(cmd); /* from_who is unknown, assume gui. */
  }
  YYACCEPT;
;
    break;}
case 2:
#line 179 "protocol.y"
{
 if (verbose) printf("VALIDn: %s\n", yyvsp[-1].s);
 sprintf(yyval.s,"%s", yyvsp[-1].s);
;
    break;}
case 3:
#line 184 "protocol.y"
{
  if (verbose) printf("VALID : %s\n", yyvsp[0].s);
  sprintf(yyval.s,"%s", yyvsp[0].s);
;
    break;}
case 4:
#line 190 "protocol.y"
{
  do_neighbor_cmd(yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
  sprintf(yyval.s,"%s %s %s", yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
;
    break;}
case 5:
#line 195 "protocol.y"
{
  do_gui_cmd(yyvsp[-2].s,yyvsp[-1].s,yyvsp[0].s);
  sprintf(yyval.s,"%s %s %s", yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
;
    break;}
case 6:
#line 200 "protocol.y"
{
  sprintf(yyval.s,"%s %s %s %s %s %s %s %s", yyvsp[-7].s, yyvsp[-6].s, yyvsp[-5].s, yyvsp[-4].s,
		  yyvsp[-3].s, yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
  do_status_cmd(yyval.s);
;
    break;}
case 7:
#line 206 "protocol.y"
{
  do_psgui_cmd(yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
  sprintf(yyval.s,"%s %s %s", yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
;
    break;}
case 8:
#line 211 "protocol.y"
{
  do_product_gui_cmd(yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
  sprintf(yyval.s,"%s %s %s", yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
;
    break;}
case 9:
#line 217 "protocol.y"
{
  do_reset_cmd(yyvsp[-3].s, yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
  sprintf(yyval.s,"%s %s %s %s", yyvsp[-3].s, yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
;
    break;}
case 10:
#line 223 "protocol.y"
{
  do_reset_cmd(yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s, NULL);
  sprintf(yyval.s,"%s %s %s", yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
;
    break;}
case 11:
#line 229 "protocol.y"
{
  do_job_cmd(yyvsp[-7].s, yyvsp[-6].s, yyvsp[-5].s, yyvsp[-4].s, yyvsp[-3].s, yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
  sprintf(yyval.s,"%s %s %s %s %s %s %s %s", yyvsp[-7].s, yyvsp[-6].s, yyvsp[-5].s, yyvsp[-4].s,
		  yyvsp[-3].s, yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
;
    break;}
case 12:
#line 237 "protocol.y"
{
  del_file(JOB_STATUS_FILE);
  sprintf(yyval.s,"%s %s %s", yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
;
    break;}
case 13:
#line 243 "protocol.y"
{
  del_file(PRODUCT_DB_FILE);
  sprintf(yyval.s,"%s %s %s", yyvsp[-2].s, yyvsp[-1].s, yyvsp[0].s);
;
    break;}
case 14:
#line 249 "protocol.y"
{yyerrok; YYACCEPT;;
    break;}
case 15:
#line 254 "protocol.y"
{
  tell_parser_from_who("neighbor");
  sprintf(yyval.s,"neighbor");
;
    break;}
case 16:
#line 259 "protocol.y"
{
  tell_parser_from_who("gui");
  sprintf(yyval.s,"gui");
;
    break;}
case 17:
#line 264 "protocol.y"
{
  tell_parser_from_who("unknown");
  yyval.s = NULL;
;
    break;}
case 20:
#line 274 "protocol.y"
{
  sprintf(yyval.s,"%s.%s", yyvsp[-2].s, yyvsp[0].s);
  if (verbose) printf("___filename__ = <%s>\n", yyval.s);
;
    break;}
case 21:
#line 279 "protocol.y"
{
  if (verbose) printf("__filename2__ = <%s>\n", yyval.s);
;
    break;}
case 28:
#line 290 "protocol.y"
{if(verbose)printf("___ADD___ = <%s>\n", yyval.s);;
    break;}
case 29:
#line 291 "protocol.y"
{if(verbose)printf("___DEL___ = <%s>\n", yyval.s);;
    break;}
case 30:
#line 294 "protocol.y"
{
  sprintf(yyval.s,"%s.%s", yyvsp[-2].s, yyvsp[0].s);
  if (verbose) printf("__hostport__ = <%s>\n", yyval.s);
;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 542 "/usr/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 317 "protocol.y"


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
