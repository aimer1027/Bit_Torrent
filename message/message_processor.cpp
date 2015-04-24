#include <cstring>
#include <cstdio>
#include <iostream>

#include "message.h"
#include "message_processor.h"
#include "../peer/peer.h"
#include "../bit_map/bit_map.h"
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


int Keep_Alive_Msg_Processor::process_msg ( Peer &peer , string& msg  )
{
  if ( msg.empty () )
  {
      // LOG
      return 0 ;
  }

  peer.peer_node.start_timestamp = time (NULL) ;
  
  return 0 ;
}

int Have_Msg_Processor::process_msg ( Peer &peer , string &msg )
{
   int rand_num ;
   unsigned char c[4] ;
   
   if ( msg.empty () )
   {
	return -1 ;
   }
   
   srand( time(NULL)  ) ;
   rand_num = rand() % 3 ;
   
   if ( peer.peer_node.state != CLOSING && peer.peer_node.state == EXCHANGING_DATA )
   {
	c[0] = msg[5] ; c[1] = msg[6];
        c[2] = msg[7] ; c[3] = msg[8] ;
     
        if (! peer.peer_node.pBitmap->bit_field.empty () )
	  peer.peer_node.pBitmap->set_bit_map_value (AnyMsg::char_to_int(c),1) ;

	if ( peer.peer_node.am_interested == 0 )
	{
	   if (  peer.peer_node.pBitmap->am_i_interested_in_peer ( _bitmap  ) ) 
		pAnyMsg = new Chock_Interested_Msg ( 2 ) ;
		pAnyMsg->create_msg (peer) ;
	}
	else
	{
	    if ( rand_num == 0 ) 
	    {
		pAnyMsg = new Chock_Interested_Msg (2) ;
		pAnyMsg->create_msg (peer) ;
            }
        }

   }

  peer.peer_node.start_timestamp = time (NULL) ;
  
  return 0 ;
}

// need add a send BitMap constructor

Bit_Field_Msg_Processor::Bit_Field_Msg_Processor( Bitmap &bitmap )
{
   _bitmap = &bitmap ;
}


int Bit_Field_Msg_Processor::process_msg ( Peer &peer , string &msg  )
{
  unsigned char c[4] ;
  
  if ( msg.empty () )
  {
	return -1 ;
  }
  
  if ( peer.peer_node.state == HAND_SHAKED || peer.peer_node.state == SEND_BIT_FIELD )
  {
	for ( int i = 0 ; i < 4 ; i++ )
		c[i] = msg[i] ;
	
	if ( peer.peer_node.pBitmap->bit_field.size ()!= 0 )
	{
		peer.peer_node.pBitmap->bit_field.clear () ;
	}
	
	peer.peer_node.pBitmap->bit_length = _bitmap->bit_length ;
	
	if ( _bitmap->bit_field.size() != (AnyMsg::char_to_int(c)-1) )
	{
		peer.peer_node.state = CLOSING ;

		// discard_send_buffer
		// clear_btcache_...
		// close( peer.peer_node.socket ) ;
		return -1 ;
	}
	
	// something shoud be rewrite here 
	//peer.peer_node.pBitmap.append ((msg+5), char_to_int(c)-1) ;
  
	if ( peer.peer_node.state == HAND_SHAKED )
	{
// something has to write , method or other thing should be written here 
// that transfer the vector<char> into string 

	  pAnyMsg = new Bit_Field_Msg ( peer.peer_node.buff_in  ) ;
	  peer.peer_node.state = EXCHANGING_DATA ;
	}
	
	if ( peer.peer_node.state == SEND_BIT_FIELD )
	   peer.peer_node.state = EXCHANGING_DATA ;
	
   	peer.peer_node.am_interested = 
		peer.peer_node.pBitmap->am_i_interested_in_peer ( _bitmap  ) ;
  
	if ( peer.peer_node.am_interested == 1 )
	{
		pAnyMsg = new Chock_Interested_Msg (2) ;
  		pAnyMsg->create_msg (peer) ;
	}
   }
	
	peer.peer_node.start_timestamp = time ( NULL ) ;
	return 0 ;
} 
