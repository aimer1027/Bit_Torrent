#ifndef PEER_H
#define PEER_H

#include <string>
#include <vector>
#include <time.h>
#include <stdint.h>
#include <cstring>


#include "../bit_map/bit_map.h"

enum state
{
  INITIAL = 0 ,
  HALF_SHAKED,
  HAND_SHAKED,
  SEND_BIT_FIELD,
  RECV_BIT_FIELD,
  EXCHAIGING_DATA,
  CLOSING
} ;

const long MSG_LEN = (1024*2 + 1024*16) ;

typedef struct _request_piece
{
  int index ;
  int begin ;
  int length ;
} request_piece_t ;

typedef struct _peer_node
{
  int socket ;
  char ip [16] ;
  unsigned short port ;
  char id[21] ;
  
  int state ;
  
  int am_chocking ;
  int am_interested ;
  int peer_chocking ;
  int peer_interested ;
	
  Bitmap *pBitmap ;
  
  char *buff_in ;
  int  buff_in_len ;
 
  char *msg_out ;
  int   msg_out_len ;
  
  char *msg_buff_out ;
  int   msg_buf_out_len ;
 
 // download reqeustes received from others
  std::vector<request_piece_t> recv_download_request_queue ;
//  uploaded requests send to others 
  std::vector<request_piece_t> send_upload_request_queue ;

  unsigned int down_total ;
  unsigned int up_total  ;

  time_t start_timestamp ;
  time_t recet_timestamp ;
 
  time_t last_down_timestamp ;
  time_t last_up_timestamp ;
 
  uint64_t down_count ;
  uint64_t up_count ;
 
  double down_rate ;
  double up_rate ;

  _peer_node ()
  {

	socket = -1 ;
	memset(ip , 0 , 16 ) ;
	port = 0 ;
	memset(id , 0 , 21 ) ;
	state = INITIAL ;
	
	buff_in = NULL ;
	buff_in_len = 0 ;	

	msg_out = NULL ;
	msg_out_len = 0 ;
	
	msg_buff_out = NULL ;
	msg_buf_out_len = 0 ;
	
	
	am_chocking = 1 ;
	am_interested = 0 ;
	peer_chocking = 1 ;
	peer_interested = 0 ;
	
	pBitmap = NULL ;
	
	down_total = 0 ;
	up_total = 0 ;
	
	last_down_timestamp = 0 ;
	last_up_timestamp = 0 ;
	
	down_count = 0;
	up_count = 0 ;
	
	down_rate = 0.0 ;
	up_rate = 0.0 ;	

  } // peer_node_t default value initialization


} peer_node_t ;

/// here we begin define the Peer class 

class Peer
{
public :
	peer_node_t peer_node ;
 
 public :
	Peer () ;
	~Peer () ;
      
       int cancel_recv_request_queue () ;
       int cancel_send_request_queue () ;
       void release_peer_node () ;
       void print () ;	
} ;


#endif // peer.h
