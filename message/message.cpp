#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <bson.h>

#include "message.h"

using namespace std ;
using namespace bson ;

/*
int MessageOperator::getMessageType ( char *pBuffer )
{
  head_t *pHeader = (head_t*)pBuffer ;
  
  int type = pHeader->type ;
  
  return type ;
}

*/

int HandShakeMsgOpt::buildMessage (char **ppBuffer ,int *pBufferSize , BSONObj *obj )
{
 int size = sizeof (hand_shake_msg_t) ;
 
 if ( obj != NULL )
    size += obj->objsize () ;

// cout <<"obj size"<< obj->objsize() << endl ;
// cout << "size " << size << endl ;
 
  *ppBuffer = (char*)malloc ( sizeof(char)*size) ;
  
  if ( *ppBuffer == NULL )
  {
	perror ("failed to malloc space") ;
	return -1 ;
  }
  
  *pBufferSize = size  ;

   pMessage = (hand_shake_msg_t*)*ppBuffer ;
  
   pMessage->header.type = HAND_SHAKE ;
   pMessage->header.len  = 0 ;
  
   

   if ( obj != NULL )
   {
	pMessage->header.len = obj->objsize() ;
   	
	memcpy (&pMessage->data[0] ,obj->objdata() , obj->objsize()) ;
   }

  return 0 ;
}

int HandShakeMsgOpt::parseMessage ( char *pBuffer , BSONObj &msgData )
{ 
    if ( pBuffer == NULL )
    {
        perror ("error , message is empty ") ;
	return -1 ;
    }
   
    pMessage = (hand_shake_msg_t*)pBuffer ;
     
    if (pMessage->header.type != HAND_SHAKE )
    {
	perror ("parser type not match") ;
	return -1 ;
    }    
   
    if ( pMessage->header.len == 0 )
    {
	printf ("message data empty \n") ;
        return 0 ;
    }
    // now extract data from message
   
    msgData = BSONObj ( &pMessage->data[0]) ;

    hash_info = msgData["hash info"].toString () ;
    peer_id   = msgData["peer_id"].toString () ;

    return 0 ;
}









