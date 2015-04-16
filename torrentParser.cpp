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

/**
  method : get_announce_list ( ListNode &announce_list , torrent_file_t &torrent_structure)

  descri : this method is used to extract ListNode.StringNode (in b-encoding)
	   and transfer it into the std::string 
	   then push it into the torrent_list.announce_list(vector<string>)
*/

void TorrentFile::get_announce_list ( ListNode *announce_list , torrent_file_t &torrent_structure) 
{
	string announce_string ;

	for ( vector<AnyNode*>::iterator it_string = announce_list->_value_list.begin () ;
				it_string != announce_list->_value_list.end () ; it_string++ )
	{
	 	get_node_value ( dynamic_cast<StringNode*>(*it_string) , announce_string ) ;
		torrent_structure.announce_list.push_back ( announce_string ) ;
	}
}


/*
this method is used as the main method to extract info dict message
call context :
	AnyNode *pAnyNode_info_dict = find_target_node ( pFileDict->_value_map , "info" ) ;
	if ( pAnyNode_info_dict != NULL )
	{
		DictNode *pInfoDict = dynamic_cast<DictNode*>(pAnyNode_info_dict) ;
		get_info_dict( pInfoDict, torrent_structure ) ;
	}

*/
void TorrentFile::get_info_dict ( DictNode *pInfoDict, torrent_file_t &torrent_structure ) 
{
	get_info_common ( pInfoDict , torrent_structure  ) ;
	
	if ( torrent_structure.info.is_multi_file )
	{
		// multi-file mode
		get_info_multi_mode ( pInfoDict, torrent_structure ) ;
	}
	else
	{
		// single file mode
		get_info_single_mode ( pInfoDict, torrent_structure) ;
	}

}
		
// this method will be called by get_info_dict
void TorrentFile::get_info_common( DictNode *pInfoDict , torrent_file_t &torrent_structure ) 
{	
	// find info.pieces from info-dict , type : string type (b-encoding)
	get_node_value ( dynamic_cast<StringNode*>
		(find_target_node( pInfoDict->_value_map , "pieces" )) , torrent_structure.info.pieces ) ;
	
	// find info.piece_length from info-dict , type : integer type (b-encoding)
	get_node_value ( dynamic_cast<IntegerNode*>
			(find_target_node( pInfoDict->_value_map, "piece length")),torrent_structure.info.piece_length) ;
	
	AnyNode *pAnyNode_files_list = find_target_node ( pInfoDict->_value_map , "files" ) ;

	if ( pAnyNode_files_list != NULL )
	{
		torrent_structure.info.is_multi_file = true ;
	}
	
	else
	{
		// single-file
		torrent_structure.info.is_multi_file = false ;
	}	
}
		
// this method will be called by get_info_dict when single-file mode
void TorrentFile::get_info_single_mode ( DictNode*pInfoDict ,torrent_file_t &torrent_structure) 
{
  // this is single-mode file
  file_t single_file ;  

  // get file name : string type 
  get_node_value ( dynamic_cast<StringNode*>
		(find_target_node ( pInfoDict->_value_map , "name" )) , single_file.file_path) ;
	
  get_node_value ( dynamic_cast<IntegerNode*>
		(find_target_node ( pInfoDict->_value_map, "length")) , single_file.file_length ) ;
	
  torrent_structure.info.file_list.push_back ( single_file ) ;	

}
		

// this method will be called by get_info_dict when multi-file mode
void TorrentFile::get_info_multi_mode ( DictNode*pInfoDict,torrent_file_t &torrent_structure ) 
{
	string main_dir ;

	// multi-file , info.name is the main path name 	
	get_node_value ( dynamic_cast<StringNode*>
			(find_target_node(pInfoDict->_value_map , "name")) , main_dir) ;

	// now the main_dir is the name of the main path of all the files 

	ListNode *pInfoFiles = dynamic_cast<ListNode*>( find_target_node ( pInfoDict->_value_map , "files" )) ;

	// traverse the files list , each element in list are in type of dict
	for ( vector<AnyNode*>::iterator it_sub_file = pInfoFiles->_value_list.begin () ;
			it_sub_file != pInfoFiles->_value_list.end () ; it_sub_file++ )
	{
		// each element in the list are dict
		DictNode *pInfoFilesSubFile = dynamic_cast<DictNode*>(*it_sub_file) ;
		
		if ( pInfoFilesSubFile != NULL )
		{
			file_t sub_file ;
			
			// get length of each sub file from the dict
			get_node_value ( dynamic_cast<IntegerNode*>
					( find_target_node ( pInfoFilesSubFile->_value_map, "length" ) ) , 		                                                                           sub_file.file_length ) ;

			AnyNode *pAnyNode_sub_path = find_target_node ( pInfoFilesSubFile->_value_map ,"path" ) ;
			if ( pAnyNode_sub_path != NULL )
			{
			  ListNode *pSubPathList = dynamic_cast<ListNode*>(pAnyNode_sub_path) ;	
				
		 	  // traverse the list and extract each sub path and append them together
		          for ( vector<AnyNode*>::iterator it_sub_path = pSubPathList->_value_list.begin() ;
			       it_sub_path != pSubPathList->_value_list.end () ; it_sub_path++ )			
			  {
				string sub_path ;

				sub_file.file_path += "//" ;
				
				get_node_value ( dynamic_cast<StringNode*>(*it_sub_path) , sub_path ) ; 
				
				sub_file.file_path += sub_path ;
			  }

			}
			else
			{
				LOG(INFO)<<"[info] sub path list is empty";
			}

			torrent_structure.info.file_list.push_back (sub_file) ;

		}
		else
		{
			LOG(INFO)<<"[info] file dict is empty " ;
		}
	}// for 	
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
	 
	// call get_announce_list method
	get_announce_list ( pAnnounceList , torrent_structure); 
	
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
		
		get_info_dict ( pInfoDict, torrent_structure ) ;
	}
	else
	{
		LOG(WARNING)<<"[warnning] .torrent file can not find info ";
		return true ;
	}

} ///info parse end 

	
  delete pFileDict ;
  return true ;
}

void TorrentFile::show_torrent_content ( torrent_file_t &torrent_structure) 
{
	
	cout << "torrent file contents after analyze"<<endl ;
	cout << "announce: "<< torrent_structure.announce << endl ;
	cout << "created by: "<< torrent_structure.created_by<< endl ;
	cout << "creation date(s):"<< torrent_structure.creation_date << endl ;
	cout << "comment :"<< torrent_structure.comment << endl ;
	cout << "encoding : "<< torrent_structure.encoding << endl ;
	cout << "announce-list:"<<endl ;

	for ( vector<string>::iterator it = torrent_structure.announce_list.begin () ; 
					it != torrent_structure.announce_list.end () ; it++ )
	{
		cout << *it << endl ;
	}  

	cout << "info"<< endl;
	cout << "info.pieces" << torrent_structure.info.pieces << endl ;
	cout << "info.piece_length" << torrent_structure.info.piece_length << endl ;
	cout << "info.is_multi_file ? " << endl ;
	
	if ( torrent_structure.info.is_multi_file )
	{
		cout << "yes , it is multi file mode" << endl ;
		cout << "here are the sub file's path and length" <<endl ;
			
		for ( vector<file_t>::iterator it = torrent_structure.info.file_list.begin () ;
						it != torrent_structure.info.file_list.end () ; it++ )
		{
			cout << "sub file length "<< it->file_length << endl ;
			cout << "sub file path "  << it->file_path   << endl ;
		}
	}
	else
	{
		cout << "no , it is single file mode " << endl ;
		
		cout << "here is the single file's path and length " <<endl ;
		cout << "single file length "<< torrent_structure.info.file_list[0].file_length << endl ;
		cout << "single file path(name) " << torrent_structure.info.file_list[0].file_path << endl ;
	}


}


