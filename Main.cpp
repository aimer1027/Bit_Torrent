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
	pAnyNode = new IntegerNode () ;
	cout << "type of integer:" <<endl ;
        pAnyNode->parser ( content  ) ;
  	pAnyNode->print () ;
	
	break ;

   case 's' :
	pAnyNode = new StringNode () ;
	cout << "type of string :" <<endl ;
	pAnyNode->parser( content ) ;
	pAnyNode->print () ;

	break ;

   case 'l' :
	pAnyNode = new ListNode () ;
	cout <<"type of list :" <<endl ;
	pAnyNode->parser ( content ) ;
	pAnyNode->print () ;	

	break ;

  case 'd' :
	pAnyNode = new DictNode () ;
	cout <<"type of dictionary" <<endl ;
	pAnyNode->parser ( content ) ;
	pAnyNode->print () ;

	break ;
 
  default :
	cout <<" error not such kind type " << endl ;
	break ;
  }

 return 0 ;	
}

