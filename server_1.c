//情報実験Ⅰ 第9回 課題1
//BP16057 高橋 美帆

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define SOCK_NAME "./socket"

int main()
{
  int i;
  int fd1, fd2;
  struct sockaddr_in saddr;
  struct sockaddr_in caddr;

  int len;
  int ret;
  char buf[1024];

  if ( ( fd1 = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
    perror( "socket" );
    exit( 1 );
  }

  memset( (char *)&saddr, 0, sizeof( saddr ) );
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr=INADDR_ANY;
  saddr.sin_port=htons(16057);
  unlink( SOCK_NAME );
  printf("IP is 127.0.0.1 , PORT is 16057\n");

  if ( bind( fd1, ( struct sockaddr * )&saddr, ( socklen_t )sizeof( saddr ) ) < 0 ) {
    perror( "bind" );
    exit( 1 );
  }

  if ( listen( fd1, 5 ) < 0 ) {
    perror( "listen" );
    exit( 1 );
  }

  while( 1 ) {
    len = sizeof( caddr );
    if ( ( fd2 = accept( fd1, ( struct sockaddr * )&caddr, ( socklen_t * ) &len ) ) < 0 ) {
      perror( "accept" );
      exit( 1 );
    }
    fprintf( stderr, "Connection established: socket %d used.\n", fd2 );

    while( ( ret = read( fd2, buf, 1024 ) ) > 0 ) {
      fprintf( stderr, "read: %s\n", buf );
      for ( i=0; i<ret; i++ )
        if ( isalpha( buf[i] ) ){
	  if(isupper( buf[i] ) )
	    buf[i] = tolower( buf[i] );
	else
	  buf[i] = toupper( buf[i] );
	}
  

      fprintf( stderr, "write: %s\n", buf );
      write( fd2, buf, 1024 );
      fsync( fd2 );
    }

    close( fd2 );
  }

  close( fd1 );

  return 0;
}
