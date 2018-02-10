/*
Developer: Andrew Thomas
Course: CIT284 - Advanced C++/OOP
Instructor: A. Richmond
Assignment: Project 1 - Credit Cards
Last Updated: 2/10/2018

Change Log:
			2/3  - Created Project and Main.cpp
				 - Created main() skeleton to accept command line arguments
				 - Added Help text to 0 user-provided arguments
				 - Added skeleton of other arg counts wih TODOs
				 - Created CreditCard class with TODOs
				 - Wrote acctNumber generator
				 - Overloaded Stream Insertion Operator [not working]
			2/6  - Fixed Overloaded stream insert Operator - call was not dereferenced
				 - Started VERIFY flag handling [untested]
				 - Started File Handling [untested]
				 - Added and then removed strupr calls for case insensitivity - need a different method for this
				 - Started overloaded extraction operator for reading from file [unfinished]
				 - Fixed off-by-one error in generating card number (card numbers were 17 digits)
			2/8  - Added error handling for most common error cases
				 - Adjusted overloaded >> operator. Compiles now. [UNTESTED]
			2/10 - Added write to file upon creation of an account [UNTESTED]
*/

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class CreditCard {
private: string accountNumber;
private: double maxCredit;
private: double availableCredit;


public: CreditCard(string symbol) {
	string cardNumber;

	if (symbol.compare("AE") == 0) {
		cardNumber = generateCardNumber(3);
	}
	else if (symbol.compare("V") == 0) {
		cardNumber = generateCardNumber(4);
	}
	else if (symbol.compare("MC") == 0) {
		cardNumber = generateCardNumber(5);
	}
	else if (symbol.compare("DIS") == 0) {
		cardNumber = generateCardNumber(6);
	}
	else if (symbol.compare("DINE") == 0) {
		cardNumber = generateCardNumber(7);
	}
	else {
		cout << "Invalid Symbol" << endl
			<< "Use AE, V, MC, DIS, or DINE" << endl;
		return;
	}

	this->accountNumber = cardNumber;
	char checkDigitChar = cardNumber.at(15);
	int checkDigitInt = checkDigitChar - '0';

	if (checkDigitInt <= 4) {
		maxCredit = 1000;
	}
	else {
		maxCredit = 500;
	}

	availableCredit = maxCredit;


	

}
public: CreditCard(string acctNum, double amount, string fileName) {
	/*
	TODO:
	***Search file for account number***
	If found, load account into new object
	If enough credit, deduct/add amount update and save. Display "AUTHORIZATION GRANTED"
	If not enough credit, display "AUTHORIZATION DENIED"
	If not, display "ACCOUNT NOT ON FILE"
	*/
	ifstream inFile;
	inFile.open(fileName);
	inFile >> *this;


}

private: string generateCardNumber(int firstDigit) {
	string cardNumber = "";
	cardNumber += to_string(firstDigit);
	for (int i = 0; i < 14; i++) {
		cardNumber += to_string(rand()%10);
	}
	string secondFour = cardNumber.substr(4, 4);
	string thirdFour = cardNumber.substr(8, 4);
	string lastThree = cardNumber.substr(12, 3);

	int lastDigit = (stoi(secondFour) + stoi(thirdFour) + stoi(lastThree)) % 11;

	if (lastDigit > 10) lastDigit -= 10;

	cardNumber += to_string(lastDigit);

	return cardNumber;
}

public: string getAccountNumber() {
	return accountNumber;
}

	friend ostream &operator << (ostream &out, const CreditCard& cc) {
		out << cc.accountNumber << " " << cc.availableCredit << " " << cc.maxCredit << endl;
		
		return out;
}
	friend istream &operator >> (istream &in, CreditCard& cc) {
		string line;
		while (getline(in, line)) {
			if (line.find(cc.accountNumber) != string::npos) {
				stringstream linestream;

				linestream << line;
				string accountNumber;
				string availableCredit;
				string maxCredit;
				linestream >> accountNumber >> availableCredit >> maxCredit;
				cc.accountNumber = accountNumber;
				cc.availableCredit = stof(availableCredit);
				cc.maxCredit = stof(maxCredit);
				return in;
			}
		}
		cout << "ACCOUNT NOT FOUND";
		return in;
	}


};

int main(int argc, char** argv) {
	const string FILE_NAME = "ccdb.txt";
	srand((unsigned int) time(0));

	if (argc == 1) {
		cout << "The following functions are available:" << endl;
		cout << "CREATE [SYMBOL] - creates an account of the type specified by SYMBOL " << endl;
		cout << "\tSymbols:" << endl
			<< "\tAE - American Express" << endl
			<< "\tV - Visa" << endl
			<< "\tMC - MasterCard" << endl
			<< "\tDIS - Discover" << endl
			<< "\tDINE - Diner's Club" << endl;
		cout << "VERIFICATION [ACCOUNT NUMBER] [AMOUNT] - attempts to charge or credit ACCOUNT NUMBER for AMOUNT" << endl
			<< "Use negative numbers to credit to account (i.e. process payments or reunds)" << endl;

	}

	if (argc == 3) {
		string flag = (*(argv + 1));
		if (flag.compare("CREATE") == 0) {
			string symbol = (*(argv + 2));
			CreditCard* cc = new CreditCard(symbol);
			ofstream outFile;
			outFile.open(FILE_NAME);
			if (outFile) {
				outFile << *cc;
				cout << "Created account:" << endl << *cc;
			}
			else {
				cout << "File error." << endl;
				return 1;
			}
		}
		else {
			cout << "Invalid argument: " << flag << endl;
		}
	}

	if (argc == 4) {
		string accountNumber = (*(argv + 1));
		if (accountNumber.compare("VERIFICATION") == 0) {


			double amount;

			try {
				amount = atof(*(argv + 2));
			}
			catch(...){
				cout << "AMOUNT must be only digits, decimals or a negative sign" << endl
					<< "EX: \'-29.99\'" << endl;
			}
			CreditCard *cc = new CreditCard(accountNumber, amount, FILE_NAME);
		}
	}

	if (argc == 2 || argc > 4) {
		// TODO - ERROR MESSAGE FOR INVALID ARGUMENT COUNT
	}

	_getch();
	return 0;
}