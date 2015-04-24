#include <cstdio>
#include <iostream>
#include <cstring>
#include <glog/logging.h>

#include "../peer/peer.h"
#include "message.h"

using namespace std ;

string info_hash="32167321673216732167" ;
string peer_id ;


/*
  method AnyMsg::int_to_char 
  is used to transfer integer value decimal
  into hexadecimal presented by char 
  char [0] --> [3] 
  high ---> low
*/

int AnyMsg::int_to_char ( int i , unsigned char c[] )
{
   c[3] = i %256 ;
   c[2] = (i-c[3] )/256%256 ;
   c[1] = (i-c[3] -c[2]*256)/256/256%256 ;
   c[0] = (i-c[3] - c[2]*256 - c[1]*256*256)/256/256/256%256 ;
}

int AnyMsg::char_to_int ( unsigned char c[] )
{
  return ( c[3] + c[2] *256 +c[1]*256*256 +c[0]*256*256*256  ) ;
}

int Hand_Shake_Msg::create_msg  ( Peer &peer )
{

 string keyword = "BitTorrent protocol" ;
 
 int remain_len = MSG_LEN - peer.peer_node.msg_out.size () ;
 
 char c_num = 19 ;
 char c = 0x00 ;

 if ( remain_len < 68 )
 {
  // LOG ... warn..
  return -1 ;
 }

 peer.peer_node.msg_out.append (1 , c_num ) ;
 peer.peer_node.msg_out.append ( keyword, 0  , keyword.size());
 peer.peer_node.msg_out.append ( 8,  c ) ;

 peer.peer_node.msg_out.append ( _info_hash, 0 , _info_hash.size() ) ;
 peer.peer_node.msg_out.append ( _peer_id , 0 , _peer_id.size() );

 return 0 ;
	
}

int Keep_Alive_Msg::create_msg ( Peer &peer )
{
   int remain_len = MSG_LEN - peer.peer_node.msg_out.size() ;
	
   if ( remain_len < 4 )
   {
	// LOG() ...
	return -1 ;
   }
	
   char c = 0 ;
   peer.peer_node.msg_out.append (4,c) ;
	
   return 0 ;
}

int Chock_Interested_Msg::create_msg  ( Peer &peer  )
{
     int remain_len = MSG_LEN - peer.peer_node.msg_out.size() ;
     
    // chock , unchock , interested ,uninterested message len = 5
     if ( remain_len < 5 )
     {
	// LOG...
	return -1 ;
     }

     char c = 0 ;

     peer.peer_node.msg_out.append (3,c) ;
     
     c = 1 ;
     peer.peer_node.msg_out.append ( 1 , c );
   
     c =_type ;
     peer.peer_node.msg_out.append ( 1, c ) ;

    return 0 ;
}

int Have_Msg::create_msg ( Peer &peer )
{
     int remain_len = MSG_LEN - peer.peer_node.msg_out.size() ;
     unsigned char c[4] ;
	
     if ( remain_len < 9 )
     {
	//LOG(ERROR)<<"[error] remain space smaller than 9 ,have message len = 9 " ;
     
	return -1 ;
     }

     char cc = 0 ;
     int len = peer.peer_node.msg_out.size() ;
  
     peer.peer_node.msg_out.append (3, cc ) ;
	
     cc = 5 ;
     peer.peer_node.msg_out.append( 1 , cc) ;
   
     cc = 4 ;
     peer.peer_node.msg_out.append(1, cc) ;

     int_to_char ( _index , c ) ;
     
     for ( int i = 0 ; i < 4 ; i++ )
     peer.peer_node.msg_out.append( 1,  c[i] ) ;
     	
     return 0 ;

}

int Bit_Field_Msg::create_msg ( Peer &peer )
{
    unsigned char c[4] ;

    int remain_len = MSG_LEN - peer.peer_node.msg_out.size() ;
	
    if ( remain_len < _bitfield.size() + 5 )
    {
	//LOG(ERROR)<<"[error] remain space too small " ;
	return -1 ;
    }	

  
  int_to_char ( (int)(_bitfield.size()) +1, c) ;

  for ( int i = 0 ; i < 4 ; i ++ )
  peer.peer_node.msg_out.append (1 , c[i]) ;

 
  int cc = 5 ;
  
  peer.peer_node.msg_out.append (1, cc) ;
 
  // append  string directly 

     peer.peer_node.msg_out.append ( _bitfield ) ;
}


int Request_Msg::create_msg ( Peer &peer )
{
   unsigned char c[4] ;
   int remain_len = MSG_LEN - peer.peer_node.msg_out.size () ;
   unsigned char cc = 0 ;	

   if ( remain_len < 17 )
   {
	//LOG(WARNING)<<"[warning] remain spaces not enough ";
	return -1 ;
   }
  
   peer.peer_node.msg_out.append(3,cc) ; 
   
   cc = 13 ;
   peer.peer_node.msg_out.append (1,cc) ; 
   
   cc = 6 ;
   peer.peer_node.msg_out.append (1,cc) ;
  
  int_to_char(_index , c ) ;  
  for ( int i = 0 ; i < 4 ; i++ )
  peer.peer_node.msg_out.append(1 ,c[i]) ;

  
  int_to_char (_begin , c ) ; 
  for ( int i = 0 ; i < 4 ; i++ )
  peer.peer_node.msg_out.append (1, c[i]) ;
 
  int_to_char ( _length , c ) ;
  for ( int i = 0 ; i < 4 ; i++ )
  peer.peer_node.msg_out.append (1, c[i] ) ;
  
  return 0 ;
}

int Piece_Msg::create_msg ( Peer &peer )
{
   int remain_len = MSG_LEN - peer.peer_node.msg_out.size () ;
   unsigned char c[4] ;
  
   if ( remain_len < _block.size() +13 )
   {
	//LOG(WARNING)<<"[warning] buffer too small ";
        return -1 ;
   }
  
   int_to_char( _block.size()+9 , c) ;
   for ( int i = 0 ; i < 4 ; i++ )
   peer.peer_node.msg_out.append (1, c[i]) ;
  
   int_to_char ( _index , c ) ;
   for ( int i = 0 ; i < 4 ; i++ )
   peer.peer_node.msg_out.append ( 1, c[i] ) ; 
  
   int_to_char (_begin , c ) ;
   for ( int i = 0 ; i < 4 ; i++ )
   peer.peer_node.msg_out.append( 1 ,c[i] ) ;
   
   peer.peer_node.msg_out.append (_block) ;   
  
   return 0 ;
}

int Cancel_Msg::create_msg ( Peer &peer )
{
   int remain_len = MSG_LEN - peer.peer_node.msg_out.size () ;
   unsigned char c[4] ;
   unsigned char cc = 0 ;   

   if ( remain_len < 17 )
   {
     //LOG(WARNING)<<"[warning] remain space too small " ;
     return -1 ;
   }
   
   peer.peer_node.msg_out.append (3, cc) ;
   
   cc = 13 ;
   peer.peer_node.msg_out.append ( 1, cc ) ;

   cc = 8 ;
   peer.peer_node.msg_out.append ( 1, cc ) ;
  
   int_to_char(_index , c ) ;
   for ( int i = 0 ; i < 4 ;i++ )
   peer.peer_node.msg_out.append ( 1,c[i] ) ;

   int_to_char (_begin , c ) ;
   for ( int i = 0 ; i < 4 ; i++ )
   peer.peer_node.msg_out.append ( 1, c[i] ) ;
   
   int_to_char (_length , c ) ;
   for ( int i = 0 ; i < 4 ; i++ )
   peer.peer_node.msg_out.append ( 1 ,c[i] ) ;

   return 0 ;
} 

int Port_Msg::create_msg ( Peer &peer )
{
   unsigned char c[4] ;
   int remain_len = MSG_LEN - peer.peer_node.msg_out.size () ;
   unsigned char cc = 0 ;   

   if ( remain_len < 7 )
   { 
      //LOG(WARNING)<<"[warning] buffer space too small "; 
      return -1 ;
   }
   
   peer.peer_node.msg_out.append ( 3, cc ) ;
   
   cc = 3 ;
   peer.peer_node.msg_out.append ( 1, cc ) ;
  
   cc = 9 ;
   peer.peer_node.msg_out.append ( 1, cc ) ;

   int_to_char( _port , c ) ;
   for ( int i = 0 ; i < 4 ; i++ )
   peer.peer_node.msg_out.append (1 ,c[i]) ; 
    
   return 0 ;
}



