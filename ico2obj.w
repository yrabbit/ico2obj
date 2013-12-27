% vim: set ai textwidth=80:
\input lib/verbatim
\input cwebmac-ru

\def\version{0.1}
\font\twentycmcsc=cmcsc10 at 20 truept

\datethis

\vskip 120pt
\centerline{\twentycmcsc ico2obj}
\vskip 20pt
\centerline{Преобразователь картинок в формате ICO в объектные файлы}
\vskip 2pt
\centerline{(Версия \version)}
\vskip 10pt
\centerline{Yellow Rabbit}
\vskip 80pt

Позволяет конвертировать графические файлы формата ICO в объектные файлы для
последующей линковки в исполняемые образы для БК11М\footnote{$^1$}{GIT-репозитарий ассемблера, 
линковщика и утилит https://github.com/yrabbit}.

Входными параметрами являются: перечень имен графических файлов, имя
выходного файла и несколько управляющих ключей.
На выходе создается объектный файл для последующей линковки.

Конвертор накладывает следующие ограничения на формат входных файлов
изображений:
\item{$\bullet$} 4 бита на точку.


@* Общая схема программы.
@c
@<Включение заголовочных файлов@>@;
@h
@<Константы@>@;
@<Собственные типы данных@>@;
@<Прототипы@>@;
@<Глобальные переменные@>@;
int
main(int argc, char *argv[])
{
	@<Данные программы@>@;
	const char *picname;

	@<Разобрать командную строку@>@;

	/* Поочередно обрабатываем все заданные файлы картинок */
	cur_input = 0;
	while ((picname = config.picnames[cur_input]) != NULL) {
		@<Открыть файл картинки@>@;
		handleOneFile(fpic);
		fclose(fpic);
		++cur_input;
	}
	return(0);
}

@ Номер текущего обрабатываемого файла картинки.
@<Глобальные переменные@>=
static int cur_input;
@ @<Данные програ...@>=
FILE *fpic;

@ @<Открыть файл картинки@>=
	fpic = fopen(picname,"r");
	if (fpic== NULL) {
		PRINTERR("Can't open %s\n", picname);
		return(ERR_CANTOPEN);
	}

@ Обработать один файл картинки.
@c
static void
handleOneFile(FILE *fpic) {
}

@ @<Прототипы@>=
static void handleOneFile(FILE *);

@* Разбор параметров командной строки.

Для этой цели используется достаточно удобная свободная библиотека 
{\sl argp}.
@d VERSION "0.1"

@ @<Константы@>=
const char *argp_program_version = "ico2obj, " VERSION;
const char *argp_program_bug_address = "<yellowrabbit@@bk.ru>";

@ @<Глобальн...@>=
static char argp_program_doc[] = "Convert ICO images to object file";
static char args_doc[] = "file [...]";

@ Распознаются следующие опции:
\smallskip
	\item {} {\tt -o} --- имя выходного файла.
	\item {} {\tt -v} --- вывод дополнительной информации (возможно указание
	дважды);
	\item {} {\tt -l LABEL} --- шаблон метки для изображения (6 символов
	RADIX50);
	\item {} {\tt -[0123]} --- номера цветов для битов.
\smallskip
@<Глобальн...@>=
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

@ Эта структура используется для получения результатов разбора параметров командной строки.
@<Собственные...@>=
typedef struct _Arguments {
	int  verbosity;
	char output_filename[FILENAME_MAX]; /* Имя файла с текстом */
	char label[7];	    /* Метка для картинок в объектном файле*/
	char **picnames;		    /* Имена файлов картинок
					 picnames[?] == NULL --> конец имен*/
	int colors[4]; /* Номера цветов для битов */				 
} Arguments;

@ @<Глобальные...@>=
static Arguments config = { 0, {0}, {0}, NULL, {0}, };


@ Задачей данного простого парсера является заполнение структуры |Arguments| из указанных
параметров командной строки.
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
		/* Имена файлов картинок */
		arguments->picnames = &state->argv[state->next - 1];
		/* Останавливаем разбор параметров */
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
@<Разобрать ком...@>=
	argp_parse(&argp, argc, argv, 0, 0, &config);@/
	/* Проверка параметров */
	if (strlen(config.output_filename) == 0) {
		PRINTERR("No output filename specified\n");
		return(ERR_SYNTAX);
	}
	if (config.picnames == NULL) {
		PRINTERR("No input filenames specified\n");
		return(ERR_SYNTAX);
	}

@ @<Включение ...@>=
#include <string.h>
#include <stdlib.h>

#ifdef __linux__
#include <stdint.h>
#endif

#include <argp.h>

@
@<Глобальные...@>=
#define PRINTVERB(level, fmt, a...) (((config.verbosity) >= level) ? printf(\
  (fmt), ## a) : 0)
#define PRINTERR(fmt, a...) fprintf(stderr, (fmt), ## a) 

@* Индекс.



