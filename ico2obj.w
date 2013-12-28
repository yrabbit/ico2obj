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
\item{$\bullet$} одна битовая плоскость;
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
	@<Записать начало объектного файла@>@;
	while ((picname = config.picnames[cur_input]) != NULL) {
		@<Открыть файл картинки@>@;
		handleOneFile(fpic, &hdr);
		fclose(fpic);
		++cur_input;
	}
	@<Закрыть объектный файл@>@;
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
	@<Проверить заголовок картинки@>@;


@ Проверяем соответствие формату ICO.
@<Собственные типы...@>=
typedef struct _ICO_Header {
	uint16_t	zero0;
	uint16_t	type;	/* должен быть 1 */
	uint16_t	imagesCount;
} ICO_Header;

@ @<Данные программы@>=
ICO_Header hdr;

@ @<Проверить заголовок...@>=
	if (fread(&hdr, sizeof(hdr), 1, fpic) != 1) {
		PRINTERR("Can't read header of %s\n", picname);
		return(ERR_CANTOPEN);
	}
	if (hdr.zero0 != 0 || hdr.type != 1 || hdr.imagesCount == 0) {
		PRINTERR("Bad file header of %s\n", picname);
		return(ERR_BADFILEHEADER);
	}
	PRINTVERB(1, "Handle file: %s.\n", picname);
	PRINTVERB(2, "Images count: %d.\n", hdr.imagesCount);
		
@* Обработать один файл картинки. 

Каждый файл может содержать несколько
изображений, которые описываются записями следующего вида.
@<Собственные типы...@>=
typedef struct _IMG_Header {
	uint8_t		width; /* если 0, то 256 */@/
	uint8_t		height; /* если 0, то 256 */@/
	uint8_t		colors;@/
	uint8_t		reserved;@/
	uint16_t	planes;@/
	uint16_t	bpp;@/
	uint32_t	size;	/* размер в байтах */@/
	uint32_t	offset; /* смещение до данных изображения от начала
	файла */
} IMG_Header;

@ @c
static void
handleOneFile(FILE *fpic, ICO_Header *hdr) {
	int cur_image;
	IMG_Header *imgs;
	@<Переменные для картинки@>@;

	imgs = (IMG_Header*)malloc(sizeof(IMG_Header) * hdr->imagesCount);

	if (imgs == NULL) {
		PRINTERR("No memory for image directory of %s.\n", config.picnames[cur_input]);
		return;
	}
	/* читаем каталог изображений */
	if (fread(imgs, sizeof(IMG_Header), hdr->imagesCount, fpic) != hdr->imagesCount) {
		PRINTERR("Can't read image directory of %s.\n", config.picnames[cur_input]);
		free(imgs);
		return;
	}
	
	for (cur_image = 0; cur_image < hdr->imagesCount; ++cur_image) {
		if (imgs[cur_image].bpp != 4) {
			PRINTERR("Bad bits per pixel (%d) for image %d of %s.\n", 
				imgs[cur_image].bpp, cur_image, config.picnames[cur_input]);
			continue;
		}
		@<Обработать одно изображение@>@;
	}

	free(imgs);
}

@* Обработать одно изображение.

@<Переменные для карт...@>=
	static uint8_t picInData[256*256/2]; /* максимальный объем памяти под одно
	изображение
	256 пикселей в ширину, 256 пикселей в высоту, 2 пиксела в байте
	*/
	static uint8_t picOutData[256*256/4];
	int i, j;
@ @<Обработать одно...@>=
	PRINTVERB(2, "Image:%d, w:%d, h:%d, colors:%d, planes:%d, bpp:%d,"
	" size:%d, offset:%x\n", cur_image,
	imgs[cur_image].width, imgs[cur_image].height, 
	imgs[cur_image].colors, imgs[cur_image].planes, imgs[cur_image].bpp,
	imgs[cur_image].size, imgs[cur_image].offset);
	
@ @<Прототипы@>=
static void handleOneFile(FILE *, ICO_Header *);

@* Работа с объектным файлом.

Объектный файл состоит из нескольких блоков, для представления 
картинки понадобятся блоки\footnote{$^2$}{AA-KX10A-TC\_PDP-11\_MACRO-11\_Reference\_Manual\_May88}
	\item {$\bullet$} GSD~---~для меток картинок и т.д.;
	\item {$\bullet$} ENDGSD~---~конец меток и прочего;
	\item {$\bullet$} RLD~---~хотя картинки и располагаются друг за другом в
	памяти, иногда придётся указывать смещение;
	\item {$\bullet$} TXT~---~собственно данные картинок;
	\item {$\bullet$} ENDMOD~---~конец модуля.
@<Глобальные...@>=
	FILE *fobj;

@ Каждый блок начинается байтами 0 и 1, двумя байтами длины блока, а
заканчивается байтом контрольной суммы.
@c
static void
write_block(uint8_t *data, uint16_t data_len) {
	uint8_t chksum;
	uint16_t len;

	len = data_len + 4;
	chksum = 0;
	
	fputc(1, fobj);
	fputc(0, fobj);
	chksum -= 1;

	fwrite(&len, sizeof(len), 1, fobj);
	chksum -= len & 0xff;
	chksum -= (len & 0xff00) >> 8;
	
	fwrite(data, data_len, 1, fobj);
	for (; data_len > 0; --data_len) {
		chksum -= *data++;
	}
	fputc(chksum, fobj);
}

@ Записать блок ENDMOD.
@c
static void
write_endmod(void) {
	uint8_t buf[2];

	buf[0] = 6; /* ENDMOD */
	buf[1] = 0;

	write_block(buf, sizeof(buf));
}

@ Записать блок ENDGSD.
@c
static void
write_endgsd(void) {
	uint8_t buf[2];

	buf[0] = 2; /* ENDGSD */
	buf[1] = 0;

	write_block(buf, sizeof(buf));
}

@ Записать начальные блоки GSD, которые содержат описания программных секций,
имени модуля и пр.
@c
static void
write_initial_gsd(void) {
	uint16_t buf[50];

	buf[0] = 1; /* GSD */

	/* Имя модуля */@|
	buf[1] = toRadix50(" PI");
	buf[2] = toRadix50("C$$");
	buf[3] = buf[4] = 0;

	write_block((uint8_t*)buf, 4 * 2);
}

@ @<Записать начало объектного файла@>=
	fobj = fopen(config.output_filename, "w");
	if (fobj == NULL) {
		PRINTERR("Can't open %s.\n", config.output_filename);
		return(ERR_CANTOPENOBJ);
	}
	write_initial_gsd();

@ @<Закрыть объектный файл@>=
	write_endgsd();
	write_endmod();
	fclose(fobj);

@ @<Прототипы...@>=
static void write_block(uint8_t *, uint16_t);
static void write_endmod(void);
static void write_endgsd(void);
static void write_initial_gsd(void);

@* Вспомогательные функции.
Упаковка строки в RADIX50.
@c
uint16_t toRadix50(char *str) {
	static char radtbl[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ$. 0123456789";
	uint32_t acc;
	char *rp;

	acc = 0;

	if(*str == 0) {
		return(acc);
	}
	rp = strchr(radtbl, toupper(*str));
	if (rp == NULL) {
		return(acc);
	}
	acc += ((uint32_t)(rp - radtbl)) * 03100;
	++str;

	if(*str == 0) {
		return(acc);
	}
	rp = strchr(radtbl, toupper(*str));
	if (rp == NULL) {
		return(acc);
	}
	acc += ((uint32_t)(rp - radtbl)) * 050;
	++str;
	
	if(*str == 0) {
		return(acc);
	}
	rp = strchr(radtbl, toupper(*str));
	if (rp == NULL) {
		return(acc);
	}
	acc += ((uint32_t)(rp - radtbl));

	return(acc);
}

@ @<Прототипы@>=
static uint16_t toRadix50(char *);

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
	\item {} {\tt -s SECTION\_NAME} --- имя программной секции (6 символов
	RADIX50);
	\item {} {\tt -a} --- создавать сецкии с атрибутом SAV;
	\item {} {\tt -t} --- транспонировать картинку;
	\item {} {\tt -[0123]} --- номера цветов для битов.
\smallskip
@<Глобальн...@>=
static struct argp_option options[] = {@/
	{ "output", 'o', "FILENAME", 0, "Output filename"},@/
	{ "verbose", 'v', NULL, 0, "Verbose output"},@/
	{ "section", 's', "SECTION_NAME", 0, "Program section name"},@|
	{ "attr", 'a', NULL, 0, "Set program section SAV attribute"},@|
	{ "label", 'l', "LABEL", 0, "Label for images"},@/
	{ "trans", 't', NULL, 0, "Transpose image"},@|
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
	char section_name[7]; /* Имя программной секции */
	int save; /* установлен атрибут SAV для секции */
	char **picnames;		    /* Имена файлов картинок
					 picnames[?] == NULL --> конец имен*/
	int colors[4]; /* Номера цветов для битов */
	int transpose;
} Arguments;

@ @<Глобальные...@>=
static Arguments config = { 0, {0}, {0}, {0}, 0, NULL,@| 
/* Начальные номера цветов */
{0, 1, 2, 3}, 0,
};


@ Задачей данного простого парсера является заполнение структуры |Arguments| из указанных
параметров командной строки.
@c
static error_t 
parse_opt(int key, char *arg, struct argp_state *state) {
 Arguments *arguments;
	arguments = (Arguments*)state->input;
 switch (key) {
	case 't':
		arguments->transpose = 1;
		break;
	case 'a':
		arguments->save = 1;
		break;
	case 'l':
		if (strlen(arg) == 0 || strlen(arg) > 6)
			return(ARGP_ERR_UNKNOWN);
		strcpy(arguments->label, arg);	
		break;
	case 's':
		if (strlen(arg) == 0 || strlen(arg) > 6)
			return(ARGP_ERR_UNKNOWN);
		strcpy(arguments->section_name, arg);	
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
@d ERR_BADFILEHEADER	4
@d ERR_CANTOPENOBJ	5

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



