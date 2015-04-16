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


int Bitmap::get_bit_map_value ( int index )
{
    int pos_byte = index / 8 ;
    int pos_bit  = index % 8 ;

    // first check whether index is leagal
    // index increase by bit

    if ( index < 0 || index > bit_length )
    {
	LOG(WARN)<<"[warning] index not illegal ";
	return -1 ;
    }
  	
    if ( (bit_field[pos_byte] >> pos_bit)&1) 
    {
	return 1 ;
    } 
	
   else
   {
	return 0 ;
   }
  
   return -1 ; // this branch could not be reach 
}

int Bitmap::set_bit_map_value ( int index, int set_value )
{
   // index legal testing , incr by bits 

   if ( index < 0 || index >bit_length  )
   {
	LOG(WARNING)<<"[warning] index illegal range";
	return -1 ;
   } 

  // then set_value
   
   if ( set_value != 0 && set_value != 1 )
   {
	LOG(WARNING)<<"[warning] input set_value error ";
	return -1 ;
   }

   int pos_byte = index / 8 ;
   int pos_bit  = index % 8 ;
   
   // now divide two branches by set_value = 1 or 0
   if ( set_value == 1)
   {
	// call set_value_one 
   	set_value_one(bit_field[pos_byte],pos_bit ) ;
	
   }
   
   if ( set_value == 0 ) 
   {
	// call set_value_zero
  	set_value_zero(bit_field[pos_byte],pos_bit) ;
   } 

  
   return 1 ;
}


void Bitmap::print ()
{
  // this method is used to output each bit in each unsigned char 
  // as element stored in each vector<char> bit_field ;
	
  for ( int i = 0 ; i < bit_field.size() ;i++  )
  {
	for ( int j = 0 ; j < 7 ; j++ ) // each unsigned char has 8 bits [0...7]
 	{
		if ( (bit_field[i] >> j) &1 )
		   printf ("1") ;
		else
		   printf ("0") ;
	} 

 }
}

