#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/uio.h>
#define DEFAULT_BAUDRATE B4800

/* ugly but robust and nice for debug */
#define work ((void *)s>=v[i].iov_base&&(void *)s<v[i].iov_base+v[i].iov_len&&i<cnt)
static void xxdv(struct iovec *v,int cnt) {
  char l[80];
  int j,pos;
  unsigned int i=0,addr;
  const unsigned char *s=v[0].iov_base;
  for(addr=0;work;addr+=16) {
    memset(l,' ',sizeof(l));
    for(j=pos=0;j<16&&work;j++,pos+=3) {
      if(8==j)
        l[pos++]=' ';
      snprintf(l+pos,sizeof(l)-pos,"%02x ",*s);
      snprintf(l+50+j,sizeof(l)-50-j,"%c",*s>0x20&&*s<0x7f?*s:'.');
      l[pos+3]=' ';
      if((void *)++s>=v[i].iov_base+v[i].iov_len&&i<cnt)
        s=v[++i].iov_base;
    }
    printf("%06x: %s\n",addr,l);
  }
}

static const speed_t str2br(const char *str) {
  if(!strcmp(str,"115200"))
    return B115200;
  if(!strcmp(str,"57600"))
    return B57600;
  if(!strcmp(str,"38400"))
    return B38400;
  if(!strcmp(str,"19200"))
    return B19200;
  if(!strcmp(str,"9600"))
    return B9600;
  if(!strcmp(str,"4800"))
    return B4800;
  if(!strcmp(str,"2400"))
    return B2400;
  if(!strcmp(str,"1200"))
    return B1200;
  if(!strcmp(str,"600"))
    return B600;
  if(!strcmp(str,"300"))
    return B300;
  return DEFAULT_BAUDRATE;
}

static const int br2i(const speed_t br) {
  if(B115200==br)
    return 115200;
  if(B57600==br)
    return 57600;
  if(B38400==br)
    return 38400;
  if(B19200==br)
    return 19200;
  if(B9600==br)
    return 9600;
  if(B4800==br)
    return 4800;
  if(B2400==br)
    return 2400;
  if(B1200==br)
    return 1200;
  if(B600==br)
    return 600;
  if(B300==br)
    return 300;
  return -1;
}

unsigned char cksum(unsigned char start,const unsigned char *s, int len) {
  unsigned char c=start;
  int i;
  for(i=0;i<len;i++)
    c+=s[i];
  return c;
}

static inline iov_set(struct iovec *v,int i,void *base,size_t len) {
  v[i].iov_base=base;
  v[i].iov_len=len;
}

int main(int argc,char *argv[]) {
  int i,fd,slen;
  char *s;
  unsigned char c;
  struct iovec iov[5];
  speed_t br=DEFAULT_BAUDRATE;
  struct termios tios;

  if(argc<3) {
    printf("syntax: %s tty [baudrate (default=%d)] text\n", *argv, br2i(br));
    return 1;
  }

  if(-1==(fd=open(argv[1],O_RDWR))) {
    perror("open");
    return 1;
  }

  if(argc>3)
    br=str2br(argv[2]);

  if(-1==tcgetattr(fd,&tios)) {
    perror("tcgetattr");
    return 1;
  }
  cfmakeraw(&tios);
  printf("setting baudrate %d\n",br2i(br));
  if(-1==cfsetspeed(&tios,br)) {
    perror("cfsetspeed");
    return 1;
  }
  if(-1==tcsetattr(fd,TCSANOW,&tios)) {
    perror("tcgetattr");
    return 1;
  }

  i=0;
  iov_set(iov,i++,"\xff",1);
  iov_set(iov,i++,"\x06\xa2",2);

  s=argv[argc>3?3:2];
  slen=strlen(s);
  iov_set(iov,i++,s,slen);

  c=cksum(6+162,s,slen);
  iov_set(iov,i++,&c,1);

  iov_set(iov,i++,"\xff",1);

  printf("writing %d bytes\n",5+slen);
  xxdv(iov,i);
  if(-1==(i=writev(fd,iov,i)))
    perror("writev");
  else
    printf("%d bytes written\n",i);

  close(fd);
  return i!=5+slen;
}
