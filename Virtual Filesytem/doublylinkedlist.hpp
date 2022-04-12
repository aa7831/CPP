#include<iostream>
#include<string>
#include<sstream>
#include<iomanip>
#include<exception>
using namespace std;

template<typename T>
class Node
{
	private:
		T elem; //data element (type string in this case)
		Node<T>* next; //Link (pointer) to the next Node
		Node<T>* prev; //Link (pointer) to the previous Node
		template<typename U>friend class DoublyLinkedList;
		
	public:
		Node(): next(NULL), prev(NULL)
		{}
		Node(T elem) : elem(elem), next(NULL), prev(NULL)
		{}
};
//==============================================================
template<typename T>
class DoublyLinkedList
{
	private:
		Node<T>* head; 	// pointer to the head of List
		Node<T>* tail; 	// pointer to the tail of List
	public:
		DoublyLinkedList (); // empty list constructor
		~DoublyLinkedList (); // destructor to clean up all nodes
		bool empty() const; // is list empty?
		const T& front() const; // get the value (element) from front Node of list
		const T& back() const;  // get the value (element) from last Node of the List 
		void addBefore(Node<T> *ptr, const T& elem);	 //add a new node before ptr
		void addFront(const T& elem); // add a new node to the front of list
		void addBack(const T & elem); //add a new node to the back of the list
		void removeFront(); // remove front node from list
		void removeBack();  // remove last node from list
		// void reverseList();	// reverse the list
		// bool isPalindrome(); //check if the List is Palindrome or not
		// void display() const;	// display all element of the list

		class iterator{
			private:
				Node<T>* it; //pointer to node

			public:
				//iterator traits
				using value_type = T;
				using pointer = T*;
				using reference = T&;
				using iterator_category = bidirectional_iterator_tag;
				using difference_type = ptrdiff_t;
				friend class DoublyLinkedList;

			public:

				iterator(Node<T>* n){
					it = n; //simple constructor
				}

				T operator*(){
					return it->elem; //fetches element of node and returns it
				}

				iterator& operator++(){
					if(it != NULL) //only increments if not already at end
						it = it->next;

					return *this;
				}

				iterator& operator--(){
					if(it!=NULL) //only decrements if not already at start
						it = it->prev;
					return *this;
				}

				bool operator==(const iterator& other)const{
					return(it == other.it); //comparing pointers to see if they both point to same thing
				}

				bool operator!=(const iterator& other)const{
					return(it != other.it); //comparing poitners to see if they both point to same thing
				}


		};

		//begin and end methods for the iterator
		iterator begin(){
			return iterator(head->next); //returns first node in list, i.e. the node after header
		}

		iterator end(){
			return iterator(tail); //returns next node after the last one in list i.e, the trailer
		}
		
};
// //===================================
// void listCommands()
// {
// 	cout<<"List of available Commands:"<<endl
// 		<<"display            : Display the Doubly Linkled List"<<endl
// 		<<"front              : Display the first element of the List"<<endl
// 		<<"back               : Display the last element of the List"<<endl
// 		<<"addFront <elem>    : Add a new Node at the begining of the List"<<endl
// 		<<"addBack  <elem>    : Add a new Node at the end of the List"<<endl
// 		<<"removeFront        : Remove the first Node of the Linked List"<<endl
// 		<<"removeBack         : Remove the last  Node of the Linked List"<<endl	
// 		<<"empty              : Check if the List is empty?"<<endl
// 		<<"reverse            : Reverse the Linked List"<<endl
// 		<<"isPalindrome       : Check if the List is Palindrome or not?"<<endl
// 		<<"help               : Display the list of available commands"<<endl
// 		<<"exit               : Exit the Program"<<endl;
// }
//===================================
// int main()
// {
// 	DoublyLinkedList myList;
// 	listCommands();
// 	string user_input;
// 	string command;
// 	string parameter;

// 	do
// 	{
// 		cout<<">";
// 		getline(cin,user_input);

// 		// parse userinput into command and parameter(s)
// 		stringstream sstr(user_input);
// 		getline(sstr,command,' ');
// 		getline(sstr,parameter);
		
// 		try
// 		{
// 			if(command=="display") 				myList.display();
// 			else if(command=="front")			cout<<"Front = "<<myList.front()<<endl;
// 			else if(command=="back")			cout<<"Back = "<<myList.back()<<endl;
// 			else if(command=="addFront")		myList.addFront(parameter),myList.display();
// 			else if(command=="addBack")		 	myList.addBack(parameter),myList.display();
// 			else if(command=="removeFront")		myList.removeFront(),myList.display();
// 			else if(command=="removeBack")		myList.removeBack(),myList.display();
// 			else if(command=="empty")			cout<<"List is "<<(myList.empty()? "Empty":"Not Empty")<<endl;
// 			else if(command=="reverse")			myList.reverseList(),myList.display();
// 			else if(command=="isPalindrome")	cout<<"List is "<<(myList.isPalindrome()? "Palindrome":"Not Palindrome")<<endl;
// 			else if(command == "help")			listCommands();
// 			else if(command == "exit")			break;
// 			else 								cout<<"Invalid Commad !!"<<endl;
// 		}
// 		catch(exception &e)
// 		{
// 			cout<<"Exception: "<<e.what()<<endl;
// 		}

// 	}while(command!="exit" and command!="quit");

// 	return EXIT_SUCCESS;
// }
//============================================================
template<typename T>
DoublyLinkedList<T>::DoublyLinkedList ()
{
	//todo
	head = new Node<T>();
	tail = new Node<T>();
	head->next = tail;
	tail->prev = head;
	
}
//============================================================
template<typename T>
DoublyLinkedList<T>::~DoublyLinkedList () // destructor to clean up all nodes
{
	//todo
	while(!empty()){
		removeFront();
	}
	delete head;
	delete tail;
}
//============================================================
template<typename T>
bool DoublyLinkedList<T>::empty() const // is list empty?
{
	//todo
	return(head->next == tail);
}
//============================================================
template<typename T>
const T& DoublyLinkedList<T>::front() const // get front element
{
	//todo
	if(empty())
		cout<<"List is empty!";
	return(head->next->elem);
}
//============================================================
template<typename T>
const T& DoublyLinkedList<T>::back() const // get front element
{
	//todo
	if(empty())
		cout<<"List is empty!";
	return(tail->prev->elem);
}
//============================================================
template<typename T>
void DoublyLinkedList<T>::addBefore(Node<T> *ptr, const T& elem)
{
	//todo
	Node<T>* u = new Node<T>(elem);
	u->next = ptr;
	u->prev = ptr->prev;
	ptr->prev->next = u;
	ptr->prev = u;
}
//============================================================
template<typename T>
void DoublyLinkedList<T>::addFront(const T& elem) // add to front of list
{
	//todo
	Node<T>* u = new Node<T>(elem);
	u->next = head->next;
	u->prev = head;
	u->next->prev = u;
	head->next = u;
}
//============================================================
template<typename T>
void DoublyLinkedList<T>::addBack(const T& elem) // add to Back of the list
{
	//todo
	Node<T>* u = new Node<T>(elem);
	u->prev = tail->prev;
	u->next = tail;
	u->prev->next = u;
	tail->prev = u;
}
//============================================================
template<typename T>
void DoublyLinkedList<T>::removeFront() // remove front item from list
{
	//todo
	if(!empty()){
		Node<T>* u = head->next;
		u->next->prev = head;
		head -> next = u->next;
		delete u;
	}
}
//============================================================
template<typename T>
void DoublyLinkedList<T>::removeBack() // remove last item from list
{
	//todo
	if(!empty()){
		Node<T>* u = tail -> prev;
		u -> prev -> next = tail;
		tail -> prev = u->prev;
		delete u;
	}
}
// //============================================================
// template<typename T>
// void DoublyLinkedList::display() const
// {
// 	Node *curr=head;
// 	while(curr!=NULL)
// 	{
// 		cout<<"+------";
// 		curr=curr->next;
// 	}
// 	cout<<"+"<<endl<<"|";
// 	curr=head;
// 	while(curr!=NULL)
// 	{
// 		if (curr == head)		cout<<" Head |";
// 		else if(curr == tail)	cout<<" Tail |";
// 		else 					cout<<setw(5)<<curr->elem<<" |";
	
// 		curr=curr->next;
// 	}
// 	curr=head;
// 	cout<<endl;
// 	while(curr!=NULL)
// 	{
// 		cout<<"+------";
// 		curr=curr->next;
// 	}
// 	cout<<"+"<<endl;
// }
// //============================================================
// void DoublyLinkedList::reverseList()
// {
//  	//todo
//  	if(!empty()){
//  		Node* current = head;
//  		Node* t;
//  		while(current != NULL){
//  			t = current->next;
 			
//  			current->next = current->prev;
 			
//  			current->prev = t;
 			
//  			t = current;
 			
//   			current = current->prev;
//  		}
 		
//  		tail = head;
//  		head = t;
 		
//  	} 		
//  	else
//  		cout<<"List is empty"<<endl;
// }
// //============================================================
// bool DoublyLinkedList::isPalindrome()
// {
// 	//todo
// 	if(!empty()){
// 		int size = 0;
// 		Node* current = head->next;
// 		while(current != tail){
// 			size++;
// 			current = current->next;
// 		}	
		
// 		//creating 2 arrays of size of list
// 		Elem* arr1 = new Elem[size];
// 		Elem* arr2 = new Elem[size];
		
// 		//populating arrays
// 		current = head->next;
// 		for(int i=0; i<size;i++){
// 			arr1[i] = current->elem;
// 			current = current -> next;
// 		}
// 		current = tail->prev;
// 		for(int i=0;i<size;i++){
// 			arr2[i] = current->elem;
// 			current = current->prev;
// 		}
		
// 		// checking if both arrays are equal
// 		bool flag = true;
// 		for(int i =0; i<size; i++){
// 			if (arr1[i] != arr2[i]){
// 				flag = false;
// 				break;
// 			}
// 		}
		
// 		//deleting both arrays
// 		delete[] arr1;
// 		delete[] arr2;
// 		return flag;
// 	}
// 	else
// 		cout<<"List is empty"<<endl;
// }
