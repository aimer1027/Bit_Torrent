AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=Message_Test
BSON_SRC_PATH=/unixC/Bson/bson/src
BOOST_LIB_PATH=/unixC/Boost/boost_1_58_0

#what source files  Message_Test needs ?
Message_Test_SOURCES=\
   Main.cpp \
   $(BSON_SRC_PATH)/bsonobj.cpp $(BSON_SRC_PATH)/util/json.cpp \
   $(BSON_SRC_PATH)/oid.cpp $(BSON_SRC_PATH)/lib/base64.cpp \
   $(BSON_SRC_PATH)/lib/md5.cpp $(BSON_SRC_PATH)/lib/nonce.cpp 


Message_Test_CXXFLAGS=\
       -I$(BOOST_LIB_PATH) -I$(BSON_SRC_PATH) -D_FILE_OFFSET_BITS=64 \
       -ggdb -Wall -O0


Message_Test_LDADD=\
       -lpthread -lm -lboost_system -lboost_thread \
       -lboost_thread -lboost_program_options -lrt

Message_Test_LDFLAGS=\
        -fPIC -rdynamic -L$(BOOST_LIB_PATH)/stage/lib -pthread     

