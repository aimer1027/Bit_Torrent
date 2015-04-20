#include <cstdio>
#include <iostream>
#include <cstring>

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


int Hand_Shake_Msg::process_msg ( Peer &peer , string & msg )
{ 
  AnyMsg *pAnyMsg = NULL ;

  if (  msg.empty()  )
  {
	// LOG warn...
	return -1 ;
  }
	
  if ( memcmp ( info_hash.c_str() , (msg.substr(28,20)).c_str()  ,20 ) != 0 )
  {
        cout << "info_hash not equal" << endl ;
	peer.peer_node.state = CLOSING ;
	// discard_send_buffer (peer) ;
	// clear_btcache_before_peer_close( peer ) ;
	// close (peer.socket) ;
 	
	// LOG warn ..
	
	return -1 ; 
 }

 // copy buff's id info into peer's id 

 strcpy(  peer.peer_node.id , msg.substr( 48 , 20 ).c_str() ) ;
 
 peer.peer_node.id[20] ='\0' ;
	
 if ( peer.peer_node.state == INITIAL )
 {
     peer.peer_node.state = HAND_SHAKED ;
     
     pAnyMsg = new Hand_Shake_Msg ( info_hash, peer_id) ;    
 
     pAnyMsg->create_msg ( peer ) ;
 }

 if ( peer.peer_node.state == HALF_SHAKED )
   peer.peer_node.state = HAND_SHAKED ;
 
   peer.peer_node.start_timestamp = time(NULL) ;
   
   return 0 ;
}












