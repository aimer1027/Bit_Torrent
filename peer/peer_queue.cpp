#include <time.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <cstring>

#include "peer.h"
#include "peer_queue.h"

using namespace std ;

Peer_Queue::Peer_Queue ()
{}

Peer_Queue::~Peer_Queue ()
{
}


int Peer_Queue::add_peer_node ( Peer *peer_node  )
{
  peer_queue.push_back ( peer_node ) ;
}


// delete the node which has the same id 
// as the input peer's 
// do not forget release the space of peer's buff_in , msg_out , msg_buff_out

int Peer_Queue::del_peer_node ( Peer *peer )
{
   int ret = -1 ;

   for ( vector<Peer*>::iterator it = peer_queue.begin () ;
			it != peer_queue.end () ; it++ )
   {
	if (!strcmp((*it)->peer_node.id , peer->peer_node.id))
	{
		cout << "success remove  peer id =  "<< (*it)->peer_node.port << " node " << endl ;
		delete *it ;
	
	        it = peer_queue.erase(it) ;
	
		ret = 0 ;
		break ;		   
     }
   }

  if ( ret == -1 ) 
    cout << "not find target peer id = " << peer->peer_node.port  <<  "node "<< endl ;
 
  return ret ;
	
}

void Peer_Queue::release_peer_queue_nodes ()
{
   for ( vector<Peer*>::iterator it = peer_queue.begin () ;
		it != peer_queue.end() ; it++  )
   {
	// *it type: Peer*
	delete *it ;               // release the Peer object
				// take it easy ,
	// here delete will call the destructor method of Peer 
	// and we have already write the release_peer_node inside ~Peer
   }

   // then clear the queue
  peer_queue.clear() ;
}


void Peer_Queue::print ()
{
   // add by Aimer 2015/4/19

   if ( peer_queue.empty () )
   {
	cout << "empty queue , without any peer node in it "<<endl ;
	return ;
   }

   for ( vector<Peer*>::iterator it = peer_queue.begin () ;
		it != peer_queue.end () ; it++ )
   {
	(*it)->print () ;
   }
}
