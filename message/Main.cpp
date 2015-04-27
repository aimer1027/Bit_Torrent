#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <bson.h>

#include "message.h"

using namespace std ;
using namespace bson ;

int main ( int argc , char **argv )
{
  string msg_hash_info ;
  string msg_peer_id ;
  BSONObj obj ;
  BSONObjBuilder builder ; 
  char *ppBuffer = NULL  ;  
  int   pBufferSize = 0;
  HandShakeMsgOpt *hand_shake_msg_opt = new HandShakeMsgOpt ()  ; 


  cout << "test HandShakeMsgOpt::buildMessage "<< endl ;
  cout << " input handshake message hash info (len=20) "<< endl ;
  cin >> msg_hash_info ;
  cout << " input handshake message peer id (len=20) "<< endl ;
  cin >> msg_peer_id  ;
 
  builder.append("hash info", msg_hash_info.c_str ()) ;

  builder.append("peer id"   , msg_peer_id.c_str ()) ;
  
  obj = builder.obj () ;
  
  cout << obj.objsize() << endl ;
  cout << obj.toString () << endl ;

 
  hand_shake_msg_opt->buildMessage( &ppBuffer , &pBufferSize , &obj ) ;
  
//  cout << "total message length " << pBufferSize << endl ;

  // now the ppBuffer contain the complete  handshake message 
  // parse it 
  
  cout << "test HandShakeMsgOut::parseMessage " << endl ;

  BSONObj msgData ;

  hand_shake_msg_opt->parseMessage ( ppBuffer, msgData ) ;
  
  cout << "parse handshake message data "<< endl ;
  cout << msgData.toString () << endl ;  

  return 0 ;
}


