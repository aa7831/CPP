#include<iostream>
#include "HashTable.hpp"
#include <fstream>
#include <vector>
#include<sstream>
#include <string>
#include<filesystem>


void displayOptions(){
	cout<<endl;
	cout<<"1- Find Frequency of a word\n2-Find word with Max Frequency\n3-Exit"<<endl;
	cout<<">";
}

void wizard(){
	vector<string> words; //vector that stores all the words
	// int size = 0;
	string path;
	string hf;
	string x;
	cout<<"Welcome to Word Count Wizard!"<<endl;
	cout<<"Please enter the full path of the input file: ";
	cin>>path;
	
	//loading data
	ifstream fin; string w;

	string line; string a; string word;
	int count = 0;
	stringstream line2;
	fin.open(path);

	while(!fin || path.substr(path.length()-3) != "txt"){
		fin.close();
		//file could not be opened
		cout<<"Could not open file!\nTry again: ";
		cin>>path;
		fin.open(path);
	}

	//file opened now

	while(fin>>w){
		word = "";
		//remove punctuation from end
		if(ispunct(w[w.length()-1])){
			w = w.substr(0, w.length()-1);
		}

		//lower case

		for(int i =0; i<w.length(); i++){
			word += tolower(w[i]);
		}

		if(word!=""){
			count++;
			words.push_back(word);
		}
	}

	fin.close();

	
	
	//asking user which hash function they want to use
	int index = 0;
	while(true){
		if(index!=0){
			cout<<"Which Hash Function would you like to use?"<<endl;
			cout<<"1-Summation\n2-Polynomial\n3-Cyclic"<<endl;
		}
		cin.clear();
		getline(cin, hf);

		if(hf.empty() && index!=0){
			cout<<"Empty Input. Choosing Default Option: Cyclic"<<endl;
			hf = "3";
		}

		if(hf == "1" || hf == "2" || hf == "3")
			break;
		else if (index != 0)
			cout<<"Invalid Input!"<<endl;

		index++;
	}

	//making hashtable of 25% more than size
	HashTable h(count * 1.25, stoi(hf));

	//populating the hash table
	for(int i =0; i<words.size(); i++){
		h.insert(words[i]);
	}


	float average = count/h.get_collisions();

	cout<<"Number of words: "<<count<<endl;
	cout<<"Number of Collisions: "<<h.get_collisions()<<endl;
	cout<<"Number of unique words: "<<h.get_unique()<<endl;
	// cout<<"Average collision per word: "<<average<<endl;

	string option; string key; string freq;
	while(true){
		displayOptions();
		cin>>option;
		if(option == "1"){
			cout<<"Which word do you want to look for? : ";
			cin>>key;
			if(key!=""){
				x = "";
				for(int i = 0; i<key.length(); i++){
					x+=tolower(key[i]);
				}
				freq = h.find_freq(x);
				cout<<"Frequency = "<<freq<<endl;
			}
			else
				cout<<"Invalid input!"<<endl;
		}

		else if(option == "2"){
			cout<<"Word = "<<h.getmax()->get_element()<<endl;
			cout<<"Frequency = "<<h.getmax()->get_key()<<endl;
		}
		else if(option == "3")
			break;
		else
			cout<<"Invalid Input!"<<endl;
	}

}

int main(){

	wizard();

	return 0;
}


