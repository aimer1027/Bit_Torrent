#include <cstdio>
#include <cstring>
#include <iostream>

#include <vector>
#include <map>

#include <glog/logging.h>

#include "parserUtil.h"
#include "torrentParser.h"

using namespace std ;

bool IntegerNode::parser ( string &content )
{
	if ( content.empty () )
	{
		LOG(WARNING)<< "[warnning] in integer's parser content is  empty";
		return false ;
	}
	
	if ( content[0] != 'i' )
	{
		LOG(WARNING)<< "[warnning] integer's parser content first element illegal";
		return false ; 
	}
	
	uint64_t pos = content.find ( 'e' , 0 ) ;
	// find the position of char e from beginning of content
	
	if ( pos == string::npos )
	{
		LOG(WARNING) << "[warnning] integer's parser content not find char e";
		return false ;
	} 
	
	string s_value = content.substr(1 , pos-1) ;
	// extract the sub-string integer from content with out begin 'i' and  end 'e'
	
	parserUtils::string_to_integer (s_value , _value ) ;
	// transfer string into integer(int64_t) 
	
	content = content.erase ( 0 , pos+1 ) ;	
	
	LOG(INFO) << "[info] integer's parser  extract message "<< _value << " remain message " << content  ;  
	return true ;
} 

bool StringNode::parser ( string & content )
{
	if ( content.empty () )
	{
	  LOG(WARNING) <<"[warnning] string type's parser content is empty ";
	  return false ;
	}
	
	if ( content.size () < 3 )
	{
	  LOG(WARNING)<<"[warnning] string type's parser content length is illegal" ;
	  return false ;
	}
	
	int64_t pos = content.find (':', 0) ;
	// find ':' in string_length:string_content structure
	
	if ( pos == string::npos )
	{
	  LOG(WARNING)<<"[warnning] string type's  it is illegal string type with out ':' ";
	  return false ;
	}
	
	int64_t  count = 0 ;
    
	parserUtils::string_to_integer (content.substr(0 , pos) , count ) ;
	// extract string 's length from content 

	_value = content.substr (pos+1, count ) ;
	cout << " in method string node parser " << _value << endl ;
	cout << " length " << count << endl ;
	// pos+1 : beginning of the string content , count : length of the string content 
	
	content = content.erase (0 , pos+count+1) ;
	// pos : number of the length , count : numbers of the string's char 
	// eraser method erase range [begin , end) 1 should be plus to (pos+count)

	LOG(INFO)<<"[info] string type parser extract message "<< _value 
			<< " remain content " << content ;
	return true ;
} 


bool ListNode::parser ( string &content )
{
	if ( content.empty () )
	{
		LOG(WARNING)<<"[warnning] list type parser content is empty ";
		return false ;
	}
	
	if ( content[0] != 'l' )
	{
		LOG(WARNING)<<"[warnning] list type parser content with out list mark" ;
		return false ;
	}
	
	content = content.erase ( 0 , 1 ) ;
	// delete 'l' from content 
	
	while ( !content.empty () )
	{
		AnyNode *anyNode = NULL ;
	
		if ( content [0] == 'l' )
		{
		   anyNode = new ListNode () ;
		}
	
		else if ( content [0] == 'd')
		{
		   anyNode = new DictNode () ;
		}
	
		else if ( content [0] == 'i' )
		{
		   anyNode = new IntegerNode () ;
		}
	
		else if (content[0] >= '1' && content[0] <= '9')
		{
	      	   anyNode = new StringNode () ;
		}
		
		else
		{
			LOG(WARNING)<<"[warnning] list parser list contain illegal message" ;
			return false; 
		}
	
		anyNode->parser ( content ) ;
		// this method will parse the content into the right value type
		// and store it into the memeber value variable (_value , _value_list ,value_map...)
		
		_value_list.push_back ( anyNode ) ;
	
		if ( content[0] == 'e' ) // is it the end of the list type ?
		{
			// here we arrived the end of the list , 
			// we should erase the 'e' in  content and break the cycle
			
			content = content.erase ( 0 , 1 ) ; 
			break ;
		}

	}// while 
	
		LOG(INFO)<<"[info] list type parser remainning content " << content  ;
		return true ; 
}

bool DictNode::parser ( string & content )
{
	if ( content[0] != 'd' )
	{
	   LOG(WARNING)<<"dictionary type parser content not begin with 'd' " ;
	   return false ;
	}
	
	if ( content.size () < 3 )
	{
	   LOG(WARNING)<<"dictionary type parser content length illegal " ;
	   return false ;
	}
	
	content = content.erase ( 0 , 1 ) ; 
	// erase 'd' from content
	
	// dict type : <key: string type><value: any type>

	while ( !content.empty () )
	{
		StringNode *key = new StringNode () ;
		key->parser ( content  ) ;
		// extract key message from content 
		
		if ( content.empty () )
		{
			LOG(INFO) << "[info] content is empty ,break ";
			break ;
		}
		
		AnyNode *value = NULL ;
	
		if ( content [0] == 'i' )
			value = new IntegerNode () ;
		else if ( content [0] == 'l' )
			value = new ListNode () ;
		else if ( content [0] == 'd' )
			value = new DictNode () ;
		else if ( content[0] >= '1' && content[0] <= '9' )
			value = new StringNode () ;
	
		else 
		{
		     LOG(WARNING)<<"[warnning] dict type parser dict contains illegal type" ;
		     return false ;
		}
		
		value->parser ( content ) ;
		_value_map[key] = value ;
		
		if ( content[0] == 'e' )
		{
			// arrived the end of the dict ; erase the 'e' in content 
			content = content.erase( 0 , 1 ); 
			break ;
		}
	}
	
	return true ;
}

// following method is used the parser torrent file which is the combination 
// of all the B encoding parsers (type: integer , string , list, dict ...)

bool TorrentFile::encode ( const string & torrent_file_content , torrent_file_t &torrent_structure )
{
  return true ;
}




