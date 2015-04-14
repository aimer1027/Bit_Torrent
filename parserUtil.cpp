#include <stdint.h>
#include <string>

#include "parserUtil.h"

using namespace std ;

void parserUtils::string_to_integer ( std::string value , int64_t &integer )
        {
                int64_t  result = 0 ;
                int temp = 0 ;

                for ( int i = 0 ; i < value.size() ; i++)
                {
                        temp = value[i] -'0' ;
                        result = result*10 ;
                        result += temp ;
                }

                integer = result ;
        }

