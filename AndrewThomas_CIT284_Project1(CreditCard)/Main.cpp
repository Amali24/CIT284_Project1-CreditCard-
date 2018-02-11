/*
Developer: Andrew Thomas
Course: CIT284 - Advanced C++/OOP
Instructor: A. Richmond
Assignment: Project 1 - Credit Cards
Last Updated: 2/11/2018

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
				 - Adjusted overloaded >> operator. Compiles now. [Seems to work]
			2/10 - Added write to file upon creation of an account [Seems to work]
			2/11 - Trying to get this thing working as intended
				 - Moved String checking on symbol to main() (removed from CC constructor)
				 - Removed overloaded istream operator. Not necessary.
				 - Fixed the constructor and file searching. [UNTESTED]
				 - NEEDS LOTS AND LOTS OF TESTING
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


public: CreditCard(int firstDigit) {
	// Takes in an int, determined in main() by which symbol is provided by user

	// Generates a card number with firstDigit as the first digit
	string cardNumber = generateCardNumber(firstDigit);
	
	// Stores the cardNumber as the account number for the object
	this->accountNumber = cardNumber;
	// The check digit is the last digit of the card number
	char checkDigitChar = cardNumber.at(15);

	// Subtract the value of '0' from the check digit to convert to int
	int checkDigitInt = checkDigitChar - '0';

	// Check digit determines credit line, 4 or lower gets 1,000, above 4 gets 500
	if (checkDigitInt <= 4) {
		maxCredit = 1000;
	}
	else {
		maxCredit = 500;
	}

	// Set currently available credit to max credit
	// These valuse must be separate to disallow accounts to have available credit greater than max credit
	availableCredit = maxCredit;


	

}
public: CreditCard(string accountNumber, double maxCredit, double curCredit) {
	
	this->accountNumber = accountNumber;
	this->availableCredit = curCredit;
	this->maxCredit = maxCredit;
}
public: void processTransaction(double amount) {
	if (amount > this->availableCredit || this->availableCredit - amount > this->maxCredit) {
		cout << "AUTHORIZATION DENIED";
		return;
	}
	this->availableCredit -= amount;
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

	friend ostream &operator << (ostream &out, const CreditCard& cc) {
		out << cc.accountNumber << " " << cc.availableCredit << " " << cc.maxCredit << endl;
		
		return out;
}
};

string toUpper(string);

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

		flag = toUpper(flag);

		if (flag.compare("CREATE") == 0) {
			string symbol = (*(argv + 2));

			symbol = toUpper(symbol);

			CreditCard *cc;

			if (symbol.compare("AE") == 0) {
				cc = new CreditCard(3);
			}
			else if (symbol.compare("V") == 0) {
				cc = new CreditCard(4);
			}
			else if (symbol.compare("MC") == 0) {
				cc = new CreditCard(5);
			}
			else if (symbol.compare("DIS") == 0) {
				cc = new CreditCard(6);
			}
			else if (symbol.compare("DINE") == 0) {
				cc = new CreditCard(7);
			}
			else {
				cout << "Invalid Symbol" << endl
					<< "Use AE, V, MC, DIS, or DINE" << endl;
				_getch();
				return 1;
			}

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
			return 1;
		}
	}

	if (argc == 4) {
		string flag = (*(argv + 1));
		string accountNumber;
		double amount;

		flag = toUpper(flag);

		if (flag.compare("VERIFICATION") == 0) {

			accountNumber = (*(argv + 2));


			try {
				amount = atof(*(argv + 3));
			}
			catch(...){
				cout << "AMOUNT must be only digits, decimals or a negative sign" << endl
					<< "EX: \'-29.99\'" << endl;
			}
		}
		
		ifstream inFile(FILE_NAME);

		if (!inFile) {
			cout << "File not found. Please create an account first." << endl;
			return 1;
		}

		string line;
		while (getline(inFile, line)) {
			if (line.find(accountNumber) != string::npos) {
				istringstream iss(line);
				double maxCredit;
				double availCredit;
				iss >> accountNumber >> availCredit >> maxCredit;
				CreditCard cc(accountNumber, availCredit, maxCredit);
				cc.processTransaction(amount);
				return 0;
			}
		}

		cout << "ACCOUNT NOT ON FILE";
		return 1;

	}

	if (argc == 2 || argc > 4) {
		cout << "Invalid number of arguments." << endl;
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
		return 1;
	}

	_getch();
	return 0;
}

string toUpper(string in) {
	for (char a : in) {
		a = toupper(a);
	}
}