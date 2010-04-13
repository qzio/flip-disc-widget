#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/uio.h>

#define MAX_MESSAGE_SIZE 1024
#define CODE_ALT        0xB0
#define CODE_CLIPX      0xD0
#define CODE_CLIPY      0xD1
#define CODE_POSX       0xD2
#define CODE_POSY       0xD3
#define CODE_FONT       0xD4


unsigned char msg[ MAX_MESSAGE_SIZE ];
unsigned char *ptr_msg;
unsigned char crc;

void fdInitMessage( void ) {
  // Init msg ptr
  ptr_msg = msg;
  // Init msg hdr
  *ptr_msg++ = 0xFF;
  *ptr_msg++ = 0x06;
  *ptr_msg++ = 0xA2;
  // Init crc (0x06 + 0xA2)
  crc = 0xA8;
}

void fdFinalizeMessage( void ) {
  *ptr_msg++ = crc;
  *ptr_msg++ = 0xFF;
}

void fdInsertText( char text[] ) {
  int len, n;
  len = strlen( text );
  //memcpy( ptr_msg, text, len );
  for( n = 0; n < len; n++ ) {
    *ptr_msg++ = text[ n ];
    crc += text[ n ];
  }
}

void fdBeginAlternate( unsigned char time ) {
  *ptr_msg++ = CODE_ALT;
  *ptr_msg++ = time;
  crc += CODE_ALT + time;
}

void fdSetClipX( unsigned char x ) {
  *ptr_msg++ = CODE_CLIPX;
  *ptr_msg++ = x;
  crc += CODE_CLIPX + x;
}

void fdSetClipY( unsigned char y ) {
  *ptr_msg++ = CODE_CLIPY;
  *ptr_msg++ = y;
  crc += CODE_CLIPY + y;
}

void fdSetClip( unsigned char x, unsigned char y ) {
  fdSetClipX( x );
  fdSetClipY( y );
}

void fdSetPosX( unsigned char x ) {
  *ptr_msg++ = CODE_POSX;
  *ptr_msg++ = x;
  crc += CODE_POSX + x;
}

void fdSetPosY( unsigned char y ) {
  *ptr_msg++ = CODE_POSY;
  *ptr_msg++ = y;
  crc += CODE_POSY + y;
}

void fdSetPos( unsigned char x, unsigned char y ) {
  fdSetPosX( x );
  fdSetPosY( y );
}

// Known fonts: 0x62 ... 0x7F
void fdSetFont( unsigned char font ) {
  *ptr_msg++ = CODE_FONT;
  *ptr_msg++ = font;
  crc += CODE_FONT + font;
}

int main(int argc,char *argv[]) {
  int fd;
  struct termios tios;

  char * str;

  if (argc <= 0) {
    printf("not enough arguments!!!1111one");
    return ( 1 );
  }
  str = argv[2];
  fdInitMessage();
  //fdSetFont( 0x78 );
  //fdSetPosX( 1 );
  //fdInsertText( "(ForskningsAvd)" );
  //fdBeginAlternate( 2 );
  //fdSetFont( 0x67 );
  //fdSetPosX( 0 );
  //fdInsertText( "((((((((((" );
  fdSetFont( 0x62 );
  printf("will try and print %s",str);
  fdInsertText(str);
  fdFinalizeMessage();


  fd = open( argv[1], O_RDWR );
  if( -1 == fd ) {
    printf("couldnt open tty\n");
    return( 1 );
  }


  if( -1 == tcgetattr( fd, &tios ) ) {
    printf("tgetattr failed\n");
    return( 1 );
  }

  cfmakeraw( &tios );

  if( -1 == cfsetspeed( &tios, B4800 ) ) {
    printf("cfsetspeed failed\n");
    return( 1 );
  }

  if( -1 == tcsetattr( fd, TCSANOW, &tios ) ) {
    printf("tcsetattr failed\n");
    return( 1 );
  }

  if( -1 == write( fd, msg, ptr_msg - msg ) ) {
    printf("write failed\n");
    return( 1 );
  }

  return( 0 );

}
