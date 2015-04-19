#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <glog/logging.h>

#include "peer.h"

using namespace std ;

Peer::Peer ()
{
  peer_node.buff_in = (char*)malloc(MSG_LEN) ;
  
  if (peer_node.buff_in == NULL )
  {
	LOG(ERROR)<<"[error] failed to allocate space to peer_node.buff_in";
  }
 
  peer_node.msg_out = (char*)malloc(MSG_LEN) ;
  
  if (peer_node.msg_out == NULL )
  {
	if ( peer_node.buff_in != NULL )
	  free ( peer_node.buff_in ) ;
	LOG(ERROR)<<"[error] failed to allocate space to peer_node.msg_out";
  }
	
  peer_node.msg_buff_out = (char*)malloc (MSG_LEN) ;
 
  if ( peer_node.msg_buff_out == NULL )
  {
	if ( peer_node.buff_in != NULL )
	  free( peer_node.buff_in ) ;
	if ( peer_node.msg_out != NULL )
	  free ( peer_node.msg_out ) ;
	
	LOG(ERROR)<<"[error] failed to allocate space to peer_node.msg_buff_out" ;
  }

 
}

Peer::~Peer ()
{
	cancel_recv_request_queue () ;
	cancel_send_request_queue () ;
 	
	release_peer_node () ;

        cout << "Peer::~Peer() is called , peer node id = "<<peer_node.id << 
		" recv and send requests are canceled " << endl ;
}

void Peer::release_peer_node ( )
{
  free ( peer_node.buff_in ) ;
  free ( peer_node.msg_out ) ;
  free ( peer_node.msg_buff_out ) ;

  cout << "Peer::release_peer_node is called , every dynamic allocated space is free now " << endl ; 
}

void Peer::print ( )
{

    cout << "my peer id ----> "<< peer_node.id << endl ;
/*
    cout << "am_chocking "<< peer_node.am_chocking << endl ;
    cout << "am_interested " << peer_node.am_interested << endl;
  
    cout << "peer_chocking" << peer_node.peer_chocking << endl ;
    cout << "peer_interested" << peer_node.peer_interested << endl ;
	
    cout << "down_rate " << peer_node.down_rate << endl ;
    cout << "up_rate " << peer_node.up_rate << endl ;
*/

    cout << "peer port " << peer_node.port << endl ;
}


int Peer::cancel_recv_request_queue (  )
{
  peer_node.recv_download_request_queue.clear() ;
}

int Peer::cancel_send_request_queue  ()
{
  peer_node.send_upload_request_queue.clear () ;
}

