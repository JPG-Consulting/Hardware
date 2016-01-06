#include "include.h"

uchar XOR_verify( uchar *msg_ptr, uchar len )
{
  uchar x;
  uchar xorResult;
  xorResult = 0;
  for ( x = 0; x < len; x++, msg_ptr++ )
    xorResult = xorResult ^ *msg_ptr;
  return ( xorResult );
}
