% vim: set ai textwidth=80:
\input lib/verbatim
\input cwebmac-ru

\def\version{0.1}
\font\twentycmcsc=cmcsc10 at 20 truept

\datethis

\vskip 120pt
\centerline{\twentycmcsc ico2obj}
\vskip 20pt
\centerline{��������������� �������� � ������� ICO � ��������� �����}
\vskip 2pt
\centerline{(������ \version)}
\vskip 10pt
\centerline{Yellow Rabbit}
\vskip 80pt

��������� �������������� ����������� ����� ������� ICO � ��������� ����� ���
����������� �������� � ����������� ������ ��� ��11�\footnote{$^1$}{GIT-����������� ����������, 
���������� � ������ https://github.com/yrabbit}.

�������� ����������� ��������: �������� ���� ����������� ������, ���
��������� ����� � ��������� ����������� ������.
�� ������ ��������� ��������� ���� ��� ����������� ��������.

��������� ����������� ��������� ����������� �� ������ ������� ������
�����������:
\item{$\bullet$} 4 ���� �� �����.


@* ����� ����� ���������.
@c
@<��������� ������������ ������@>@;
@h
@<���������@>@;
@<����������� ���� ������@>@;
@<���������@>@;
@<���������� ����������@>@;
int
main(int argc, char *argv[])
{
	@<������ ���������@>@;
	const char *picname;

	@<��������� ��������� ������@>@;

	/* ���������� ������������ ��� �������� ����� �������� */
	cur_input = 0;
	while ((picname = config.picnames[cur_input]) != NULL) {
		@<������� ���� ��������@>@;
		handleOneFile(fpic);
		fclose(fpic);
		++cur_input;
	}
	return(0);
}

@ ����� �������� ��������������� ����� ��������.
@<���������� ����������@>=
static int cur_input;
@ @<������ ������...@>=
FILE *fpic;

@ @<������� ���� ��������@>=
	fpic = fopen(picname,"r");
	if (fpic== NULL) {
		PRINTERR("Can't open %s\n", picname);
		return(ERR_CANTOPEN);
	}

@ ���������� ���� ���� ��������.
@c
static void
handleOneFile(FILE *fpic) {
}

@ @<���������@>=
static void handleOneFile(FILE *);

@* ������ ���������� ��������� ������.

��� ���� ���� ������������ ���������� ������� ��������� ���������� 
{\sl argp}.
@d VERSION "0.1"

@ @<���������@>=
const char *argp_program_version = "ico2obj, " VERSION;
const char *argp_program_bug_address = "<yellowrabbit@@bk.ru>";

@ @<��������...@>=
static char argp_program_doc[] = "Convert ICO images to object file";
static char args_doc[] = "file [...]";

@ ������������ ��������� �����:
\smallskip
	\item {} {\tt -o} --- ��� ��������� �����.
	\item {} {\tt -v} --- ����� �������������� ���������� (�������� ��������
	������);
	\item {} {\tt -l LABEL} --- ������ ����� ��� ����������� (6 ��������
	RADIX50);
	\item {} {\tt -[0123]} --- ������ ������ ��� �����.
\smallskip
@<��������...@>=
static struct argp_option options[] = {@/
	{ "output", 'o', "FILENAME", 0, "Output filename"},@/
	{ "verbose", 'v', NULL, 0, "Verbose output"},@/
	{ "label", 'l', "LABEL", 0, "Label for images"},@/
	{ "color0", '0', "COLOR", 0, "Color number for bits 00"},@/
	{ "color1", '1', "COLOR", 0, "Color number for bits 01"},@/
	{ "color2", '2', "COLOR", 0, "Color number for bits 10"},@/
	{ "color3", '3', "COLOR", 0, "Color number for bits 11"},@/
	{ 0 }@/
};
static error_t parse_opt(int, char*, struct argp_state*);@!
static struct argp argp = {options, parse_opt, args_doc, argp_program_doc};

@ ��� ��������� ������������ ��� ��������� ����������� ������� ���������� ��������� ������.
@<�����������...@>=
typedef struct _Arguments {
	int  verbosity;
	char output_filename[FILENAME_MAX]; /* ��� ����� � ������� */
	char label[7];	    /* ����� ��� �������� � ��������� �����*/
	char **picnames;		    /* ����� ������ ��������
					 picnames[?] == NULL --> ����� ����*/
	int colors[4]; /* ������ ������ ��� ����� */				 
} Arguments;

@ @<����������...@>=
static Arguments config = { 0, {0}, {0}, NULL, {0}, };


@ ������� ������� �������� ������� �������� ���������� ��������� |Arguments| �� ���������
���������� ��������� ������.
@c
static error_t 
parse_opt(int key, char *arg, struct argp_state *state) {
 Arguments *arguments;
	arguments = (Arguments*)state->input;
 switch (key) {
	case 'l':
		if (strlen(arg) == 0 || strlen(arg) > 6)
			return(ARGP_ERR_UNKNOWN);
		strcpy(arguments->label, arg);	
		break;
	case 'v':
		++arguments->verbosity;
		break;
	case 'o':
		if (strlen(arg) == 0)
			return(ARGP_ERR_UNKNOWN);
		strncpy(arguments->output_filename, arg, FILENAME_MAX - 1);
		break;
	case '0' :
		arguments->colors[0] = atoi(arg);
		break;
	case '1' :
		arguments->colors[1] = atoi(arg);
		break;
	case '2' :
		arguments->colors[2] = atoi(arg);
		break;
	case '3' :
		arguments->colors[3] = atoi(arg);
		break;
	case ARGP_KEY_ARG:
		/* ����� ������ �������� */
		arguments->picnames = &state->argv[state->next - 1];
		/* ������������� ������ ���������� */
		state->next = state->argc;
		break;
	default:
		break;
		return(ARGP_ERR_UNKNOWN);
	}
	return(0);
}
@ 
@d ERR_SYNTAX		1
@d ERR_CANTOPEN		2
@d ERR_CANTCREATE	3
@<��������� ���...@>=
	argp_parse(&argp, argc, argv, 0, 0, &config);@/
	/* �������� ���������� */
	if (strlen(config.output_filename) == 0) {
		PRINTERR("No output filename specified\n");
		return(ERR_SYNTAX);
	}
	if (config.picnames == NULL) {
		PRINTERR("No input filenames specified\n");
		return(ERR_SYNTAX);
	}

@ @<��������� ...@>=
#include <string.h>
#include <stdlib.h>

#ifdef __linux__
#include <stdint.h>
#endif

#include <argp.h>

@
@<����������...@>=
#define PRINTVERB(level, fmt, a...) (((config.verbosity) >= level) ? printf(\
  (fmt), ## a) : 0)
#define PRINTERR(fmt, a...) fprintf(stderr, (fmt), ## a) 

@* ������.



