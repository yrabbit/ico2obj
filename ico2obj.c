/*1:*/
#line 35 "ico2obj.w"

/*31:*/
#line 453 "ico2obj.w"

#include <string.h> 
#include <stdlib.h> 

#ifdef __linux__
#include <stdint.h> 
#endif

#include <argp.h> 

/*:31*/
#line 36 "ico2obj.w"

#define VERSION "0.1" \

#define ERR_SYNTAX 1
#define ERR_CANTOPEN 2
#define ERR_CANTCREATE 3
#define ERR_BADFILEHEADER 4
#define ERR_CANTOPENOBJ 5 \


#line 37 "ico2obj.w"

/*24:*/
#line 319 "ico2obj.w"

const char*argp_program_version= "ico2obj, "VERSION;
const char*argp_program_bug_address= "<yellowrabbit@bk.ru>";

/*:24*/
#line 38 "ico2obj.w"

/*5:*/
#line 79 "ico2obj.w"

typedef struct _ICO_Header{
uint16_t zero0;
uint16_t type;
uint16_t imagesCount;
}ICO_Header;

/*:5*//*8:*/
#line 105 "ico2obj.w"

typedef struct _IMG_Header{
uint8_t width;
uint8_t height;
uint8_t colors;
uint8_t reserved;
uint16_t planes;
uint16_t bpp;
uint32_t size;
uint32_t offset;

}IMG_Header;

/*:8*//*27:*/
#line 358 "ico2obj.w"

typedef struct _Arguments{
int verbosity;
char output_filename[FILENAME_MAX];
char label[7];
char section_name[7];
int save;
char**picnames;

int colors[4];
int transpose;
}Arguments;

/*:27*/
#line 39 "ico2obj.w"

/*12:*/
#line 166 "ico2obj.w"

static void handleOneFile(FILE*,ICO_Header*);

/*:12*//*20:*/
#line 262 "ico2obj.w"

static void write_block(uint8_t*,uint16_t);
static void write_endmod(void);
static void write_endgsd(void);
static void write_initial_gsd(void);

/*:20*//*22:*/
#line 310 "ico2obj.w"

static uint16_t toRadix50(char*);

/*:22*/
#line 40 "ico2obj.w"

/*2:*/
#line 64 "ico2obj.w"

static int cur_input;
/*:2*//*13:*/
#line 179 "ico2obj.w"

FILE*fobj;

/*:13*//*25:*/
#line 323 "ico2obj.w"

static char argp_program_doc[]= "Convert ICO images to object file";
static char args_doc[]= "file [...]";

/*:25*//*26:*/
#line 340 "ico2obj.w"

static struct argp_option options[]= {
{"output",'o',"FILENAME",0,"Output filename"},
{"verbose",'v',NULL,0,"Verbose output"},
{"section",'s',"SECTION_NAME",0,"Program section name"},
{"attr",'a',NULL,0,"Set program section SAV attribute"},
{"label",'l',"LABEL",0,"Label for images"},
{"trans",'t',NULL,0,"Transpose image"},
{"color0",'0',"COLOR",0,"Color number for bits 00"},
{"color1",'1',"COLOR",0,"Color number for bits 01"},
{"color2",'2',"COLOR",0,"Color number for bits 10"},
{"color3",'3',"COLOR",0,"Color number for bits 11"},
{0}
};
static error_t parse_opt(int,char*,struct argp_state*);
static struct argp argp= {options,parse_opt,args_doc,argp_program_doc};

/*:26*//*28:*/
#line 371 "ico2obj.w"

static Arguments config= {0,{0},{0},{0},0,NULL,

{0,1,2,3},0,
};


/*:28*//*32:*/
#line 464 "ico2obj.w"

#define PRINTVERB(level, fmt, a...) (((config.verbosity) >= level) ? printf(\
  (fmt), ## a) : 0)
#define PRINTERR(fmt, a...) fprintf(stderr, (fmt), ## a)

/*:32*/
#line 41 "ico2obj.w"

int
main(int argc,char*argv[])
{
/*3:*/
#line 66 "ico2obj.w"

FILE*fpic;

/*:3*//*6:*/
#line 86 "ico2obj.w"

ICO_Header hdr;

/*:6*/
#line 45 "ico2obj.w"

const char*picname;

/*30:*/
#line 441 "ico2obj.w"

argp_parse(&argp,argc,argv,0,0,&config);

if(strlen(config.output_filename)==0){
PRINTERR("No output filename specified\n");
return(ERR_SYNTAX);
}
if(config.picnames==NULL){
PRINTERR("No input filenames specified\n");
return(ERR_SYNTAX);
}

/*:30*/
#line 48 "ico2obj.w"



cur_input= 0;
/*18:*/
#line 249 "ico2obj.w"

fobj= fopen(config.output_filename,"w");
if(fobj==NULL){
PRINTERR("Can't open %s.\n",config.output_filename);
return(ERR_CANTOPENOBJ);
}
write_initial_gsd();

/*:18*/
#line 52 "ico2obj.w"

while((picname= config.picnames[cur_input])!=NULL){
/*4:*/
#line 69 "ico2obj.w"

fpic= fopen(picname,"r");
if(fpic==NULL){
PRINTERR("Can't open %s\n",picname);
return(ERR_CANTOPEN);
}
/*7:*/
#line 89 "ico2obj.w"

if(fread(&hdr,sizeof(hdr),1,fpic)!=1){
PRINTERR("Can't read header of %s\n",picname);
return(ERR_CANTOPEN);
}
if(hdr.zero0!=0||hdr.type!=1||hdr.imagesCount==0){
PRINTERR("Bad file header of %s\n",picname);
return(ERR_BADFILEHEADER);
}
PRINTVERB(1,"Handle file: %s.\n",picname);
PRINTVERB(2,"Images count: %d.\n",hdr.imagesCount);

/*:7*/
#line 75 "ico2obj.w"



/*:4*/
#line 54 "ico2obj.w"

handleOneFile(fpic,&hdr);
fclose(fpic);
++cur_input;
}
/*19:*/
#line 257 "ico2obj.w"

write_endgsd();
write_endmod();
fclose(fobj);

/*:19*/
#line 59 "ico2obj.w"

return(0);
}

/*:1*//*9:*/
#line 118 "ico2obj.w"

static void
handleOneFile(FILE*fpic,ICO_Header*hdr){
int cur_image;
IMG_Header*imgs;
/*10:*/
#line 152 "ico2obj.w"

static uint8_t picInData[256*256/2];



static uint8_t picOutData[256*256/4];
int i,j;
/*:10*/
#line 123 "ico2obj.w"


imgs= (IMG_Header*)malloc(sizeof(IMG_Header)*hdr->imagesCount);

if(imgs==NULL){
PRINTERR("No memory for image directory of %s.\n",config.picnames[cur_input]);
return;
}

if(fread(imgs,sizeof(IMG_Header),hdr->imagesCount,fpic)!=hdr->imagesCount){
PRINTERR("Can't read image directory of %s.\n",config.picnames[cur_input]);
free(imgs);
return;
}

for(cur_image= 0;cur_image<hdr->imagesCount;++cur_image){
if(imgs[cur_image].bpp!=4){
PRINTERR("Bad bits per pixel (%d) for image %d of %s.\n",
imgs[cur_image].bpp,cur_image,config.picnames[cur_input]);
continue;
}
/*11:*/
#line 159 "ico2obj.w"

PRINTVERB(2,"Image:%d, w:%d, h:%d, colors:%d, planes:%d, bpp:%d,"
" size:%d, offset:%x\n",cur_image,
imgs[cur_image].width,imgs[cur_image].height,
imgs[cur_image].colors,imgs[cur_image].planes,imgs[cur_image].bpp,
imgs[cur_image].size,imgs[cur_image].offset);

/*:11*/
#line 144 "ico2obj.w"

}

free(imgs);
}

/*:9*//*14:*/
#line 184 "ico2obj.w"

static void
write_block(uint8_t*data,uint16_t data_len){
uint8_t chksum;
uint16_t len;

len= data_len+4;
chksum= 0;

fputc(1,fobj);
fputc(0,fobj);
chksum-= 1;

fwrite(&len,sizeof(len),1,fobj);
chksum-= len&0xff;
chksum-= (len&0xff00)>>8;

fwrite(data,data_len,1,fobj);
for(;data_len> 0;--data_len){
chksum-= *data++;
}
fputc(chksum,fobj);
}

/*:14*//*15:*/
#line 209 "ico2obj.w"

static void
write_endmod(void){
uint8_t buf[2];

buf[0]= 6;
buf[1]= 0;

write_block(buf,sizeof(buf));
}

/*:15*//*16:*/
#line 221 "ico2obj.w"

static void
write_endgsd(void){
uint8_t buf[2];

buf[0]= 2;
buf[1]= 0;

write_block(buf,sizeof(buf));
}

/*:16*//*17:*/
#line 234 "ico2obj.w"

static void
write_initial_gsd(void){
uint16_t buf[50];

buf[0]= 1;


buf[1]= toRadix50(" PI");
buf[2]= toRadix50("C$$");
buf[3]= buf[4]= 0;

write_block((uint8_t*)buf,4*2);
}

/*:17*//*21:*/
#line 270 "ico2obj.w"

uint16_t toRadix50(char*str){
static char radtbl[]= " ABCDEFGHIJKLMNOPQRSTUVWXYZ$. 0123456789";
uint32_t acc;
char*rp;

acc= 0;

if(*str==0){
return(acc);
}
rp= strchr(radtbl,toupper(*str));
if(rp==NULL){
return(acc);
}
acc+= ((uint32_t)(rp-radtbl))*03100;
++str;

if(*str==0){
return(acc);
}
rp= strchr(radtbl,toupper(*str));
if(rp==NULL){
return(acc);
}
acc+= ((uint32_t)(rp-radtbl))*050;
++str;

if(*str==0){
return(acc);
}
rp= strchr(radtbl,toupper(*str));
if(rp==NULL){
return(acc);
}
acc+= ((uint32_t)(rp-radtbl));

return(acc);
}

/*:21*//*29:*/
#line 380 "ico2obj.w"

static error_t
parse_opt(int key,char*arg,struct argp_state*state){
Arguments*arguments;
arguments= (Arguments*)state->input;
switch(key){
case't':
arguments->transpose= 1;
break;
case'a':
arguments->save= 1;
break;
case'l':
if(strlen(arg)==0||strlen(arg)> 6)
return(ARGP_ERR_UNKNOWN);
strcpy(arguments->label,arg);
break;
case's':
if(strlen(arg)==0||strlen(arg)> 6)
return(ARGP_ERR_UNKNOWN);
strcpy(arguments->section_name,arg);
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
/*:29*/
