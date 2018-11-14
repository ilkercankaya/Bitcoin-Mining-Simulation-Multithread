#define _VARIADIC_MAX 10
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <mutex>
#include <atomic>

using namespace std;

typedef unsigned int uint;

struct transaction {
	transaction(int randint ,thread::id threadID, uint hash,int transactionID ): rand(randint),threadID(threadID),prev(hash),current(transactionID){next=nullptr;}
	int current;//Transaction ID Read from file
	int rand; //Random Solution
	thread::id threadID; //The current transaction's solver meaning threads ID 
	uint prev; //HASH of previous node hashTransaction(prevNode);
	transaction* next; //Need this
};

/* This function hashes a transaction to an unsigned integer value. 
	It shifts curent value by 16, previous value by 8 and the random 
	value by 1 bit to the left and uses std::hash to hash. */
uint hashTransaction(transaction tran)
{
	uint result = (tran.current << 16) ^ (tran.prev << 8) ^ (tran.rand << 1);
	std::hash<uint> uint_hash;
	return uint_hash(result);
}

struct transactionChain {
	transactionChain(transaction* initTran):head(initTran){}
	~transactionChain()
	{
		transaction * temp = head;
			while(temp)
			{
				transaction * tempSaver = temp->next;
				delete temp;
				temp=tempSaver;
			}
	}
	uint getHash()
		{
			transaction * temp = head;
			while(temp->next!=nullptr) {temp=temp->next;}
			uint hash = hashTransaction(*temp);
			return hash; 
		}
	void add (transaction* current)
		{
			transaction * temp = head;
			while(temp->next!=nullptr) {temp=temp->next;}
			temp->next = current;
		}
	transaction * head;
};

bool transactionValidator(transactionChain &tChain, uint threshold)
{
	transaction *temp = tChain.head->next;
	bool valid = true;

	while (temp != nullptr)
	{
		if (hashTransaction(*temp) > threshold)
			valid = false;

		temp = temp->next;
	}
	return valid;
}
 
void mine(transactionChain &tChain, uint* transactionIDs, uint threshold, mutex& sharedMutex, int& lastAddedTransaction, bool &flag,uint AOF)
{
	uint bitcoins = 0; // A private unsigned integer for keeping track of the bitcoins mined by this thread.
	srand((hash<thread::id>()(this_thread::get_id())) ^ 2 % 10000); // We should use different seeds for each thread to find different random numbers.
	while(flag==false)
		{
			this_thread::yield();
		}
 	for(uint catcher=0;catcher<AOF; catcher++)//generates transaction until size is reached
		{
			bool answer=false;
				while(answer==false) //attacks until an answer is found
			{ 
				transaction * hashTransactiona = new transaction (rand(), std::this_thread::get_id(), tChain.getHash() ,transactionIDs[catcher]);
				uint currentValue = hashTransaction(*hashTransactiona);
					if(currentValue<threshold)//if answer is found
					{ 
							sharedMutex.lock();//mutex allowing only 1 change if both enter only 1 gains coin
							    if(catcher==lastAddedTransaction)
								{  
									    lastAddedTransaction+=1;
										tChain.add(hashTransactiona);
										bitcoins++;
										answer=true; 	
									 }
							
								sharedMutex.unlock();
								if(catcher!=lastAddedTransaction)
								{ break; }
					} 
					if(catcher!=lastAddedTransaction)
					{ break; }
					if(currentValue>threshold)
					{delete hashTransactiona;}
					
			}
		}
	
	sharedMutex.lock();
	cout << "Thread " << this_thread::get_id() << " has " << bitcoins << " bitcoin(s)" << endl;
	sharedMutex.unlock();
}


int main()
{ 
	cout << "Enter difficulty level (between 1-10):";
	uint threshold; cin>>threshold;

	while(threshold>10 ||threshold<1)//Case handling
	{
		cout << "Please enter a valid difficulty level!!!!!:";
		cin >> threshold;
	}
	threshold=1<<(32-threshold);
	cout << threshold << " is the threshold." << endl;
	cout << "Enter the filename of the input file:";
	string filename; cin >> filename;
	ifstream input;
	input.open(filename);			//Opening the file
	while(input.fail())				//Case handling
	{
		cout << "Invalid input" << endl;
		cout << "Enter the filename of the input file:";
		cin >> filename;
		input.open(filename);	
	}
	uint amountOftransaction;		
	input >> amountOftransaction;
	uint *transactionIDs = new uint [amountOftransaction]; //initiliazing heap
	int helper=0;
	while(input)				    //Setting up the array
	{
		input >> transactionIDs[helper];
		helper++;
	}
	input.close();			    	//Need to close the file
	cout << "Enter the number of miners:";
	int numberOfMiners; cin >> numberOfMiners; //Number Of Threads
	transaction *mainThread = new transaction (0,std::this_thread::get_id(),0,0);
	transactionChain tChain (mainThread);
	bool flag=false; //Race method
	int initial=0;
	thread *storage = new thread[numberOfMiners];
	mutex sharedMutex;
	for(int init=0 ; init<numberOfMiners ; init++)	
	{
		storage[init] = thread(&mine, ref(tChain), transactionIDs, threshold, ref(sharedMutex), ref(initial), ref(flag),amountOftransaction);
	}
	cout << "--------START--------" << endl;
	flag=true; //Race begins
	bool result = transactionValidator(tChain, threshold);
	for(int finAl=0 ; finAl<numberOfMiners ; finAl++)
	    {storage[finAl].join();}
	if (result == true)
		cout << "The transaction is valid." << endl;
	else
		cout << "The transaction is not valid!" << endl;
	delete [] transactionIDs;
	delete [] storage;
	return 0;
}