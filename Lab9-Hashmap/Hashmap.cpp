#include "Hashmap.h"

Hashmap::Hashmap(){
    mapSize = 0;
    for(int i = 0; i < BUCKETS; i++){
        buckets[i] = NULL;
    }
}

Hashmap::~Hashmap(){
    clear();
}

void Hashmap::insert(string key, int value){
    int &valptr = at(key);
    valptr = value;
}

bool Hashmap::contains(string key) const{
    int hasval = hash(key);
    Node *ptr = buckets[hasval];
    while(ptr != NULL) {
        if(key == ptr->key) {
            return true;
        }
        else {
            ptr = ptr->next;
        }
    }
    return false;
}

int Hashmap::get(string key) const{
    int hasval = hash(key);
    Node *ptr = buckets[hasval];
    while(ptr != NULL) {
        if(key == ptr->key) {
            return ptr->value;
        }
        else {
            ptr = ptr->next;
        }
    }
    throw invalid_argument( "invalid_argument exception" );
}

int& Hashmap::operator [](string key){
    return at(key);
}

int& Hashmap::at(string name) {
	int hashval = hash(name); // Compute the hash value for this name, for now assume it is 0
	Node *ptr = buckets[hashval];
	while(ptr != NULL) {
		//cout <<"comparing "<<name<<" first "<<ptr->key<<endl;
		if(name == ptr->key) { // We found it
		    break;
		} else {
		  //cout << "ptr"<< ptr<<"next "<< ptr->next<<endl;
		  ptr = ptr->next;
		}
	}
	if(ptr == NULL) { // We need to create a new node, put it on the front of the list
		ptr = new Node;
		ptr->key = name;
		ptr->next = NULL;
		ptr->prev = NULL;
		//cout << "new node "<< static_cast<void*>(&(ptr->key))<<endl;
	    ptr->next = buckets[hashval];
		buckets[hashval] = ptr;
		ptr->key = name;
		if(ptr->next) {
		    ptr->next->prev = ptr;
		}
		mapSize++;
	}
	//cout << "return "<< static_cast<void*>(&(ptr->key))<<endl;
	return(ptr->value);
}

bool Hashmap::remove(string key){
    int hasval = hash(key);
    Node *ptr = buckets[hasval];
    while((ptr != NULL) && (ptr->key != key)) {
        ptr = ptr->next;   
    }
    if (ptr == NULL) {
        return false;
    }
    if(ptr->next != NULL) {
        ptr->next->prev = ptr->prev;   
    }
    if(ptr->prev == NULL) {
        buckets[hasval] = ptr->next;
    }
    else {
        ptr->prev->next = ptr->next;   
    }
    delete ptr;
    mapSize--;
    return true;
}

void Hashmap::clear(){
	for(int i = 0; i < BUCKETS; i++) {
		while (buckets[i] != NULL) {
		    Node* temp = buckets[i];
		    buckets[i] = buckets[i]->next;
		    delete temp;
		}
	}
	mapSize = 0;
}

string Hashmap::toString() const{
    stringstream ss;
    for(int i = 0; i < BUCKETS; i++) {
        Node *head = buckets[i];
        if(head == NULL) {   //Has bucket was empty
            ss << "[" << i << "]" << endl;
        }
        else {    //Has bucket was not empty
            Node *current = head;
            ss << "[" << i << "]";
            while(current != NULL) {
                ss << current->key << " => " << current->value;
                if(current->next != NULL) 
                    ss << ", ";
                    current = current->next;
            }
            ss << endl;
        }
    }
    return ss.str();
}

int Hashmap::size() const{
    return mapSize;
}

string Hashmap::toSortedString() const{
  stringstream ss;
  priority_queue<Node*, vector<Node*>, NodeCompare> nodeHeap;
  for(int i = 0; i < BUCKETS; i++)
  {
    Node * ptr = buckets[i];
    while(ptr != NULL) {
        nodeHeap.push(ptr);
        ptr = ptr->next;
    }
    // Iterate through each bucket. Use nodeHeap.push to push all Node* onto heap.
  }
  while(!nodeHeap.empty())
  {
    Node* top = nodeHeap.top(); // Get top node (next node in sorted order)
    ss << top->key << " => " << top->value << endl; // Add node data to stringstream
    nodeHeap.pop(); // Pop it off
  }
  return ss.str();
}

unsigned int Hashmap::hash(string key) const{
    unsigned int hashCode = 0;
    for(int i = 0; i < key.length(); i++) {
        hashCode = hashCode * 31 + key.at(i);
    }
    return hashCode % BUCKETS;
    /*unsigned char *ptr = (unsigned char *)key.c_str();
    int sum = 0;
    for(int i = 0; i < key.size(); i++) {
        sum += ptr[i];
    }
    int hasVal = sum % sizeof(BUCKETS);
    return hasVal;*/
}
