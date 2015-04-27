#ifndef MESSAGE_H
#define MESSAGE_H

#include <bson.h>

enum MESSAGE_TYPE
{
  NONE = 0 ,
  HAND_SHAKE,
  KEEP_ALIVE,
  CHOCK_INTERESTED,
  HAVE
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


typedef struct chock_interested_msg
{
   head_t header ;
   char   data[0] ;
} chock_interested_msg_t ;

typedef struct have_msg
{
   head_t header ;
   char   data[0] ;
} ;


class HandShakeMsgOpt 
{
 public  :
	hand_shake_msg_t *pMessage ;
	string hash_info ;
	string peer_id ;
  public :

 int buildMessage ( char **ppBuffer ,int *pBufferSize ,	bson::BSONObj *obj ) ;
 int parseMessage ( char *pBuffer ,  bson::BSONObj &msgData ) ;

 ~HandShakeMsgOpt ()   {}

} ;


class KeepAliveMsgOpt 
{
   private :
	keep_alive_msg_t *pMessage ;
  
 public :
 int buildMessage ( char **ppBuffer , int *pBufferSize , bson::BSONObj *obj ) ;
 int parseMessage ( char *pBuffer ,bson::BSONObj &msgData ) ;

 ~KeepAliveMsgOpt () {}

} ;

enum CHOCK_INTERESTED_TYPE
{
   CHOCKED = 0 ,
   UNCHOCKED   ,
   INTERESTED  ,
   UNINTERESTED
} ;

class ChockInterestedMsgOpt
{
  public :
	int type ;
        chock_interested_msg_t *pMessage ;

  public :
	int buildMessage ( char **ppBuffer , int *pBufferSize , 
					bson::BSONObj *obj ) ;
	int parseMessage ( char *pBuffer , bson::BSONObj &msgData ) ;
  	
	~ChockInterestedMsgOpt () {}
} ;

class HaveMsgOpt
{
  public :
	int index ;
	have_msg_t *pMessage ;
  public :
	int buildMessage ( char **ppBuffer , int *pBufferSize ,
					bson::BSONObj *obj ) ;
	int parseMessage ( char *pBuffer , bson::BSONObj &msgData ) ;
	
	~HaveMsgOpt () {}
} ;




#endif // message
