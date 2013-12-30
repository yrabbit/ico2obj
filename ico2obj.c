/*1:*/
#line 49 "ico2obj.w"

<<<<<<< HEAD
/*37:*/
#line 664 "ico2obj.w"
=======
/*54:*/
#line 889 "ico2obj.w"
>>>>>>> a6ae7eecc15d10ce3fa82b1a345db8738a938084

#include <string.h> 
#include <stdlib.h> 
#include <libgen.h> 

#ifdef __linux__
#include <stdint.h> 
#endif

#include <argp.h> 

/*:54*/
#line 50 "ico2obj.w"

#define VERSION "0.1" \

#define ERR_SYNTAX 1
#define ERR_CANTOPEN 2
#define ERR_CANTCREATE 3
#define ERR_BADFILEHEADER 4
#define ERR_CANTOPENOBJ 5 \


#line 51 "ico2obj.w"

/*30:*/
#line 529 "ico2obj.w"

const char*argp_program_version= "ico2obj, "VERSION;
const char*argp_program_bug_address= "<yellowrabbit@bk.ru>";

/*:30*//*47:*/
#line 815 "ico2obj.w"

const char*argp_fixpal_program_version= "fix-pal, "VERSION;
const char*argp_fixpal_program_bug_address= "<yellowrabbit@bk.ru>";

/*:47*/
#line 52 "ico2obj.w"

/*5:*/
#line 93 "ico2obj.w"

typedef struct _ICO_Header{
uint16_t zero0;
uint16_t type;
uint16_t imagesCount;
}ICO_Header;

/*:5*//*8:*/
#line 119 "ico2obj.w"

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

/*:8*//*33:*/
#line 568 "ico2obj.w"

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

/*:33*//*50:*/
#line 838 "ico2obj.w"

typedef struct _fixpal_Arguments{
int palette;
int verbosity;
char**picnames;

}fixpal_Arguments;

/*:50*/
#line 53 "ico2obj.w"

/*14:*/
#line 283 "ico2obj.w"

static void handleOneFile(FILE*,ICO_Header*);
static uint8_t recodeColor(uint8_t);

/*:14*//*26:*/
#line 467 "ico2obj.w"

static void write_block(uint8_t*,uint16_t);
static void write_block_with_header(uint8_t*,uint16_t,uint8_t*,uint8_t);
static void write_endmod(void);
static void write_endgsd(void);
static void write_initial_gsd(void);
static void write_rld(void);
static void write_label(void);
static void write_text(uint8_t*,int);

/*:26*//*28:*/
#line 520 "ico2obj.w"

static uint16_t toRadix50(char*);

/*:28*//*40:*/
#line 703 "ico2obj.w"

static void fixpal_handleOneFile(FILE*,ICO_Header*);

/*:40*/
#line 54 "ico2obj.w"

/*2:*/
#line 78 "ico2obj.w"

static int cur_input;
/*:2*//*15:*/
#line 297 "ico2obj.w"

FILE*fobj;

/*:15*//*21:*/
#line 410 "ico2obj.w"

static int location= 0;
static int label_count= 0;
/*:21*//*31:*/
#line 533 "ico2obj.w"

static char argp_program_doc[]= "Convert ICO images to object file";
static char args_doc[]= "file [...]";

/*:31*//*32:*/
#line 550 "ico2obj.w"

static struct argp_option options[]= {
{"output",'o',"FILENAME",0,"Output filename"},
{"verbose",'v',NULL,0,"Verbose output (-vv --- more debug info)"},
{"section",'s',"SECTION_NAME",0,"Program section name"},
{"attr",'a',NULL,0,"Set program section SAV attribute"},
{"label",'l',"LABEL",0,"Label for images"},
{"trans",'t',NULL,0,"Transpose image (-tt --- transpose by word)"},
{"color0",'0',"COLOR",0,"Color number for bits 00"},
{"color1",'1',"COLOR",0,"Color number for bits 01"},
{"color2",'2',"COLOR",0,"Color number for bits 10"},
{"color3",'3',"COLOR",0,"Color number for bits 11"},
{0}
};
static error_t parse_opt(int,char*,struct argp_state*);
static struct argp argp= {options,parse_opt,args_doc,argp_program_doc};

/*:32*//*34:*/
#line 581 "ico2obj.w"

static Arguments config= {0,{0},{'P','I','C',0,0,0,0},
{'S','P','I','C','T',' ',0},0,NULL,

{0,1,2,3},0,
};


<<<<<<< HEAD
/*:34*//*38:*/
#line 675 "ico2obj.w"
=======
/*:34*//*36:*/
#line 654 "ico2obj.w"

static char prog_name[FILENAME_MAX+1];

/*:36*//*43:*/
#line 723 "ico2obj.w"

static uint32_t bkPalette[16][4]= {
{0,0x0000ff,0x00ff00,0xff0000},
{0,0xffff00,0xff00ff,0xff0000},
{0,0x00ffff,0x00ff00,0xff00ff},
{0,0x00ff00,0x00ffff,0xffff00},
{0,0xff00ff,0x00ffff,0xffffff},
{0,0xffffff,0xffffff,0xffffff},
{0,0xcc0000,0x800000,0xff0000},
{0,0x80ff00,0x00ff00,0xffff00},
{0,0x8000ff,0x3333cc,0xff00ff},
{0,0x80ff00,0x3333cc,0x800000},
{0,0x00ffff,0x00ff00,0xcc0000},
{0,0x00ffff,0xffff00,0xff0000},
{0,0xff0000,0x00ff00,0x00ffff},
{0,0x00ffff,0xffff00,0xffffff},
{0,0xffff00,0x00ff00,0xffffff},
{0,0x00ffff,0x00ff00,0xffffff},
};

/*:43*//*48:*/
#line 819 "ico2obj.w"

static char argp_fixpal_program_doc[]= "Set BK palette in ICO file";
static char args_fixpal_doc[]= "file [...]";

/*:48*//*49:*/
#line 827 "ico2obj.w"

static struct argp_option fixpal_options[]= {
{"palette",'p',"NUM",0,"BK palette number"},
{"verbose",'v',NULL,0,"Verbose output (-vv --- more debug info)"},
{0}
};
static error_t parse_fixpal_opt(int,char*,struct argp_state*);
static struct argp argp_fixpal= {fixpal_options,parse_fixpal_opt,args_fixpal_doc,
argp_fixpal_program_doc};

/*:49*//*51:*/
#line 846 "ico2obj.w"

static fixpal_Arguments fixpal_config= {10,0,NULL};


/*:51*//*55:*/
#line 901 "ico2obj.w"
>>>>>>> a6ae7eecc15d10ce3fa82b1a345db8738a938084

#define PRINTVERB(level, fmt, a...) (((config.verbosity) >= level) ? printf(\
  (fmt), ## a) : 0)
#define PRINTVERBFIX(level, fmt, a...) (((fixpal_config.verbosity) >= level) ? printf(\
  (fmt), ## a) : 0)
#define PRINTERR(fmt, a...) fprintf(stderr, (fmt), ## a)

/*:55*/
#line 55 "ico2obj.w"

int
main(int argc,char*argv[])
{
/*3:*/
#line 80 "ico2obj.w"

FILE*fpic;

/*:3*//*6:*/
#line 100 "ico2obj.w"

ICO_Header hdr;

/*:6*/
#line 59 "ico2obj.w"

const char*picname;

<<<<<<< HEAD
/*36:*/
#line 652 "ico2obj.w"
=======
/*37:*/
#line 657 "ico2obj.w"
>>>>>>> a6ae7eecc15d10ce3fa82b1a345db8738a938084


strncpy(prog_name,argv[0],FILENAME_MAX);
prog_name[FILENAME_MAX]= '\0';

if(strcmp("fix-pal",basename(prog_name))==0){
/*38:*/
#line 677 "ico2obj.w"

/*53:*/
#line 878 "ico2obj.w"

argp_parse(&argp_fixpal,argc,argv,0,0,&fixpal_config);

if(fixpal_config.palette> 15){
PRINTERR("Bad palette number:%d\n",fixpal_config.palette);
return(ERR_SYNTAX);
}
if(fixpal_config.picnames==NULL){
PRINTERR("No input filenames specified\n");
return(ERR_SYNTAX);
}
/*:53*/
#line 678 "ico2obj.w"

while((picname= fixpal_config.picnames[cur_input])!=NULL){
fpic= fopen(picname,"r+");
/*39:*/
#line 687 "ico2obj.w"

if(fpic==NULL){
PRINTERR("Can't open %s\n",picname);
return(ERR_CANTOPEN);
}
if(fread(&hdr,sizeof(hdr),1,fpic)!=1){
PRINTERR("Can't read header of %s\n",picname);
return(ERR_CANTOPEN);
}
if(hdr.zero0!=0||hdr.type!=1||hdr.imagesCount==0){
PRINTERR("Bad file header of %s\n",picname);
return(ERR_BADFILEHEADER);
}
PRINTVERBFIX(1,"Handle file: %s.\n",picname);
PRINTVERBFIX(2,"Images count: %d.\n",hdr.imagesCount);

/*:39*/
#line 681 "ico2obj.w"

fixpal_handleOneFile(fpic,&hdr);
fclose(fpic);
++cur_input;
}

/*:38*/
#line 663 "ico2obj.w"

return(0);
}
argp_parse(&argp,argc,argv,0,0,&config);

if(strlen(config.output_filename)==0){
PRINTERR("No output filename specified\n");
return(ERR_SYNTAX);
}
if(config.picnames==NULL){
PRINTERR("No input filenames specified\n");
return(ERR_SYNTAX);
}
/*:37*/
#line 62 "ico2obj.w"



cur_input= 0;
/*24:*/
#line 449 "ico2obj.w"

fobj= fopen(config.output_filename,"w");
if(fobj==NULL){
PRINTERR("Can't open %s.\n",config.output_filename);
return(ERR_CANTOPENOBJ);
}

fseek(fobj,9*2+5,SEEK_SET);
write_rld();

/*:24*/
#line 66 "ico2obj.w"

while((picname= config.picnames[cur_input])!=NULL){
/*4:*/
#line 83 "ico2obj.w"

fpic= fopen(picname,"r");
if(fpic==NULL){
PRINTERR("Can't open %s\n",picname);
return(ERR_CANTOPEN);
}
/*7:*/
#line 103 "ico2obj.w"

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
#line 89 "ico2obj.w"



/*:4*/
#line 68 "ico2obj.w"

handleOneFile(fpic,&hdr);
fclose(fpic);
++cur_input;
}
/*25:*/
#line 459 "ico2obj.w"

write_endgsd();
write_endmod();

fseek(fobj,0,SEEK_SET);
write_initial_gsd();
fclose(fobj);

/*:25*/
#line 73 "ico2obj.w"

return(0);
}

/*:1*//*9:*/
#line 132 "ico2obj.w"

static void
handleOneFile(FILE*fpic,ICO_Header*hdr){
int cur_image;
IMG_Header*imgs;



int img_width,img_height;

/*10:*/
#line 189 "ico2obj.w"

static uint8_t picInData[256*256/2];



static uint8_t picOutData[256*256/4];
int i,j,k;
uint8_t acc;

/*:10*/
#line 142 "ico2obj.w"


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
img_width= imgs[cur_image].width;
if(img_width==0){
img_width= 256;
}
img_height= imgs[cur_image].height;
if(img_height==0){
img_height= 256;
}
if(imgs[cur_image].bpp!=4){
PRINTERR("Bad bits per pixel (%d) for image %d of %s.\n",
imgs[cur_image].bpp,cur_image,config.picnames[cur_input]);
continue;
}
if(img_width%4!=0){
PRINTERR("Bad width (%d) for image %d of %s.\n",
img_width,cur_image,config.picnames[cur_input]);
continue;
}
if(imgs[cur_image].size+location> 0xffff){
PRINTERR("Section size (%d) too big for image %d of %s.\n",
imgs[cur_image].size+location,cur_image,config.picnames[cur_input]);
continue;
}
/*11:*/
#line 198 "ico2obj.w"

PRINTVERB(2,"Image:%d, w:%d, h:%d, colors:%d, planes:%d, bpp:%d,"
" size:%d, offset:%x\n",cur_image,
img_width,img_height,
imgs[cur_image].colors,imgs[cur_image].planes,imgs[cur_image].bpp,
imgs[cur_image].size,imgs[cur_image].offset);
write_label();
fseek(fpic,imgs[cur_image].offset+40+16*4,SEEK_SET);
fread(picInData,imgs[cur_image].size,1,fpic);
/*:11*//*12:*/
#line 208 "ico2obj.w"

k= 0;
if(config.transpose==0){
for(i= img_height-1;i>=0;--i){
for(j= 0;j<img_width/2;++j){
acc= 0;
acc+= recodeColor(picInData[i*img_width/
2+j]&0xf)<<2;
acc+= recodeColor((picInData[i*img_width/
2+j]&0xf0)>>4);
++j;
acc+= recodeColor(picInData[i*img_width/
2+j]&0xf)<<6;
acc+= recodeColor((picInData[i*img_width/
2+j]&0xf0)>>4)<<4;
picOutData[k++]= acc;
}
}
}else if(config.transpose==1){
for(j= 0;j<img_width/2;j+= 2){
for(i= img_height-1;i>=0;--i){
acc= 0;
acc+= recodeColor(picInData[i*img_width/
2+j]&0xf)<<2;
acc+= recodeColor((picInData[i*img_width/
2+j]&0xf0)>>4);
acc+= recodeColor(picInData[i*img_width/
2+j+1]&0xf)<<6;
acc+= recodeColor((picInData[i*img_width/
2+j+1]&0xf0)>>4)<<4;
picOutData[k++]= acc;
}
}
}else{
for(j= 0;j<img_width/2;j+= 4){
for(i= img_height-1;i>=0;--i){
acc= 0;
acc+= recodeColor(picInData[i*img_width/
2+j]&0xf)<<2;
acc+= recodeColor((picInData[i*img_width/
2+j]&0xf0)>>4);
acc+= recodeColor(picInData[i*img_width/
2+j+1]&0xf)<<6;
acc+= recodeColor((picInData[i*img_width/
2+j+1]&0xf0)>>4)<<4;
picOutData[k++]= acc;

acc= 0;
acc+= recodeColor(picInData[i*img_width/
2+j+2]&0xf)<<2;
acc+= recodeColor((picInData[i*img_width/
2+j+2]&0xf0)>>4);
acc+= recodeColor(picInData[i*img_width/
2+j+3]&0xf)<<6;
acc+= recodeColor((picInData[i*img_width/
2+j+3]&0xf0)>>4)<<4;
picOutData[k++]= acc;
}
}
}
write_text(picOutData,k);
/*:12*/
#line 181 "ico2obj.w"

}

free(imgs);
}

/*:9*//*13:*/
#line 270 "ico2obj.w"

static uint8_t
recodeColor(uint8_t col){
int i;

for(i= 0;i<4;++i){
if(col==config.colors[i]){
return(i);
}
}
return(0);
}

/*:13*//*16:*/
#line 302 "ico2obj.w"

static void
write_block_with_header(uint8_t*data,uint16_t data_len,uint8_t*hdr,uint8_t hdr_len){
uint8_t chksum;
uint16_t len;

len= data_len+hdr_len+4;
chksum= 0;

fputc(1,fobj);
fputc(0,fobj);
chksum-= 1;

fwrite(&len,sizeof(len),1,fobj);
chksum-= len&0xff;
chksum-= (len&0xff00)>>8;

if(hdr_len!=0){
fwrite(hdr,hdr_len,1,fobj);
for(;hdr_len> 0;--hdr_len){
chksum-= *hdr++;
}
}

fwrite(data,data_len,1,fobj);
for(;data_len> 0;--data_len){
chksum-= *data++;
}
fputc(chksum,fobj);
}

static void
write_block(uint8_t*data,uint16_t data_len){
write_block_with_header(data,data_len,NULL,0);
}

/*:16*//*17:*/
#line 339 "ico2obj.w"

static void
write_endmod(void){
uint8_t buf[2];

buf[0]= 6;
buf[1]= 0;

write_block(buf,sizeof(buf));
}

/*:17*//*18:*/
#line 351 "ico2obj.w"

static void
write_endgsd(void){
uint8_t buf[2];

buf[0]= 2;
buf[1]= 0;

write_block(buf,sizeof(buf));
}

/*:18*//*19:*/
#line 365 "ico2obj.w"

static void
write_initial_gsd(void){
uint16_t buf[9];

buf[0]= 1;


buf[1]= toRadix50(" PI");
buf[2]= toRadix50("C$$");
buf[3]= buf[4]= 0;


buf[5]= toRadix50(config.section_name);
buf[6]= toRadix50(config.section_name+3);

buf[7]= 0x500+040+config.save;

buf[8]= location;

write_block((uint8_t*)buf,9*2);
}

/*:19*//*20:*/
#line 389 "ico2obj.w"

static void
write_rld(void){
uint8_t buf[2];

buf[0]= 4;
buf[1]= 0;

buf[2]= 7;
buf[3]= 0;

((uint16_t*)(buf+4))[0]= toRadix50(config.section_name);
((uint16_t*)(buf+4))[1]= toRadix50(config.section_name+3);

buf[8]= buf[9]= 0;
write_block((uint8_t*)buf,10);
}

/*:20*//*22:*/
#line 414 "ico2obj.w"

static void
write_label(void){
uint16_t buf[5];
char name[7],label[7];
int len;

buf[0]= 1;


snprintf(label,6,"%d",label_count++);
len= strlen(label);
strcpy(name,config.label);
name[6-len]= '\0';
strcat(name,label);

buf[1]= toRadix50(name);
buf[2]= toRadix50(name+3);
buf[3]= 0150+4*256;
buf[4]= location;
write_block((uint8_t*)buf,5*2);
}

/*:22*//*23:*/
#line 438 "ico2obj.w"

static void
write_text(uint8_t*data,int len){
uint16_t hdr[2];

hdr[0]= 3;
hdr[1]= location;
write_block_with_header(data,len,(uint8_t*)hdr,4);
location+= len;
}

/*:23*//*27:*/
#line 480 "ico2obj.w"

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

/*:27*//*35:*/
#line 591 "ico2obj.w"

static error_t
parse_opt(int key,char*arg,struct argp_state*state){
Arguments*arguments;
arguments= (Arguments*)state->input;
switch(key){
case't':
++arguments->transpose;
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
/*:35*//*46:*/
#line 764 "ico2obj.w"

static void
fixpal_handleOneFile(FILE*fpic,ICO_Header*hdr){
int cur_image;
IMG_Header*imgs;



int img_width,img_height;

/*45:*/
#line 755 "ico2obj.w"

static uint8_t picInData[256*256/2];



static uint32_t picPalette[16];
int i;

/*:45*/
#line 774 "ico2obj.w"


imgs= (IMG_Header*)malloc(sizeof(IMG_Header)*hdr->imagesCount);

if(imgs==NULL){
PRINTERR("No memory for image directory of %s.\n",config.picnames[cur_input]);
return;
}

if(fread(imgs,sizeof(IMG_Header),hdr->imagesCount,fpic)!=hdr->imagesCount){
PRINTERR("Can't read image directory of %s.\n",fixpal_config.picnames[cur_input]);
free(imgs);
return;
}

for(cur_image= 0;cur_image<hdr->imagesCount;++cur_image){
img_width= imgs[cur_image].width;
if(img_width==0){
img_width= 256;
}
img_height= imgs[cur_image].height;
if(img_height==0){
img_height= 256;
}
if(imgs[cur_image].bpp!=4){
PRINTERR("Bad bits per pixel (%d) for image %d of %s.\n",
imgs[cur_image].bpp,cur_image,fixpal_config.picnames[cur_input]);
continue;
}
if(img_width%4!=0){
PRINTERR("Bad width (%d) for image %d of %s.\n",
img_width,cur_image,fixpal_config.picnames[cur_input]);
continue;
}
/*41:*/
#line 706 "ico2obj.w"

PRINTVERBFIX(2,"Image:%d, w:%d, h:%d, colors:%d, planes:%d, bpp:%d,"
" size:%d, offset:%x\n",cur_image,
img_width,img_height,
imgs[cur_image].colors,imgs[cur_image].planes,imgs[cur_image].bpp,
imgs[cur_image].size,imgs[cur_image].offset);
fseek(fpic,imgs[cur_image].offset+40+16*4,SEEK_SET);
fread(picInData,imgs[cur_image].size,1,fpic);
/*:41*//*42:*/
#line 716 "ico2obj.w"

for(i= 0;i<imgs[cur_image].size;++i){
picInData[i]&= 0x33;
}
fseek(fpic,imgs[cur_image].offset+40+16*4,SEEK_SET);
fwrite(picInData,imgs[cur_image].size,1,fpic);
/*:42*//*44:*/
#line 743 "ico2obj.w"

fseek(fpic,imgs[cur_image].offset+40,SEEK_SET);
fread(picPalette,16*sizeof(uint32_t),1,fpic);
for(i= 0;i<4;++i){
picPalette[i]= bkPalette[fixpal_config.palette][i];
}
for(;i<16;++i){
picPalette[i]= 0;
}
fseek(fpic,imgs[cur_image].offset+40,SEEK_SET);
fwrite(picPalette,16*sizeof(uint32_t),1,fpic);

/*:44*/
#line 808 "ico2obj.w"

}

free(imgs);
}

/*:46*//*52:*/
#line 852 "ico2obj.w"

static error_t
parse_fixpal_opt(int key,char*arg,struct argp_state*state){
fixpal_Arguments*arguments;
arguments= (fixpal_Arguments*)state->input;
switch(key){
case'v':
++arguments->verbosity;
break;
case'p':
arguments->palette= atoi(arg);
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
/*:52*/
