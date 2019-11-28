/*
	Sarah Hartley
	Jodie Laurenson
	Amy Gourlay
*/

#include <time.h>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

const int column = 32;
int row, lineNum;
bool stop = false;
vector<vector<int>> store(32, vector<int>(32));


/*
	Method to read in the machine code from a text file and save it in the store
	Parameters: 
	store - A 2D vector of ints
*/
void readFromFile(){
	string filePath;
	filePath = "BabyTest1-MC.txt";
	fstream readFile;
	readFile.open(filePath, ios::in);
	string currLine;
	int counter = 0;
	if (readFile.is_open()){
		while (getline(readFile, currLine)){

			for(int i=0; i<column; i++){
				int bin = currLine.at(i);
				bin = bin-48;
				store[counter][i] = bin;
			}
			counter++;
			
		}
		readFile.close();
	}
}



/*
	Method to return the operand from the PI
	Parameters:
	PI- a vector of ints containing the PI
*/
vector<int> fetchOperand(vector<int> PI){

	vector<int> operand(5);

	for (int i = 4; i>=0; i--){
		operand.at(i) = PI.at(i);
	}
	return operand;
}



/*
	Method to return the function from the PI
	Parameters:
	PI- a vector of ints containing the PI
*/
vector<int> fetchFunction(vector<int> PI){
	vector<int> function(3);
	int count = 0;

	for (int a = 15; a>=13; a--){
		function.at(count) = PI.at(a);
		count++;
	}
	return function;
}


/*
	Method to work out which instruction to execute
	Parameters:
	function- a vector of ints containing the function
*/
int decodeFunction(vector<int> function){
	string cells[8] = {"000", "001", "010", "011", "100", "101", "110", "111"}; //big endian
	string instruction;
	int instructionSet;

	for(int k = 0; k<3; k++){
		instruction += to_string(function.at(k));
	}  

	for (int j = 0; j<8; j++){
		if(cells[j].compare(instruction) == 0){
			instructionSet = j;
		}
	}
	
	return instructionSet;
}



/*
	Method to convert a binary number to decimal
	Parameters:
	reverseBinary- a vector of ints containing the binary number to convert
	size - an integer containing the size of the binary number
*/
long binaryToDecimal(vector<int> reverseBinary, int size){

	vector<int> binaryNum (size);

	int i,j;
	for(i = size-1, j = 0; i>=0; i--, j++){
		binaryNum.at(j) = reverseBinary.at(i);
	}

	long bin = binaryNum.at(0);
	for(int h = 1; h<size; h++){
		bin *= 10;
		bin += binaryNum.at(h);
	}

	long base = 1;
	long decimal = 0;

	while(bin > 0){
		decimal += (bin%10) * base;
		bin /= 10;
		base *= 2;
	}

	return decimal;
}




/*
	Method to convert a decimal number to its binary equivalent
	Parameters:
	decimal - an integer containing the decimal number
	size - an integer containing the size of the binary number
*/
vector<int> decimalToBinary(int decimal, int size){
	string tempBinary;
	vector<int> reverseBinary (size);
	vector<int> binary (size);
	while (decimal != 0){
		tempBinary = (decimal % 2 == 0 ? "0":"1") + tempBinary;
		decimal /= 2;
	}

	while (tempBinary.length() < 32){//#####################################################################
		tempBinary = "0" + tempBinary;
	}

	for(int i = 0; i<size; i++){
		reverseBinary.at(i) = tempBinary.at(i) - '0';
	}

	int i,j;
	for(i = size-1, j = 0; i>=0; i--, j++){
		binary.at(j) = reverseBinary.at(i);
	}
	return binary;
}


/*
	Method to convert a decimal number to its binary equivalent
	Parameters:
	decimal - an integer containing the decimal number
	size - an integer containing the size of the binary number
*/
vector<int> decimalToBinaryForSUB(int decimal, int size){
	string tempBinary;
	vector<int> reverseBinary (size);
	vector<int> binary (size);
	while (decimal != 0){
		tempBinary = (decimal % 2 == 0 ? "0":"1") + tempBinary;
		decimal /= 2;
	}

	while (tempBinary.length() < 31){//#####################################################################
		tempBinary = "0" + tempBinary;
	}

	for(int i = 0; i<size-1; i++){
		reverseBinary.at(i) = tempBinary.at(i) - '0';
	}

	int i,j;
	for(i = size-1, j = 0; i>=0; i--, j++){
		binary.at(j) = reverseBinary.at(i);
	}

	if(decimal<0){
		binary.at(31) = 1;
	}
	else{
		binary.at(31) = 0;
	}


	return binary;
}


/*
	
*/
vector<int> getVar(int memoryAddress){
	vector<int> variable (32);
	for (int i = 0; i <32; i++){
		variable.at(i) = store.at(memoryAddress).at(i);
	}
	return variable;
}




/*
	Method to implement the JMP instruction
	Parameters:
	CI - a vector of ints containing the line number in binary
	PI - a vector of ints containing the PI
	store - a 2D vector of ints containing the store
	memoryAddress - The  memory address to go to 
*/
void JMP(vector<int> CI, vector<int> PI, int memoryAddress){
	int line = binaryToDecimal(CI, 32);
	for(int i = 0; i<column; i++){
		PI.at(i) = store.at(line).at(memoryAddress);
	}
}


/*
	Method to implement the JRP instruction
	Parameters:
	CI - a vector of ints containing the line number in binary
	PI - a vector of ints containing the PI
	store - a 2D vector of ints containing the store
	memoryAddress - The  memory address to go to 
*/
void JRP(vector<int> CI, vector<int> PI, int memoryAddress){
	int line = binaryToDecimal(CI, 32);
	for(int i = 0; i<column; i++){
		PI.at(i) = PI.at(i) + store.at(line).at(memoryAddress);
	}
}



vector<int> negative(vector<int> variable){
	variable.at(31) = 1;
	return variable;
}


/*
	Method to implement the LDN instruction
	Parameters:
	CI - a vector of ints containing the line number in binary
	store - a 2D vector of ints containing the store
	A - a vector of ints containing the Accumulator
	memoryAddress - The  memory address to go to 
*/
vector<int> LDN(vector<int> CI, vector<int> A, int memoryAddress){

	vector<int> variable = getVar(memoryAddress);
	vector<int> temp = negative(variable);

	for(int i = 0; i<column; i++){
		A.at(i) = temp.at(i);
	}

	return A;
}



/*
	Method to implement the STO instruction
	Parameters:
	CI - a vector of ints containing the line number in binary
	store - a 2D vector of ints containing the store
	A - a vector of ints containing the Accumulator
	memoryAddress - The  memory address to go to 
*/
void STO(vector<int> CI, vector<int> A, int memoryAddress){

	for(int i = 0; i<column; i++){
		store.at(memoryAddress).at(i) = A.at(i);
	}

}



vector<int> subtract(vector<int> num2, vector<int> num1){

	int s = binaryToDecimal(num2, 31);
	int a = binaryToDecimal(num1, 31);

	if(num1.at(31) == 1){
		a=-a;
	}

	if(num2.at(31) == 1){
		s=-s;
	}

	a = a-s;

	vector<int> result (32);

	result = decimalToBinaryForSUB(a, 32);

	return result;
}

/*
	Method to implement the SUB instruction
	Parameters:
	CI - a vector of ints containing the line number in binary
	store - a 2D vector of ints containing the store
	A - a vector of ints containing the Accumulator
	memoryAddress - The  memory address to go to 
*/
vector<int> SUB(vector<int> CI, vector<int> A, int memoryAddress){
	vector<int> tempVar (32);
	tempVar = getVar(memoryAddress);

	A = subtract(tempVar, A);
	return A;
}



/*
	Method to implement the LDN instruction
	Parameters:
	
	A - a vector of ints containing the Accumulator
	PI - a vector of ints containing the PI
	CI - a vector of ints containing the line number in binary
*/
void CMP(vector<int> A, vector<int> PI, vector<int> CI){

	int num = binaryToDecimal(CI, 32);

	for(int i = 0; i<column; i++){
		if(A.at(i)<0){
			num++;
			vector<int> binary = decimalToBinary(num, 32);
			CI.at(i) = binary.at(i);
		}
	}
}



/*
	Method to implement the STP instruction
*/
void STP(){
	stop = true;
}



/*
	Method to call the appropriate function to execute the instruction
	Parameters:
	instructionSet - an integer stating which instruction to execute
	operand - a vector of ints containing the operand
	CI - a vector of ints containing the CI
	store - a 2D array containing the store
	PI - a vector of ints containing the PI
	A - a vector if ints containing the Accumulator
	stop - a boolean value stating whether or not the STP instruction has been executed
*/
vector<int> execute(int instructionSet, vector<int> operand, vector<int> CI, vector<int> PI, vector<int> A){
	
	int number = binaryToDecimal(operand, 5); 

	switch (instructionSet){
		case 0:
			JMP(CI, PI, number);
			break;
		case 1:
			JRP(CI, PI, number);
			break;
		case 2:
			A = LDN(CI, A, number);
			return A;
			break;
		case 3:
			STO(CI, A, number);
			break;
		case 4:
			A = SUB(CI, A, number);
			return A;
			break;
		case 5:
			A = SUB(CI, A, number);
			return A;
			break;
		case 6:	
			CMP(A, PI, CI);
			break;	
		case 7:
			STP();
			break;
	}
	return A;
}


/*
	Method to display the Store, CI, PI and Accumulator
	Parameters:
	store - a 2D array of vectors containing the store
	CI - a vector of ints containing the CI
	PI - a vector of ints containing the PI
	A - a vector of ints containing Accumulator
*/
void displayAll(vector<int> CI, vector<int> PI, vector<int> A){

	cout << endl;
	cout << endl;
	cout << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~Manchester Baby~~~~~~~~~~~~~~~~~~~~~~~~~" <<endl;

	cout << "CI: " << endl;

	for(int i = 0; i<column; i++){
		cout << CI.at(i);
	}

	cout << endl;
	cout << endl;

	cout << "PI: " << endl;
	for(int i = 0; i<column; i++){
		cout << PI.at(i);
	}

	cout << endl;
	cout << endl;

	cout << "Accumulator: " << endl;

	for(int i = 0; i<column; i++){
		cout << A.at(i);
	}

	cout << endl;
	cout << endl;


	cout << "Store: " << endl;

	for (int i = 0; i<32; i++){
		for (int j = 0; j<32; j++){
			if(store.at(i).at(j) == 0){
				cout << " \u25AB";
			}
			else{
				cout << " \u25AA";
			}
		}
		cout << endl;
	}

}



/*
	Main method
*/
int main(){

	vector<int> CI(32);
	vector<int> A(32);
	vector<int> PI(32);
	vector<int> binary (32);

	readFromFile();

	for(int i = 0; i<32; i++){

		binary = decimalToBinary(i,32);

		for(int i = 0; i < 32; i++){
			CI.at(i) = binary.at(i);
		}
		

		for(int j = 0; j<column; j++){
			PI.at(j) = store.at(i).at(j);
		}

		vector<int> operand = fetchOperand(PI);
		vector<int> function = fetchFunction(PI);

		int instructionSet = decodeFunction(function);
		

		A = execute(instructionSet, operand, CI, PI, A);

		displayAll(CI, PI, A);

		if (stop==true){
			return 0;
		}
		
	}
	return 0;
}
