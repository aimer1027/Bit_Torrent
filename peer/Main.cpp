#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "peer.h"
#include "peer_queue.h"

using namespace std ;

int main ( int argc , char **argv )
{
  // testify : here we are going to test 
  // 1. Peer::Peer () 
  // 2. Peer_Queue::add_peer_node ( Peer *peer_node)
  // 3. Peer_Queue::print() 
  // 4. Peer::print () // in queue print method  should test whether it is empty
  

  Peer *pPeer ;  
  Peer_Queue peer_queue ;

  for (  int i = 0 ; i < 20 ; i++ )
  {
	pPeer = new Peer () ;	
	pPeer->peer_node.port = (i+1) ;
        peer_queue.add_peer_node (pPeer) ;
  }

 //peer_queue.print () ;

  // tesitfy Peer_Queue::del_peer_node( Peer *peer_node )

/*
  int len = peer_queue.peer_queue.size() ;

  for ( int i = 0 ; i < len ; i++ )
  {
      peer_queue.del_peer_node ( peer_queue.peer_queue[i] )  ;
  }
*/
 
// test Peer_Queue::release_peer_queue_nodes()
         peer_queue.release_peer_queue_nodes () ;


//  peer_queue.print () ;


// here we test delete one special in the peer_queue
   for ( int i = 0 ; i < 15 ; i++ )
   {
	pPeer = new Peer () ;
	pPeer->peer_node.port = i*10  ;
        
        if (i == 7 )
           strcpy (pPeer->peer_node.id , "delete_this_node") ;
	else
	   strcpy (pPeer->peer_node.id , "no id") ;

	// after this , this id will be the "delete_this_node"
         peer_queue.add_peer_node (pPeer) ;

  }
    
  peer_queue.print () ;
 
// here we are going to test : delete the i = 7 by its id value

// and in case of leaking of the memory
// we write somthing in the ~Peer and release_peer_node methods to testfy 
// whether these method will be executed by calling del_peer_node's delete command


  pPeer = peer_queue.peer_queue[7] ;
  
  peer_queue.del_peer_node( pPeer ) ;

peer_queue.print () ;




  return 0 ;
}
