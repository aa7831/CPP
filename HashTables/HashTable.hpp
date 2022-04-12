#include<iostream>
#include<list>
#include<math.h>
#include "heap.hpp"

using namespace std;

class HashNode{
private:
	string key;
	int value;
public:
	HashNode(string k, int v){
		//constructor
		key = k;
		value = v;
	}
	friend class HashTable;
};

class HashTable{
private:
	list<HashNode> *buckets; //array of list of hashnodes
	int capacity; //max capacity of array
	int size;	// size
	int collisions; //number of collisions
	int unique; //number of unique words
	int hf; //hash fucntion to be used
	Heap* heap; // heap

public:
	HashTable(int c, int f = 3){
		capacity = c;
		buckets = new list<HashNode>[capacity];
		size = 0;
		unique = 0;
		hf = f;
		collisions=0;
		heap = new Heap(capacity);
	}

	unsigned long hashCode1(string key); //hash function 1
	unsigned long hashCode2(string key); //hash function 2
	unsigned long hashCode3(string key); //hash function 3
	// unsigned long hashCode4(string key);
	void insert(string key);
	string find_freq(string key); //returns value of key, that is, the frequency of the work
	int get_collisions() {
		//returns number of total collisions
		return collisions;
	}
	int get_words(){
	 //returns number of entered words
		return size;
	}
	int get_unique(){
		//return number of unique words
		return unique;
	}

	HeapNode* getmax(){
		return heap->getMax();
	}
	~HashTable(); //destructor

};

unsigned long HashTable::hashCode1(string key){
	//summation method
	unsigned long h = 0;
	for(int i =0 ; i<key.length(); i++){
		h += int(key[i]);
	}

	return h % capacity;
}

unsigned long HashTable::hashCode2(string key){
	//polynomial method
	unsigned long h=0;
	for(int i =0; i<key.length(); i++){
		h += int(key[i]) * pow(2,i);
	}

	return h % capacity;
}

unsigned long HashTable::hashCode3(string key){
	//cyclic shift
	unsigned long h = 0;
	for(int i =0 ; i<key.length(); i++){
		//cycle shifting 5 bits to the left
		h = (h<<5) | (h>>27);
		h += int(key[i]);
	}

	return h % capacity;
}

void HashTable::insert(string key){
	//increasing size and unique count
	unique++;
	size++;
	int v;
	//getting the hash code
	unsigned long h = 0;

	if(hf == 1)
		h = hashCode1(key);
	else if (hf == 2)
		h = hashCode2(key);
	else
		h = hashCode3(key);

	//check if list is empty at that index
	if(buckets[h].empty()){
		buckets[h].push_back(HashNode(key,1));
		v = 1;
	}
	else{
		//list not empty
		collisions++;
		list<HashNode>::iterator it = buckets[h].begin();
		while(it != buckets[h].end()){
			if(it->key == key){
				if(it->value >= 1)
					unique--; //reduce unique 
				it->value++;
				v = it->value;
				collisions--;
				break;
			}
			it++;
		}
		if(it == buckets[h].end()){
			//we reached the end of bucket, didnt find the key so just insert it
			buckets[h].push_back(HashNode(key,1));
			v = 1;
		}
	}

	HeapNode* elem = new HeapNode(key,v);
	heap->insert(elem);
}

string HashTable::find_freq(string key){
	//this is the same as search, just return value of the key
	//getting the hash code
	unsigned long h = 0;
	if(hf == 1)
		h = hashCode1(key);
	else if (hf == 2)
		h = hashCode2(key);
	else
		h = hashCode3(key);

	list<HashNode>::iterator it = buckets[h].begin();
	while(it!=buckets[h].end()){
		if(it->key == key){
			return to_string(it->value);
		}
	}

	//not found
	return "Key does not exist! \n";
}

HashTable::~HashTable(){
	//destructor
	for(int i =0; i<capacity; i++){
		buckets[i].clear();
	}

	delete heap;

	delete[] buckets;
}
