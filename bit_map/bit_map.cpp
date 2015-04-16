#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <glog/logging.h>

#define set_value_one(value,index) value|=(1<<index)
#define set_value_zero(value,index) value&=!(1<<pos)


using namespace std ;

int Bitmap::set_all ( int set_value )
{
  char v ;

  // first , we check set_value : this should be either 0 or 1
  if ( set_value != 1 && set_value != 0  )
  {
	LOG(WARNING)<<"[warning] set value should be 0 or 1";
	return -1 ;
  }
  
 // justify set_value = 1 or = 0
	
 if ( set_value == 0 )
 {
	v = 0x00 ;
 }
 
 if ( set_value == 1 )
 {
	v = 0xff ;
 }
 for ( int i = 0 ; i < bit_field.size() ; i++ )
 {
	bit_field[i] = v ;
 }
 
  return 0 ;
} 

