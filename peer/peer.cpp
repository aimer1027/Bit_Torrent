#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <glog/logging.h>

#include "peer.h"

using namespace std ;

Peer::Peer ()
{
 
}

Peer::~Peer ()
{
	cancel_recv_request_queue () ;
	cancel_send_request_queue () ;
 	
       // cout << "Peer::~Peer() is called , peer node id = "<<peer_node.id << 
	//	" recv and send requests are canceled " << endl ;
}

void Peer::print ( )
{

    cout << "my peer id ----> "<< peer_node.id << endl ;
    cout << "my msg out ----->" << peer_node.msg_out << endl ;
/*
    cout << "am_chocking "<< peer_node.am_chocking << endl ;
    cout << "am_interested " << peer_node.am_interested << endl;
  
    cout << "peer_chocking" << peer_node.peer_chocking << endl ;
    cout << "peer_interested" << peer_node.peer_interested << endl ;
	
    cout << "down_rate " << peer_node.down_rate << endl ;
    cout << "up_rate " << peer_node.up_rate << endl ;


    cout << "peer port " << peer_node.port << endl ;
*/
    cout << "peer state " << endl ;
	
    switch ( peer_node.state )
    {
	case INITIAL:
	{
	   cout << "INITIAL " << endl ;
	   break ;
	}
	case HALF_SHAKED :
	{
	   cout << "HALF_SHAKED " << endl ;
	   break ;
	}
	
	case HAND_SHAKED:
	{
	   cout << "HAND_SHAKED "<< endl ;
	   break ;
	}
	
	case SEND_BIT_FIELD:
	{
	   cout << "SEND_BIT_FIELD "<<endl ;
	   break ;
	}
	
	case RECV_BIT_FIELD:
	{
	   cout << "RECV_BIT_FIELD " << endl ;
	   break ;
	}
	
	case EXCHANGING_DATA:
	{
	   cout << "EXCHANGING_DATA "<<endl ;
	   break ;
	}
	
	case CLOSING :
	{
	   cout << "CLOSING "<< endl ;
	   break ;
	}
	default: 
	  cout <<"unknow state "<< endl ;
    }

}


int Peer::cancel_recv_request_queue (  )
{
  peer_node.recv_download_request_queue.clear() ;
}

int Peer::cancel_send_request_queue  ()
{
  peer_node.send_upload_request_queue.clear () ;
}

