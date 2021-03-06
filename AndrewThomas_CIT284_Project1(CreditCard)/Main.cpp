/*
Developer: Andrew Thomas
Course: CIT284 - Advanced C++/OOP
Instructor: A. Richmond
Assignment: Project 1 - Credit Cards
Last Updated: 2/16/2018
Due Date: 2/17/2018

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
			2/15 - Changed file to open in append mode so we don't delete our database every time we create an account
				 - Working on getting account information to overwrite previous information so we don't end up using
				   initial info every single time.
			2/16 - Well, it might not be the prettiest code ever written, but it works. Finally got the file operations
				   to work. Tweaked the to upper function I wrote, and that seems to work now too. I'm willing to call
				   this done at this point.
*/

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

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
public: CreditCard(string accountNumber, double curCredit, double maxCredit) {
	
	// Three argument constructor takes relevant information from command line and from file
	this->accountNumber = accountNumber;
	this->availableCredit = curCredit;
	this->maxCredit = maxCredit;
}
public: void processTransaction(double amount) {
	// Process transaction to check against max and current credit.
	if (amount > this->availableCredit || this->availableCredit - amount > this->maxCredit) {
		// If the transaction amount is greater than available credit (positive numbers used for credit deductions)
		// Or the current credit, minus the transaction amount is less than max credit (negative numbers used for credit increases
		// But you can't "overpay" your credit card bill to have more credit than max credit

		// If either is true, deny the transaction and return
		cout << "AUTHORIZATION DENIED";
		return;
	}
	// Otherwise, subtract the amount (this leads to increasing credit for negative transactions (i.e. bill payments)
	this->availableCredit -= amount;
	cout << fixed << showpoint << setprecision(2) << "AUTHORIATION GRANTED" 
		<< (amount > 0 ? " charged " : " credited ") << "account number: " << accountNumber << " $" << amount << endl;
	cout << "Remaining credit: " << "$" << this->availableCredit;
}

		// Generates a credit card number for usage in new accounts
		// First digit is given by type of account to create(e.g.Visa, AmEx, etc)
private: string generateCardNumber(int firstDigit) {
	// Start with empty string
	string cardNumber = "";
	// Convert the firstDigit to a string and add it into the empty string
	cardNumber += to_string(firstDigit);
	
	// Add 14 random digits
	for (int i = 0; i < 14; i++) {
		cardNumber += to_string(rand()%10);
	}

	// Parse the number on where the spaces would be if printed on a card
	string secondFour = cardNumber.substr(4, 4);
	string thirdFour = cardNumber.substr(8, 4);
	string lastThree = cardNumber.substr(12, 3);

	// The lastDigit is the "check digit" and is generated by mod 11 of the sum of each "section"
	int lastDigit = (stoi(secondFour) + stoi(thirdFour) + stoi(lastThree)) % 11;

	// If the above line results in a number greater than ten, subtract ten to get a single digit
	if (lastDigit > 10) lastDigit -= 10;

	// Add the last digit as a string onto the end of the account number
	cardNumber += to_string(lastDigit);

	return cardNumber;
}
		 // Overloaded stream insertion operator for ease of printing to console or file
	friend ostream &operator << (ostream &out, const CreditCard& cc) {

		// Print account number, available credit, and max credit, in that order, separated byspaces
		out << cc.accountNumber << " " << cc.availableCredit << " " << cc.maxCredit << endl;
		
		return out;
}
};

// simple function to convert a string to uppercase to make command line arguments case insensitive
string toUpper(string&);
void printVectorToFile(vector<CreditCard>, fstream&);

int main(int argc, char** argv) {
	// String constant holds default filename
	const string FILE_NAME = "ccdb.txt";
	// Seed random number generator with the current time in milliseconds
	srand((unsigned int) time(0));


	if (argc == 1) {
		// First argument is the program name, thus 0 user-provided arguments
		// Typical action is to provide help/list of commands
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
		// If there are three arguments, the first user-provided argument should be "CREATE"
		// and the second should be the SYMBOL
		string flag = (*(argv + 1));

		// Convert 'flag' (first user-provided argument) to uppercase
		flag = toUpper(flag);

		// Holds all the cards found in file in memory
		vector<CreditCard> ccArray;

		// String to hold each line of file
		string line;

		// Open the file in input mode to read in from our "database"
		fstream dbFile(FILE_NAME, fstream::in);

		// While input is valid
		while (getline(dbFile, line)) {
			// Create a stringstream from each line
			istringstream iss(line);
			// Plus variables for the three components of a credit card
			string accountNumberFromFile;
			double maxCredit, availCredit;
			// Read into those variables
			iss >> accountNumberFromFile >> availCredit >> maxCredit;
			// Create a credit card object
			CreditCard cc(accountNumberFromFile, availCredit, maxCredit);
			// put it in the vector
			ccArray.push_back(cc);
		}

		// Clear the file object, reset pointers to 0 and close it
		dbFile.clear();
		dbFile.seekp(0);
		dbFile.seekg(0);
		dbFile.close();

		if (flag.compare("CREATE") == 0) {
			// If the flad is CREATE, we can continue

			// Put the second user argument in symbol
			string symbol = (*(argv + 2));

			// And convert to upper
			symbol = toUpper(symbol);

			// Create a new pointer to a CreditCard
			CreditCard *cc;

			// Depending on symbol, we create a CC object with the correct first digit
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
				// Otherwise we show and error and quit the program
				cout << "Invalid Symbol" << endl
					<< "Use AE, V, MC, DIS, or DINE" << endl;
				_getch();
				return 1;
			}

			// Put the credit card info into the vector
			ccArray.push_back(*cc);

			// And confirm it in the console.
			cout << "Created account:" << endl << *cc;

			// Open the file
			dbFile.open(FILE_NAME, fstream::out);

			// Write the vector to the file
			printVectorToFile(ccArray, dbFile);

		}
		else {
			// If the first argument is not CREATE, we error out and quit
			cout << "Invalid argument: " << flag << endl;
			_getch();
			return 1;
		}
	}

	if (argc == 4) {
		// If there are 4 arguments it should be in the form $programName VERIFICATION $accountNumber $transactionAmount
		
		// The first user-provided argument goes into flag
		string flag = (*(argv + 1));

		// Create variables to hold account number and transaction amount (declared here for use throughout this if block)
		string accountNumber;
		double amount;

		// Convert the flag to uppercase
		flag = toUpper(flag);

		if (flag.compare("VERIFICATION") == 0) {
			// If our first argument is VERIFICATION, let's go for it

			// Then we put the second argument into accountNumber
			accountNumber = (*(argv + 2));

			// Try/Catch block to handle any stray symbols or letters in where the amount should be
			try {
				// Try to put the amount into a double variable (after converting from string)
				amount = atof(*(argv + 3));
			}
			catch(...){
				// If there is any exception thrown, bail out
				// Tell the user to do better next time
				cout << "AMOUNT must be only digits, decimals or a negative sign" << endl
					<< "EX: \'-29.99\'" << endl;
				_getch();
			}
		}

		// If we've made it this far, we need to open the db file to pull any stored CC info
		fstream dbFile(FILE_NAME, fstream::in | fstream::out);

		if (!dbFile) {
			// If the file doesn't exist, it should be made. If it can't be, show an error.
			cout << "Unable to create file." << endl;
			_getch();
			return 1;
		}

		// String object to hold one line at a time from db file. Each CC prints on its own line
		string line;

		// Will hold the creditcard info from the file (all cards)
		vector<CreditCard> ccArray;
		// Flag to denote if account is found
		bool accountFound = false;

		// While there is valid input
		while (getline(dbFile, line)){
			// Create a stringstream for each line
			istringstream iss(line);
			// And a string for the account number found
			string accountNumberFromFile;
			// And doubles for the credit numbers
			double maxCredit, availCredit;
			// Then read them all into their variables from the string stream
			iss >> accountNumberFromFile >> availCredit >> maxCredit;
			// And create a Credit Card object using those values
			CreditCard cc(accountNumberFromFile, availCredit, maxCredit);

			// If the account number provided matches one found in the file
			if (accountNumber.compare(accountNumberFromFile) == 0) {
				// process the transaction and flip the found flag to true
				cc.processTransaction(amount);
				accountFound = true;
			}

			// Add the credit card created to the vector
			ccArray.push_back(cc);
		}
		// Clear the file, reset both pointers to 0, and close the file
		dbFile.clear();
		dbFile.seekp(0);
		dbFile.seekg(0);
		dbFile.close();

		// Re-open the file in out mode (this overwrites the contents
		dbFile.open(FILE_NAME, fstream::out);

		// Print the vector to the file
		printVectorToFile(ccArray, dbFile);

		// If the account number isn't found, say so and bail
		if (!accountFound) {
			cout << "ACCOUNT NOT ON FILE";
			_getch();
			return 1;
		}

	}

	if (argc == 2 || argc > 4) {
		// If there are 2 or more than 4 arguments, it doesn't match anything useful
		// Tell the user they messed up, and show the help
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
		_getch();
		return 1;
	}

	_getch();
	return 0;
}

string toUpper(string &in) {
	// Simple. Loop the string, capitalize each letter, store it where the original letter was.
	for (int i = 0; i < in.length(); i++) {
		in[i] = toupper(in[i]);
	}
	return in;
}

void printVectorToFile(vector<CreditCard> arr, fstream &file) {
	for (CreditCard cc : arr) {
		file << cc;
	}
}