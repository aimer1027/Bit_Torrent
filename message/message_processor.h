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
     Any_Msg_Processor (  ) 
     {
		pAnyMsg = NULL ;
     }
   
     virtual int process_msg (Peer &peer , std::string &msg ) = 0 ;
} ;


class Hand_Shake_Msg_Processor : public Any_Msg_Processor 
{
  public :
	Hand_Shake_Msg_Processor ( std::string &info__hash , std::string &peer__id )
    	{
		info_hash = info__hash ;
		peer_id   = peer__id ;
        }

	virtual int process_msg ( Peer &peer , std::string &msg ) ;
} ;



#endif // message_processor.h
