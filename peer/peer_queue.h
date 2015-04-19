#incldue <time.h>
#include <vector>

#include "peer.h"

class Peer_Queue
{
  private :
	vector<Peer*> peer_queue ;
  public :
	Peer_Queue () ;
	~Peer_QUeue () ;
        
       int add_peer_node ( Peer *peer_node ) ;
       int del_peer_node ( Peer *peer_node ) ;
       void release_peer_queue_nodes () ;
       void print() ; 
} ;
