/*1:*/
#line 34 "ico2obj.w"

/*15:*/
#line 195 "ico2obj.w"

#include <string.h> 
#include <stdlib.h> 

#ifdef __linux__
#include <stdint.h> 
#endif

#include <argp.h> 

/*:15*/
#line 35 "ico2obj.w"

#define VERSION "0.1" \

#define ERR_SYNTAX 1
#define ERR_CANTOPEN 2
#define ERR_CANTCREATE 3

#line 36 "ico2obj.w"

/*8:*/
#line 88 "ico2obj.w"

const char*argp_program_version= "ico2obj, "VERSION;
const char*argp_program_bug_address= "<yellowrabbit@bk.ru>";

/*:8*/
#line 37 "ico2obj.w"

/*11:*/
#line 120 "ico2obj.w"

typedef struct _Arguments{
int verbosity;
char output_filename[FILENAME_MAX];
char label[7];
char**picnames;

int colors[4];
}Arguments;

/*:11*/
#line 38 "ico2obj.w"

/*6:*/
#line 79 "ico2obj.w"

static void handleOneFile(FILE*);

/*:6*/
#line 39 "ico2obj.w"

/*2:*/
#line 61 "ico2obj.w"

static int cur_input;
/*:2*//*9:*/
#line 92 "ico2obj.w"

static char argp_program_doc[]= "Convert ICO images to object file";
static char args_doc[]= "file [...]";

/*:9*//*10:*/
#line 105 "ico2obj.w"

static struct argp_option options[]= {
{"output",'o',"FILENAME",0,"Output filename"},
{"verbose",'v',NULL,0,"Verbose output"},
{"label",'l',"LABEL",0,"Label for images"},
{"color0",'0',"COLOR",0,"Color number for bits 00"},
{"color1",'1',"COLOR",0,"Color number for bits 01"},
{"color2",'2',"COLOR",0,"Color number for bits 10"},
{"color3",'3',"COLOR",0,"Color number for bits 11"},
{0}
};
static error_t parse_opt(int,char*,struct argp_state*);
static struct argp argp= {options,parse_opt,args_doc,argp_program_doc};

/*:10*//*12:*/
#line 130 "ico2obj.w"

static Arguments config= {0,{0},{0},NULL,{0},};


/*:12*//*16:*/
#line 206 "ico2obj.w"

#define PRINTVERB(level, fmt, a...) (((config.verbosity) >= level) ? printf(\
  (fmt), ## a) : 0)
#define PRINTERR(fmt, a...) fprintf(stderr, (fmt), ## a)

/*:16*/
#line 40 "ico2obj.w"

int
main(int argc,char*argv[])
{
/*3:*/
#line 63 "ico2obj.w"

FILE*fpic;

/*:3*/
#line 44 "ico2obj.w"

const char*picname;

/*14:*/
#line 183 "ico2obj.w"

argp_parse(&argp,argc,argv,0,0,&config);

if(strlen(config.output_filename)==0){
PRINTERR("No output filename specified\n");
return(ERR_SYNTAX);
}
if(config.picnames==NULL){
PRINTERR("No input filenames specified\n");
return(ERR_SYNTAX);
}

/*:14*/
#line 47 "ico2obj.w"



cur_input= 0;
while((picname= config.picnames[cur_input])!=NULL){
/*4:*/
#line 66 "ico2obj.w"

fpic= fopen(picname,"r");
if(fpic==NULL){
PRINTERR("Can't open %s\n",picname);
return(ERR_CANTOPEN);
}

/*:4*/
#line 52 "ico2obj.w"

handleOneFile(fpic);
fclose(fpic);
++cur_input;
}
return(0);
}

/*:1*//*5:*/
#line 74 "ico2obj.w"

static void
handleOneFile(FILE*fpic){
}

/*:5*//*13:*/
#line 136 "ico2obj.w"

static error_t
parse_opt(int key,char*arg,struct argp_state*state){
Arguments*arguments;
arguments= (Arguments*)state->input;
switch(key){
case'l':
if(strlen(arg)==0||strlen(arg)> 6)
return(ARGP_ERR_UNKNOWN);
strcpy(arguments->label,arg);
break;
case'v':
++arguments->verbosity;
break;
case'o':
if(strlen(arg)==0)
return(ARGP_ERR_UNKNOWN);
strncpy(arguments->output_filename,arg,FILENAME_MAX-1);
break;
case'0':
arguments->colors[0]= atoi(arg);
break;
case'1':
arguments->colors[1]= atoi(arg);
break;
case'2':
arguments->colors[2]= atoi(arg);
break;
case'3':
arguments->colors[3]= atoi(arg);
break;
case ARGP_KEY_ARG:

arguments->picnames= &state->argv[state->next-1];

state->next= state->argc;
break;
default:
break;
return(ARGP_ERR_UNKNOWN);
}
return(0);
}
/*:13*/
