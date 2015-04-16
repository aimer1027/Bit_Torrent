#ifndef TORRENT_PARSER_H
#define TORRENT_PARSER_H

#include <string>
#include <stdint.h> // int64_t
#include <map>
#include <vector>
#include <iostream>

using namespace std ;

typedef struct _file
{
	int64_t  file_length ;
	string   file_path ;
} file_t;


typedef struct _info
{
	int64_t piece_length ; // length of each file block (B)
	string   pieces   ;     // hash value length of 20*n
	
	bool     is_multi_file ; 
	vector<file_t>  file_list ;  
} info_t ;

typedef struct _torrent_file
{
	string   announce ;
	vector<string> announce_list ;
	string   comment ;
	string   created_by ;
	int64_t creation_date ;
	string   encoding ;
	info_t   info ;
} torrent_file_t ;


class AnyNode
{
	public :
		virtual ~AnyNode () {}
		virtual void print () = 0 ;
		virtual bool parser (string &content) = 0 ;
} ;

class IntegerNode : public AnyNode
{
  	public :
		virtual ~IntegerNode () {}
		virtual bool parser( std::string &content ) ;
		
		virtual void print () 
		{
		  cout<<"integer type , value ["<< _value <<"]" <<endl;
		}
		
		int64_t  _value ;
} ;

class StringNode : public AnyNode
{
public :
	virtual ~StringNode () {}
	virtual bool parser ( string &content ) ;

	virtual void print ()
	{
		cout<<"string type ,value ["<< _value <<"]"<<endl;
	}

	string _value ;
} ;

class ListNode : public AnyNode
{
public :
	virtual ~ListNode ()
	{
		for ( vector<AnyNode*>::iterator it = _value_list.begin () ;
				it != _value_list.end  () ; it++ )
		{
			delete *it ; // equal delete AnyNode *
		} 
		
		_value_list.clear () ;
	}
	
	virtual bool parser ( string &content ) ;
	virtual void print ()
	{
		cout << "type list "<< endl ;
		for ( vector<AnyNode*>::iterator it = _value_list.begin () ; 
			it != _value_list.end () ; it++ )
		{
			AnyNode* it1 = *it ;

			cout << "value " ;
			it1->print () ;
			cout << endl ;
		}
	}
	
	vector<AnyNode*> _value_list ;
} ;

class DictNode : public AnyNode
{
public :
	~DictNode ()
	{
		for ( map<StringNode *, AnyNode*>::iterator it = _value_map.begin () ;
		      it != _value_map.end () ; it++ )
		{
			delete it->first ;
			delete it->second ;
		} 

		_value_map.clear () ;
	}

	virtual bool parser ( string &content ) ;

	virtual void  print ()
	{
		StringNode  *it1 ;
		AnyNode     *it2 ;

		cout << "type dictionary "<< endl ;

		for ( map<StringNode *, AnyNode *>::iterator it = _value_map.begin () ;
			it != _value_map.end () ; it++ )
		{
			it1 = it->first ;
			it2 = it->second ;
		
			cout << "key " << endl ;
			it1->print();
			cout << "value " << endl ;
			it2->print () ;
		}
	}

	map<StringNode*, AnyNode*> _value_map ;
} ;

class TorrentFile
{
	public :	
		torrent_file_t torrent_file ;

		static bool encode ( const string & torrent_file_content ,
				torrent_file_t &torrent_structure ) ;

// update date : 2015/4/15 by Aimer

		static void  get_node_value ( IntegerNode *pIntegerNode , int64_t &integer_value ) ;
	
		static void get_node_value ( StringNode *pStringNode , string &string_value ) ;

		static AnyNode *find_target_node ( map<StringNode *, AnyNode*> &hash_map , const string &key ) ; 	

		static void get_announce_list ( ListNode*pAnnounceList , torrent_file_t &torrent_structure  ) ;

		static void get_info_dict ( DictNode*pInfoDict , torrent_file_t &torrent_structure ) ;
		
		// this method will be called by get_info_dict
		static void get_info_common( DictNode*pInfoDict , torrent_file_t &torrent_structure ) ;
		
		// this method will be called by get_info_dict when single-file mode
		static void get_info_single_mode ( DictNode* pInfoDict ,torrent_file_t &torrent_structure) ;
		
		// this method will be called by get_info_dict when multi-file mode
		static void get_info_multi_mode ( DictNode* pInfoDict , torrent_file_t &torrent_structure ) ;


		// this method will print out the messages stored inside in the torrent_file_t 
		static void show_torrent_content ( torrent_file_t &torrent_structure) ;

} ;

#endif
