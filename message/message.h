#ifndef MESSAGE_H
#define MESSAGE_H

#include <bson.h>

enum MESSAGE_TYPE
{
  NONE = 0 ,
  HAND_SHAKE,
  KEEP_ALIVE
} ;

typedef struct msg_head
{
  int len ;
  int type ;
 
   msg_head () : len(0) , type(NONE)
   {}
}head_t ;

typedef struct hand_shake_msg
{
  head_t header ;
  char   data[0] ;
} hand_shake_msg_t ;

typedef struct keep_alive_msg
{
  head_t header ;
} keep_alive_msg_t ;



class HandShakeMsgOpt 
{
  private :
	hand_shake_msg_t *pMessage ;
	string hash_info ;
	string peer_id ;
  public :

 int buildMessage ( char **ppBuffer ,int *pBufferSize ,	bson::BSONObj *obj ) ;
 int parseMessage ( char *pBuffer ,  bson::BSONObj &msgData ) ;

 ~HandShakeMsgOpt ()   {}
} ;

/*
class KeepAliveMsgOpt 
{
   private :
	keep_alive_msg_t *pMessage ;
  
 public :
 int buildMessage ( char **ppBuffer , int *pBufferSize , bson::BSONObj *obj ) ;
 int parseMessage ( char *pBuffer ,bson::BSONObj &msgData ) ;

 ~KeepAliveMsgOpt () {}
} ;
*/


#endif // message
