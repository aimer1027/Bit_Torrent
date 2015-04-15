#include <cstdio>
#include <string>
#include <iostream>

#include <glog/logging.h>


#include "torrentParser.h"
#include "parserUtil.h"

using namespace std ;


int main ( int argc , char * argv[] )
{
  string content ; 
  char   type ;

  AnyNode *pAnyNode = NULL ;

  // first let init the google log 
 google::InitGoogleLogging(argv[0]) ;
  
// and then set the log file store path
   FLAGS_log_dir = "./log/" ;
  
	
  cout << "input parser's type [i->integer] [s->string] [l->list] [d->dictionary]" << endl ;
  cin >> type ;

  cout << "input content " << endl ;
  cin >> content ;
 
  switch ( type )
  {
    case 'i':
    {
	pAnyNode = new IntegerNode () ;
	cout << "type of integer:" <<endl ;
        pAnyNode->parser ( content  ) ;
  	pAnyNode->print () ;
	
	// update : 2015/4/15 by Aimer
	// test get_node_value
	int64_t integer_value ;
	
	TorrentFile::get_node_value ( dynamic_cast<IntegerNode*>( pAnyNode) , integer_value ) ;
	cout << "integer value "<< integer_value << endl ;
		
	break ;
    }
   
   case 's' :
   {
	pAnyNode = new StringNode () ;
	cout << "type of string :" <<endl ;
	pAnyNode->parser( content ) ;
	pAnyNode->print () ;

	// update : 2015/4/15 by Aimer
	// test TorrentFile::get_node_value , string type
	string string_value ;
	
	TorrentFile::get_node_value ( dynamic_cast<StringNode*>( pAnyNode) , string_value ) ;
	cout<< "string value "<< string_value << endl ;

	break ;
   }
   case 'l' :
   {
	pAnyNode = new ListNode () ;
	cout <<"type of list :" <<endl ;
	pAnyNode->parser ( content ) ;
	pAnyNode->print () ;	

	break ;
   }
  case 'd' :
  {
	pAnyNode = new DictNode () ;
	cout <<"type of dictionary" <<endl ;
	pAnyNode->parser ( content ) ;
	pAnyNode->print () ;

	// update : 2015/4/15 , test find_target_node method
	string key ;

	cout << "input searching key words"<< endl ;
	cin>> key ;
	
	AnyNode *pAnyNode_value  = TorrentFile::find_target_node ( (dynamic_cast<DictNode*>(pAnyNode))->_value_map, key ) ;
	cout << "key->" << key << "value->";
	pAnyNode_value->print () ;

	break ;
  } 
  default :
	cout <<" error not such kind type " << endl ;
	break ;
  }

 return 0 ;	
}

