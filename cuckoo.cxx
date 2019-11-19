// Assignment 3: Cuckoo Hashing algorithm
// Carla Jacobsen CPSC 335 Fall 2019 2:30-3:45pm Doina Bein
// An open addressing method called Cuckoo Hashing
// INPUT: an input file containing strings of maximum 255 characters, 
// one string per line
// OUTPUT: a detailed list of where the strings are inserted.     

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>

using namespace std;

// cuckoo tables' size                                                        
const int tablesize = 17;
// combine the two 1-dimensional table into one 2-dimensional table           
string  t[tablesize][2];

// define the prime number used in calculating the hash values
const int prime = 39;

// compute the hash functions
size_t f(string, size_t);

// place a string in one of the hash tables
bool place_in_hash_tables (string);

int main() {

  // the strings to be stored in the hash tables
  string s;
  size_t i, len;
  bool placed;

  // clear the tables
  for(i=0; i< tablesize; i++) {
      t[i][0] = "";
      t[i][1] = "";
  }

  char filename[255] = "";

   // display the header
  cout << endl << "CPSC 335.01 - Programming Assignment #3: ";
  cout << "Cuckoo Hashing algorithm" << endl;
    
  // read the strings from a file
  cout << "Input the file name (no spaces)!" << endl;
  cin >> filename;

  // open the file for reading
  ifstream infile(filename);
  /* read line by line from the file */
  while ( getline(infile,s) ) {
    // place null character at the end of the line instead of <return>
    len = s.size();
    s[len-1]='\0';
    // insert the string in the cuckoo table
    placed = place_in_hash_tables(s);
    // check whether the placement was successful
    if (!placed) {
      cout << "Placement has failed" << endl;
      return -1;
    }
  }
  infile.close();
  return 0;
}


bool place_in_hash_tables (string s) {
  
  bool placed;
  size_t pos;
  int index;
  string replacer, evicted;
  
  replacer = s;

  // use a counter to detect loops; if counter >= 2*tablesize, then loop
  int counter = 0;
  
  // start with table T1
  index = 0;
  
  placed = false;

  pos = f(replacer, index);

  while((!placed ) && (counter < 2*tablesize)) {//while
    if (t[pos][index] == "") {
      cout << "String <" << replacer << "> will be placed at";
      cout  << " t[" << pos <<"][" << index << "]" << endl;
      t[pos][index] = replacer;
      placed = true;
      return placed;
    }
    else {//else
      // The entry at index <pos> in the <index> hash table is occupied so
      // it needs to be evicted and replaced by <replacer>.
      // Store in <evicted> the entry t[pos][index] and place instead
      // the string <replacer> there.
      // Now the string <evicted> needs to be placed in the other table
      cout << "String <" << replacer << "> will be placed at" << " t[" << pos;
      cout <<"][" << index << "]" << " replacing <" << t[pos][index] << ">";
      cout << endl;
      // STORE IN evicted THE STRING STORED AT
      // t[pos][index] AND STORE IN t[pos][index] THE STRING replacer
	evicted = t[pos][index];
	t[pos][index] = replacer;
      // NOW evicted CONTAINING THE EVICTED STRING NEEDS TO BE STORED 
      // IN THE OTHER TABLE

      // SET index TO INDICATE THE OTHER TABLE
	//change tables
	//cout << "index = " << index << endl;
	if(index == 0)
		{
		index = 1;
		}
	else
		{
		index = 0;
		}
      //CALCULATE IN pos THE HASH VALUE FOR evicted
	pos = f(evicted, index);
	replacer = evicted;//replacer needs to be updated so that the if statement will look at the correct string
      counter ++;
    }//else
  }//while
  return placed;
};


// oompute the hash functions
size_t f(string s, size_t index) {
  size_t po, len;
  int i, val, evicted;
  po = 1;

  len = s.size();

  if (index == 0) {
    val = s[0];
    val = val % tablesize;
    if (val < 0) val += tablesize;

    if (len == 1) 
      return val;
    
    for (i = 1; i < len; i++) 
    {
      evicted = s[i];
      po *= prime;

      po = po % tablesize;
      if (po < 0) po += tablesize;
      
      val += evicted * po;
      val = val % tablesize;

      if (val < 0) val += tablesize;
    }    
    return val;
}
  else {//else
    // CALCULATE THE SECOND 
    // HASH FUNCTION in <val>
	val = s[len - 1];//s[8]*39^0 for len 9
	val = val % tablesize;
	if (val < 0) val += tablesize;
	
	if (len == 1) 
      		return val;
	
	for (i = 1; i < len; i++) 
      		{//for
		evicted = s[len - i - 1]; 	
		po *= prime;

      		po = po % tablesize;//keep from being too big
      		if (po < 0) po += tablesize;
      
      		val += evicted * po;
		val = val % tablesize;

      		if (val < 0) val += tablesize;
      		}//for
    	return val;
  }//else
}
