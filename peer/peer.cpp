#include <cstdio>
#include <iostream>
#include <vector>
#include <glog/logging.h>

#include "peer.h"

vector<Peer> peer_queue ;

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
	if ( peer_node.buf_in != NULL )
	  free ( peer_node.buf_in ) ;
	LOG(ERROR)<<"[error] failed to allocate space to peer_node.msg_out";
  }
	
  peer_node.msg_buff_out = (char*)malloc (MSG_LEN) ;
 
  if ( peer_node.msg_buff_out == NULL )
  {
	if ( peer_node.buf_in != NULL )
	  free( peer_node.buf_in ) ;
	if ( peer_node.msg_out != NULL )
	  free ( peer_node.msg_out ) ;
	
	LOG(ERROR)<<"[error] failed to allocate space to peer_node.msg_buff_out" ;
  }

}

peer_node_t & Peer::add_peer_node ( vector<peer_node_t>& peer_nodes_queue )
{
  peer_nodes_list.push_back ( peer_node ) ;
}

int Peer::del_peer_node ( vector<peer_node_t>& peer_nodes_queue )
{
  int ret = -1 ;

  for ( vector<peer_node_t>::iterator it = peer_node_queue.begin() ;
		it ! peer_node_queue.end () ; it++ )
  {
	if ( !strcmp (it->id, peer_node.id))
        {
		peer_node_t &pDelNode = *it ;
		
		// remove target node from queue 
		it = peer_node_queue.erase( it  ) ;
		
		// after remove node from queue , release its space
		release_peer_node (pDelNode) ;
		
		ret = 0 ;
		break ;
	}
  }

  return ret ;

}

void Peer::release_peer_node ( peer_node_t &release_peer_node  )
{
  free ( release_peer_node.buff_in ) ;
  free ( release_peer_node.msg_out ) ;
  free ( release_peer_node.msg_buff_out ) ;
	
}

void Peer::print_peer_node ( peer_node_t &p_node )
{
  cout << "my peer id "<< p_node.id << endl ;
  cout << "am_chocking "<< p_node.am_chocking << endl ;
  cout << "am_interested " << p_node.am_interested < endl;
  
  cout << "peer_chocking" << p_node.peer_chocking << endl ;
  cout << "peer_interested" << p_node.peer_interested << endl ;
	
  cout << "down_rate " << p_node.down_rate << endl ;
  cout << "up_rate " << p_node.up_rate << endl ;

}

void Peer::print ( vector<peer_node_t>& peer_queue )
{
  for (vector<peer_node_t>::iterator it = peer_queue.begin() ;
		it != peer_queue.end () ; it++  )
  {
		print_peer_node (*it) ;
  }
}


int Peer::cancel_recv_download_request(  )
{
  recv_download_request_queue.clear() ;
}

int Peer::cancel_send_upload_request ()
{
  send_upload_request_queue.clear () ;
}

