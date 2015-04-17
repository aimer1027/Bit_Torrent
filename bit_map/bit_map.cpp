#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <glog/logging.h>

#include "bit_map.h"

#define set_value_one(value,index) value|=(1<<index)
#define set_value_zero(value,index) value&=~(1<<index)


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
	LOG(WARNING)<<"[warning] index not illegal ";
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
   int pos_byte = index / 8 ;
   int pos_bit  = index % 8 ;

   printf ("pos_byte %d  pos_bit %d\n", pos_byte , pos_bit) ;

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

   char value = bit_field[pos_byte] ;
   // now divide two branches by set_value = 1 or 0
   if ( set_value == 1)
   {
	
	printf ("before %x \n", bit_field[pos_byte]) ;
	// call set_value_one 
   	set_value_one(value,pos_bit ) ;
        bit_field[pos_byte] = value ;
	printf ("after  %x \n",bit_field[pos_byte]) ;
   }
   
   if ( set_value == 0 ) 
   {
	printf ("before %x \n", bit_field[pos_byte]) ;
	// call set_value_zero
  	set_value_zero(value,pos_bit) ;
  	bit_field[pos_byte] = value ;
	printf ("after %x \n", bit_field[pos_byte]) ;
   } 

  
   return 0 ;
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
	
//	printf ("\n") ;
 }

 printf ("\n") ;
}


// add by Aimer 2015/4/17
int Bitmap::restore_bitmap  ( char *file_path )
{
	int 	fd ;
	char 	file_name[64] ;
	
	if (file_path==NULL) 
	{
		LOG(WARNING)<<"[warning]  file_path is empty" ;
		return -1 ;
	}
	
	sprintf (file_name, "%s//bitmap%d.dat" , file_path , bit_length ) ;
	fd = open ( file_name , O_RDWR | O_CREAT | O_TRUNC , 0666  ) ;
	if ( fd < 0 )
	{
		LOG(WARNING)<<"[warning] failed to open file  %s "<< file_name ;
		return -1 ;
	}

	for ( int i = 0 ; i < bit_field.size () ;i++ )
	{
		if ((write ( fd , &bit_field[i], 1 ) ) != 1 )
		{
			LOG(WARNING)<<"[warning] failed in writing into file ,error in %d byte "<< i ;
			return -1 ;
		}
	}
	
	close ( fd ) ;
	return 0 ;
}

// add by Aimer 2015/4/17
int Bitmap::get_download_piece_num ()
{
	unsigned char test_char [8] =
	{
		0x80, // 1000 0000
		0x40, // 0100 0000
		0x20, // 0010 0000
		0x10, // 0001 0000
		0x08, // 0000 1000
		0x04, // 0000 0100
		0x02, // 0000 0010
		0x01, // 0000 0001  
	} ;
	
	int download_piece_num = 0 ;
	for ( int i = 0 ; i < (byte_length-1); i++ )
	{
		for ( int j = 0 ; j < 8 ; j++ )
		{
			if ( test_char[j] & bit_field[i] )
				download_piece_num++ ;
		}
	}
	
	// the bit_length % 8 , mod remain bits count
	int limit = (bit_length%8)?(bit_length%8):8 ;

	for ( int j = 0 ; j < limit ; j++ )
		if ( test_char[j] & bit_field[byte_length-1] )
			download_piece_num ++ ;
	
	return download_piece_num ;
}
