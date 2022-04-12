#include<iostream>
#include<fstream>
#include<ctime>
#include<string>
#include<sstream>
#include<iomanip>

using namespace std;

void to_lowercase(string& s); //function to convert given string "s" to lower case


//exception
class exception1{
private:
	string error;
public:
	exception1(const string& s){error = s;}
	string get_err(){return error;}

};

//====user defined classes
class bool_node{
public:
	bool bool_val;
	string x;
};

class arr_int{
public:
	string* arr;
	int x;
	
	int get_x(){
		return x;
	}
};

class FCLinkedList;
//====/

//=======singly linked list========
class SNode{
	private:
		string elem;
		SNode* next;
		SNode* prev;
		friend class SLinkedList;
		friend class FCLinkedList;
};

class SLinkedList{
	private:
		SNode* head;
		SNode* tail;
	public:
		friend class FCLinkedList;
		SLinkedList(){ //constructor
			head = new SNode;
			tail = new SNode;
			head->next = tail;
			tail->prev = head;
			head->prev = NULL;
			tail->next = NULL;
		}
		~SLinkedList() {
			while(!empty()){
				removeFront();
			}
		}
			
		bool empty() const {return head->next == tail;} 
		
		bool search(const string& e);
		
		void addFront(const string& e) { 
			if(e!=""){
				if(!search(e)){
					SNode* u = new SNode;
					u->elem = e;
					u->next = head->next;
					u->prev = head;
					u->next->prev = u;
					head->next = u;
				}
			}
		}
		
		void removeFront(){
			SNode* old = head;
			head = old->next;
			delete old;
		}
		
		int size(){
			SNode* x = head->next;
			int size = 0;
			while(x!=tail && x->elem!=""){
				size++;
				x = x->next;
			}
			return size;
		}
		arr_int* list(){
			//returns array of whatever is in this list
			int s = size();
// 			cout<<"s = "<<s<<endl; //debugging purposes
			arr_int* y = new arr_int;
// 			cout<<s<<endl; //debugging purposes
			string* array = new string[s];
			SNode* u = head->next;
			for(int i = 0; i< s; i++){
				array[i] = u->elem;
				u = u->next;
			}
// 			array[s] = to_string(s);
			
			y->arr = array;
			y->x = s;
			return y;
		}
		
		void remove(const string& SID);
		
};

bool SLinkedList::search(const string& e){
	if(!empty()){
		SNode* current = head->next;
		while(current!=tail){
			if(current->elem == e)
				return true;
			
			current = current->next;
		}
	}
	return false;

}

void SLinkedList::remove(const string& SID){
	SNode* current = head->next;
	if(!empty()){
		while(current!=tail){
			if(current->elem == SID){
				//node found that has this session ID, now we delete it
				SNode* u = current->prev;
				SNode* w = current->next;
				u->next = w;
				w->prev = u;
				delete current; //deleting current
			}
			
			current = current->next;
		}
	}
}
//===========================
class session{
	public:
		string ID;
		string className;
		string CID;
		int maxcap;
		string date; string time; 
		string room;
		SLinkedList registered_members;
		session* next;
		session* prev;
		

		friend class ssLinkedList;
};

class ssLinkedList{
	private:
		session* head;
		session* tail;
	public:
		ssLinkedList(){ //constructor
			head = new session;
			head->prev = NULL;
			tail = new session;
			tail->next = NULL;
			head->next = tail;
			tail->prev = head;
		}
		~ssLinkedList() {
			while(!empty()){
				removeFront();
			}
		}
					
		bool empty() const {return head->next == tail;} 
		
		int Size(); //returns size of session list
		bool maxCap(session* u); //return true if max capacity has been reached, false otherwise
		
		session* lookup(const string& CID); //lookup according to class ID
		session* Slookup(const string& SID); // lookup according to session ID
		
		session* front(){return head->next;} //returns head of list
		
		void addSession(session*u, string& id){
			u->registered_members.addFront(id);
		}
		
		void addFront(const string& classname, const string& ID, const string& CID, const string& date, const string& time, const string& room, string booking = "", bool split = false, int maxcap = 20) { 
// 			cout<<"im here"<<endl; //debugging purposes
			session* u = new session;
			
			
			u->ID = ID;
			u->className = classname;
			u->CID = CID;
			u->maxcap = 20;
			u->date = date;
			u->time = time;
			u->room = room;
			
			//updating pointers
			u->next = head->next;
			u->prev = head;
			u->next->prev = u;
			head -> next= u;
			
			
			//dealing with bookings list
			if(!split){
				if(booking != ""){
					if(!maxCap(u)){
						addSession(u, booking);
					}
				}
			}else{
				string a;
				booking = booking.substr(1,booking.size()-2); //this line removes the square brackets [1,2,3] = 1,2,3
				stringstream ss(booking);
				while(ss.good()){
					getline(ss,a,',');
					if(a!="")
						addSession(u,a);
				}
			}
		}
		
		void removeFront(){
			session* old = head;
			head = old->next;
			delete old;
		}
		
		bool update(const string& ID);
		bool_node* remove(const string& ID);
		bool view(const string& ID, bool member);
		arr_int* return_members(const string& ID);
		
		void viewVacancies(const string& CID);
		void book(const string& UID, const string& SID);
		
		bool checkSID(const string& SID);
		
		void cancel(const string& UID, const string&SID);
		
		string returnCID(const string& SID);
};

int ssLinkedList::Size(){
	int size;
	if(!empty()){
		session* current = head;
		while(current != tail){
			size ++;
			current = current->next;
		}
		return size;
	}
	else
		return 0;
}

bool ssLinkedList::maxCap(session* u){
	return (Size()>= u->maxcap);
}

session* ssLinkedList::lookup(const string& CID){ //looks up according to class ID
	session* current = head;
	if(!empty()){
		while(current != tail){ //this is resulting in an infinte loop if there is just one node in the list. [SOLVED]
			if (current->CID == CID){
				return current;
			}
			current = current->next;
		}
	}
	return current;
}

session* ssLinkedList::Slookup(const string& SID){
	session* current = head;
	if(!empty()){
		while(current != tail){ 
			if (current->ID == SID){
				return current;
			}
			current = current->next;
		}
	}
	return current;

}
bool ssLinkedList::update(const string& ID){
	session* x = Slookup(ID);
	if(x == head || x == tail){
		//Session ID not found
		cout<<"Session ID not found"<<endl;
		return false;
	}
	else{
		string option; string value;
		//asking user which attribute to edit
		cout<<"1- Capacity"<<endl;
		cout<<"2- Room"<<endl;
		cout<<"3- Date"<<endl;
		cout<<"4- Time"<<endl;
		cout<<"Select an attribute to edit (enter number): ";
		cin>>option;
		if(option == "1"){
			cout<<"Enter new capacity: ";
			cin>>value;
			x->maxcap = stoi(value);
			
		}else if(option == "2"){
			cout<<"Enter new room number: ";
			cin>>value;
			x->room = value;
			
		}else if(option == "3"){
			cout<<"Enter new Date: ";
			cin>>value;
			x->date = value;
			
		}else if(option == "4"){
			cout<<"Enter new Time: ";
			cin>>value;
			x->time = value;
			
		}else
			return false;
	return true;
}
}

bool_node* ssLinkedList::remove(const string& ID){
	session* x = Slookup(ID);
	bool_node* y = new bool_node;
	if(x == head || x == tail){
		//Session ID not found
		y->bool_val = false;
		cout<<"Session ID not found"<<endl;
		return y;
	}
	else{
		//session ID found, so we delete it
		bool_node* y = new bool_node;
		y->bool_val = true;
		y->x = x->CID;
		session* u = x->prev;
		session* w = x->next;
		u->next = w;
		w->prev = u;
		delete x;
		return y;
	}
}

bool ssLinkedList::view(const string& ID, bool member){
	session* x = Slookup(ID);
	if(x == head || x == tail)
		return false;
	else{
		if(!member){
			cout<<"Viewing Session Information."<<endl;
			cout<<"Class Name: "<<x->className<<endl;
			cout<<"Class ID: "<< x->CID <<endl;
			cout<<"Date: "<< x->date <<endl;
			cout<<"Time: "<< x->time <<endl;
			cout<<"Room: "<< x->room <<endl;
			cout<<"Maximum Capacity: "<< x->maxcap <<endl;
			return true;
		}
		else{
			//displaying class name, date and time only
			cout<<"Class Name: "<< x->className << " || ";
			cout<<"Date: "<<x->date <<" || ";
			cout<<"Time: "<<x->time << endl;
			return true;
		}
	}
}

arr_int* ssLinkedList::return_members(const string& ID){
	session* x = Slookup(ID);
	arr_int* z = new arr_int;
	
	if(x == head || x == tail){
		z->x = -1;
		return z;
	}
	else{
		// session ID found, getting array of member ID
		z = x->registered_members.list();
		return z;
	}

}

void ssLinkedList::viewVacancies(const string& CID){
	//we go over all the sessions to look for vacancies
	bool flag = false;
	session* current = head->next;
	while(current != tail){
		if (current->registered_members.size() < current->maxcap){
			//vacancy found, printing information
			if(CID==""){
				cout<<"Vacant Session"<<endl;
				cout<<"Session ID: "<<current->ID<<" || ";
				cout<<"Class Name: "<<current->className<<" || ";
				cout<<"Max Capacity: "<<current->maxcap<<" || ";
				cout<<"Date: "<<current->date<<" || ";
				cout<<"Time: "<<current->time<<endl;
				flag = true;
			}
			else{ //look for specific class ID
				if(current->CID == CID){
					cout<<"Vacant Session"<<endl;
					cout<<"Session ID: "<<current->ID<<" || ";
					cout<<"Class Name: "<<current->className<<" || ";
					cout<<"Max Capacity: "<<current->maxcap<<" || ";
					cout<<"Date: "<<current->date<<" || ";
					cout<<"Time: "<<current->time<<endl;
					flag = true;
				}
			}
		}
		current = current->next;
	}
	if(!flag)
		cout<<"No Vacant Session!"<<endl;
}

void ssLinkedList::book(const string& UID, const string& SID){
	session*x = Slookup(SID); //finding session that has corressponding SID.
	if(x != head && x!= tail){
		// session found so we add user
		x->registered_members.addFront(UID);
		cout<<endl<<"Session booked successfully!"<<endl;
	}
	else //session not found
		cout<<"Session not Found!"<<endl;
}

bool ssLinkedList::checkSID(const string& SID){
	session* x = Slookup(SID);
	if(x!=head && x!= tail)
		return true;
	
	return false;
}

void ssLinkedList::cancel(const string& UID, const string&SID){
	session*x = Slookup(SID); //getting session that has corressponding SID.
	if(x!= head && x!=tail){
		//session found, we delete user
		x->registered_members.remove(UID);
		cout<<"Canceled Successfully!"<<endl;
	}
	else
		cout<<"Session not Found!"<<endl;
}

string ssLinkedList::returnCID(const string& SID){
	session* x = Slookup(SID);
	return x->CID;
}
//==============================
class fitnessClass{
	public:
		string ID;
		string name;
		fitnessClass* next;
		fitnessClass* prev;
		SLinkedList sessions; //singly linked list of session IDs
		
		friend class FCLinkedList;
};



class FCLinkedList{ //doubly list of classes
	private:
		fitnessClass* head;
		fitnessClass* tail;
		ssLinkedList sessions_detailed; //linked list of sessions, with details
	public:
		FCLinkedList(){ //constructor
			head = new fitnessClass;
			head->prev = NULL;
			tail = new fitnessClass;
			tail->next = NULL;
			
			head->next = tail;
			tail->prev = head;
		}
		
		~FCLinkedList() {
			while(!empty()){
				removeFront();
			}
		}
		
		void loadSessions(string SID, string CID, string date, string time, string room, string list_of_bookings, int maxcap){ //loads sessions after files are read
			fitnessClass* u = lookup(CID);
			sessions_detailed.addFront(u->name, SID, CID, date, time, room, list_of_bookings, maxcap);
			addSession(u, SID);
		}
		
		fitnessClass* lookup(const string& ID); // function to find node of corresponding ID.
			
		bool empty() const {return head->next == tail;} 
		void add(const string& ID, const string& name) { 
			fitnessClass* u = new fitnessClass;
			u->ID = ID;
			u->name = name;
			
			u->next = head->next;
			u->next->prev = u;
			u->prev = head;
			head -> next = u;
		}
		
		bool addSession(const string& id);
		
		void addSession(fitnessClass*u, const string& ID){
			u->sessions.addFront(ID);
		}
		
		void addFront(const string& ID, const string& name, string booking, bool split = false) { 
			fitnessClass* u = new fitnessClass;
			u -> ID = ID;
			u -> name = name;
			
			u->next = head->next;
			u->prev = head;
			u->next->prev = u;
			head->next = u;
						
			if(!split){
				addSession(u,booking);
			}
			else{
				string a;
				booking = booking.substr(1,booking.size()-2); //this line removes the square brackets [1,2,3] = 1,2,3
				stringstream ss(booking);
				while(ss.good()){
					getline(ss,a,',');
					if(a!=""){
						addSession(u,a);
					}
				}
			}
		}
		
		bool remove(const string& ID);
		
		bool update(const string& ID);
		
		void removeFront(){
			fitnessClass* old = head;
			head = old->next;
			delete old;
		}
		
		fitnessClass* get_head(){ return head;}
		
		fitnessClass* get_tail(){return tail;}
		
		bool update_session(const string&SID);
		bool remove_session(const string& SID);
		bool view_session(const string& SID, bool member = false);
		arr_int* return_members(const string& SID);
		
		void viewVacancies(const string& CID = "");
		
		void book(const string& UID, const string&SID){
			sessions_detailed.book(UID, SID);
		}
		
		bool checkSID(const string& SID);
		
		void cancelBooking(const string& UID, const string& SID){
			sessions_detailed.cancel(UID,SID);
		}
		
		string getCID(const string& SID){ //returns class ID, takes SID as argument
			string CID;
			CID = sessions_detailed.returnCID(SID);
			return CID;
		}
		
		bool_node* getSLine();
		bool_node* getLine();
};

bool FCLinkedList::remove(const string& ID){
	if(!empty()){
		
		fitnessClass* current= head->next;
		while(current != NULL){
			if(current->ID == ID){
				//deleting all sessions
				// current->sessions.~SLinkedList();
				current->prev->next = current->next;
				current->next->prev = current->prev;
				delete current;
				return true;
			}
			current = current->next;
		}
	}
	return false;
}

bool FCLinkedList::update(const string& ID){
	if(!empty()){
		fitnessClass* current= head->next;
		while(current != NULL){
			if(current->ID == ID){
				string name;
				cout<<"Enter new name: ";
				cin >> name;
				//updating value
				current -> name = name;
				return true;
			}
			current = current->next;
		}
	}
	return false;
}

fitnessClass* FCLinkedList::lookup(const string& ID){
	fitnessClass* current = head->next;
	if(!empty()){
		while(current != tail){
			if (current->ID == ID){
				return current;
			}
			current = current->next;
		}
	}
	return current;
}

bool FCLinkedList::addSession(const string& ID){
	string SID; string maxcap; string date; string time; string room;
	fitnessClass* u;
	u = lookup(ID);
	if(u!=tail){
		//node was found
		cout<<"Enter session ID: ";
		cin>>SID;
		cout<<"Enter maximum capacity: ";
		cin>>maxcap;
		cout<<"Enter session date: ";
		cin>>date;
		cout<<"Enter session time: ";
		cin>>time;
		cout<<"Enter room number: ";
		cin>>room;
		
		sessions_detailed.addFront(u->name,SID, ID, date, time, room, "", false, stoi(maxcap));
		u->sessions.addFront(SID);
		return true;
	}
	return false;
}


bool FCLinkedList::update_session(const string& SID){ //function to update detailed sessions according to session ID
	//calling update method of sessions_detailed list
	bool flag;
	flag = sessions_detailed.update(SID);
	return flag;
}

bool FCLinkedList::remove_session(const string& SID){
	bool flag;
	bool_node* y;
	string CID;
	fitnessClass* u;
	y = sessions_detailed.remove(SID);
	flag = y->bool_val;
	if(flag){
		//we also need to remove session ID from singly list of session IDs only
		CID = y->x; //value of CID
		u = lookup(CID);
		SNode* x = u-> sessions.head;
		while(x != NULL){
			if(x->elem == SID){
				//Session ID found, so deleting it
				x->prev->next = x->next;
				delete x;
				break; //breaking loop as job is done
			}
			x = x->next;
		}
	}
	return flag;
}

bool FCLinkedList::view_session(const string& SID, bool member){
	bool flag;
	flag = sessions_detailed.view(SID, member);
	return flag;
}

arr_int* FCLinkedList::return_members(const string& SID){
	arr_int* y;
	y = sessions_detailed.return_members(SID);
	return y;
}

void FCLinkedList::viewVacancies(const string& CID){
	//we check all the sessions for vacancies
	sessions_detailed.viewVacancies(CID);
}

bool FCLinkedList::checkSID(const string& SID){
	return(sessions_detailed.checkSID(SID));
}

bool_node* FCLinkedList::getSLine(){
	bool_node* x = new bool_node;
	if(sessions_detailed.empty()){
		x->bool_val = false;
	}
	else{
		string s;
		session* u = sessions_detailed.front();
		
		arr_int* y = u->registered_members.list();
		
		bool flag = false;
		
		int size = y->x;
		string* array = y->arr;
		string registered = "[";
		for(int i=0; i<size; i++){
			registered += array[i] + ",";
			flag = true;
		}
		if(flag)
			registered = registered.substr(0, registered.size()-1);
		
		registered += "]";
		
		s = u->ID + "," + u->CID + "," + to_string(u->maxcap) + "," + u->date + "," + u->time + "," + u->room + "," + registered;
		
		x->x = s;
		x->bool_val = true;
		
		sessions_detailed.removeFront();
		
	}
	
	return x;
		
	}

bool_node* FCLinkedList::getLine(){
	bool_node* x = new bool_node;
	if(empty()){
		x->bool_val = false;
	}
	else{
		string s;
		fitnessClass* u = head->next;
		
		arr_int* y = u->sessions.list();
		// cout<<"size : "<<y->x<<endl; //debugging purposes
		
		bool flag = false;
		
		int size = y->x;
		string* array = y->arr;
		string sessions = "[";
		for(int i=0; i<size; i++){
			sessions += array[i] + ",";
			flag = true;
		}
		if(flag)
			sessions = sessions.substr(0, sessions.size()-1);
		
		sessions += "]";
		
		s = u->ID + "," + u->name + "," + sessions;
		
		x->x = s;
		x->bool_val = true;
		
		removeFront();
		
	}
	
	return x;
		
}


//==============================

class admin{ 
	public:
		string username; 
		string password; 
		string ID;
		string firstname;
		string lastname;
		admin* next;
		friend class ALinkedList;
};

class ALinkedList{
	public:
		ALinkedList(): head(NULL){};//constructor
		bool empty() const {return head==NULL;} 
		void addFront(const string& username, const string& password, const string& id, const string& firstname, const string& lastname) { 
			admin* u = new admin;
			u -> username = username;
			u -> password = password;
			u -> ID = id;
			u->firstname = firstname;
			u->lastname = lastname;
			
			//updating pointers
			u->next = head;
			head = u;
		}
		void removeFront(){
			if(!empty()){
				admin* old = head;
				head = old->next;
				delete old;
			}
		}
		
		bool_node* checkCredentials(string username, string password);
		bool_node* getLine();
		
		~ALinkedList(){
			  while(!empty()){
			  	removeFront();
			  }
		}
	private:
		admin* head;
};

bool_node* ALinkedList::checkCredentials(string username, string password){
	bool_node* x = new bool_node;
	if(!empty()){
		admin* current = head;
		while(current != NULL){
			if(current->username == username){
				if (current->password == password){
					x->bool_val = true;
					x->x = current->ID;
					return x;
				}
				else{
					x->bool_val = false;
					return x;
				}
			}
			current = current->next;
		}
	}
	x->bool_val = false;
	return x;
}

bool_node* ALinkedList::getLine(){ //used to return lines to dump in file at end of program
	bool_node* x = new bool_node;
	if(empty()){
		x->bool_val = false;
	}
	else{
		string s;
		//ID,firstname,lastname,username,password
		admin* u = head;
	
		s = u->ID + "," + u->firstname + "," + u->lastname + "," + u->username + "," + u->password;
		x->x = s;
		x->bool_val = true;
		//remove front element from this list
		removeFront();
	}
	
	return x;
}

//==========================================================

class name{
public:
	string first;
	string last;
};


//===
class member{
	public:
		string username;
		string password;
		string ID;
		string firstname;
		string lastname;
		member* next;
		member* prev;
		SLinkedList bookings;
		friend class MLinkedList;
};

class MLinkedList{
	public:
		MLinkedList(){ //constructor
			head = new member; tail = new member;
			head->next = tail;
			tail->prev = head;
		}
		bool empty() const {return head->next == tail;} 
		void addFront(const string& username, const string& password, const string& id, const string& firstname, const string& lastname, string booking = "", bool split = false) { 
			member* u = new member;
			u -> username = username;
			u -> password = password;
			u -> ID = id;
			u -> firstname = firstname;
			u -> lastname = lastname;

			u->next = head->next;
			u->prev = head;
			u->next->prev = u;
			head->next = u;
			
			if (booking!=""){
				if(!split){
				addBooking(u,booking);
				}
				else{
					string a;
					booking = booking.substr(1,booking.size()-2); //this line removes the square brackets [1,2,3] = 1,2,3
					stringstream ss(booking);
					while(ss.good()){
						getline(ss,a,',');
						if(a!= "")
							addBooking(u,a);
					}
				}
			}
		}
		
		void addBooking(member* u, string& id){
			u->bookings.addFront(id);
		}
		void removeFront(){
			member* old = head;
			head = old->next;
			delete old;
		}
		
		member* get_head(){ return head;}
		member* get_tail(){return tail;}
		
		bool uniqueID(int& id);
		bool_node* checkCredentials(string username, string password);
		name* get_member(const string& ID);
		string get_firstname(const string& lastname); //takes last name as argument, returns entire name 
		arr_int* return_bookings(const string& ID); //takes ID as argument, returns list of bookings of this member
		member* get_MNode(const string& ID); //takes ID as argument, returns node which has same ID.
		
		~MLinkedList(){
			  while(!empty()){
			  	removeFront();
			  }
		}
		
		int size(){
			member* current = head->next;
			int size = 0;
			while(current!=tail){
				size++;
				current = current->next;
			}
			return size;
		}
		
		bool_node* getLine();
	private:
		member* head;
		member* tail;
};

bool_node* MLinkedList::checkCredentials(string username, string password){
	bool_node* x = new bool_node;
	if(!empty()){
		member* current = head;
		while(current != tail){
			if(current->username == username){
				if (current->password == password){
					x->bool_val =true;
					x->x = current->ID;
					return x;
				}
				else{
					x->bool_val = false;
					return x;
				}
			}
			current = current->next;
		}
	}
	x->bool_val = false;
	return x;
}

bool MLinkedList::uniqueID(int& i){
	string id = to_string(i);
	if(!empty()){
		member* current = head;
		while(current != NULL){
			if(current->ID == id)
				return false;
			
			current = current->next;
		}
	}
	
	return true;
}


name* MLinkedList::get_member(const string& ID){
	member* current = head;
	name* n = new name;
	while(current != NULL){
		if(current->ID == ID){
			n->first = current->firstname;
			n->last = current->lastname;
			return n;
		}
		current = current->next;
	}
}

string MLinkedList::get_firstname(const string& lastname){
	member* current = head;
	while(current != NULL){
		if(current->lastname == lastname){
			//last name found, returning entire name now 
			return (current->firstname);
		}
		current = current->next;
	}

}

arr_int* MLinkedList::return_bookings(const string& ID){
	//finding member with this ID
	member* current = head;
	arr_int* y;
	while(current!=NULL){
		if(current->ID == ID){
			//found
			y = current->bookings.list();
			return y;
		}
		current = current->next;
	}
	
	//not found
	y->x = -1;
	return y;
}

member* MLinkedList::get_MNode(const string& ID){
	member* current = head;
	while(current!=NULL){
		if(current->ID == ID)
			return current;
		
		current = current->next;
	}
	return current; // this line never runs as we make sure that ID does exist.
}

bool_node* MLinkedList::getLine(){ //used to return lines to dump in file at end of program
	bool_node* x = new bool_node;
	if(empty()){
		x->bool_val = false;
	}
	else{
		string s;
		//ID,firstname,lastname,username,password,list of bookings -> [1,2,3]
		member* u = head->next;
		
		//getting array of bookings
		arr_int* y = u->bookings.list();
		
		bool flag = false;
		int size = y->x;	
		string* array = y->arr;	
		string book = "[";
		for(int i =0; i<size; i++){
			book += array[i] +",";
			flag = true;
		}
		if(flag)
			book = book.substr(0,book.size()-1);
		book += "]";
		
		s = u->ID + "," + u->firstname + "," + u->lastname + "," + u->username + "," + u->password + "," + book;
		x->x = s;
		x->bool_val = true;
		//remove front element from this list
		removeFront();
	}
	
	return x;
}
//==========================================================



void registerMember(MLinkedList &members){
	string f; string l;
	cout<<"Enter first name: ";
	cin>>f;
	cout<<"Enter last name: ";
	cin>>l;
	//generating random number and making sure it is unique
	int id;
	srand(time(NULL));
	do{
		id = rand() % 9000 + 1000;
	}while(!members.uniqueID(id));
		
	string idstr = to_string(id);
	string username = f;
	to_lowercase(username); //converting to lower case
	username += idstr; //concatenating with id
	
	//generating new 4 digit number for password
	int nid;
	nid = rand()%9000 + 1000;
	string password = l;
	to_lowercase(password);
	password += to_string(nid);
	
	//adding to members list
	members.addFront(username, password, idstr, f, l);
	
	//printing info
	cout<<endl; //empty line
	cout<<setw(15)<<"Member Created: "<<endl;
	cout<<"First Name: "<<f<<endl;
	cout<<"Last Name: "<<l<<endl;
	cout<<"Username: "<<username<<endl;
	cout<<"Password: "<<password<<endl;
}

template <typename T>
bool_node* login(T& user){
	bool_node* x = new bool_node;
	string username; string password;
	x->bool_val = false;
	do{
		cout<<"Enter 'EXIT' to go back"<<endl;
		cout<<"Enter username: ";
		cin>>username;
		if(username == "EXIT"){
			x->bool_val = false;
			break;
		}
		cout<<"Enter password: ";
		cin>>password;
		x = user.checkCredentials(username, password);
	}while(!x->bool_val);
	
	// cout<<x->bool_val<<endl;
	cout<<endl; //empty line
	return x;
}

//function to ocnvert string to lower case
void to_lowercase(string& s){
	for(int i = 0; i<s.size();i++){
		s[i] = tolower(s[i]);
	}
}

void addClass(FCLinkedList& fitnessClasses){
	string ID; string name;
	cout<<"Enter Class ID: ";
	cin>>ID;
	cout<<"Enter Class Name: ";
	cin>>name;
	
	fitnessClasses.add(ID, name);
	cout<<"Added successfully"<<endl;
}

void removeClass(FCLinkedList& fitnessClasses){
	string ID;
	bool flag;
	cout<<"Enter class ID: ";
	cin>>ID;
	flag = fitnessClasses.remove(ID);
	if(flag)
		cout<<"Removed successfully"<<endl;
	else
		cout<<"Failed."<<endl;
}

void updateClass(FCLinkedList& fitnessClasses){
	string ID;
	bool flag;
	cout<<"Enter class ID: ";
	cin>>ID;
	flag = fitnessClasses.update(ID);
	if(flag)
		cout<<"Updated successfully"<<endl;
	else
		cout<<"Failed."<<endl;
}

void addSession(FCLinkedList& fitnessClasses){
	bool flag;
	string ID;
	cout<<"Enter class ID: ";
	cin>>ID;
	flag = fitnessClasses.addSession(ID);
	if(flag)
		cout<<"Session added successfully"<<endl;
	else
		cout<<"Failed. Class ID not found."<<endl;
}

void updateSession(FCLinkedList& fitnessClasses){
	bool flag;
	string ID;
	cout<<"Enter Session ID: ";
	cin>>ID;
	flag = fitnessClasses.update_session(ID);
	if(flag)
		cout<<"Successfully updated!"<<endl;
	else
		cout<<"Failed. Something went wrong."<<endl;
}

void removeSession(FCLinkedList& fitnessClasses){
	bool flag;
	string ID;
	cout<<"Enter Session ID: ";
	cin>>ID;
	flag = fitnessClasses.remove_session(ID);
	if(flag){
		cout<<"Sucessfully removed!"<<endl;
	}
	else
		cout<<"Failed. Something went wrong."<<endl;
}

void viewSession(FCLinkedList& fitnessClasses, bool member = false, string ID = ""){
	bool flag;
	if (!member){
		cout<<"Enter Session ID: ";
		cin>>ID;
		flag = fitnessClasses.view_session(ID);
	}
	else
		flag = fitnessClasses.view_session(ID, true);
	if(!flag)
		cout<<"Session ID not found."<<endl;
}

void viewMembers(FCLinkedList& fitnessClasses, MLinkedList& members, bool sorted = false){
	arr_int* y;
	string ID;
	cout<<"Enter Session ID: ";
	cin>>ID;
	int size;
	y = fitnessClasses.return_members(ID);
	size = y->get_x();
	
	if(size == -1)
		cout<<"Session ID not found."<<endl;
	else{

		string* array = y->arr;
		name* n = new name;
		if(!sorted){
			for(int i = 0; i<size;i++){
				n = members.get_member(array[i]);
				string firstname; string lastname;
				firstname = n->first;
				lastname = n->last;
				cout<<i+1<<"-"<<firstname<<" "<< lastname<<endl;
			}
		}
		else{
			//we need to print sorted now, let's make another array and sort it
			string* sorted_lastnames = new string[size]; //this array will hold last names only
			for(int i = 0; i<size; i++){
				n = members.get_member(array[i]);
				sorted_lastnames[i] = n->last;
			}


			// now lets sort this array using bubble sort
			bool flag;
			string temp;
			for (int i = 0; i<size-1; i++){
				for(int j = 0; j<size-1;j++){
					if(sorted_lastnames[j] > sorted_lastnames[j+1]){
						//swap 
						temp = sorted_lastnames[j];
						sorted_lastnames[j] = sorted_lastnames[j+1];
						sorted_lastnames[j+1] = temp;
					}
				}
			}

			string* sorted_firstnames = new string[size];
			for(int i =0;i<size;i++){
				sorted_firstnames[i] = members.get_firstname(sorted_lastnames[i]);
			}
			// cout<<123<<endl;
			//array sorted, now we just print
			// string name;
			for(int i = 0; i<size;i++){
				//we have array of last names, we need to print these along with corressponding first names
				// name = members.get_firstname(sorted_lastnames[i]);
				cout<<i+1<<"-"<<sorted_firstnames[i]<<" "<<sorted_lastnames[i]<<endl;
			}
		}
	}
}

void viewBookings(FCLinkedList& fitnessClasses, MLinkedList& members){
	arr_int* y;
	string ID;
	cout<<"Enter Member ID: "<<endl;
	cin>>ID;
	y = members.return_bookings(ID);
	
	int size;
	size = y->get_x();
	if(size == -1){
		//member ID was not found
		cout<<"Member ID not found."<<endl;
	}
	else{
		//member ID found.
		string* array = y->arr; //array of all bookings
		for(int i = 0; i<size; i++){
			cout<<i+1<<"- ";
			viewSession(fitnessClasses, true, array[i]);
		}
	}
}

void viewVacancies(FCLinkedList& fitnessClasses, const string& CID = ""){
	cout<<endl;
	if(CID == "")
		fitnessClasses.viewVacancies();
	else
		fitnessClasses.viewVacancies(CID);
}

void bookSession(FCLinkedList& fitnessClasses, MLinkedList& members, const string& UID){
	//we have the user ID in UID
	string CID;
	cout<<"Enter Class ID: ";
	cin>>CID;
	
	//find the class ID 
	fitnessClass* head = fitnessClasses.get_head();
	fitnessClass* tail = fitnessClasses.get_tail();

	bool flag = false; //check if class ID was found or not
	
	fitnessClass* current = head->next;
	while(current != tail){
		if(current->ID == CID){
			//CID found
			flag = true;
			//ask user which session they would like to book by showing only vacant sessions
			viewVacancies(fitnessClasses, CID);
			cout<<endl<<"Enter Session ID you would like to book: ";
			string SID;
			cin>>SID;

			cout<<endl;
			
			//we have the session ID now. Update the relevant lists
			//list of sessions in members list first
			//get corressponding member in member list
			member* member = members.get_MNode(UID);
			//updating sessions list
			member->bookings.addFront(SID);
			
			//updating registered members list in sessions
			fitnessClasses.book(UID, SID);
		}
		current = current->next;
	}

	if(!flag)	cout<<endl<<"Class ID not found."<<endl<<endl;

}

void cancelSession(FCLinkedList& fitnessClasses, MLinkedList& members, const string& UID){
	//asking for session ID
	cout<<"Cancel Booking"<<endl;
	cout<<"Enter Session ID: ";
	string SID;
	cin>>SID;
	
	//first check if session ID exists
	bool valid;
	valid = fitnessClasses.checkSID(SID);
	if(valid){
		//get member from members in list
		member* member = members.get_MNode(UID);
		//remove session from member's list of registered sessions.
		member->bookings.remove(SID);
		
		//remove member's ID from session details
		fitnessClasses.cancelBooking(UID, SID);
	}
	else
		cout<<"Invalid Session ID!"<<endl;
}

void commonMembers(FCLinkedList& fitnessClasses, MLinkedList& members){
	cout<<"List of members with 2 or more common classes:"<<endl;
	//go over every member and check with each other
	member* mhead = members.get_head();
	member* mtail = members.get_tail();
	
	arr_int* x; arr_int*y;
	
	member* prev = mhead;
	member* next = mtail;
	
	int size = members.size();
	
	int common;
	int count = 0;
	
	for(int a = 0; a<size; a++){
		prev = prev->next;
		next = mtail->prev;
		while(prev != next){ //this loop compares node in the linked list with all other nodes
			//compare the classes of the two nodes here.
			common = 0;
			x = prev->bookings.list();
			y = next->bookings.list();
			
			string* array1 = x->arr;
			int size1 = x->x;
// 			cout<<size1<<endl; //debugging purposes
			string* array2 = y->arr;
			int size2 = x->x;
			
			string c1; string c2;
		
			for(int i = 0; i < size1; i++){
				c1 = fitnessClasses.getCID(array1[i]);
				for(int z = 0; z<size2;z++){
					c2 = fitnessClasses.getCID(array2[z]);
					if(c1 == c2)
						common++;
				}
			}
			
			if(common>1){
				//print these two members in a list
				count ++;
				cout<<count<<"- "<<prev->firstname<<" "<<prev->lastname<<" AND "<< next->firstname<<" "<<next->lastname<<endl;
			}
			next = next->prev;
		}
	}
	
	if(count == 0)
		cout<<"No members with common classes."<<endl;
	
}

//=======================================

void quit(ALinkedList& admins, FCLinkedList& fitnessClasses, MLinkedList& members){
	// writing all the information to files
	ofstream adminFile("Admins.txt",ios_base::trunc);
	ofstream memberFile("Members.txt",ios_base::trunc);
	ofstream classFile("FitnessClasses.txt",ios_base::trunc);
	ofstream sessionFile("Sessions.txt", ios_base::trunc);
	
	bool flag;
	string s;
	bool_node* x;
	//writing to admin file
	adminFile<<"admin_ID,first_name,last_name,username,password"<<endl; //first line is always the same
	x = admins.getLine();
	flag = x->bool_val;
	while(flag){
		s = x->x;
		//write this string in line
		adminFile<<s;
		x=admins.getLine();
// 		cout<<"im here"<<endl;
		flag = x->bool_val;
		if(flag) //this line avoids an extra line at the end of the document
			adminFile<<endl;
	}
	
	//writing to member file
	memberFile<<"member_ID,first_name,last_name,username,password,list_of_bookings"<<endl;
	x = members.getLine();
	flag = x->bool_val;
	while(flag){
		s = x->x;
		//write this string in line
		memberFile<<s;
		x=members.getLine();
// 		cout<<"im here"<<endl;
		flag = x->bool_val;
		if(flag) //this line avoids an extra line at the end of the document
			memberFile<<endl;
	}
	

	//writing to sessions file
	sessionFile<<"session_ID,class_ID,max_cap,session_date,session_time,room_number,list_of_registered_members"<<endl;
	x = fitnessClasses.getSLine();
	flag = x->bool_val;
	while(flag){
		s = x->x;
		//write this string in line
		sessionFile<<s;
		x=fitnessClasses.getSLine();
// 		cout<<"im here"<<endl;
		flag = x->bool_val;
		if(flag) //this line avoids an extra line at the end of the document
			sessionFile<<endl;
	}
	
	//writing to fitnessClass file
	classFile<<"class_ID,class_name,list_of_sessions"<<endl;
	x = fitnessClasses.getLine();
	flag = x->bool_val;
	while(flag){
		s = x->x;
		//write this string in line
		classFile<<s;
// 		cout<<s<<endl;
		x=fitnessClasses.getLine();
// 		cout<<"im here"<<endl;
		flag = x->bool_val;
		if(flag) //this line avoids an extra line at the end of the document
			classFile<<endl;
	}
	
	memberFile.close();
	adminFile.close();
	sessionFile.close();
	classFile.close();
	

}


//==========
int main(){
	//loading the files
	ifstream adminFile("Admins.txt");
	ifstream memberFile("Members.txt");
	ifstream classFile("FitnessClasses.txt");
	ifstream sessionFile("Sessions.txt");
	
	//creating lists of admin and members
	ALinkedList admins; MLinkedList members;
	
	//creating list of fitness class
	FCLinkedList fitnessClasses;
		
	//=======READING FILES==========
	//ignoring first line of every file
	string a;
	string ID; string firstname; string lastname; string username; string password;
	getline(adminFile,a);
	getline(memberFile, a);
	getline(classFile,a);
	getline(sessionFile,a);
	
	
	//admin file and adding all admins to list
	while(!adminFile.eof()){
		getline(adminFile, ID, ',');
		getline(adminFile, firstname, ',');
		getline(adminFile, lastname, ',');
		getline(adminFile, username, ',');
		getline(adminFile, password);
		if(ID!="")
			admins.addFront(username, password, ID, firstname, lastname);
	}
	
	
	
	//member file and adding all members to list
	string list_of_bookings;
	while(!memberFile.eof()){
		getline(memberFile, ID, ',');
		getline(memberFile, firstname, ',');
		getline(memberFile, lastname, ',');
		getline(memberFile, username, ',');
		getline(memberFile, password, ',');
		getline(memberFile, list_of_bookings);
		if(ID!="")
			members.addFront(username, password, ID, firstname, lastname, list_of_bookings, true);
	}
	
	//classes file and adding all members to list
	string name;
	while(!classFile.eof()){
		getline(classFile, ID, ',');
		getline(classFile, name, ',');
		getline(classFile, list_of_bookings);
		if(ID!="")
			fitnessClasses.addFront(ID, name, list_of_bookings, true); //list of sessions in class
	}
	
	//sessions file and adding all members to list
	string SID; string CID; string date; string time; string room;
	string maxcap;
	while(!sessionFile.eof()){
		getline(sessionFile, SID, ',');
		getline(sessionFile, CID, ',');
		getline(sessionFile, maxcap, ',');
		getline(sessionFile, date, ',');
		getline(sessionFile, time, ',');
		getline(sessionFile, room, ',');
		getline(sessionFile, list_of_bookings); //registered members list in session
// 		sessions.addFront(SID, CID, date, time, room, list_of_bookings, stoi(maxcap));
		if(SID!="")
			fitnessClasses.loadSessions(SID, CID, date, time, room, list_of_bookings, stoi(maxcap));
	}

	
	
	
	adminFile.close();
	memberFile.close();
	classFile.close();
	sessionFile.close();
	
// 	fitnessClasses.loadSessions(sessions);
	//==============================
	
	cout<<setw(15)<<"Welcome"<<endl;
	string option; 
	bool end = false;
	do{
		cout<<"1- Admin"<<endl;
		cout<<"2- Member"<<endl;
		cout<<"3- Exit" <<endl;
		cout<<"Choose an option (enter number): ";
		cin>>option;
		cout<<endl;
		
		if(option == "3"){
			end = true;
		}
		else if(option == "2"){ //members
			do{
				cout<<"1-Login"<<endl;
				cout<<"2-Register"<<endl;
				cout<<"3-Back"<<endl;
				cout<<"Choose an option (enter number): ";
				cin>>option;
				cout<<endl;
				
				if(option=="2"){
					registerMember(members);
					cout<<endl;
				}
				else if(option=="1"){ 
					string UID;
					bool_node* x;
					x = login(members);
					if(x->bool_val){
						//successfully logged in
						cout<<setw(15)<<"Logged In"<<endl;
						cout<<endl;
						UID = x->x;
						do{
							cout<<"1-View Vacant Sessions"<<endl;
							cout<<"2- Book a session"<<endl;
							cout<<"3- Cancel a session"<<endl;
							cout<<"4- Exit"<<endl;
							cout<<"Choose an option (enter number): ";
							cin>>option;
							
							if(option=="1"){
								viewVacancies(fitnessClasses);
								cout<<endl;
							}
							else if(option == "2"){
								//booking a session
								bookSession(fitnessClasses, members, UID);
							}
							else if(option == "3"){
								//cancelling a session
								cancelSession(fitnessClasses, members, UID);
							}
							else if(option == "4")
								end = true;
						} while(option!="4");
						
					}
				}
			}while(option!="3" && !end);
			
		}
	
	else if(option == "1"){ //admin
		bool_node* x;
		x = login(admins);
		if(x->bool_val){
			//logged in 
			cout<<endl;
			cout<<"Logged In"<<endl<<endl;
			cout<<endl;
			do{
				cout<<"1-Register a member"<<endl;
				cout<<"2-Add a new fitness class"<<endl;
				cout<<"3-Delete a fitness class"<<endl;
				cout<<"4-Update name of class"<<endl;
				cout<<"5- Add a session to existing fitness class"<<endl;
				cout<<"6- Update Session details"<<endl;
				cout<<"7- Delete a Session"<<endl;
				cout<<"8- View Session Details"<<endl;
				cout<<"9- View Members in a Session"<<endl;
				cout<<"10- View Members in a Session (sorted)"<<endl;
				cout<<"11- View bookings of a member"<<endl;
				cout<<"12- View Sessions with Vacancies"<<endl;
				cout<<"13- View List of Members with Common Classes"<<endl;
				cout<<"14- Exit"<<endl;
				cout<<"Choose an option (enter number): ";
				
				cin>>option;
				cout<<endl;
				
				if(option == "1"){
					registerMember(members);
					cout<<endl;
				}
				else if(option == "2"){
					addClass(fitnessClasses);
					cout<<endl;
				}
				else if(option == "3"){
					removeClass(fitnessClasses);
					cout<<endl;
				}
				else if(option =="4"){
					updateClass(fitnessClasses);
					cout<<endl;
				}
				else if(option == "5"){
					addSession(fitnessClasses);
					cout<<endl;
				}
				else if(option == "6"){
					updateSession(fitnessClasses);
					cout<<endl;
				}
				else if(option == "7"){
					//delete session
					removeSession(fitnessClasses);
					cout<<endl;
				}
				else if(option == "8"){
					viewSession(fitnessClasses);
					cout<<endl;
				}
				
				else if(option == "9"){
					viewMembers(fitnessClasses, members);
					cout<<endl;
				}
				else if(option == "10"){
					viewMembers(fitnessClasses, members, true);
					cout<<endl;
				}
				else if(option == "11"){
					viewBookings(fitnessClasses, members);
					cout<<endl;
				}
				else if(option == "12"){
					viewVacancies(fitnessClasses);
					cout<<endl;
				}
				else if(option == "13"){
					commonMembers(fitnessClasses, members);
					cout<<endl;
				}
				else if(option=="14"){
					end = true;
				}
				
			}while(option!="14");
		}
	}
			
	}while(!end);	
	
	//program ending, calling quit function
	quit(admins, fitnessClasses, members);
	
	// members.~MLinkedList();	
	return 0;
}
