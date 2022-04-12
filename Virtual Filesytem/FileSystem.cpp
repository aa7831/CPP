#include <iostream>
#include "tree.hpp"
#include <iomanip>

using namespace std;

void display_commands();


int main(){

	//initializing the tree
	Tree tree;
	// tree.loadData();

	//uncomment the below code to test 
	// // testing 
	// tree.mkdir("system");
	// tree.mkdir("user");
	// tree.cd("system");
	// tree.mkdir("tmp");
	// tree.touch("init.r", 150);
	// tree.touch("mem.conf", 512);
	// tree.cd("..");
	// tree.cd("user");
	// tree.mkdir("Data");
	// tree.touch("contact.txt", 45);
	// tree.cd("Data");
	// tree.touch("exp1.txt", 1886);
	// tree.touch("exp2.txt", 22232);
	//end of testing
	tree.load();
	display_commands();
	string input;
	string command1; string command2; //command 1 is the command, command 2 & 3 are inputs
	string command3;
	bool end = false;
	while(!end){
		try{
			cout<<">";
			getline(cin,input);
			command1 = input.substr(0,input.find(" "));
			string temp;
			command2 = input.substr(input.find(" ")+1, input.length() - command1.length());
			if(input == "help")
				display_commands();

			else if(input == "pwd")
				tree.pwd();

			else if(command1 == "realpath")
				tree.realpath(command2);
			
			else if(input == "ls")
				tree.ls();
			else if(input == "ls sort")
				tree.ls(true);

			else if(command1 == "mkdir")
				tree.mkdir(command2);

			else if(command1 == "touch"){
				temp = command2.substr(0,command2.find(" "));
				command3 = command2.substr(command2.find(" ")+1);
				tree.touch(temp,stoi(command3));
			}

			else if(command1 == "cd"){
				if(input == "cd")
					tree.cd(""); //changes back to root

				else
					tree.cd(command2);
			}

			else if(command1 == "find")
				tree.find(command2);

			else if(command1 == "mv"){
				temp = command2.substr(0,command2.find(" "));
				command3 = command2.substr(command2.find(" ")+1);
				if(temp[0] == '/') //it is a path
					tree.mv(temp,command3, true);
				else
					tree.mv(temp,command3);
			}

			else if(command1 == "rm"){
				if(command2[0] == '/') // it is a path
					tree.rm(command2, true);
				else
					tree.rm(command2);
			}

			else if(command1 == "size")
				tree.size(command2);

			else if(command1=="emptybin")
				tree.emptyBin();

			else if(command1=="showbin")
				tree.showBin();

			else if(command1 == "recover")
				tree.recover();

			else if(input=="Exit"){
				tree.dump();
				end = true;
			}
			else
				cout<<"Invalid Command!"<<endl;
		}
		catch(Exception& e){
			cout<<e.get_err()<<endl;
		}
	}
}

void display_commands(){
	cout<<"help                         : Prints the menu of commands"<<endl
	<<"pwd                          : Prints the path of current directory"<<endl
	<<"realpath filename            : Prints full path of a given filename or file within current directory"<<endl
	<<"ls                           : Prints the files/folders inside current directory"<<endl
	<<"ls sort                      : Prints the files/folders inside current directory in descending file size"<<endl
	<<"mkdir foldername             : Creates folder in current directory"<<endl
	<<"touch filename size          : Creates a file in current directory with specified filename and size"<<endl
	<<"cd foldername                : Change current directory to specified folder"<<endl
	<<"cd ..                        : change current directory to parent directory"<<endl
	<<"cd -	                     : Change current directory to previous working directory"<<endl
	<<"cd /my/path/namespace        : Changes current directory to specified path"<<endl
	<<"find foldername or filename  : Returns path of file or folder if it is found"<<endl
	<<"mv filename foldername       : Moves file in current directory to a folder in current directory"<<endl
	<<"rm foldername or filename    : Removes specified folder or file to bin."<<endl
	<<"size foldername or filename  : Prints size of folder or size"<<endl
	<<"emptybin                     : Empties the bin"<<endl
	<<"showbin                      : Shows content of bin"<<endl
	<<"recover                      : Recovers the oldest deleted file"<<endl
	<<"Exit                         : Exits the program"<<endl;
}
