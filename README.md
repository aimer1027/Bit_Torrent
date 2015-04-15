This project is used to parser and anylyze a .torrent file,which is in B-encoded .

You can compile the whole project by running "make" command 
on your linux platform in which g++ compiler is installed .

You have to input the b-coding message manually , in the future this project can 
read the .torrent file automatically by reading and analyzing the leagal path you input into your console. 

In this project , we use the google log to print the {info , warnning , error ...}
messages , if you want the project running correctly on your own computer, 
I am afraid you have to download and compile the library by yourself.

but , it is very simple to compile glog and use , for example , you can 
git clone the project from : https://github.com/google/glog.git 

and unzip the package on proper path , then ./configure && make && make install 
all the things will be done , if you write your #include <glog/logging.h> into your header file .

A lot of files and changes need to add to this project .
You can use the git pull command to track the lastest version  
