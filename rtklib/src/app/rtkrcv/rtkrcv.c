/*------------------------------------------------------------------------------
* rtkrcv.c : rtk-gps/gnss receiver console ap
*
*          Copyright (C) 2009-2011 by T.TAKASU, All rights reserved.
*
* notes   :
*     current version does not support win32 without pthread library
*
* version : $Revision:$ $Date:$
* history : 2009/12/13 1.0  new
*           2010/07/18 1.1  add option -m
*           2010/08/12 1.2  fix bug on ftp/http
*           2011/01/22 1.3  add option misc-proxyaddr,misc-fswapmargin
*           2011/08/19 1.4  fix bug on size of arg solopt arg for rtksvrstart()
*-----------------------------------------------------------------------------*/
#ifndef WIN32
#define _POSIX_C_SOURCE 2
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#endif
#include "rtklib.h"

static const char rcsid[]="$Id:$";

#define PRGNAME     "rtkrcv"            /* program name */
#define CMDPROMPT   "rtkrcv> "          /* command prompt */
#define MAXARG      10                  /* max number of args in a command */
#define MAXCMD      256                 /* max length of a command */
#define MAXSTR      1024                /* max length of a string */
#define MAXBUFF     1024                /* max input buffer */
#define MAXRCVCMD   4096                /* max receiver command */
#define OPTSDIR     "."                 /* default config directory */
#define OPTSFILE    "rtkrcv.conf"       /* default config file */
#define NAVIFILE    "rtkrcv.nav"        /* navigation save file */
#define STATFILE    "rtkrcv.stat"       /* solution status file */
#define TRACEFILE   "rtkrcv.trace"      /* trace file */
#define INTKEEPALIVE 1000               /* keep alive interval (ms) */

#define NUL         (char)0x00          /* nul */
#define BS          (char)0x08          /* backspace */
#define CR          (char)0x0D          /* cr */
#define LF          (char)0x0A          /* lf/nl */
#define DEL         (char)0x7F          /* del */
#define ESC         (char)0x1B          /* escape */
#define CTRC        (char)0x03          /* ctr-C */
#define ECHO        (char)1             /* telnet echo */           
#define SUPPGA      (char)3             /* telnet suppress go ahead */          
#define BRK         (char)243           /* telnet break */          
#define IP          (char)244           /* telnet interrupt */
#define WILL        (char)251           /* telnet option negotiation */
#define WONT        (char)252           /* telnet option negotiation */
#define DO          (char)253           /* telnet option negotiation */
#define DONT        (char)254           /* telnet option negotiation */
#define IAC         (char)255           /* telnet interpret as command */

#define MIN(x,y)    ((x)<(y)?(x):(y))
#define MAX(x,y)    ((x)>(y)?(x):(y))
#define SQRT(x)     ((x)<=0.0?0.0:sqrt(x))

/* type definition -----------------------------------------------------------*/
typedef struct {                        /* virtual terminal type */
    int type;                           /* type (0:stdio,1:remote,2:device) */
    int state;                          /* state(0:close,1:open) */
    int in,out;                         /* input/output file descriptor */
    int nbuff;                          /* number of data */
    char buff[MAXBUFF];                 /* input buffer */
    pthread_t svr;                      /* input server */
    pthread_t parent;                   /* parent thread */
    pthread_mutex_t lock;               /* lock flag */
    pthread_cond_t event;               /* event flag */
} vt_t;

/* function prototypes -------------------------------------------------------*/
extern int pthread_kill(pthread_t, int);
extern FILE *popen(const char *, const char *);
extern int pclose(FILE *);

/* global variables ----------------------------------------------------------*/
static rtksvr_t svr;                    /* rtk server struct */
static stream_t moni;                   /* monitor stream */

static int intflg       =0;             /* interrupt flag (2:shtdown) */
static int echomode     =1;             /* echo mode */
static FILE *logfp      =NULL;          /* log file pointer */

static char passwd[MAXSTR]="admin";     /* login password */
static int timetype     =0;             /* time format (0:gpst,1:utc,2:jst,3:tow) */
static int soltype      =0;             /* sol format (0:dms,1:deg,2:xyz,3:enu,4:pyl) */
static int solflag      =2;             /* sol flag (1:std+2:age/ratio/ns) */
static int strtype[]={                  /* stream types */
    STR_SERIAL,STR_NONE,STR_NONE,STR_NONE,STR_NONE,STR_NONE,STR_NONE,STR_NONE
};
static char strpath[8][MAXSTR]={""};    /* stream paths */
static int strfmt[]={                   /* stream formats */
    STRFMT_UBX,STRFMT_RTCM3,STRFMT_SP3,SOLF_LLH,SOLF_NMEA
};
static int svrcycle     =10;            /* server cycle (ms) */
static int timeout      =10000;         /* timeout time (ms) */
static int reconnect    =10000;         /* reconnect interval (ms) */
static int nmeacycle    =5000;          /* nmea request cycle (ms) */
static int buffsize     =32768;         /* input buffer size (bytes) */
static int navmsgsel    =0;             /* navigation mesaage select */
static char proxyaddr[256]="";          /* http/ntrip proxy */
static int nmeareq      =0;             /* nmea request type (0:off,1:lat/lon,2:single) */
static double nmeapos[] ={0,0};         /* nmea position (lat/lon) (deg) */
static char rcvcmds[3][MAXSTR]={""};    /* receiver commands files */
static char startcmd[MAXSTR]="";        /* start command */
static char stopcmd [MAXSTR]="";        /* stop command */
static int modflgr[256] ={0};           /* modified flags of receiver options */
static int modflgs[256] ={0};           /* modified flags of system options */
static int moniport     =0;             /* monitor port */
static int keepalive    =0;             /* keep alive flag */
static int fswapmargin  =30;            /* file swap margin (s) */

static prcopt_t prcopt;                 /* processing options */
static solopt_t solopt[2]={{0}};        /* solution options */
static filopt_t filopt  ={""};          /* file options */

/* help text -----------------------------------------------------------------*/
static const char *helptxt[]={
    " start            : start rtk server",
    " stop             : stop rtk server",
    " restart          : restart rtk sever",
    " solution [cycle] : show solution",
    " status [cycle]   : show rtk status",
    " satellite [cycle]: show satellite status",
    " observ [cycle]   : show observation data",
    " navidata [cycle] : show navigation data",
    " stream [cycle]   : show stream status",
    " error            : show error/warning messages",
    " option [opt]     : show option(s)",
    " set opt [val]    : set option",
    " load [file]      : load options from file",
    " save [file]      : save options to file",
    " log [file|off]   : start/stop log to file",
    " help|? [path]    : print help",
    " exit             : exit and logout console",
    " shutdown         : shutdown rtk server",
    " !command [arg...]: execute command in shell",
    ""
};
static const char *pathopts[]={         /* path options help */
    "stream path formats",
    " serial   : port[:bit_rate[:byte[:parity(n|o|e)[:stopb[:fctr(off|on)]]]]]",
    " file     : path[::T[::+offset][::xspeed]]",
    " tcpsvr   : :port",
    " tcpcli   : addr:port",
    " ntripsvr : user:passwd@addr:port/mntpnt[:str]",
    " ntripcli : user:passwd@addr:port/mntpnt",
    " ftp      : user:passwd@addr/path[::T=poff,tint,off,rint]",
    " http     : addr/path[::T=poff,tint,off,rint]",
    ""
};
/* receiver options table ----------------------------------------------------*/
#define TIMOPT  "0:gpst,1:utc,2:jst,3:tow"
#define CONOPT  "0:dms,1:deg,2:xyz,3:enu,4:pyl"
#define FLGOPT  "0:off,1:std+2:age/ratio/ns"
#define ISTOPT  "0:off,1:serial,2:file,3:tcpsvr,4:tcpcli,7:ntripcli,8:ftp,9:http"
#define OSTOPT  "0:off,1:serial,2:file,3:tcpsvr,4:tcpcli,6:ntripsvr"
#define FMTOPT  "0:rtcm2,1:rtcm3,2:oem4,3:oem3,4:ubx,5:ss2,6:hemis,7:skytraq,8:gw10,9:javad,15:sp3"
#define NMEOPT  "0:off,1:latlon,2:single"
#define SOLOPT  "0:llh,1:xyz,2:enu,3:nmea"
#define MSGOPT  "0:all,1:rover,2:base,3:corr"

static opt_t rcvopts[]={
    {"console-passwd",  2,  (void *)passwd,              ""     },
    {"console-timetype",3,  (void *)&timetype,           TIMOPT },
    {"console-soltype", 3,  (void *)&soltype,            CONOPT },
    {"console-solflag", 0,  (void *)&solflag,            FLGOPT },
    
    {"inpstr1-type",    3,  (void *)&strtype[0],         ISTOPT },
    {"inpstr2-type",    3,  (void *)&strtype[1],         ISTOPT },
    {"inpstr3-type",    3,  (void *)&strtype[2],         ISTOPT },
    {"inpstr1-path",    2,  (void *)strpath [0],         ""     },
    {"inpstr2-path",    2,  (void *)strpath [1],         ""     },
    {"inpstr3-path",    2,  (void *)strpath [2],         ""     },
    {"inpstr1-format",  3,  (void *)&strfmt [0],         FMTOPT },
    {"inpstr2-format",  3,  (void *)&strfmt [1],         FMTOPT },
    {"inpstr3-format",  3,  (void *)&strfmt [2],         FMTOPT },
    {"inpstr2-nmeareq", 3,  (void *)&nmeareq,            NMEOPT },
    {"inpstr2-nmealat", 1,  (void *)&nmeapos[0],         "deg"  },
    {"inpstr2-nmealon", 1,  (void *)&nmeapos[1],         "deg"  },
    {"outstr1-type",    3,  (void *)&strtype[3],         OSTOPT },
    {"outstr2-type",    3,  (void *)&strtype[4],         OSTOPT },
    {"outstr1-path",    2,  (void *)strpath [3],         ""     },
    {"outstr2-path",    2,  (void *)strpath [4],         ""     },
    {"outstr1-format",  3,  (void *)&strfmt [3],         SOLOPT },
    {"outstr2-format",  3,  (void *)&strfmt [4],         SOLOPT },
    {"logstr1-type",    3,  (void *)&strtype[5],         OSTOPT },
    {"logstr2-type",    3,  (void *)&strtype[6],         OSTOPT },
    {"logstr3-type",    3,  (void *)&strtype[7],         OSTOPT },
    {"logstr1-path",    2,  (void *)strpath [5],         ""     },
    {"logstr2-path",    2,  (void *)strpath [6],         ""     },
    {"logstr3-path",    2,  (void *)strpath [7],         ""     },
    
    {"misc-svrcycle",   0,  (void *)&svrcycle,           "ms"   },
    {"misc-timeout",    0,  (void *)&timeout,            "ms"   },
    {"misc-reconnect",  0,  (void *)&reconnect,          "ms"   },
    {"misc-nmeacycle",  0,  (void *)&nmeacycle,          "ms"   },
    {"misc-buffsize",   0,  (void *)&buffsize,           "bytes"},
    {"misc-navmsgsel",  3,  (void *)&navmsgsel,          MSGOPT },
    {"misc-proxyaddr",  2,  (void *)proxyaddr,           ""     },
    {"misc-fswapmargin",0,  (void *)&fswapmargin,        "s"    },
    
    {"misc-startcmd",   2,  (void *)startcmd,            ""     },
    {"misc-stopcmd",    2,  (void *)stopcmd,             ""     },
    
    {"file-cmdfile1",   2,  (void *)rcvcmds[0],          ""     },
    {"file-cmdfile2",   2,  (void *)rcvcmds[1],          ""     },
    {"file-cmdfile3",   2,  (void *)rcvcmds[2],          ""     },
    
    {"",0,NULL,""}
};
/* keyboard interrupt handler ------------------------------------------------*/
static void sigint(int sig)
{
    trace(3,"sigint: sig=%d\n",sig);
    
    intflg|=1;
    signal(sig,sigint); /* reset signal handler */
}
/* external stop signal ------------------------------------------------------*/
static void sigshut(int sig)
{
    trace(3,"sigshut: sig=%d\n",sig);
    
    intflg|=2;
}
/* discard space characters at tail ------------------------------------------*/
static void chop(char *str)
{
    char *p;
    for (p=str+strlen(str)-1;p>=str&&!isgraph((int)*p);p--) *p='\0';
}
/* thread to send keep alive for monitor port --------------------------------*/
static void *sendkeepalive(void *arg)
{
    trace(3,"sendkeepalive: start\n");
    
    while (keepalive) {
        strwrite(&moni,(unsigned char *)"\r",1);
        sleepms(INTKEEPALIVE);
    }
    trace(3,"sendkeepalive: stop\n");
    return NULL;
}
/* open monitor port ---------------------------------------------------------*/
static int openmoni(int port)
{
    pthread_t thread;
    char path[64];
    
    trace(3,"openmomi: port=%d\n",port);
    
    sprintf(path,":%d",port);
    if (!stropen(&moni,STR_TCPSVR,STR_MODE_RW,path)) return 0;
    strsettimeout(&moni,timeout,reconnect);
    keepalive=1;
    pthread_create(&thread,NULL,sendkeepalive,NULL);
    return 1;
}
/* close monitor port --------------------------------------------------------*/
static void closemoni(void)
{
    trace(3,"closemoni:\n");
    keepalive=0;
    
    /* send disconnect message */
    strwrite(&moni,(unsigned char *)MSG_DISCONN,strlen(MSG_DISCONN));
    
    /* wait fin from clients */
    sleepms(1000);
    
    strclose(&moni);
}
/* accept client connection --------------------------------------------------*/
static int accsock(int port)
{
    struct sockaddr_in saddr,addr;
    socklen_t len=sizeof(addr);
    int ssock,sock,on=1;
    
    trace(3,"accsock: port=%d\n",port);
    
    if ((ssock=socket(AF_INET,SOCK_STREAM,0))<0) {
        fprintf(stderr,"socket error (%d)\n",errno);
        return -1;
    }
    setsockopt(ssock,SOL_SOCKET,SO_REUSEADDR,(const char *)&on,sizeof(on));
    
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(port);
    
    if (bind(ssock,(struct sockaddr *)&saddr,sizeof(saddr))<0) {
        fprintf(stderr,"bind error (%d)\n",errno);
        close(ssock);
        return -1;
    }       
    listen(ssock,5);
    while ((sock=accept(ssock,(struct sockaddr *)&addr,&len))<0) {
        if (errno==EINTR) continue;
        fprintf(stderr,"accept error (%d)\n",errno);
        close(ssock);
        return -1;
    }
    close(ssock);
    return sock;
}
/* telnet command response ---------------------------------------------------*/
static int telnetrsp(vt_t *vt, char *buff, int n)
{
    char *p=buff,msg[3]={IAC};
    int size=0;
    
    trace(3,"telnetcmd: n=%d\n",n);
    
    buff[n]='\0';
    while ((p=strchr(p,IAC))&&p+1<buff+n) { /* IAC */
        if (*(p+1)==WILL) {
            msg[1]=*(p+2)==ECHO||*(p+2)==SUPPGA?DO:DONT;
            msg[2]=*(p+2);
            if (write(vt->out,msg,3)<3) return 0;
            size=3;
        }
        else if (*(p+1)==DO) {
            msg[1]=*(p+2)==ECHO||*(p+2)==SUPPGA?WILL:WONT;
            msg[2]=*(p+2);
            if (write(vt->out,msg,3)<3) return 0;
            size=3;
        }
        else if (*(p+1)==WONT||*(p+1)==DONT) {
            msg[1]=*(p+1)==WONT?DONT:WONT;
            msg[2]=*(p+2);
            if (write(vt->out,msg,3)<3) return 0;
            size=3;
        }
        else if (*(p+1)==BRK||*(p+1)==IP) { /* break/interrupt */
            kill(0,SIGINT);
            size=2;
        }
        else {
            trace(2,"unsupported cmd: IAC %d\n",(unsigned char)*(p+1));
            fprintf(stderr,"unsupported cmd: IAC %d\n",(unsigned char)*(p+1));
            size=2;
        }
        n-=size;
        memcpy(p,p+size,buff+n-p);
    }
    return n;
}
/* remote echo ---------------------------------------------------------------*/
static int remoteecho(vt_t *vt, char *buff, int n)
{
    const char bs[]={0x08,' ',0x08}; /* backspace seqence */
    
    trace(3,"remoteecho: n=%d\n",n);
    
    if (n<1) return 0;
    else if (buff[n-1]==DEL||buff[n-1]==BS) { /* del or backspace */
        if (vt->nbuff>0) vt->nbuff--; else return 0;
        if (echomode&&write(vt->in,bs,3)<3) return -1;
        return 0;
    }
    else if (buff[n-1]==LF) {
        buff[n-1]=CR;
        buff[n++]=LF;
    }
    else if (buff[n-1]==CR) {
        buff[n++]=LF;
    }
    else if (n>=2&&buff[n-2]==CR&&buff[n-1]==NUL) {
        buff[n-1]=LF;
    }
    else if (buff[n-1]==CTRC) { /* ctr-c */
        kill(0,SIGINT);
        if (echomode&&write(vt->in,"^C",2)<2) return -1;
        return 0;
    }
    else if (n>=2&&buff[0]==ESC) {
        return 0;
    }
    else if (!isprint((int)buff[n-1])) {
        return 0;
    }
    if (echomode&&write(vt->in,buff,n)<n) return -1;
    return n;
}
/* input server thread -------------------------------------------------------*/
static void *inpsvrthread(void *arg)
{
    vt_t *vt=(vt_t *)arg;
    char buff[MAXBUFF];
    int n;
    
    trace(3,"inpsvrthread: start\n");
    
    for (vt->state=1;vt->state;) {
        
        if ((n=read(vt->in,buff,MAXBUFF-vt->nbuff-1))<=0) break;
        
        /* telnet command response */
        n=telnetrsp(vt,buff,n);
        
        /* remote echo */
        if ((n=remoteecho(vt,buff,n))<0) break;
        if (n<=0) continue;
        
        pthread_mutex_lock(&vt->lock);
        memcpy(vt->buff+vt->nbuff,buff,n); vt->nbuff+=n;
        pthread_mutex_unlock(&vt->lock);
        
        if (vt->buff[vt->nbuff-1]=='\n') {
            pthread_cond_signal(&vt->event);
        }
    }
    vt->state=0;
    close(vt->in); 
    pthread_cond_signal(&vt->event); /* release read wait */
    
    trace(3,"inpsvrthread: stop\n");
    return NULL;
}
/* open remote console -------------------------------------------------------*/
static int openremote(vt_t *vt, int port)
{
    const char charmode[]={IAC,WILL,SUPPGA,IAC,WILL,ECHO};
    int sock;
    
    trace(3,"openremote: port=%d\n",port);
    
    pthread_mutex_init(&vt->lock,NULL);
    pthread_cond_init(&vt->event,NULL);
    
    /* accept client connection */
    if ((sock=accsock(port))<0) return 0;
    
    vt->in=vt->out=sock;
    vt->parent=pthread_self();
    
    /* character mode for telnet */
    if (write(vt->out,charmode,6)<6) return 0;
    
    if (pthread_create(&vt->svr,NULL,inpsvrthread,vt)) {
        trace(2,"pthread_create error (%d)\n",errno);
        close(sock);
        return 0;
    }
    return 1;
}
/* close remote consoler -----------------------------------------------------*/
static void closeremote(vt_t *vt)
{
    trace(3,"closeremote:\n");
    
    vt->state=0;
    pthread_kill(vt->svr,SIGINT);
    pthread_join(vt->svr,NULL);
}
/* read remote console -------------------------------------------------------*/
static int readremote(vt_t *vt, char *buff, int nmax)
{
    int n=0;
    
    trace(4,"readremote:\n");
    
    if (!vt->state) return 0;
    pthread_cond_wait(&vt->event,&vt->lock);
    if (vt->state) {
        n=MIN(nmax,vt->nbuff);
        memcpy(buff,vt->buff,n);
        vt->nbuff=0;
    }
    pthread_mutex_unlock(&vt->lock);
    return n;
}
/* write remote console ------------------------------------------------------*/
static int writeremote(vt_t *vt, char *buff, int n)
{
    char *p,*q,crlf[]="\r\n";;
    
    trace(4,"writeremote: n=%d\n",n);
    
    if (!vt->state) return 0;
    
    for (p=buff;p<buff+n;p=q+1) {
        if (!(q=strchr(p,'\n'))) {
            if (write(vt->out,p,buff+n-p)<buff+n-p) return 0;
            break;
        }
        if (write(vt->out,p,q-p)<q-p) return 0;
        if (write(vt->out,crlf,2)<2) return 0;
    }
    return 1;
}
/* open console --------------------------------------------------------------*/
static int openvt(vt_t *vt, int port, const char *dev)
{
    int fd;
    
    trace(3,"openvt: port=%d dev=%s\n",port,dev);
    
    vt->type=vt->state=vt->nbuff=0;
    vt->in =STDIN_FILENO;
    vt->out=STDOUT_FILENO;
    
    if (port) { /* remote console */
        vt->type=1;
        if (!openremote(vt,port)) return 0;
    }
    else if (*dev) { /* serial */
        vt->type=2;
        if ((fd=open(dev,O_RDWR))<0) {
            trace(2,"console device open error: %s\n",dev);
            fprintf(stderr,"console device open error: %s\n",dev);
            return 0;
        }
        vt->in=vt->out=fd;
    }
    vt->state=1;
    return 1;
}
/* close console -------------------------------------------------------------*/
static void closevt(vt_t *vt)
{
    trace(3,"closevt:\n");
    
    if (vt->type==1) { /* remote console */
        closeremote(vt);
    }
    else if (vt->type==2) { /* serial */
        close(vt->in);
    }
    vt->state=0;
}
/* output to console ---------------------------------------------------------*/
static int outvt(vt_t *vt, char *buff, int n)
{
    trace(4,"outvt: n=%d\n",n);
    
    if (logfp) fwrite(buff,n,1,logfp);
    if (vt->type==1) return writeremote(vt,buff,n);
    return write(vt->out,buff,n)==n;
}
/* input line from console ---------------------------------------------------*/
static int inpvt(vt_t *vt, char *buff, int nmax)
{
    int n;
    
    trace(4,"inpvt:\n");
    
    if (vt->type==1) n=readremote(vt,buff,nmax-1);
    else n=read(vt->in,buff,nmax-1);
    if (n<=0) {
        outvt(vt,"\n",1);
        return 0;
    }
    buff[n]='\0';
    if (logfp) fprintf(logfp,"%s",buff);
    chop(buff);
    return 1;
}
/* printf to console ---------------------------------------------------------*/
static int printvt(vt_t *vt, const char *format, ...)
{
    va_list ap;
    int n;
    char buff[MAXSTR];
    
    trace(4,"prvt:\n");
    
    if (!vt->state) {
        va_start(ap,format);
        vfprintf(stderr,format,ap);
        va_end(ap);
        return 1;
    }
    va_start(ap,format);
    n=vsprintf(buff,format,ap);
    va_end(ap);
    return outvt(vt,buff,n);
}
/* confirm overwrite ---------------------------------------------------------*/
static int confwrite(vt_t *vt, const char *file)
{
    FILE *fp;
    char buff[MAXSTR],*p;
    
    strcpy(buff,file);
    if ((p=strstr(buff,"::"))) *p='\0'; /* omit options in path */
    if (!vt->state||!(fp=fopen(buff,"r"))) return 1; /* no existing file */
    fclose(fp);
    printvt(vt,"overwrite %-16s ? (y/n): ",buff);
    if (!inpvt(vt,buff,sizeof(buff))) return 0;
    return toupper((int)buff[0])=='Y';   
}
/* login ---------------------------------------------------------------------*/
static int login(vt_t *vt)
{
    char buff[256];
    
    trace(3,"login: passwd=%s type=%d\n",passwd,vt->type);
    
    if (!*passwd||vt->type==0) return 1;
    echomode=0;
    
    while (!(intflg&2)) {
        if (!printvt(vt,"passwd: ")) return 0;
        if (!inpvt(vt,buff,sizeof(buff))) return 0;
        if (!strcmp(buff,passwd)) break;
        printvt(vt,"\ninvalid password\n");
    }
    printvt(vt,"\n");
    echomode=1;
    return 1;
}
/* read receiver commands ----------------------------------------------------*/
static int readcmd(const char *file, char *cmd, int type)
{
    FILE *fp;
    char buff[MAXSTR],*p=cmd;
    int i=0;
    
    trace(3,"readcmd: file=%s\n",file);
    
    if (!(fp=fopen(file,"r"))) return 0;
    
    while (fgets(buff,sizeof(buff),fp)) {
        if (*buff=='@') i=1;
        else if (i==type&&p+strlen(buff)+1<cmd+MAXRCVCMD) {
            p+=sprintf(p,"%s",buff);
        }
    }
    fclose(fp);
    return 1;
}
/* read antenna file ---------------------------------------------------------*/
static void readant(vt_t *vt, prcopt_t *opt, nav_t *nav)
{
    const pcv_t pcv0={0};
    pcvs_t pcvr={0},pcvs={0};
    pcv_t *pcv;
    gtime_t time=timeget();
    int i;
    
    trace(3,"readant:\n");
    
    opt->pcvr[0]=opt->pcvr[1]=pcv0;
    if (!*filopt.rcvantp) return;
    
    if (readpcv(filopt.rcvantp,&pcvr)) {
        for (i=0;i<2;i++) {
            if (!*opt->anttype[i]) continue;
            if (!(pcv=searchpcv(0,opt->anttype[i],time,&pcvr))) {
                printvt(vt,"no antenna %s in %s",opt->anttype[i],filopt.rcvantp);
                continue;
            }
            opt->pcvr[i]=*pcv;
        }
    }
    else printvt(vt,"antenna file open error %s",filopt.rcvantp);
    
    if (readpcv(filopt.satantp,&pcvs)) {
        for (i=0;i<MAXSAT;i++) {
            if (!(pcv=searchpcv(i+1,"",time,&pcvs))) continue;
            nav->pcvs[i]=*pcv;
        }
    }
    else printvt(vt,"antenna file open error %s",filopt.satantp);
    
    free(pcvr.pcv); free(pcvs.pcv);
}
/* start rtk server ----------------------------------------------------------*/
static int startsvr(vt_t *vt)
{
    double pos[3],npos[3];
    char s[3][MAXRCVCMD]={"","",""},*cmds[]={NULL,NULL,NULL};
    char *ropts[]={"","",""};
    char *paths[]={
        strpath[0],strpath[1],strpath[2],strpath[3],strpath[4],strpath[5],
        strpath[6],strpath[7]
    };
    int i,ret,stropt[8]={0};
    
    trace(3,"startsvr:\n");
    
    /* read start commads from command files */
    for (i=0;i<3;i++) {
        if (!*rcvcmds[i]) continue;
        if (!readcmd(rcvcmds[i],s[i],0)) {
            printvt(vt,"no command file: %s\n",rcvcmds[i]);
        }
        else cmds[i]=s[i];
    }
    /* confirm overwrite */
    for (i=3;i<8;i++) {
        if (strtype[i]==STR_FILE&&!confwrite(vt,strpath[i])) return 0;
    }
    if (prcopt.refpos==4) { /* rtcm */
        for (i=0;i<3;i++) prcopt.rb[i]=0.0;
    }
    pos[0]=nmeapos[0]*D2R;
    pos[1]=nmeapos[1]*D2R;
    pos[2]=0.0;
    pos2ecef(pos,npos);
    
    /* read antenna file */
    readant(vt,&prcopt,&svr.nav);
    
    /* open geoid data file */
    if (solopt[0].geoid>0&&!opengeoid(solopt[0].geoid,filopt.geoid)) {
        trace(2,"geoid data open error: %s\n",filopt.geoid);
        printvt(vt,"geoid data open error: %s\n",filopt.geoid);
    }
    for (i=0;*rcvopts[i].name;i++) modflgr[i]=0;
    for (i=0;*sysopts[i].name;i++) modflgs[i]=0;
    
    /* set stream options */
    stropt[0]=timeout;
    stropt[1]=reconnect;
    stropt[2]=1000;
    stropt[3]=buffsize;
    stropt[4]=fswapmargin;
    strsetopt(stropt);
    
    if (strfmt[2]==8) strfmt[2]=STRFMT_SP3;
    
    /* set ftp/http directory and proxy */
    strsetdir(filopt.tempdir);
    strsetproxy(proxyaddr);
    
    /* execute start command */
    if (*startcmd&&(ret=system(startcmd))) {
        trace(2,"command exec error: %s (%d)\n",startcmd,ret);
        printvt(vt,"command exec error: %s (%d)\n",startcmd,ret);
    }
    /* start rtk server */
    if (!rtksvrstart(&svr,svrcycle,buffsize,strtype,paths,strfmt,navmsgsel,
                     cmds,ropts,nmeacycle,nmeareq,npos,&prcopt,solopt,&moni)) {
        trace(2,"rtk server start error\n");
        printvt(vt,"rtk server start error\n");
        return 0;
    }
    return 1;
}
/* stop rtk server -----------------------------------------------------------*/
static void stopsvr(vt_t *vt)
{
    char s[3][MAXRCVCMD]={"","",""},*cmds[]={NULL,NULL,NULL};
    int i,ret;
    
    trace(3,"stopsvr:\n");
    
    if (!svr.state) return;
    
    /* read stop commads from command files */
    for (i=0;i<3;i++) {
        if (!*rcvcmds[i]) continue;
        if (!readcmd(rcvcmds[i],s[i],1)) {
            printvt(vt,"no command file: %s\n",rcvcmds[i]);
        }
        else cmds[i]=s[i];
    }
    /* stop rtk server */
    rtksvrstop(&svr,cmds);
    
    /* execute stop command */
    if (*stopcmd&&(ret=system(stopcmd))) {
        trace(2,"command exec error: %s (%d)\n",stopcmd,ret);
        printvt(vt,"command exec error: %s (%d)\n",stopcmd,ret);
    }
    if (solopt[0].geoid>0) closegeoid();
}
/* print time ----------------------------------------------------------------*/
static void prtime(vt_t *vt, gtime_t time)
{
    double tow;
    int week;
    char tstr[64]="";
    
    if (timetype==1) {
        time2str(gpst2utc(time),tstr,1);
    }
    else if (timetype==2) {
        time2str(timeadd(gpst2utc(time),9*3600.0),tstr,1);
    }
    else if (timetype==3) {
        tow=time2gpst(time,&week); sprintf(tstr,"  %04d %8.1f",week,tow);
    }
    else time2str(time,tstr,1);
    printvt(vt,"%s ",tstr);
}
/* print solution ------------------------------------------------------------*/
static void prsolution(vt_t *vt, const sol_t *sol, const double *rb)
{
    const char *solstr[]={"------","FIX","FLOAT","SBAS","DGPS","SINGLE","PPP",""};
    double pos[3]={0},Qr[9],Qe[9]={0},dms1[3]={0},dms2[3]={0},bl[3]={0};
    double enu[3]={0},pitch=0.0,yaw=0.0,len;
    int i;
    
    trace(4,"prsolution:\n");
    
    if (sol->time.time==0||!sol->stat) return;
    prtime(vt,sol->time);
    printvt(vt,"(%-6s)",solstr[sol->stat]);
    
    if (norm(sol->rr,3)>0.0&&norm(rb,3)>0.0) {
        for (i=0;i<3;i++) bl[i]=sol->rr[i]-rb[i];
    }
    len=norm(bl,3);
    Qr[0]=sol->qr[0];
    Qr[4]=sol->qr[1];
    Qr[8]=sol->qr[2];
    Qr[1]=Qr[3]=sol->qr[3];
    Qr[5]=Qr[7]=sol->qr[4];
    Qr[2]=Qr[6]=sol->qr[5];
    
    if (soltype==0) {
        if (norm(sol->rr,3)>0.0) {
            ecef2pos(sol->rr,pos);
            covenu(pos,Qr,Qe);
            deg2dms(pos[0]*R2D,dms1);
            deg2dms(pos[1]*R2D,dms2);
            if (solopt[0].height==1) pos[2]-=geoidh(pos); /* geodetic */
        }       
        printvt(vt," %s:%2.0f %02.0f %07.4f",pos[0]<0?"S":"N",fabs(dms1[0]),dms1[1],dms1[2]);
        printvt(vt," %s:%3.0f %02.0f %07.4f",pos[1]<0?"W":"E",fabs(dms2[0]),dms2[1],dms2[2]);
        printvt(vt," H:%8.3f",pos[2]);
        if (solflag&1) {
            printvt(vt," (N:%6.3f E:%6.3f U:%6.3f)",SQRT(Qe[4]),SQRT(Qe[0]),SQRT(Qe[8]));
        }
    }
    else if (soltype==1) {
        if (norm(sol->rr,3)>0.0) {
            ecef2pos(sol->rr,pos);
            covenu(pos,Qr,Qe);
            if (solopt[0].height==1) pos[2]-=geoidh(pos); /* geodetic */
        }       
        printvt(vt," %s:%11.8f",pos[0]<0.0?"S":"N",fabs(pos[0])*R2D);
        printvt(vt," %s:%12.8f",pos[1]<0.0?"W":"E",fabs(pos[1])*R2D);
        printvt(vt," H:%8.3f",pos[2]);
        if (solflag&1) {
            printvt(vt," (E:%6.3f N:%6.3f U:%6.3fm)",SQRT(Qe[0]),SQRT(Qe[4]),SQRT(Qe[8]));
        }
    }
    else if (soltype==2) {
        printvt(vt," X:%12.3f",sol->rr[0]);
        printvt(vt," Y:%12.3f",sol->rr[1]);
        printvt(vt," Z:%12.3f",sol->rr[2]);
        if (solflag&1) {
            printvt(vt," (X:%6.3f Y:%6.3f Z:%6.3f)",SQRT(Qr[0]),SQRT(Qr[4]),SQRT(Qr[8]));
        }
    }
    else if (soltype==3) {
        if (len>0.0) {
            ecef2pos(rb,pos);
            ecef2enu(pos,bl,enu);
            covenu(pos,Qr,Qe);
        }       
        printvt(vt," E:%12.3f",enu[0]);
        printvt(vt," N:%12.3f",enu[1]);
        printvt(vt," U:%12.3f",enu[2]);
        if (solflag&1) {
            printvt(vt," (E:%6.3f N:%6.3f U:%6.3f)",SQRT(Qe[0]),SQRT(Qe[4]),SQRT(Qe[8]));
        }
    }
    else if (soltype==4) {
        if (len>0.0) {
            ecef2pos(rb,pos);
            ecef2enu(pos,bl,enu);
            covenu(pos,Qr,Qe);
            pitch=asin(enu[2]/len);
            yaw=atan2(enu[0],enu[1]); if (yaw<0.0) yaw+=2.0*PI;
        }
        printvt(vt," P:%12.3f",pitch*R2D);
        printvt(vt," Y:%12.3f",yaw*R2D);
        printvt(vt," L:%12.3f",len);
        if (solflag&1) {
            printvt(vt," (E:%6.3f N:%6.3f U:%6.3f)",SQRT(Qe[0]),SQRT(Qe[4]),SQRT(Qe[8]));
        }
    }
    if (solflag&2) {
        printvt(vt," A:%4.1f R:%5.1f N:%2d",sol->age,sol->ratio,sol->ns);
    }
    printvt(vt,"\n");
}
/* print status --------------------------------------------------------------*/
static void prstatus(vt_t *vt)
{
    rtk_t rtk;
    const char *svrstate[]={"stop","run"};
    const char *sol[]={"-","fix","float","SBAS","DGPS","single","PPP",""};
    const char *mode[]={
         "single","DGPS","kinematic","static","moving-base","fixed",
         "PPP-kinema","PPP-static"
    };
    const char *freq[]={"-","L1","L1+L2","L1+L2+L5","","",""};
    rtcm_t rtcm[2];
    int i,j,n,thread,cycle,state,rtkstat,nsat0,nsat1,prcout;
    int cputime,nb[3]={0},nmsg[3][10]={{0}};
    char tstr[64],s[1024],*p;
    double runtime,rt[3]={0},dop[4]={0},rr[3],bl1=0.0,bl2=0.0;
    double azel[MAXSAT*2],pos[3],vel[3];
    
    trace(4,"prstatus:\n");
    
    rtksvrlock(&svr);
    rtk=svr.rtk;
    thread=(int)svr.thread;
    cycle=svr.cycle;
    state=svr.state;
    rtkstat=svr.rtk.sol.stat;
    nsat0=svr.obs[0][0].n;
    nsat1=svr.obs[1][0].n;
    cputime=svr.cputime;
    prcout=svr.prcout;
    for (i=0;i<3;i++) nb[i]=svr.nb[i];
    for (i=0;i<3;i++) for (j=0;j<10;j++) {
        nmsg[i][j]=svr.nmsg[i][j];
    }
    if (svr.state) {
        runtime=(double)(tickget()-svr.tick)/1000.0;
        rt[0]=floor(runtime/3600.0); runtime-=rt[0]*3600.0;
        rt[1]=floor(runtime/60.0); rt[2]=runtime-rt[1]*60.0;
    }
    for (i=0;i<2;i++) rtcm[i]=svr.rtcm[i];
    rtksvrunlock(&svr);
    
    for (i=n=0;i<MAXSAT;i++) {
        if (rtk.opt.mode==PMODE_SINGLE&&!rtk.ssat[i].vs) continue;
        if (rtk.opt.mode!=PMODE_SINGLE&&!rtk.ssat[i].vsat[0]) continue;
        azel[  n*2]=rtk.ssat[i].azel[0];
        azel[1+n*2]=rtk.ssat[i].azel[1];
        n++;
    }
    dops(n,azel,0.0,dop);
    
    printvt(vt,"\n");
    printvt(vt,"%-28s: %s\n","rtklib version",VER_RTKLIB);
    printvt(vt,"%-28s: %d\n","rtk server thread",thread);
    printvt(vt,"%-28s: %s\n","rtk server state",svrstate[state]);
    printvt(vt,"%-28s: %d\n","processing cycle (ms)",cycle);
    printvt(vt,"%-28s: %s\n","positioning mode",mode[rtk.opt.mode]);
    printvt(vt,"%-28s: %s\n","frequencies",freq[rtk.opt.nf]);
    printvt(vt,"%-28s: %02.0f:%02.0f:%04.1f\n","accumulated time to run",rt[0],rt[1],rt[2]);
    printvt(vt,"%-28s: %d\n","cpu time for a cycle (ms)",cputime);
    printvt(vt,"%-28s: %d\n","missing obs data count",prcout);
    printvt(vt,"%-28s: %d,%d\n","bytes in input buffer",nb[0],nb[1]);
    printvt(vt,"%-28s: obs(%d),nav(%d),gnav(%d),ion(%d),sbs(%d),pos(%d),dgps(%d),err(%d)\n",
            "# of input data rover",nmsg[0][0],nmsg[0][1],nmsg[0][6],nmsg[0][2],
            nmsg[0][3],nmsg[0][4],nmsg[0][5],nmsg[0][9]);
    printvt(vt,"%-28s: obs(%d),nav(%d),gnav(%d),ion(%d),sbs(%d),pos(%d),dgps(%d),err(%d)\n",
            "# of input data base",nmsg[1][0],nmsg[1][1],nmsg[1][6],nmsg[1][2],
            nmsg[1][3],nmsg[1][4],nmsg[1][5],nmsg[1][9]);
    for (i=0;i<2;i++) {
        p=s; *p='\0';
        for (j=1;j<64;j++) {
            if (rtcm[i].nmsg2[j]==0) continue;
            p+=sprintf(p,"%s%d(%d)",p>s?",":"",j,rtcm[i].nmsg2[j]);
        }
        if (rtcm[i].nmsg2[0]>0) {
            sprintf(p,"%sother2(%d)",p>s?",":"",rtcm[i].nmsg2[0]);
        }
        for (j=1;j<64;j++) {
            if (rtcm[i].nmsg3[j]==0) continue;
            p+=sprintf(p,"%s%d(%d)",p>s?",":"",j+1000,rtcm[i].nmsg3[j]);
        }
        if (rtcm[i].nmsg3[0]>0) {
            sprintf(p,"%sother3(%d)",p>s?",":"",rtcm[i].nmsg3[0]);
        }
        printvt(vt,"%-15s %-9s: %s\n","# of rtcm messages",i?"base":"rover",s);
    }
    printvt(vt,"%-28s: %s\n","solution status",sol[rtkstat]);
    time2str(rtk.sol.time,tstr,9);
    printvt(vt,"%-28s: %s\n","time of receiver clock rover",rtk.sol.time.time?tstr:"-");
    printvt(vt,"%-28s: %.9f\n","time sys offset (glo-gps)(s)",rtk.sol.dtr[1]);
    printvt(vt,"%-28s: %.3f\n","solution interval (s)",rtk.tt);
    printvt(vt,"%-28s: %.3f\n","age of differential (s)",rtk.sol.age);
    printvt(vt,"%-28s: %.3f\n","ratio for ar validation",rtk.sol.ratio);
    printvt(vt,"%-28s: %d\n","# of satellites rover",nsat0);
    printvt(vt,"%-28s: %d\n","# of satellites base",nsat1);
    printvt(vt,"%-28s: %d\n","# of valid satellites",rtk.sol.ns);
    printvt(vt,"%-28s: %.1f,%.1f,%.1f,%.1f\n","GDOP/PDOP/HDOP/VDOP",dop[0],dop[1],dop[2],dop[3]);
    printvt(vt,"%-28s: %d\n","# of real estimated states",rtk.na);
    printvt(vt,"%-28s: %d\n","# of all estimated states",rtk.nx);
    printvt(vt,"%-28s: %.3f,%.3f,%.3f\n","pos xyz single (m) rover",
            rtk.sol.rr[0],rtk.sol.rr[1],rtk.sol.rr[2]);
    if (norm(rtk.sol.rr,3)>0.0) ecef2pos(rtk.sol.rr,pos); else pos[0]=pos[1]=pos[2]=0.0;
    printvt(vt,"%-28s: %.8f,%.8f,%.3f\n","pos llh single (deg,m) rover",
            pos[0]*R2D,pos[1]*R2D,pos[2]);
    ecef2enu(pos,rtk.sol.rr+3,vel);
    printvt(vt,"%-28s: %.3f,%.3f,%.3f\n","vel enu (m/s) rover",vel[0],vel[1],vel[2]);
    printvt(vt,"%-28s: %.3f,%.3f,%.3f\n","pos xyz float (m) rover",
            rtk.x?rtk.x[0]:0,rtk.x?rtk.x[1]:0,rtk.x?rtk.x[2]:0);
    printvt(vt,"%-28s: %.3f,%.3f,%.3f\n","pos xyz float std (m) rover",
            rtk.P?SQRT(rtk.P[0]):0,rtk.P?SQRT(rtk.P[1+1*rtk.nx]):0,rtk.P?SQRT(rtk.P[2+2*rtk.nx]):0);
    printvt(vt,"%-28s: %.3f,%.3f,%.3f\n","pos xyz fixed (m) rover",
            rtk.xa?rtk.xa[0]:0,rtk.xa?rtk.xa[1]:0,rtk.xa?rtk.xa[2]:0);
    printvt(vt,"%-28s: %.3f,%.3f,%.3f\n","pos xyz fixed std (m) rover",
            rtk.Pa?SQRT(rtk.Pa[0]):0,rtk.Pa?SQRT(rtk.Pa[1+1*rtk.na]):0,rtk.Pa?SQRT(rtk.Pa[2+2*rtk.na]):0);
    printvt(vt,"%-28s: %.3f,%.3f,%.3f\n","pos xyz (m) base",
            rtk.rb[0],rtk.rb[1],rtk.rb[2]);
    if (norm(rtk.rb,3)>0.0) ecef2pos(rtk.rb,pos); else pos[0]=pos[1]=pos[2]=0.0;
    printvt(vt,"%-28s: %.8f,%.8f,%.3f\n","pos llh (deg,m) base",
            pos[0]*R2D,pos[1]*R2D,pos[2]);
    ecef2enu(pos,rtk.rb+3,vel);
    printvt(vt,"%-28s: %.3f,%.3f,%.3f\n","vel enu (m/s) base",
            vel[0],vel[1],vel[2]);
    if (rtk.opt.mode>0&&rtk.x&&norm(rtk.x,3)>0.0) {
        for (i=0;i<3;i++) rr[i]=rtk.x[i]-rtk.rb[i]; bl1=norm(rr,3);
    }
    if (rtk.opt.mode>0&&rtk.xa&&norm(rtk.xa,3)>0.0) {
        for (i=0;i<3;i++) rr[i]=rtk.xa[i]-rtk.rb[i]; bl2=norm(rr,3);
    }
    printvt(vt,"%-28s: %.3f\n","baseline length float (m)",bl1);
    printvt(vt,"%-28s: %.3f\n","baseline length fixed (m)",bl2);
    printvt(vt,"%-28s: %d\n","monitor port",moniport);
}
/* print satellite -----------------------------------------------------------*/
static void prsatellite(vt_t *vt)
{
    rtk_t rtk;
    double az,el;
    char id[32];
    int i,j,fix;
    
    trace(4,"prsatellite:\n");
    
    rtksvrlock(&svr);
    rtk=svr.rtk;
    rtksvrunlock(&svr);
    
    printvt(vt,"\nSAT C1");
    for (j=0;j<NFREQ;j++) printvt(vt," L%d",j+1);
    for (j=0;j<NFREQ;j++) printvt(vt,"  Fix%d",j+1);
    for (j=0;j<NFREQ;j++) printvt(vt,"  Sl%d",j+1);
    for (j=0;j<NFREQ;j++) printvt(vt,"  Lock%d",j+1);
    for (j=0;j<NFREQ;j++) printvt(vt," Rj%d",j+1);
    printvt(vt," ");
    for (j=0;j<NFREQ;j++) printvt(vt," P%dRes",j+1);
    for (j=0;j<NFREQ;j++) printvt(vt,"  L%dRes",j+1);
    printvt(vt,"    Az   El\n");
    
    for (i=0;i<MAXSAT;i++) {
        if (rtk.ssat[i].azel[1]<=0.0) continue;
        satno2id(i+1,id);
        printvt(vt,"%3s %2s",id,rtk.ssat[i].vs?"OK":"-");
        for (j=0;j<NFREQ;j++) printvt(vt," %2s",rtk.ssat[i].vsat[j]?"OK":"-");
        for (j=0;j<NFREQ;j++) {
            fix=rtk.ssat[i].fix[j];
            printvt(vt," %5s",fix==1?"FLOAT":(fix==2?"FIX":(fix==3?"HOLD":"-")));
        }
        for (j=0;j<NFREQ;j++) printvt(vt," %4d",rtk.ssat[i].slipc[j]);
        for (j=0;j<NFREQ;j++) printvt(vt," %6d",rtk.ssat[i].lock [j]);
        for (j=0;j<NFREQ;j++) printvt(vt," %3d",rtk.ssat[i].rejc [j]);
        printvt(vt," ");
        for (j=0;j<NFREQ;j++) printvt(vt,"%6.3f",rtk.ssat[i].resp[j]);
        for (j=0;j<NFREQ;j++) printvt(vt,"%7.4f",rtk.ssat[i].resc[j]);
        az=rtk.ssat[i].azel[0]*R2D; if (az<0.0) az+=360.0;
        el=rtk.ssat[i].azel[1]*R2D;
        printvt(vt," %5.1f %4.1f\n",az,el);
    }
}
/* print observation data ----------------------------------------------------*/
static void probserv(vt_t *vt)
{
    obsd_t obs[MAXOBS*2];
    char tstr[64],id[32];
    int i,j,n=0;
    
    trace(4,"probserv:\n");
    
    rtksvrlock(&svr);
    for (i=0;i<svr.obs[0][0].n&&n<MAXOBS*2;i++) {
        obs[n++]=svr.obs[0][0].data[i];
    }
    for (i=0;i<svr.obs[1][0].n&&n<MAXOBS*2;i++) {
        obs[n++]=svr.obs[1][0].data[i];
    }
    rtksvrunlock(&svr);
    
    printvt(vt,"\n");
    printvt(vt,"   GPST    SAT R       P1(m)       P2(m)      L1(cyc)      L2(cyc)  D1(Hz)  D2(Hz) S1 S2 LLI\n");
    for (i=0;i<n;i++) {
        time2str(obs[i].time,tstr,1);
        satno2id(obs[i].sat,id);
        printvt(vt,"%s %3s %d",tstr+11,id,obs[i].rcv);
        for (j=0;j<2;j++) printvt(vt,"%12.2f",obs[i].P[j]);
        for (j=0;j<2;j++) printvt(vt,"%13.2f",obs[i].L[j]);
        for (j=0;j<2;j++) printvt(vt,"%8.2f" ,obs[i].D[j]);
        for (j=0;j<2;j++) printvt(vt,"%2.0f" ,obs[i].SNR[j]*0.25);
        for (j=0;j<2;j++) printvt(vt,"%2d"   ,obs[i].LLI[j]);
        printvt(vt,"\n");
    }
}
/* print navigation data -----------------------------------------------------*/
static void prnavidata(vt_t *vt)
{
    eph_t eph[MAXSAT];
    double ion[8],utc[4];
    gtime_t time;
    char id[32],s1[64],s2[64],s3[64];
    int i,valid,sys,prn,leaps;
    
    trace(4,"prnavidata:\n");
    
    rtksvrlock(&svr);
    time=svr.rtk.sol.time;
    for (i=0;i<MAXSAT;i++) eph[i]=svr.nav.eph[i];
    for (i=0;i<8;i++) ion[i]=svr.nav.ion_gps[i];
    for (i=0;i<4;i++) utc[i]=svr.nav.utc_gps[i];
    leaps=svr.nav.leaps;
    rtksvrunlock(&svr);
    
    printvt(vt,"\n");
    printvt(vt,"SAT Stat   IOD  Acc SVH        Toe                 Toc                Ttrans        L2C L2P\n");
    for (i=0;i<MAXSAT;i++) {
        valid=eph[i].toe.time!=0&&!eph[i].svh&&fabs(timediff(time,eph[i].toe))<=MAXDTOE;
        sys=satsys(i+1,&prn);
        if (sys!=SYS_GPS&&sys!=SYS_GAL&&sys!=SYS_QZS) continue;
        satno2id(i+1,id);
        if (eph[i].toe.time!=0) time2str(eph[i].toe,s1,0); else strcpy(s1,"-");
        if (eph[i].toc.time!=0) time2str(eph[i].toc,s2,0); else strcpy(s2,"-");
        if (eph[i].ttr.time!=0) time2str(eph[i].ttr,s3,0); else strcpy(s3,"-");
        printvt(vt,"%3s %3s %3d %3d %3d  %02X %19s %19s %19s %3d %3d\n",
                id,valid?"OK":"-",eph[i].iode,eph[i].iodc,eph[i].sva,eph[i].svh,
                s1,s2,s3,eph[i].code,eph[i].flag);
    }
    printvt(vt,"ION: %9.2E %9.2E %9.2E %9.2E %9.2E %9.2E %9.2E %9.2E\n",
            ion[0],ion[1],ion[2],ion[3],ion[4],ion[5],ion[6],ion[7]);
    printvt(vt,"UTC: %9.2E %9.2E %9.2E %9.2E LEAPS: %d\n",utc[0],utc[1],utc[2],utc[3],leaps);
}
/* print error/warning messages ----------------------------------------------*/
static void prerror(vt_t *vt)
{
    int n;
    
    trace(4,"prerror:\n");
    
    rtksvrlock(&svr);
    if ((n=svr.rtk.neb)>0) {
        outvt(vt,svr.rtk.errbuf,n);
        svr.rtk.neb=0;
    }
    rtksvrunlock(&svr);
}
/* print stream --------------------------------------------------------------*/
static void prstream(vt_t *vt)
{
    const char *ch[]={
        "input rover","input base","input corr","output sol1","output sol2",
        "log rover","log base","log ephem","monitor"
    };
    const char *type[]={
        "-","serial","file","tcpsvr","tcpcli","udp","ntrips","ntripc","ftp","http"
    };
    const char *fmt[]={"rtcm2","rtcm3","oem4","oem3","ubx","ss2","hemis","skytreq",
                       "gw10","javad","lexr","","","","","sp3","","",""};
    const char *sol[]={"llh","xyz","enu","nmea"};
    stream_t stream[9];
    int i,format[9]={0};
    
    trace(4,"prstream:\n");
    
    rtksvrlock(&svr);
    for (i=0;i<8;i++) stream[i]=svr.stream[i];
    for (i=0;i<3;i++) format[i]=svr.format[i];
    for (i=3;i<5;i++) format[i]=svr.solopt[i-3].posf;
    stream[8]=moni;
    format[8]=SOLF_LLH;
    rtksvrunlock(&svr);
    
    printvt(vt,"\n");
    printvt(vt,"Stream       Type   Fmt   S   In-byte  In-bps  Out-byte Out-bps Message\n");
    for (i=0;i<9;i++) {
        printvt(vt,"%-12s %-6s %-5s %s %9d %7d %9d %7d %s\n",
            ch[i],type[stream[i].type],i<3?fmt[format[i]]:(i<5||i==8?sol[format[i]]:"-"),
            stream[i].state<0?"E":(stream[i].state?"C":"-"),
            stream[i].inb,stream[i].inr,stream[i].outb,stream[i].outr,stream[i].msg);
    }
}
/* start command -------------------------------------------------------------*/
static void cmd_start(char **args, int narg, vt_t *vt)
{
    trace(3,"cmd_start:\n");
    
    if (!startsvr(vt)) return;
    printvt(vt,"rtk server start\n");
}
/* stop command --------------------------------------------------------------*/
static void cmd_stop(char **args, int narg, vt_t *vt)
{
    trace(3,"cmd_stop:\n");
    
    stopsvr(vt);
    printvt(vt,"rtk server stop\n");
}
/* restart command -----------------------------------------------------------*/
static void cmd_restart(char **args, int narg, vt_t *vt)
{
    trace(3,"cmd_restart:\n");
    
    stopsvr(vt);
    if (!startsvr(vt)) return;
    printvt(vt,"rtk server restart\n");
}
/* solution command ----------------------------------------------------------*/
static void cmd_solution(char **args, int narg, vt_t *vt)
{
    int i,cycle=0;
    
    trace(3,"cmd_solution:\n");
    
    if (narg>1) cycle=(int)(atof(args[1])*1000.0);
    
    if (cycle>0) svr.nsol=0;
    
    for (intflg=0;!intflg;) {
        rtksvrlock(&svr);
        for (i=0;i<svr.nsol;i++) prsolution(vt,&svr.solbuf[i],svr.rtk.rb);
        svr.nsol=0;
        rtksvrunlock(&svr);
        if (cycle>0) sleepms(cycle); else return;
    }
}
/* status command ------------------------------------------------------------*/
static void cmd_status(char **args, int narg, vt_t *vt)
{
    int cycle=0;
    
    trace(3,"cmd_status:\n");
    
    if (narg>1) cycle=(int)(atof(args[1])*1000.0);
    
    for (intflg=0;!intflg;) {
        prstatus(vt);
        if (cycle>0) sleepms(cycle); else return;
    }
    printvt(vt,"\n");
}
/* satellite command ---------------------------------------------------------*/
static void cmd_satellite(char **args, int narg, vt_t *vt)
{
    int cycle=0;
    
    trace(3,"cmd_satellite:\n");
    
    if (narg>1) cycle=(int)(atof(args[1])*1000.0);
    
    for (intflg=0;!intflg;) {
        prsatellite(vt);
        if (cycle>0) sleepms(cycle); else return;
    }
    printvt(vt,"\n");
}
/* observ command ------------------------------------------------------------*/
static void cmd_observ(char **args, int narg, vt_t *vt)
{
    int cycle=0;
    
    trace(3,"cmd_observ:\n");
    
    if (narg>1) cycle=(int)(atof(args[1])*1000.0);
    
    for (intflg=0;!intflg;) {
        probserv(vt);
        if (cycle>0) sleepms(cycle); else return;
    }
    printvt(vt,"\n");
}
/* navidata command ----------------------------------------------------------*/
static void cmd_navidata(char **args, int narg, vt_t *vt)
{
    int cycle=0;
    
    trace(3,"cmd_navidata:\n");
    
    if (narg>1) cycle=(int)(atof(args[1])*1000.0);
    
    for (intflg=0;!intflg;) {
        prnavidata(vt);
        if (cycle>0) sleepms(cycle); else return;
    }
    printvt(vt,"\n");
}
/* error command -------------------------------------------------------------*/
static void cmd_error(char **args, int narg, vt_t *vt)
{
    trace(3,"cmd_error:\n");
    
    rtksvrlock(&svr);
    svr.rtk.neb=0;
    rtksvrunlock(&svr);
    
    for (intflg=0;!intflg;) {
        prerror(vt);
        sleepms(100);
    }
    printvt(vt,"\n");
}
/* stream command ------------------------------------------------------------*/
static void cmd_stream(char **args, int narg, vt_t *vt)
{
    int cycle=0;
    
    trace(3,"cmd_stream:\n");
    
    if (narg>1) cycle=(int)(atof(args[1])*1000.0);
    
    for (intflg=0;!intflg;) {
        prstream(vt);
        if (cycle>0) sleepms(cycle); else return;
    }
    printvt(vt,"\n");
}
/* option command ------------------------------------------------------------*/
static void cmd_option(char **args, int narg, vt_t *vt)
{
    char buff[MAXSTR],*p;
    int i,n;
    
    trace(3,"cmd_option:\n");
    
    for (i=0;*rcvopts[i].name;i++) {
        if (narg>=2&&!strstr(rcvopts[i].name,args[1])) continue;
        p=buff;
        p+=sprintf(p,"%-18s =",rcvopts[i].name);
        p+=opt2str(rcvopts+i,p);
        if (*rcvopts[i].comment) {
            if ((n=(int)(buff+30-p))>0) p+=sprintf(p,"%*s",n,"");
            p+=sprintf(p," # (%s)",rcvopts[i].comment);
        }
        printvt(vt,"%s%s\n",modflgr[i]?"*":" ",buff);
    }
    for (i=0;*sysopts[i].name;i++) {
        if (narg>=2&&!strstr(sysopts[i].name,args[1])) continue;
        p=buff;
        p+=sprintf(p,"%-18s =",sysopts[i].name);
        p+=opt2str(sysopts+i,p);
        if (*sysopts[i].comment) {
            if ((n=(int)(buff+30-p))>0) p+=sprintf(p,"%*s",n,"");
            p+=sprintf(p," # (%s)",sysopts[i].comment);
        }
        printvt(vt,"%s%s\n",modflgs[i]?"*":" ",buff);
    }
}
/* set command ---------------------------------------------------------------*/
static void cmd_set(char **args, int narg, vt_t *vt)
{
    opt_t *opt;
    int *modf;
    char buff[MAXSTR];
    
    trace(3,"cmd_set:\n");
    
    if (narg<2) {
        printvt(vt,"specify option type\n");
        return;
    }
    if ((opt=searchopt(args[1],rcvopts))) {
        modf=modflgr+(int)(opt-rcvopts);
    }
    else if ((opt=searchopt(args[1],sysopts))) {
        modf=modflgs+(int)(opt-sysopts);
    }
    else {
        printvt(vt,"no option type: %s\n",args[1]);
        return;
    }
    if (narg<3) {
        printvt(vt,"%s",opt->name);
        if (*opt->comment) printvt(vt," (%s)",opt->comment);
        printvt(vt,": ");
        if (!inpvt(vt,buff,sizeof(buff))) return;
    }
    else strcpy(buff,args[2]);
    
    chop(buff);
    if (!str2opt(opt,buff)) {
        printvt(vt,"invalid option value: %s %s\n",opt->name,buff);
        return;
    }
    getsysopts(&prcopt,solopt,&filopt);
    
    printvt(vt,"option %s changed.",opt->name);
    if (strncmp(opt->name,"console",7)) {
        *modf=1;
        printvt(vt," restart to enable it");
    }
    printvt(vt,"\n");
}
/* load command --------------------------------------------------------------*/
static void cmd_load(char **args, int narg, vt_t *vt)
{
    char file[MAXSTR]="";
    
    trace(3,"cmd_load:\n");
    
    if (narg>=2) {
        strcpy(file,args[1]);
    }
    else {
        sprintf(file,"%s/%s",OPTSDIR,OPTSFILE);
    }
    resetsysopts();
    if (!loadopts(file,sysopts)) {
        printvt(vt,"no options file: %s\n",file);
        return;
    }
    getsysopts(&prcopt,solopt,&filopt);
    
    if (!loadopts(file,rcvopts)) {
        printvt(vt,"no options file: %s\n",file);
        return;
    }
    printvt(vt,"options loaded from %s. restart to enable them\n",file);
}
/* save command --------------------------------------------------------------*/
static void cmd_save(char **args, int narg, vt_t *vt)
{
    char file[MAXSTR]="",comment[256],s[64];
    
    trace(3,"cmd_save:\n");
    
    if (narg>=2) {
        strcpy(file,args[1]);
    }
    else {
        sprintf(file,"%s/%s",OPTSDIR,OPTSFILE);
    }
    if (!confwrite(vt,file)) return;
    time2str(utc2gpst(timeget()),s,0);
    sprintf(comment,"%s options (%s, v.%s)",PRGNAME,s,VER_RTKLIB);
    setsysopts(&prcopt,solopt,&filopt);
    if (!saveopts(file,"w",comment,rcvopts)||!saveopts(file,"a",NULL,sysopts)) {
        printvt(vt,"options save error: %s\n",file);
        return;
    }
    printvt(vt,"options saved to %s\n",file);
}
/* log command ---------------------------------------------------------------*/
static void cmd_log(char **args, int narg, vt_t *vt)
{
    trace(3,"cmd_log:\n");
    
    if (narg<2) {
        printvt(vt,"specify log file\n");
        return;
    }
    if (!strcmp(args[1],"off")) {
        if (logfp) fclose(logfp);
        logfp=NULL;
        printvt(vt,"log off\n");
        return;
    } 
    if (!confwrite(vt,args[1])) return;
    
    if (!(logfp=fopen(args[1],"w"))) {
        printvt(vt,"log open error: %s\n",args[1]);
        return;
    }
    printvt(vt,"log on: %s\n",args[1]);
}
/* help command --------------------------------------------------------------*/
static void cmd_help(char **args, int narg, vt_t *vt)
{
    char str[]="path";
    int i;
    
    if (narg<2) {
        printvt(vt,"%s ver.%s commands\n",PRGNAME,VER_RTKLIB);
        for (i=0;*helptxt[i];i++) printvt(vt,"%s\n",helptxt[i]);
    }
    else if (strstr(str,args[1])==str) {
        for (i=0;*pathopts[i];i++) printvt(vt,"%s\n",pathopts[i]);
    }
    else {
        printvt(vt,"unknown help: %s\n",args[1]);
    }
}
/* exec command --------------------------------------------------------------*/
static int cmd_exec(const char *cmd, vt_t *vt)
{
    FILE *fp;
    int ret;
    char buff[MAXSTR];
    
    if (!(fp=popen(cmd,"r"))) {
        printvt(vt,"command exec error\n");
        return -1;
    }
    for (intflg=0;fgets(buff,sizeof(buff),fp)&&!intflg;) {
        printvt(vt,buff);
    }
    if ((ret=pclose(fp))) {
        printvt(vt,"command exec error (%d)\n",ret);
    }
    return ret;
}
/* command interpreter -------------------------------------------------------*/
static void cmdshell(vt_t *vt)
{
    const char *cmds[]={
        "start","stop","restart","solution","status","satellite","observ",
        "navidata","stream","error","option","set","load","save","log","help",
        "?","exit","shutdown",""
    };
    int i,j,narg,ret;
    char buff[MAXCMD],*args[MAXARG],*p;
    
    trace(3,"cmdshell:\n");
    
    while (!(intflg&2)) {
        
        /* output prompt */
        if (!printvt(vt,"%s",CMDPROMPT)) break;
        
        /* input command */
        if (!inpvt(vt,buff,sizeof(buff))) continue;
        
        if (buff[0]=='!') { /* shell escape */
            ret=cmd_exec(buff+1,vt);
            continue;
        }
        /* parse command */
        narg=0;
        for (p=strtok(buff," \t\n");p&&narg<MAXARG;p=strtok(NULL," \t\n")) {
            args[narg++]=p;
        }
        if (narg==0) continue;
        
        for (i=0,j=-1;*cmds[i];i++) {
            if (strstr(cmds[i],args[0])==cmds[i]) j=i;
        }
        switch (j) {
            case  0: cmd_start    (args,narg,vt); break;
            case  1: cmd_stop     (args,narg,vt); break;
            case  2: cmd_restart  (args,narg,vt); break;
            case  3: cmd_solution (args,narg,vt); break;
            case  4: cmd_status   (args,narg,vt); break;
            case  5: cmd_satellite(args,narg,vt); break;
            case  6: cmd_observ   (args,narg,vt); break;
            case  7: cmd_navidata (args,narg,vt); break;
            case  8: cmd_stream   (args,narg,vt); break;
            case  9: cmd_error    (args,narg,vt); break;
            case 10: cmd_option   (args,narg,vt); break;
            case 11: cmd_set      (args,narg,vt); break;
            case 12: cmd_load     (args,narg,vt); break;
            case 13: cmd_save     (args,narg,vt); break;
            case 14: cmd_log      (args,narg,vt); break;
            case 15: cmd_help     (args,narg,vt); break;
            case 16: cmd_help     (args,narg,vt); break;
            case 17: return;      /* exit */
            case 18:              /* shutdown */
                printvt(vt,"shutdown %s process ? (y/n): ",PRGNAME);
                if (!inpvt(vt,buff,sizeof(buff))) continue;
                if (toupper((int)buff[0])=='Y') intflg|=2;
                break;
            default:
                printvt(vt,"unknown command: %s.\n",args[0]);
                break;
        }
    }
    trace(3,"cmdshell: exit\n");
}
/* rtkrcv main -----------------------------------------------------------------
* sysnopsis
*     rtkrcv [-s][-p port|-d dev][-o file][-t level]
*
* description
*     A command line version of the real-time positioning AP by rtklib. To start
*     or stop RTK server, to configure options or to print solution/status,
*     login a console and input commands. As default, stdin/stdout are used for
*     the console. Use -p option for network login with telnet protocol. To show
*     the available commands, type ? or help on the console. The initial 
*     processing options are loaded from default file rtkrcv.conf. To change the
*     file, use -o option. To configure the processing options, edit the options
*     file or use set, load or save command on the console. To shutdown the
*     program, use shutdown command on the console or send USR2 signal to the
*     process.
*
* option
*     -s         start RTK server on program startup
*     -p port    port number for telnet console
*     -m port    port number for monitor stream
*     -d dev     terminal device for console
*     -o file    processing options file       
*     -r level   output solution status file (0:off,1:states,2:residuals)
*     -t level   debug trace level (0:off,1-5:on)
*
* command
*     start
*       Start RTK server. No need the command if the program runs with -s
*       option.
*
*     stop
*       Stop RTK server.
*
*     restart
*       Restart RTK server. If the processing options are set, execute the
*       command to enable the changes.
*
*     solution [cycle]
*       Show solutions. Without option, only one solution is shown. With
*       option, the soluiton is displayed at intervals of cycle (s). To stop
*       cyclic display, send break (ctr-C).
*
*     status [cycle]
*       Show RTK status. Use option cycle for cyclic display.
*
*     satellite [cycle]
*       Show satellite status. Use option cycle for cyclic display.
*
*     observ [cycle]
*       Show observation data. Use option cycle for cyclic display.
*
*     navidata [cycle]
*       Show navigation data. Use option cycle for cyclic display.
*
*     stream [cycle]
*       Show stream status. Use option cycle for cyclic display.
*
*     error
*       Show error/warning messages. To stop messages, send break (ctr-C).
*
*     option [opt]
*       Show the values of processing options. Without option, all options are
*       displayed. With option, only pattern-matched options are displayed.
*
*     set opt [val]
*       Set the value of a processing option to val. With out option val,
*       prompt message is shown to input the value. The change of the 
*       processing option is not enabled before RTK server is restarted.
*
*     load [file]
*       Load processing options from file. Without option, default file
*       rtkrcv.conf is used. To enable the changes, restart RTK server.
*
*     save [file]
*       Save current processing optons to file. Without option, default file
*       rtkrcv.conf is used.
*
*     log [file|off]
*       Record console log to file. To stop recording the log, use option off.
*
*     help|? [path]
*       Show the command list. With option path, the stream path options are
*       shown.
*
*     exit
*       Exit and logout console. The status of RTK server is not affected by
*       the command.
*
*     shutdown
*       Shutdown RTK server and exit the program.
*
*     !command [arg...]
*       Execute command by the operating system shell. Do not use the
*       interactive command.
*
* notes
*     Short form of a command is allowed. In case of the short form, the
*     command is distinguished according to header characters.
*     
*-----------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    vt_t vt={0};
    int i,start=0,port=0,outstat=0,trace=0;
    char *dev="",file[MAXSTR]="";
    
    for (i=1;i<argc;i++) {
        if      (!strcmp(argv[i],"-s")) start=1;
        else if (!strcmp(argv[i],"-p")&&i+1<argc) port=atoi(argv[++i]);
        else if (!strcmp(argv[i],"-m")&&i+1<argc) moniport=atoi(argv[++i]);
        else if (!strcmp(argv[i],"-d")&&i+1<argc) dev=argv[++i];
        else if (!strcmp(argv[i],"-o")&&i+1<argc) strcpy(file,argv[++i]);
        else if (!strcmp(argv[i],"-r")&&i+1<argc) outstat=atoi(argv[++i]);
        else if (!strcmp(argv[i],"-t")&&i+1<argc) trace=atoi(argv[++i]);
        else fprintf(stderr,"Unknown option: %s\n",argv[i]);
    }
    if (trace>0) {
        traceopen(TRACEFILE);
        tracelevel(trace);
    }
    /* initialize rtk server and monitor port */
    rtksvrinit(&svr);
    strinit(&moni);
    
    /* load options file */
    if (!*file) sprintf(file,"%s/%s",OPTSDIR,OPTSFILE);
    
    resetsysopts();
    if (!loadopts(file,rcvopts)||!loadopts(file,sysopts)) {
        fprintf(stderr,"no options file: %s. defaults used\n",file);
    }
    getsysopts(&prcopt,solopt,&filopt);
    
    /* read navigation data */
    if (!readnav(NAVIFILE,&svr.nav)) {
        fprintf(stderr,"no navigation data: %s\n",NAVIFILE);
    }
    if (outstat>0) {
        rtkopenstat(STATFILE,outstat);
    }
    /* open monitor port */
    if (moniport>0&&!openmoni(moniport)) {
        fprintf(stderr,"monitor port open error: %d\n",moniport);
        return -1;
    }
    /* start rtk server */
    if (start&&!startsvr(&vt)) return -1;
    
    signal(SIGINT, sigint);     /* keyboard interrupt */
    signal(SIGUSR2,sigshut);    /* external shutdown signal */
    signal(SIGPIPE,SIG_IGN);
    
    while (!(intflg&2)) {
        
        /* open console */
        if (!openvt(&vt,port,dev)) break;
        
        printvt(&vt,"** %s ver.%s console (h:help) **\n",PRGNAME,VER_RTKLIB);
        
        /* command interpreter */
        if (login(&vt)) cmdshell(&vt);
        
        /* close console */
        closevt(&vt);
    }
    /* stop rtk server */
    stopsvr(&vt);
    
    if (moniport>0) closemoni();
    
    if (outstat>0) rtkclosestat();
    
    if (trace>0) traceclose();
    
    /* save navigation data */
    if (!savenav(NAVIFILE,&svr.nav)) {
        fprintf(stderr,"navigation data save error: %s\n",NAVIFILE);
    }
    return 0;
}
