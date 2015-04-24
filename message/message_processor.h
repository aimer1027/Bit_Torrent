#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include <cstring>
#include <cmath>
#include <cstdlib>

#include "../peer/peer.h"
#include "../bit_map/bit_map.h"
#include "message.h"

class Any_Msg_Processor
{
  public :
     std::string info_hash ;
     std::string peer_id   ;
     AnyMsg *pAnyMsg ;
     Bitmap *_bitmap ;

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


class Keep_Alive_Msg_Processor : public Any_Msg_Processor
{
  public :
	virtual int process_msg ( Peer &peer , std::string &msg ) ;
} ;


class Have_Msg_Processor : public Any_Msg_Processor
{
  public :
       virtual int process_msg ( Peer &peer , std::string &msg ) ;
} ;

class Bit_Field_Msg_Processor : public Any_Msg_Processor
{
  public :
        Bit_Field_Msg_Processor ( Bitmap &bitmap );
	virtual int process_msg ( Peer &peer , std::string &msg ) ;
} ;


class Request_Msg_Processor : public Any_Msg_Processor
{
   public :
	virtual int process_msg ( Peer &peer , std::string &msg ) ;
} ;

class Piece_Msg_Processor : public Any_Msg_Processor
{
   public :
	virtual int process_msg ( Peer &peer , std::string &msg ) ;
} ;

class Cancel_Msg_Processor : public Any_Msg_Processor
{
   public :
	virtual int process_msg ( Peer &peer , std::string &msg ) ;
} ;


class Port_Msg_Processor : public Any_Msg_Processor
{
   public :
	virtual int process_msg ( Peer &peer , std::string &msg ) ;
} ;


#endif // message_processor.h
