#include <time.h>
#include <vector>
#include <stdint.h>

#include "peer.h"

class Peer_Queue
{
  public :
	std::vector < Peer* > peer_queue ;
  public :
	Peer_Queue () ;
	~Peer_Queue () ;
        
       int add_peer_node ( Peer *peer_node ) ;
       int del_peer_node ( Peer *peer_node ) ;
       void release_peer_queue_nodes () ;
       void print() ; 
} ;
