#include <cstring>
#include <cstdio>
#include <iostream>

#include "message.h"
#include "message_processor.h"
#include "../peer/peer.h"

using namespace std ;


int Hand_Shake_Msg_Processor::process_msg ( Peer &peer , string &msg )
{
  	
  if ( msg.empty () )
  {
	// LOG ..
	return -1 ;
  }  
 
  if ( memcmp ( info_hash.c_str() ,(msg.substr(28,20)).c_str() , 20 ) != 0 )
  {
      cout << "info_hash " << info_hash << endl ;
      cout << "msg sub str " << msg.substr(28,20 ) << endl ;
	
      cout << "info_hash not qual " << endl ;
      peer.peer_node.state = CLOSING ;
	

     return -1 ;
  }

  strcpy (peer.peer_node.id , msg.substr(48,20).c_str()) ;
  peer.peer_node.id[20] = '\0' ;
	
  if ( peer.peer_node.state = INITIAL )
  {
	peer.peer_node.state = HAND_SHAKED ;
	
  	pAnyMsg = new Hand_Shake_Msg (info_hash, peer_id) ;
	
	pAnyMsg->create_msg ( peer  ) ;
  }

  if ( peer.peer_node.state == HALF_SHAKED )
	peer.peer_node.state = HAND_SHAKED ;
	
  peer.peer_node.start_timestamp = time (NULL) ;
 
  return 0 ;
}
