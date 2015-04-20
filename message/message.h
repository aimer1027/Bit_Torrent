#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstring>
#include "../peer/peer.h"

class AnyMsg
{
  public :
     static int int_to_char( int i , unsigned char c[4]);
     static int char_to_int ( unsigned char c[4] ) ;

     virtual int create_msg ( Peer &peer ) ;
     virtual int parse_msg  ( Peer &peer , unsigned char *msg , int msg_len );
} ;

class Hand_Shake_Msg : public AnyMsg 
{
  private :
	char * _info_hash ;
	char * _peer_id ;
  public :
	Hand_Shake_Msg ( char *info_hash , char *peer_id )
	{
		_info_hash = info_hash ;
		_peer_id   = peer_id  ;
	}
	
	virtual int create_msg ( Peer &peer ) ;
	virtual int parse_msg  ( Peer &peer , unsigned char *msg , int msg_len ) ;
    
} ;

class Keep_Alive_Msg : public AnyMsg
{
  public :
	virtual int create_msg ( Peer &peer ) ;
	virtual int parse_msg  ( Peer &peer , unsigned char *msg , int msg_len ) ;
} ;

class Chock_Interested_Msg : public AnyMsg 
{
  private :
	int _type ;
  
  public :
	Chock_Interested_Msg ( int type )
	{
		_type = type ;
        }
	virtual int create_msg ( Peer &peer ) ;
	virtual int parse_msg  ( Peer &peer , unsigned char*msg , int msg_len ) ;
} ;


class Have_Msg : public AnyMsg 
{
  private :
	int _index ;
	
  public :
	Have_Msg ( int index ) 
	{
		_index = index ;
	}
	virtual int create_msg ( Peer &peer ) ;
	virtual int parse_msg  ( Peer &peer , unsigned char *msg , int msg_len) ;
} ;

class Bit_Field_Msg : public AnyMsg 
{
 private:
	char *_bitfield ;
	int   _bitfield_len ;
 public :
	Bit_Field_Msg ( char *bitfield , int bitfield_len )
	{
		_bitfield = bitfield ;
		_bitfield_len = bitfield_len ;
        }
	
	virtual int create_msg ( Peer &peer ) ;
	virtual int parse_msg  ( Peer &peer , unsigned char *msg , int msg_len ) ;
} ;

class Request_Msg : public AnyMsg 
{
 private :
	int _index ;
	int _begin ;
	char *_block ;
	int _block_len ;
 public :
	Request_Msg ( int index , int begin , char *block , int block_len )
	{
		_index = index ;
		_begin = begin ;
		_block = block ;
		_block_len = block_len ;
	}
	
	virtual int create_msg ( Peer &peer ) ;
	virtual int parse_msg  ( Peer &peer , unsigned char *msg , int msg_len ) ;
} ;

class Piece_Msg : public  AnyMsg 
{
  private :
	int _index ;
	int _begin ;
	char *_block ;
	int _block_len ;
	
  public :
	Piece_Msg ( int index , int begin , char *block , int block_len )
	{
		_index = index ;
		_begin = begin ;
		_block = block ;
		_block_len = block_len ;
	}
	
	virtual int create_msg ( Peer &peer ) ;
	virtual int parse_msg  ( Peer &peer , unsigned char *msg , int msg_len ) ; 
} ;

class Cancel_Msg : public AnyMsg 
{
   private:
	int _index ;
	int _begin ;
	int _length ;
   public :
	Cancel_Msg ( int index , int begin , int length )
	{
		_index = index ;
		_begin = begin ;
		_length = length ;
	}

	virtual int create_msg ( Peer &peer ) ;
	virtual int parse_msg  ( Peer &peer , unsigned char *msg , int msg_len );
} ;

class Port_Msg : public AnyMsg
{
 private :
	int _port ;
 public :
	Port_Msg ( int port )
        {
	    _port = port ;
	} 
	
	virtual int create_msg ( Peer &peer ) ;
	virtual int parse_msg  ( Peer &peer , unsigned char *msg , int msg_len ) ;
} ;

#endif // message.h
