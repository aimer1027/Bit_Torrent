#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include <cstring>

#include "../peer/peer.h"
#include "message.h"

class Any_Msg_Processor
{
  public :
     std::string info_hash ;
     std::string peer_id   ;
     AnyMsg *pAnyMsg ;
  public :

     virtual int process_msg (Peer &peer , string &msg ) ;
} ;

class Hand_Shake_Msg_Processor : public Any_Msg_Processor 
{
  public :
	virtual int process_msg ( Peer &peer , string &msg ) ;
} ;



#endif // message_processor.h
