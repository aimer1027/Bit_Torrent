#ifndef BIT_MAP_H
#define BIT_MAP_H

#include <vector>

class Bitmap
{
private:
	std::vector<char> bit_field ;
	int byte_length ;
	int bit_length ;
public :
	Bitmap () {}
	Bitmap ( int need_bit_length )
	{
		bit_length = need_bit_length ;
		byte_length = need_bit_length / 8 ;
	
		// i am afriaid we need one more byte 
		if ( (need_bit_length%8) != 0  )
		   byte_length += 1 ;
		
		for ( int i = 0 ; i < byte_length ; i++ )
		{
			unsigned char v = 0x00 ; //all bit initialized into 0
			bit_field.push_back ( v  ) ;
		}
	}

	~Bitmap ()
	{
		bit_field.clear () ;
	}

	
	int get_bit_map_value ( int index  ) ;
	int set_bit_map_value ( int index , int set_value ) ;
	int set_all ( int set_value ) ;
	void print () ;

// add by Aimer 2015/4/17
	int restore_bitmap ( char *file_path) ;

// add by Aimer 2015/4/17
	int get_download_piece_num () ;

// add by Aimer 2015/4/17
	bool am_i_interested_in_peer ( Bitmap *peer ) ;
	
} ;

#endif
