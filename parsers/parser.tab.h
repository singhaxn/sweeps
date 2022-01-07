#ifndef BISON_PARSER_TAB_H
# define BISON_PARSER_TAB_H

# ifndef YYSTYPE
#  define YYSTYPE int
#  define YYSTYPE_IS_TRIVIAL 1
# endif
# define	tokNumber	257
# define	tokFunction	258
# define	tokOperator	259
# define	tokErr	260
# define	tokEOF	261
# define	tokVariable	262


extern YYSTYPE yylval;

#endif /* not BISON_PARSER_TAB_H */
