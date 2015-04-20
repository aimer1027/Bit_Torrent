#include <cstdio>
#include <iostream>
#include <cstring>

#include "../peer/peer.h"
#include "message.h"

using namespace std ;

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
 peer.peer_node.msg_out.append (1,  c ) ;

 peer.peer_node.msg_out.append ( _info_hash, 0 , _info_hash.size() ) ;
 peer.peer_node.msg_out.append ( _peer_id , 0 , _peer_id.size() );

 return 0 ;
	
}

int Hand_Shake::parse_msg ( Peer &peer , string & msg )
{
  if ( peer == NULL || msg == NULL )
  {
	// LOG warn...
	return -1 ;
  }
	
  if ( memcmp ( info_hash.c_str() , (msg.substr(28,20)).c_str  ,20 ) != 0 )
  {
	peer.state = CLOSING ;
	// discard_send_buffer (peer) ;
	// clear_btcache_before_peer_close( peer ) ;
	// close (peer.socket) ;
 	
	// LOG warn ..
	
	return -1 ; 
 }

 // copy buff's id info into peer's id 
 peer.id.assign( msg, 48, 20  ) ;
 
 peer.id[20] ='\0' ;
	
 if ( peer.state == INITIAL )
 {
     peer.state = HAND_SHAKED ;
     create_msg ( ) ;
 }

}












