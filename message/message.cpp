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

    hash_info = msgData["hash_info"].toString () ;
    peer_id   = msgData["peer_id"].toString () ;

    return 0 ;
}


int KeepAliveMsgOpt::buildMessage( char **ppBuffer , int *pBufferSize , 
					bson::BSONObj *obj )
{
  int size = 0 ;
  keep_alive_msg_t *pMessage = NULL ;  

  size += sizeof(keep_alive_msg_t) ; 
  
  // keep alive message's data content is null 
  
  *ppBuffer = (char*)malloc(sizeof(char)*size) ;
  
   if ( *ppBuffer == NULL )
   {
	perror ("failed to allocate space for ppBuffer") ;
	return -1 ;
   } 
  
  pMessage = (keep_alive_msg_t*)*ppBuffer ;
  pMessage->header.len  = 0 ; // data content size is 0
  pMessage->header.type = KEEP_ALIVE ;
   
  return 0 ;
}

int KeepAliveMsgOpt::parseMessage ( char *pBuffer , bson::BSONObj &msgData )
{
   // is null or is type correct
   keep_alive_msg_t *pMessage = NULL ;  

   if ( pBuffer == NULL )
   {
	perror ("failed, message is empty") ;
	return -1 ;
   }
   
   pMessage = (keep_alive_msg_t*)pBuffer ;
   
   if ( pMessage->header.type != KEEP_ALIVE )
   {
	perror ("failed , message type not match with parser type") ;
	return -1 ;
   }

   return 0 ;
}

int ChockInterestedMsgOpt::buildMessage ( char **ppBuffer , int *pBufferSize ,
				BSONObj *obj )
{
   int size = 0 ;
   chock_interested_msg_t *pMessage = NULL ;
   
   size += sizeof(chock_interested_msg_t) ;
   
   if ( obj != NULL )
	size += obj->objsize () ;

   *ppBuffer = (char*)malloc(size*sizeof(char)) ;
   
   if ( *ppBuffer == NULL )
   {
	perror ("failed to allocate space to ppBuffer") ;
	return -1 ;
   }   
 
  *pBufferSize = size ;
  pMessage = (chock_interested_msg_t*)*ppBuffer ;
  
  pMessage->header.type = CHOCK_INTERESTED ;
  pMessage->header.len  = 0 ;
  
  if ( obj != NULL )
  {
	pMessage->header.len = obj->objsize () ;
	memcpy (&pMessage->data[0] , obj->objdata()  , obj->objsize() ) ;
  }
 
  return 0 ;
}

int ChockInterestedMsgOpt::parseMessage ( char *pBuffer , BSONObj &msgData )
{
  chock_interested_msg_t *pMessage = NULL ;

  if ( pBuffer == NULL )
  {
	perror ("message is empty") ;
	return -1 ;
  }
  
  pMessage = (chock_interested_msg_t*)pBuffer ;
 
  if ( pMessage->header.type != CHOCK_INTERESTED )
  {
	perror ("message type not match with parse type, error") ;
	return -1 ;
  } 
 
  msgData = BSONObj ( &pMessage->data[0]) ;
  type = msgData["type"].number() ;
  
  return 0 ;
} 

int HaveMsgOpt::buildMessage ( char **ppBuffer , int *pBufferSize ,
				       BSONObj *obj )
{
  int size = 0 ;
   chock_interested_msg_t *pMessage = NULL ;

   size += sizeof(chock_interested_msg_t) ;

   if ( obj != NULL )
        size += obj->objsize () ;

   *ppBuffer = (char*)malloc(size*sizeof(char)) ;

   if ( *ppBuffer == NULL )
   {
        perror ("failed to allocate space to ppBuffer") ;
        return -1 ;
   }

  *pBufferSize = size ;
  pMessage = (chock_interested_msg_t*)*ppBuffer ;

  pMessage->header.type = HAVE ;
  pMessage->header.len  = 0 ;

  if ( obj != NULL )
  {
        pMessage->header.len = obj->objsize () ;
        memcpy (&pMessage->data[0] , obj->objdata()  , obj->objsize() ) ;
  }
  
  return 0 ;
}

int HaveMsgOpt::parseMessage ( char *pBuffer , BSONObj &msgData )
{
   have_msg_t *pMessage = NULL ;

   if ( pBuffer == NULL )
   {
	perror ("message empty , failed to parse") ;
	return -1 ;
   }
  
  pMessage = (have_msg_t*)pBuffer ;
  
  if (pMessage->header.type != HAVE )
  {
	perror ("message type not match with parser type , failed") ;
	return -1 ;
  } 
  
  msgData = BSONObj(&pMessage->data[0]) ;
  index = msgData["index"].number () ;
  
  return 0;
}
