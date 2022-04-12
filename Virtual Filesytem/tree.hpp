#include<iostream>
#include<ctime>
#include<vector>
#include<stack>
#include<queue>
#include<sstream>
#include<fstream>

using namespace std;



class Exception{
	//class to throw exceptions
	private:
		string errmsg;
	public:
		Exception(string e){
			errmsg = e;
		}
		string get_err(){
			return errmsg;
		}

};

string get_date(){
	//returns current date
	string date; //format = "dd-mm-yyyy"
	time_t t = time(0);
	tm *dat = localtime(&t);
	date = to_string(dat->tm_mday);
	date += "-" + to_string(dat->tm_mon+1) + "-" + to_string(dat->tm_year + 1900 - 2000);

	return date;
}


class iNode{ //iNode class
	private:
		string name;
		int size;
		int originalSize;
		string date;
		bool folder;
		iNode* parent; //pointer to the parent
		vector<iNode*> children; //vector that holds pointers to the children iNodes
	public:
		iNode(string n, int s, string d, iNode* p, bool f = false){
			name = n;
			size = s;
			originalSize = s;
			date = d;
			parent = p;
			folder = f;
		}

		iNode* getParent(){
			//returns pointer to parent
			return parent;
		}

		int original(){
			//returns the original size of folder/file before adding stuff to it
			return originalSize;
		}
		int getSize(){
			//returns size of node
			return size;
		}

		string get_dt(){
			return date;
		}

		void addSize(int a){
			size += a;
		}

		void subSize(int a){
			//subtracts a from size
			size -= a;
		}

		void setDate(string d){
			date = d; //changes date of node to d.
		}

		bool isFolder(){
			return folder;
		}

		void addChild(iNode* u){
			int s = u->getSize();
			// size += s;
			children.push_back(u);

			//we need tocl add size for all parent nodes as well
			iNode* a = u->getParent();

			while(a != NULL){
				a->addSize(s);
				a = a->getParent();
			}
		}

		vector<iNode*> getChildren(){
			//returns vector of children
			return children;
		}

		string getName(){
			return name; //returns name of the file/folder
		}

		string getDate(){
			return date;
		}

		void ls(bool sorted = false){
			//if sorting is needed, use bubble sorted
			if(sorted){
			//BUBBLE SORT
				for(int i =0; i<children.size() - 1; i++){
					for (int j = 0; j<children.size()-i-1;j++){
						if(children[j]->getSize() < children[j+1]->getSize()){
							//swap these 2
							iNode* temp = children[j];
							children[j] = children[j+1];
							children[j+1] = temp;
						}
					}
				}
			}

			//print children of current node
			for(int i =0; i<children.size(); i++){
				if(children[i]->isFolder())
					cout<<"dir ";
				else
					cout<<"file ";

				cout<<children[i]->getName()<<" "<<children[i]->getDate()<<" "<<children[i]->getSize()<<" bytes"<<endl;
			}
		}

		iNode* cd(string name){
			//find name in children
			bool found = false;
			for(int i =0; i<children.size(); i++){
				if(children[i]->getName() == name){
					found = true;
					iNode* u = children[i];
					if(u->isFolder())
						return u;
					else{
						cout<<"Cannot change directory to file"<<endl;
						break;
					}
				}
			}
			cout<<"Directory not found!"<<endl;
			return this;
		}

		void rm(int index){
			//remove child at given index
			int s = children[index]->getSize();
			size-= s;
			children.erase(children.begin()+index);

			//we need to change the size of all parent nodes as well
			iNode* pr = getParent();
			while(pr != NULL){
				pr->subSize(s);
				pr = pr->getParent();
			}
		}

		void changeParent(iNode* pr){
			parent = pr;
		}

};


class Tree{
	//tree Class
	private:
		iNode* current; //pointer to the current node we are at
		iNode* root; //pointer to the root
		iNode* previous; //previous working directory
		vector<iNode*> nodes; //vector that holds all the nodes of the tree
		queue<iNode*> bin; //recycling bin for deleting and recovering files
	
	public:
		Tree(){
			//creates an empty root
			root = new iNode("/", 0, get_date(), NULL, true);
			nodes.push_back(root);
			current = root;
			previous = root;
		}

		~Tree(){
			//destructor
			//deleting in a post-order manner
			//Delete the children first, then the parents until you reach the root
			current = root;
			DeletePostOrder(current);

			//empty the bin
			for(int i = 0; i<bin.size(); i++)
				bin.pop();

			//empty the nodes vector
			nodes.clear();
		}

		void DeletePostOrder(iNode* n){
			vector<iNode*> list = n->getChildren();
			for(int i=0; i<list.size(); i++){
				DeletePostOrder(list[i]);
			}

			delete n;
		}

		iNode* getRoot(){
			//returns pointer to root
			return root;
		}

		void pwd(){
			//print the current working directory
			iNode* temp = current;
			stack<string> s;
			while(temp!=NULL){
				s.push(temp->getName()); //push all names in the stack until top of tree is reached. Note: this is added in reverse order so we use a stack to print correct order
				temp = temp->getParent();
			}

			s.pop(); //popping the root from the stack
			if(s.empty())
				cout<<"/"<<endl;
			while(!s.empty()){
				cout<<"/";
				cout<<s.top();
				s.pop();
			}
			cout<<endl;
		}

		string pwd(iNode* temp, bool val = false){
			//print the current working directory
			stack<string> s;
			string c = "";

			while(temp!=NULL){
				s.push(temp->getName()); //push all names in the stack until top of tree is reached. Note: this is added in reverse order so we use a stack to print correct order
				temp = temp->getParent();
			}

			s.pop(); //popping the root from the stack
			while(!s.empty()){
				c+="/";
				c+= s.top();
				if(!val){
					cout<<"/";
					cout<<s.top();
				}
				s.pop();
			}
			if(!val)
				cout<<endl;

			return c;
		}


		void realpath(string name){
			//find the name in the children first
			bool flag;
			vector<iNode*> list = current->getChildren();
			for(int i =0; i<list.size(); i++){
				if (list[i]->getName() == name){
					flag = true;
					iNode* u = list[i];
					pwd(u);
					break;
				}
			}

			if(!flag){
				throw(Exception("File/Folder name not found!"));				
			}
		}

		void ls(bool sorted = false){
			//print all children of current node
			current->ls(sorted);

		}


		bool isUnique(string n){
			//check if name n is unique under current directory
			vector<iNode*> list = current->getChildren();
			for(int i =0; i<list.size(); i++){
				if(list[i]->getName() == n)
					return false; //not unique
			}
			return true; //is unique
		}

		bool isValid(string n){
			//check if name n in valid 
			for(int i =0; i<n.length(); i++){
				if(!isalnum(n[i]) && n[i] != '.')
					return false; //not valid
			}

			return true; // valid
		}

		void mkdir(string name, string date = get_date()){
			//make a folder in current working directory
			if(isUnique(name) && isValid(name)){
				//valid, now we can add it to the directory
				iNode* u = new iNode(name, 10, date, current, true);
				//add child to current node
				current->addChild(u);
				//add node to vector
				nodes.push_back(u);
			}
			else
				throw(Exception("Invalid File Name!"));
		}

		void touch(string name, int size, string date = get_date()){
			//make file in current working directory
			if(isUnique(name) && isValid(name)){
				//valid, add to directory
				iNode* u = new iNode(name, size, date, current);
				current->addChild(u);
				//add node to vector
				nodes.push_back(u);
			}
			else
				throw(Exception("Invalid File Name!"));
		}

		void cd(string name){
			//change current directory to string entererd
			//5 cases: we have a name, or .. or -, or a path or change to root
			if (name == ".."){
				//go to parent
				previous = current;
				if(current->getParent() != NULL)
					current = current->getParent();
			}
			else if(name =="-"){
				//change to previous working directory
				iNode* temp = previous;
				previous = current;
				current = temp;
			}
			else if(name == ""){
				//empty string, change current to root
				previous = current;
				current = root;
			}
			else{
				//determining if file name or path
				if (name[0] == '/'){
					// it is a path
					//removing first / 
					name = name.substr(1,name.length());
					stringstream n(name); //converting names to a string stream of variable n
					//adding names of all files and folders in a vector
					vector<string> names;
					string a;
					while(n.good()){
						getline(n,a,'/'); //retrieving names of all files/folders from the string
						names.push_back(a); //adding these to a vector
					} 

					//now we have a vector of names, need to check if valid path or not
					iNode* temp1 = current; //holds current node in case there is not a valid path, we will move back to this and return an error
					current = root;
					iNode* temp2; //used to check if valid or not in the for loop
					for(int i =0; i<names.size(); i++){
						//iterating over the vector
						temp2 = current;
						current = current->cd(names[i]);
						if(current==temp2){
							//invalid name occured so we retrieve current directory from temp1 and break the loop
							current = temp1;
							break;
						}
					}
				}
				else
					current = current->cd(name); //simply change specified folder
			}
		}

		void find(string name){
			//return the path of filename/folder if it is found
			bool found = false;
			//check all nodes for name
			for(int i=0; i<nodes.size();i++){
				if (nodes[i]->getName() == name){
					//node found
					//print the path of the node
					pwd(nodes[i]);
					found = true;
					break;
				}
			}

			if(!found)
				cout<<"Not found!"<<endl;
		}

		void mv(string filename, string foldername){
			//first find the filename in the children
			bool found = false;
			bool found2 = false;
			iNode* file;
			iNode* folder;
			int index; //used to save index of file so we can remove it later
			vector<iNode*> list = current->getChildren(); //get list of children
			for(int i =0; i<list.size(); i++){
				if(list[i]->getName() == filename && !list[i]->isFolder()){
					//found
					found = true;
					file = list[i];
					index = i;
					break;
				}
			}

			if(found){
				//file found
				//find the folder now
				for(int i =0; i<list.size(); i++){
					if(list[i]->getName() == foldername && list[i]->isFolder()){
						//folder found
						found2 = true;
						folder = list[i];
						break;
					}
				}

				if(found2){
					//both file and folder have been found. lets move the file now
					file->changeParent(folder); //changing parent of file
					folder->addChild(file); //adding file as child of folder
					current->rm(index);
				}
				else
					throw(Exception("Folder name not found!"));
			}
			else
				throw(Exception("File name not found!"));
		}

		void mv(string path1, string path2, bool path){
			//mv for arbritrary path
			iNode* temp = current; //holds current value
			current = root;
			//adding path1 to a vector and moving cd there
			path1 = path1.substr(1,path1.length());
			stringstream p1(path1);
			vector<string> names1;
			string a;
			while(p1.good()){
				getline(p1,a,'/');
				names1.push_back(a);
			}

			//navigating to the path specified in path1
			for(int i = 0; i<names1.size()-1;i++){
				cd(names1[i]);
				names1.erase(names1.begin());
			}

			//checking if the specified node is folder or file
			vector<iNode*> list = current->getChildren();
			bool found = false;
			iNode* file;
			iNode* fileParent; //holds the parent of the file
			int index;
			for(int i =0; i<list.size(); i++){
				if(list[i]->getName() == names1[0] && !list[i]->isFolder()){
					//found
					fileParent = current;
					file = list[i];
					found = true;
					index = i;
					break;
				}
			}

			if(found){
				//now we check the second path to find the folder
				path2 = path2.substr(1,path2.length());
				stringstream p2(path2);
				vector<string> names2;
				string a;
				while(p2.good()){
					getline(p2,a,'/');
					names2.push_back(a);
				}

				//navigating to specified path
				cd("..");
				for(int i = 0; i<names2.size()-1; i++){
					cd(names2[i]);
					names2.erase(names2.begin());
				}

				//checking if specified node is a folder or file
				list.clear();
				vector<iNode*> list = current->getChildren();
				iNode* folder;
				bool found2 = false;
				for(int i =0; i<list.size(); i++){
					if(list[i]->getName() == names2[0] && list[i]->isFolder()){
						//found
						folder = list[i];
						found2 = true;
						break;
					}
				}

				if(found2){
					//now we move the file from path 1 to the folder in path 2.
					file->changeParent(folder);
					folder->addChild(file);
					fileParent->rm(index);
				}
				else
					throw(Exception("Folder not found!"));

			}
			else
				throw(Exception("File not found!"));
		}

		void rm(string name){
			//remove file or folder corresponding to given name
			int index;
			bool found = false;
			iNode* u;
			//find the name
			vector<iNode*> list = current->getChildren();
			for(int i = 0; i<list.size();i++){
				if(list[i]->getName() == name){
					//found
					u = list[i];
					index = i;
					found = true;
					break;
				}
			}

			if(!found)
				throw(Exception("Name not found!"));
			else{
				if(bin.size()>=10)
					bin.pop();

				bin.push(u);
				current->rm(index);
			}
		}

		void rm(string name, bool path){
			//implementing rm on arbitrary inode location
			iNode* temp = current; //holding current directory in temp variable
			current = root;
			//adding all names to a vector
			name = name.substr(1,name.length()); // removing first /
			vector<string> names;
			stringstream n(name);
			string a;
			while(n.good()){
				getline(n,a,'/');
				names.push_back(a);
			}

			for(int i = 0; i<names.size()-1; i++){
				//changing directory to second last element in path
				cd(names[i]);
				names.erase(names.begin()); //removing these as we are done with them
			}

			//removing the node
			rm(names[0]);

			//changing cd back to original place
			current = temp;

		}

		void size(string name){
			//returns size of given name
			if(name[0] == '/'){
				if(name == "/"){
					//root
					cout<<root->getSize()<<" bytes"<<endl;
				}
				else{
					//path given as argument
					iNode* temp = current; //holding current in temp to change back later
					vector<string> names;
					stringstream n(name);
					string a;
					while(n.good()){
						getline(n,a,'/');
						names.push_back(a);
					}
					for(int i = 0; i<names.size()-1;i++){
						cd(names[i]);
						names.erase(names.begin());
					}

					size(names[0]);
				}
			}
			else{
				//find name
				bool found = false;
				for(int i = 0; i<nodes.size(); i++){
					if(nodes[i]->getName() == name){
						cout<<nodes[i]->getSize()<<" bytes"<<endl;
						found = true;
						break;
					}
				}

				if(!found)
					cout<<"Name not found!"<<endl;
		}	}

		void emptyBin(){
			//empties the bin
			while(!bin.empty())
				bin.pop();
		}

		void showBin(){
			//print oldest node in bin
			if(!bin.empty()){
				cout<<"Next Element to remove: ";
				pwd(bin.front());
				cout<<"Size = "<<bin.front()->getSize()<<" bytes, Date = "<<bin.front()->get_dt()<<endl;
			}
			else
				cout<<"Empty bin"<<endl;
		}

		void recover(){
			//recovers the oldest node
			iNode* u = bin.front();
			//find parent of the node
			iNode* pr = u->getParent();
			pr->addChild(u);
		}

		bool exists(vector<iNode*> test, string a){
			//check if node with name a exists in vector
			for(int i = 0; i<test.size(); i++){
				if (test[i]->getName() == a)
					return true;
			}

			return false;
		}

		string preorder(iNode* u, string& s){

			s+= pwd(u, true);
			s+=",";
			s+=to_string(u->original());
			s+= ",";
			s+=u->get_dt();
			s+="\n";

			vector<iNode*>list = u->getChildren();
			for(int i =0; i<list.size(); i++){
				preorder(list[i],s);
			}

			return s;
		}

		void dump(){
			//dumps data to file at the end
			ofstream dat("vfs.dat");
			string s = "/";
			//pre order traversal used here
			preorder(root, s);

			dat<<s;
			dat.close();
		}

		void load(){
			ifstream dat("vfs.dat");
			//each line has: path, size and date
			string path; string size; string date; string name;
			string line;
			int index;

			if (dat.is_open()){

				while(!dat.eof()){
					current = root;
					getline(dat, line);
					if(line!=""){
						// cout<<line<<endl; //debugging purposes
						path = line.substr(0,line.find(',')); //path
						line = line.substr(line.find(',')+1); //cutting down line
						size = line.substr(0,line.find(',')); //getting size
						line = line.substr(line.find(',')+1); //cutting down line
						//now that path and size have been cut out, date = line.
						date = line;

						//discarding the first backslash
						path = path.substr(1);
						index = path.find('/');

						if(index == -1 && size == "0"){
							//it is the root
							root->setDate(date);
						}
						else{
							//not the root so we traverse the rest of the path
							while(path.find('/')!=-1){
								cd(path.substr(0,path.find('/')));
								path = path.substr(path.find('/')+1);
							}

							name = path;
							// cout<<"name = "<<name<<endl; //debugging purposes
							if(name.find('.')!=-1){
								//it is a file
								touch(name,stoi(size),date);
							}
							else //folder
								mkdir(name,date);
						}
					}
				}
		dat.close();
			}
		}

};
