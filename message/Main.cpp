#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <bson.h>

#include "message.h"
#include "../bit_map/bit_map.h" // Bitmap will use


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
 
  builder.append("hash_info", msg_hash_info.c_str ()) ;

  builder.append("peer_id"   , msg_peer_id.c_str ()) ;
  
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

  cout << "peer_info" << hand_shake_msg_opt->peer_id << endl ;
  cout << "hash_info" << hand_shake_msg_opt->hash_info << endl ;

// test chock interested message operator

cout << "-------------test ChockInterestedMsgOpt------------------" << endl ;

  int   type  ; // used to receive user input 
  ChockInterestedMsgOpt *chock_interested_msg_opt  = new ChockInterestedMsgOpt () ;
  char *pp_chock_interested_msg = NULL ;
  int   chock_interested_msg_size = 0 ;
  BSONObj chock_interested_obj ;
  BSONObjBuilder chock_interested_builder ;

  cout << "input the chock interested type " << endl ;
  cout << "0--> chock 1-->unchock 2-->interested 3-->uninterested" << endl ;
  cin  >> type  ;
  
  chock_interested_builder.appendNumber ("type" , type ) ;
  
  chock_interested_obj = chock_interested_builder.obj () ;
  
  cout << " before build message obj " << chock_interested_obj.toString () << endl ;
  cout << chock_interested_obj["type"].number() << endl ;

  chock_interested_msg_opt->buildMessage (&pp_chock_interested_msg , &chock_interested_msg_size ,
				&chock_interested_obj) ;  
  cout << "total msg size " << chock_interested_msg_size << endl ;
  
  // test parser
  
  cout << "here we test parser " << endl ;
  BSONObj chock_interested_msg_data ;
  chock_interested_msg_opt->parseMessage ( pp_chock_interested_msg , chock_interested_msg_data ) ;
  
  cout << "parsed message data content"<< chock_interested_msg_data.toString () << endl ;
 
  cout << " type " << chock_interested_msg_opt->type << endl ;

// test HaveMsgOpt

cout << "---------------test HaveMsgOpt---------------------" << endl ;

int index_have ;
BSONObj have_msg_obj ;
BSONObjBuilder have_msg_builder ;
char *pp_have_msg_buffer = NULL ;
int  p_have_msg_buffer_size = 0 ;

HaveMsgOpt *have_msg_opt = new HaveMsgOpt () ;
 
cout << "input index of piece"<<endl;
cin >> index_have ;

have_msg_builder.appendNumber ("index" , index_have) ;
have_msg_obj = have_msg_builder.obj () ;

cout << "before build message obj content " << have_msg_obj.toString () << endl ;

have_msg_opt->buildMessage (&pp_have_msg_buffer , &p_have_msg_buffer_size , &have_msg_obj) ;
 
cout << "after building message total length "<< p_have_msg_buffer_size << endl ;


cout << "not test parse message method "<< endl ;

BSONObj have_msg_data ;
have_msg_opt->parseMessage (pp_have_msg_buffer , have_msg_data ) ;
 
cout << "after parsing message "<<endl ;
cout << "have message data content "<< have_msg_data.toString () << endl ;
cout << "have_msg_opt->index = " << have_msg_opt->index << endl ;


cout << "------------ test bitfield message --------------------"<< endl ;

Bitmap bitmap (1024) ;  // here we create a bit map with bit number = 1024
			// and all the bits in bit map initialized = 0

BitFieldMsgOpt *bitfield_msg_opt = new BitFieldMsgOpt ();
BSONObj bitfield_obj ;
BSONObjBuilder bitfield_builder ;
char *pp_bitfield_buffer ;
int   p_bitfield_buffer_size ;


bitmap.set_bit_map_value ( 27 , 1 ) ;
// we set the 27th bit's value in bitmap 1

bitmap.print () ;  
// here we print all the bits value in the bitmap 

//load info from bitmap into ---- > BSONObjBuilder 
bitfield_builder.appendNumber("bit_length",bitmap.bit_length ) ;
bitfield_builder.appendNumber("byte_length", bitmap.byte_length) ;
bitfield_builder.append ("bit_field", bitmap.bit_field) ;

bitfield_obj = bitfield_builder.obj () ;

// print out the bitfield_obj 's content

cout << "bitfield_obj content "<< endl ;
cout << bitfield_obj.toString () << endl ;

// build message

bitfield_msg_opt->buildMessage (&pp_bitfield_buffer , &p_bitfield_buffer_size ,
			&bitfield_obj ) ; 

cout << "total message length " << p_bitfield_buffer_size << endl ;

// parse message

BSONObj bitfield_msg_data ;

bitfield_msg_opt->parseMessage (pp_bitfield_buffer , bitfield_msg_data ) ;

cout << "after parsing message , extracted message data "<< endl ;
cout << bitfield_msg_data.toString() <<endl;

cout << "here is the Bitmap * contents in BitFieldMsgOpt  " << endl ;

cout <<"bit map bit length "<< bitfield_msg_opt->pBitmap->bit_length <<endl ;
cout << "bit map byte length " << bitfield_msg_opt->pBitmap->byte_length << endl ;
cout <<" bit map bits print " << endl ;
cout << endl ;
 bitfield_msg_opt->pBitmap->print () ;

  return 0 ;
}


