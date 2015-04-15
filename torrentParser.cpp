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

// update date : 2015/4/15 by Aimer
void TorrentFile::get_node_value (StringNode *pStringNode, string &string_value) 
{
	if ( pStringNode == NULL )
	{
	  LOG(WARNING)<<"[warnning] can not get string type value , empty" ;
	  string_value = "" ;
	}
	else
	{
	  string_value = pStringNode->_value ;
	}
}

void TorrentFile::get_node_value ( IntegerNode *pIntegerNode , int64_t &integer_value )
{
	if ( pIntegerNode == NULL )
	{
	   LOG(WARNING)<<"[warnning] can not get integer type value , empty";
	   integer_value = -1 ;
	}
	else
	{
	   integer_value = pIntegerNode->_value ;
	}
}

AnyNode * TorrentFile::find_target_node ( map<StringNode*, AnyNode*>&hash_map , 
		const string &key )
{
    for ( map<StringNode*,AnyNode*>::iterator it_map = hash_map.begin() ;
		it_map != hash_map.end() ; it_map++ )
    {
	StringNode *pStringNode = dynamic_cast<StringNode*>(it_map->first) ;
	if ( pStringNode == NULL )
	{
	  LOG(WARNING)<<"[warnning] hash map key is NULL ";
	  return NULL ;
	}
	
	if ( pStringNode->_value  == key )
	   return it_map->second ; 
    }

   return NULL ;  // not find
}












// following method is used the parser torrent file which is the combination 
// of all the B encoding parsers (type: integer , string , list, dict ...)

bool TorrentFile::encode ( const string & torrent_file_content , torrent_file_t &torrent_structure )
{
  string file_string = torrent_file_content ;
  DictNode *pFileDict = new DictNode () ;

  pFileDict->parser ( file_string  ) ;
  
  // key word : "announce" ;  type: string 
  get_node_value ( dynamic_cast<StringNode*>
	(find_target_node( pFileDict->_value_map , "announce" )) , torrent_structure.announce ) ; 
 
 // key word : "created by" ; type : string
  get_node_value ( dynamic_cast<StringNode*>
	(find_target_node(pFileDict->_value_map, "created by")) , torrent_structure.created_by) ;

// key word : "creatation date" ; type :integer
  get_node_value ( dynamic_cast<IntegerNode*>
	( find_target_node( pFileDict->_value_map , "creation date")) , torrent_structure.creation_date ) ;
 
// key word : "comment" ; type : string
  get_node_value ( dynamic_cast<StringNode*>
	(find_target_node( pFileDict->_value_map , "comment" ) ), torrent_structure.comment ) ;

// key word : "encoding" ; type : string
  get_node_value ( dynamic_cast<StringNode*>
	(find_target_node (pFileDict->_value_map , "encoding" )), torrent_structure.encoding ) ;


// key word : "announce-list" ;  type : list< contain element type : string >
  AnyNode *pAnyNode_list = find_target_node ( pFileDict->_value_map , "announce-list" ) ;
 
 if ( pAnyNode_list != NULL) // "announce-list" exists in .torrent file
 {
	ListNode *pAnnounceList = dynamic_cast<ListNode*>(pAnyNode_list) ;
	
	// traverse the list , extract string into --> torrent_strucutre.announce_list :(vector<string> type)
	for ( vector<AnyNode*>::iterator it = pAnnounceList->_value_list.begin () ;
				it != pAnnounceList->_value_list.end () ; it++ )
	{
		string string_announce ;
	
		get_node_value ( dynamic_cast<StringNode*>(*it) , string_announce ) ;
		
		torrent_structure.announce_list.push_back (string_announce) ;
	}
 } 
 
 else
 {
	LOG(INFO)<<"[info] no key words : <announce-list> exists in torrent file"  ;
 }
 
 // key word : "info" ; type : dictionary 
//  if single file mode , dictionary elements type : string
//  if multi-file mode ,  dictionary elements type : string , list < element type : dictinary >
//							dictionary element type : string<length>
//								                  list <element type : string> 
//
 
{

	// first get info|type : dict ;  from pFileDict
	AnyNode *pAnyNode_dict = find_target_node ( pFileDict->_value_map , "info") ;
	
	if ( pAnyNode_dict != NULL ) // get "info" key word
	{
		DictNode *pInfoDict = dynamic_cast<DictNode*>(pAnyNode_dict) ;
		
		// get key word "pieces" from info -------->  info.pieces type : string
	    	get_node_value ( dynamic_cast<StringNode*>
			(find_target_node( pInfoDict->_value_map , "pieces" )) , torrent_structure.info.pieces ) ;
		
		// get key word "piece_length" from info --->  info.piece_length type : integer
		get_node_value ( dynamic_cast<IntegerNode*>
			(find_target_node(pInfoDict->_value_map , "piece length")), torrent_structure.info.piece_length ) ;
		
		// get info.files from info ------> info.files type: list
		AnyNode *pAnyNode_info_list = find_target_node ( pInfoDict->_value_map , "files" ) ;
		
		if ( pAnyNode_info_list != NULL  )
		{
			LOG(INFO)<<"[info] multi-file mode" ;
			torrent_structure.info.is_multi_file = true ;

			
			// files : list type exists , transfer it into ListType *
			ListNode *pInfoFileList = dynamic_cast<ListNode*>( pAnyNode_info_list ) ;
			
			// get main dir from info , type : string , k-words "name"
			string main_dir ;
			
			get_node_value ( dynamic_cast<StringNode*>
				(find_target_node(pInfoDict->_value_map, "name")) , main_dir ) ;
			
			// traverse the list : info.files list with element in dict type
			for ( vector<AnyNode*>::iterator it_dict = pInfoFileList->_value_list.begin() ;
						it_dict != pInfoFileList->_value_list.end() ; it_dict++ )
			{
				DictNode *pDictNode = dynamic_cast<DictNode*>(*it_dict) ;
				
				if ( pDictNode != NULL)
				{
					// here on dict refers a sub file node 
					// a sub file node contain 
					// { integer type : file_length , list<string>: file sub-path  }
			
					file_t sub_file ;
					sub_file.file_path += main_dir ; // first append the main dictionary into sub file
					
					// extract sub file length from pDictNode
					get_node_value ( dynamic_cast<IntegerNode*>
						(find_target_node( pDictNode->_value_map , "length"  ) ) , 
										  sub_file.file_length ) ;
					
					// extract sub file path from pDictNode
					// first find the path-list from pDictNode
					AnyNode* pAnyNode_path_list = find_target_node ( pDictNode->_value_map , "path" ) ;
					if ( pAnyNode_path_list != NULL  )
					{
						// second if it is not null , transfer it into List type
						ListNode *pSubPathList = dynamic_cast<ListNode*>(pAnyNode_path_list) ;
							
						// third traverse the list's _value_list , extract each string 
						// append them together into the absolute path of subfile
						
						// take care : the *it_sub_path is in type of StringNode *

					for ( vector<AnyNode*>::iterator it_sub_path = pSubPathList->_value_list.begin() ;
							it_sub_path != pSubPathList->_value_list.end () ; it_sub_path++ )
					{
						sub_file.file_path += "//" ; // first you should add seperator
						
						get_node_value ( dynamic_cast<StringNode*>(*it_sub_path  ), 
												sub_file.file_path ) ;
					} 
					

					}
					else
					{
						LOG(WARNING)<<"[warning] oh ,no , sub file path is empty ";
						return false ;
					}

					torrent_structure.info.file_list.push_back (sub_file) ;
				}
				else
				{
					LOG(WARNING)<<"[warnning] failed transfer AnyNode* into DictNode* ";
					return false ;
				}
			}
			
			
			
		}
		else
		{
			LOG(INFO)<<"[info] single file mode";
			
			torrent_structure.info.is_multi_file = false ;
			
			file_t single_file ;

			// get info.files.file_length from info , type : integer , k-words "length"
			get_node_value ( dynamic_cast<IntegerNode*>
				(find_target_node(pInfoDict->_value_map , "length")) , single_file.file_length) ; 
			
			// get info.files.file_name from into , type : string , k-words "name"
			get_node_value ( dynamic_cast<StringNode*>
				(find_target_node(pInfoDict->_value_map , "name")) , single_file.file_path ) ;
			
			torrent_structure.info.file_list.push_back (single_file) ;
			
		}
		

	}
	
	else
	{
		LOG(WARNING)<<"[warnning] can not find key word :<info> in torrent file " ;
		return false ;
	}
 }




  delete pFileDict ;
  return true ;
}




