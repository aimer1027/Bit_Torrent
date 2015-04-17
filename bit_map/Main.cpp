#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <glog/logging.h>
#include "bit_map.h"

using namespace std ;

int main ( int argc , char **argv )
{
    int need_bit_length = 1024 ;

    // initialize log system
    google::InitGoogleLogging( argv[0] ) ;
    // set log path
    FLAGS_log_dir = "./log/" ;
	
// test Bitmap's constructor 
    Bitmap bitMap(need_bit_length) ; 
 
// test Bitmap's set all 
    bitMap.set_all( 1 ) ;
    
// test Bitmap's print
    bitMap.print () ;
  
    int index = 5 ;
   
// test Bitmap's set_bit_map_value
    bitMap.set_bit_map_value ( index , 0 ) ; 

    bitMap.print () ;
	
// test Bitmap's get_bit_map_value
   
    int result = bitMap.get_bit_map_value ( index ) ;
    // it must should be 0
   
    printf (" get value = %d \n",result  ) ;

// test Bitmap's restore_bitmap
   char file_path [64] ;
   
   printf ("input file path to store bitmap content into file \n") ;
   scanf ("%s" , file_path) ;
    bitMap.restore_bitmap ( file_path ) ; 


// test Bitmap's get_download_piece_num 
	int download_counter = 
		bitMap.get_download_piece_num () ;
// 1023  should be the right result 
   	printf ("download_piece_num : %d \n" , download_counter  ) ;
	

// test Bitmap's am_i_interested_in_peer 
	Bitmap peer_bitMap ( 1024 ) ;
	peer_bitMap.set_all (1) ;
	
        if ( bitMap.am_i_interested_in_peer ( &peer_bitMap ) ) 
	{
		printf ("I am interested in peer's resource pieces \n") ;
	}
	else
	{
		printf ("I am not interested in peer's reousrce pieces \n") ;
	}

    return 0 ;
}
