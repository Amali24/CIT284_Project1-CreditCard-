/*
Developer: Andrew Thomas
Course: CIT284 - Advanced C++/OOP
Instructor: A. Richmond
Assignment: Project 1 - Credit Cards
Last Updated: 2/6/2018

Change Log:
			2/3 - Created Project and Main.cpp
				- Created main() skeleton to accept command line arguments
				- Added Help text to 0 user-provided arguments
				- Added skeleton of other arg counts wih TODOs
				- Created CreditCard class with TODOs
				- Wrote acctNumber generator
				- Overloaded Stream Insertion Operator [NOT WORKING]
			2/6 - Fixed Overloaded stream insert Operator - call was not dereferenced
				- Started VERIFY flag handling
*/

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::ostream;

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
		// TODO - ERROR
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
public: CreditCard(string acctNum, float amount) {
	/*
	TODO:
	***Search file for account number***
	If found, load account into new object
	If enough credit, deduct/add amount update and save. Display "AUTHORIZATION GRANTED"
	If not enough credit, display "AUTHORIZATION DENIED"
	If not, display "ACCOUNT NOT ON FILE"
	*/
}

private: string generateCardNumber(int firstDigit) {
	string cardNumber = "";
	cardNumber += to_string(firstDigit);
	for (int i = 0; i <= 14; i++) {
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
		out << "Account number: " << cc.accountNumber << endl
			<< "Available Credit: " << cc.availableCredit << endl
			<< "Credit Limit: " << cc.maxCredit << endl;
		
		return out;
}


};

int main(int argc, char** argv) {
	srand(time(0));

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
		string flag = strupr(*(argv + 1));
		if (flag.compare("CREATE") == 0) {
			string symbol = strupr(*(argv + 2));
			CreditCard* cc = new CreditCard(symbol);
			cout << *cc;
		}
		else {
			// TODO - ERROR FOR INVALID ARGUMENT
		}
	}

	if (argc == 4) {
		string accountNumber = strupr(*(argv + 1));
		if (accountNumber.compare("VERIFICATION") == 0) {
			float amount;
			try {
				amount = atof(*(argv + 2));
			}
			catch(...){
				//TODO - ERROR
			}
			CreditCard *cc = new CreditCard(accountNumber, amount);
		}
	}

	if (argc == 2 || argc > 4) {
		// TODO - ERROR MESSAGE FOR INVALID ARGUMENT COUNT
	}

	_getch();
	return 0;
}
