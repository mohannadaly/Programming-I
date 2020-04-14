/*
Mohanad Ayman Aly
20180296
Group 5
*/

#include <iostream>
#include <string.h>
#include <iomanip>

using namespace std;

// List of constants used throughout the code

const int TRANSACTIONS = 10, CLIENTS = 20;      // Boundaries for numbers of clients and transactions
const int MAX_ADDRESS = 50, MIN_ADDRESS = 5;    // Boundaries for client's addresses
const int MAX_NAME = 30, MIN_NAME = 3;          // Boundaries for client's names
const int MAX_TEMP = 20;                        // The maximum length of c-strings that store choices before passing them on to loops
const int BASEBALANCE = 5000;                   // The starting balance for client's accounts
const char BREAK = '.';                         // The character used to break out of input

// List of global variables used throughout the code

int citiesCounts[CLIENTS];                      // Array containing counts of each city in "cities"
int lastClient = 0;                             // The actual number of clients entered by the client
int userMaxTransactions, userMaxClients;        // Number of clients and transactions that the user wants to enter
int uniqueCityID = 0;							// ID of last stored unique city
char cities[CLIENTS][MAX_ADDRESS];              // Array used to store the unique cities entered by the user

/////////////////////////////////////////////////////////////
// Start of Function Prototypes

//Primary Functions

void GetSizes();																									// Get the number of clients and transactions that the user wants to enter
void PrintMain(int& i);																								// Print the text for the main menu
void ClientInfo(int startID, int endID);																			// Get (name, address and transactions) when adding a new client
void GetName(char name[], int& exitName);																			// Get and check validity of names
void GetAddress(int id, int& exitAddress);																			// Get and check validity of addresses
void ClientTransactions(int id, int startID, int endID, int& exitTransaction);										// Get and check validity of transactions
void EditInfo();																									// Edit customer's info
void ClientsInCities();																								// Print number of clients in cities
void PrintBalanceOf();																								// Print customer's balance
void PrintAllBalances();																							// Print all customer names and their balances
void PrintGreatestBalance();																						// Print the greatest balance of all clients

// Supplementary Functions

int CheckString(char variable[], unsigned int MIN, unsigned int MAX);	// Check a string for characters other than letters or spaces (0 if invalid, 1 if valid, 2 if first letter is break)
int isNumeric(char variable[]);											// Check if a string contains only numbers (0 if not numeric, 1 if numeric)
int FindClient(char name[]);											// Find a client's ID (-1 if not found, id if found)
int FindCity(char city[]);												// Find a city's index in "cities" (-1 if not found, id if found)
int CharToInt(char variable[]);											// Cast an array of numbers to an integer (-1 if not numeric, integer if numeric)
void EditTransactions(int id);											// Edit client's transactions
void SetFormat(char variable[]);										// Set the first letter and each letter after a space to uppercase
void CalculateBalance(int id, char transaction, int amount);			// Calculate client's balance using their transactions and BASEBALANCE

// End of Function Prototypes
/////////////////////////////////////////////////////////////

struct Clients
{
	int tAmounts[TRANSACTIONS] = {};		// Transactions amounts
	int balance = BASEBALANCE;				// Client's total balance
	int lastTransaction = 0;                // Last transaction made by this specific client
	int noAddress = 1;						// Whether or not the client has an address
	char name[MAX_NAME] = {};				// Client's name
	char number[MAX_ADDRESS / 4] = {};		// Client's street number
	char street[MAX_ADDRESS] = {};			// Client's street name
	char neighborhood[MAX_ADDRESS] = {};	// Client's neighborhood
	char city[MAX_ADDRESS] = {};			// Client's city
	char tTypes[TRANSACTIONS] = {};			// Transactions types (W for withdraw/D for deposit)
};

Clients client[CLIENTS];

int main()
{
	int choice, i, exit = 0;			// choice : user's actual choice; i : flag to show or hide "Add new clients"; exit : flag to exit the program
	char preCHOICE[MAX_ADDRESS] = {};	// Temp variable to store user's choice
	GetSizes();							// Get the number of clients and transactions that the user wants to enter
	ClientInfo(0, userMaxClients);		// Get client's info on the first run
	PrintMain(i);						// Print the text for the main menu
	while (true) {
		cin.getline(preCHOICE, MAX_ADDRESS);		// Get user's input to preCHOICE
		if (CharToInt(preCHOICE) != -1) {			// Check if user's input is a number (Function returns -1 when parameter is not numeric)
			choice = CharToInt(preCHOICE);			// Convert user's input from char to int type
			if (choice > 5 + i) cout << "\nInvalid choice, please try again." << endl; // Check if user's choice is a valid one
			else {
				switch (choice)
				{
				case 0: {exit = 1; break; }                                  // (0) Exit
				case 1: {PrintAllBalances(); break; }                        // (1) Print All Balances
				case 2: {PrintBalanceOf(); break; }                          // (2) Print Customer's Balance
				case 3: {PrintGreatestBalance(); break; }                    // (3) Print Highest Balance
				case 4: {EditInfo(); break; }                                // (4) Change Customer's Info
				case 5: {ClientsInCities(); break; }                         // (5) Print Numbers of Clients in Cities
				case 6: {ClientInfo(lastClient, userMaxClients); break; }    // (6) Add new clients
				default: {break; }
				}
				if (exit == 1) { // Break out of the while loop if exit equals 1
					cout << "\nThank you for using our system!" << endl;
					break;
				}
				PrintMain(i);
			}
		}
		else cout << "\nPlease enter a positive integer number" << endl; // Error message for when entering anything other than a number or a negative one
	}
	return 0;
}

/////////////////////////////////////////////////////////////
// Start of Function Definitions

void GetSizes() // Get the number of clients and transactions that the user wants to enter
{
	char preCLIENTS[MAX_ADDRESS] = {}, preTRANSACTIONS[MAX_ADDRESS] = {}; // Temp variable to store user's inputs
	cout << "\nPlease enter these values before starting the program\n" << endl;
	while (true) {
		cout << "How Many Clients Would You Like To Add?" << endl;
		cin.getline(preCLIENTS, MAX_ADDRESS);
		if (CharToInt(preCLIENTS) != -1) {            // Check if user's input is a number (Function returns -1 when parameter is not numeric)
			userMaxClients = CharToInt(preCLIENTS); // Convert users input from char to int type
			if (userMaxClients > CLIENTS) {           // Check if user's input is greater than defined maximum number of clients
				cout << "\nCan't add more than " << CLIENTS << " clients." << endl;
				continue;
			}
			break;
		}
		else cout << "\nPlease enter a positive integer number" << endl;    // Error message for when entering anything other than a number or a negative one
	}
	while (true) {
		cout << "\nHow Many Transactions Would You Like To Add?" << endl;
		cin.getline(preTRANSACTIONS, MAX_ADDRESS);
		if (CharToInt(preTRANSACTIONS) != -1) {                   // Check if user's input is a number (Function returns -1 when parameter is not numeric)
			userMaxTransactions = CharToInt(preTRANSACTIONS);   // Convert users input from char to int type
			if (userMaxTransactions > TRANSACTIONS) {             // Check if user's input is greater than defined maximum number of transactions
				cout << "\nCan't add more than " << TRANSACTIONS << " transactions." << endl;
				continue;
			}
			break;
		}
		else cout << "\nPlease enter a positive integer number" << endl;    // Error message for when entering anything other than a number or a negative one
	}
}

// Print the text for the main menu (flag for whether to show option for adding new clients or not)
void PrintMain(int& i) {
	cout << "\nWelcome To Bank Management System" << endl << endl;    // Print choices that are available by default
	cout << "(0) Exit" << endl;
	cout << "(1) Print All Balances" << endl;
	cout << "(2) Print Customer's Balance" << endl;
	cout << "(3) Print Highest Balance" << endl;
	cout << "(4) Change Customer's Info" << endl;
	cout << "(5) Print Numbers of Clients in Cities" << endl;
	if (lastClient < userMaxClients) {    // If the user added a number of clients less than the number they defined
		i = 1;                          // show an option to add the rest of the clients
		cout << "(6) Add " << userMaxClients - lastClient << " new clients" << endl;
	}
	else i = 0;
	cout << "\nEnter the number for your choice: ";
}

// Get (name, address and transactions) when adding a new client (add clients from startID till endID)
void ClientInfo(int startID, int endID)
{
	int exitName = 0, exitAddress = 0, exitTransaction = 0;   // Flag for when wanting to stop adding a client or their address
	for (int i = startID; i < endID; i++) {   // i represents client's ID
		cout << "\nPlease Enter Client " << i + 1 << "'s Name:" << endl;
		GetName(client[i].name, exitName);  // Get and validate client's name
		if (exitName == 1) break;    // Break out of the for loop if the user inputs break character, which sets exitName to 1
		cout << "\nPlease Enter Their Address\n(Number, Street, Neighborhood, City)" << endl;
		GetAddress(i, exitAddress); // Get and validate client's address
		cout << "\nPlease Enter Their Transactions In The Following Format" << endl;
		cout << "\n( W for Withdraw / D for Deposit ) (Numerical Amount)" << endl;
		cout << "\n(Maximum " << userMaxTransactions << " Transactions)" << endl;
		cout << "\n(Enter '" << BREAK << " 0' to stop)" << endl;
		ClientTransactions(i, 0, userMaxTransactions, exitTransaction);  // Get and validate client's transactions
		cin.ignore();
		lastClient++;   // Increment the global variable lastClient to indicate a new client
	}                   // has been added since the last time the function has been run
}

void GetName(char name[], int& exitName)    // Get and check validity of names (the name to check, flag for entering BREAK character)
{
	char originalName[MAX_NAME] = {};	// Store original name
	strcpy(originalName, name);			//
	if (lastClient > 0) cout << "\n(Enter '" << BREAK << "' To Stop)" << endl;
	exitName = 0;   // By default, the flag to stop asking for a correct name is set to 0
	cin.getline(name, MAX_NAME);
	if (CheckString(name, MIN_NAME, MAX_NAME) == 0) { // Check if the name is too short or too long than specified boundaries
		cout << "\nInvalid or too short name, please try again." << endl;
		GetName(name, exitName);    // If the name is of incorrect length, start the function again
	}
	else if (CheckString(name, MIN_NAME, MAX_NAME) == 2) { // If BREAK character is entered
		if (lastClient < 1) {	// If the user entered less than 1 clients
			cout << "\nPlease enter info of at least 1 customer" << endl;
			GetName(name, exitName);
		}
		else {
			strcpy(name, originalName);	// Restore original name
			exitName = 1;				// Set flag to 1
		}
	}
	SetFormat(name);
}

// Get and check validity of addresses (client's ID, flag for entering BREAK character)
void GetAddress(int id, int& exitAddress)
{
	exitAddress = 0;
	char originalNumber[MAX_ADDRESS / 4] = {};					//
	char originalStreet[MAX_ADDRESS] = {};						//
	char originalNeighborhood[MAX_ADDRESS] = {};				//
	char originalCity[MAX_ADDRESS] = {};						//
	strcpy(originalNumber, client[id].number);					// Store original address
	strcpy(originalStreet, client[id].street);					//
	strcpy(originalNeighborhood, client[id].neighborhood);		//
	strcpy(originalCity, client[id].city);						//
	while (true) {
		cout << "\n(Enter '" << BREAK << ",' To Stop)" << endl;
		cin.getline(client[id].number, MAX_ADDRESS / 4, ',');
		cin.ignore();
		if (CheckString(client[id].number, MIN_ADDRESS, MAX_ADDRESS / 4) == 2) // If BREAK character is entered
		{
			if (client[id].noAddress == 0) {							// If there's an address stored
				strcpy(client[id].number, originalNumber);				//
				strcpy(client[id].street, originalStreet);				//
				strcpy(client[id].neighborhood, originalNeighborhood);	// Restore previous address
				strcpy(client[id].city, originalCity);					//
				exitAddress = 1;										// Set flag to 1
				break;
			}
			else {	// If there's no address stored
				cout << "\nYou must enter an address at least once" << endl;
				continue;
			}
		}
		else {
			cin.getline(client[id].street, MAX_ADDRESS, ',');							// If input is not BREAK character,
			cin.getline(client[id].neighborhood, MAX_ADDRESS, ',');						// ask for the rest of the address
			cin.getline(client[id].city, MAX_ADDRESS);									//
			
			if (isNumeric(client[id].number) == 0 ||									// Check if number contains anything other than
				CheckString(client[id].street, MIN_ADDRESS, MAX_ADDRESS) == 0 ||		// numbers and if the rest of the address
				CheckString(client[id].neighborhood, MIN_ADDRESS, MAX_ADDRESS) == 0 ||	// contains anything other than letters
				CheckString(client[id].city, MIN_ADDRESS, MAX_ADDRESS) == 0) {			// and spaces
				cout << "\nInvalid or too short/long address, please try again." << endl;
			}
			else {
				SetFormat(client[id].street);			// Remove the space before the specified
				SetFormat(client[id].neighborhood);		// part of the address and set the first
				SetFormat(client[id].city);				// letter of this part to uppercase
				client[id].noAddress = 0;				// Set flag for no address stored to 0
				// Count numbers of clients in cities
				int matching = 1;	// Flag for finding a match in "cities"
				if (FindCity(client[id].city) == -1) matching = 0;
				if (matching == 1) {	// If they end up matching, increase the count of city in cityCounts
					citiesCounts[FindCity(client[id].city)]++;
				}
				if (matching == 0) { // If they are not matching, store the city and initialize its count with 1
					strcpy(cities[uniqueCityID], client[id].city);
					citiesCounts[uniqueCityID] = 1;
					uniqueCityID++;
				}
				break;
			}
		}
	}
}

//	Get and check validity of transactions (client's ID, input transaction from startID till endID, flag for stopping transaction input)
void ClientTransactions(int id, int startID, int endID, int& exitTransaction)
{
	exitTransaction = 0;	// Set the flag for stopping transaction input to 0 by default
	for (int i = startID; i < endID; i++) {
		int originalAmount = client[id].tAmounts[i];	// Store original transaction type
		char originalType = client[id].tTypes[i];		// and amount
		cin >> client[id].tTypes[i] >> client[id].tAmounts[i];
		if (client[id].tTypes[i] >= 97 && client[id].tTypes[i] <= 122) client[id].tTypes[i] -= 32; // Set transaction type to be uppercase
		if (client[id].tTypes[i] == BREAK && client[id].tAmounts[i] == 0) {	// If the BREAK character is entered
			client[id].tTypes[i] = originalType;	// Restore original transaction type and amount
			client[id].tAmounts[i] = originalAmount;//
			exitTransaction = 1;					// Set flag for stopping transaction input
			break;
		}
		else if ((client[id].tTypes[i] != 'W' && client[id].tTypes[i] != 'D') || client[id].tAmounts[i] <= 0) {	// If type is anything
			cout << "\nInvalid transaction, please try again." << endl;											// other than W/D
			i--;																								// or amount is negative
			continue;
		}
		else {
			if (client[id].tTypes[i] == 'W') {	// If the client is withdrawing and the amount is greater than the balance
				if (client[id].balance >= client[id].tAmounts[i]) CalculateBalance(id, client[id].tTypes[i], client[id].tAmounts[i]);
				else {
					cout << "\nInsufficient Balance For Transaction, Please Try Again." << endl;	// If the amount is less than balance
					i--;
					continue;
				}
			}
			else CalculateBalance(id, client[id].tTypes[i], client[id].tAmounts[i]);
			client[id].lastTransaction++;	// Increment the ID of the last transaction made this specific client
		}
	}
}

void EditInfo()	// Edit customer's info
{
	int exitName = 0, exitAddress = 0, choice = 0, id = 0;	// Flags for stopping name/address input, user's choice, client's ID
	char name[MAX_NAME] = {}, preCHOICE[MAX_TEMP] = {};		// Temp variables to store client's name and user's choice
	cout << "\nWhose info would you like to edit?" << endl;	//
	cout << "\nAvailable clients:" << endl;					// Show the user the names of clients whose info they can edit
	for (int i = 0; i < lastClient; i++) cout << '-' << client[i].name << endl;
	cout << endl;
	while (true) {
		GetName(name, exitName);	// Take user's input
		id = FindClient(name);		// Fetch the client's ID
		if (id != -1 || exitName == 1) break;	// Stop asking for input if the client is found or user stopped entering a name
		else cout << "\nClient not found, please try again." << endl;
	}
	if (exitName != 1) {
		cout << "\nChange " << client[id].name << "'s Info" << endl;
		while (true) {
			cout << "\n(0) Exit" << endl;							//
			cout << "(1) Name" << endl;								//
			cout << "(2) Address" << endl;							// Print the user's choices
			cout << "(3) Transactions" << endl;						//
			cout << "(4) Balance" << endl;							//
			cout << "\nEnter the number of your choice:" << endl;
			cin.getline(preCHOICE, MAX_TEMP);
			if (CharToInt(preCHOICE) != -1) {	// If the user's choice can be converted to a number,
				choice = CharToInt(preCHOICE);	// let the choice go through to the actual choice var
				if (choice > 4) cout << "\nInvalid choice, please try again." << endl;	// If the choice is greater than the last option
				else if (choice >= 1 && choice <= 4) {	// If the choice is not (0) Exit
					switch (choice) {
					case 1: {	// Change client's name
						cout << "\nPlease enter client's new name:" << endl;
						GetName(client[id].name, exitName);	// Ask for new name
						if (exitName != 1) cout << "\nClient's current name is " << client[id].name << endl;
						else cout << "\nClient's name not changed" << endl;	// If the user stopped entering a name
						break;
					}
					case 2: {	// Change client's address
						char originalCity[MAX_ADDRESS] = {};	// Store original city for re-calculating numbers
						strcpy(originalCity, client[id].city);	// of clients in cities
						cout << "\nPlease enter " << client[id].name << "'s new address\n(Number, Street, Neighborhood, City)" << endl;
						GetAddress(id, exitAddress);
						if (exitAddress != 1) {	// As long as the user didn't stop entering an address
							int originalCityID = FindCity(originalCity);	// Find the ID of the original city
							citiesCounts[originalCityID]--;					// Decrease number of clients in it by 1							
							if (citiesCounts[originalCityID] == 0) {		// If there are no more clients from this city
								uniqueCityID--;								// Open a spot in "cities"
								int newCityID = FindCity(client[id].city);	// Find the ID of the new city
								strcpy(cities[originalCityID], cities[newCityID]);		// Replace the old city by the new one in "cities"
								cities[newCityID][0] = '\0';							// Erase the old city
								citiesCounts[originalCityID] = citiesCounts[newCityID];	// Move the number of clients of the new city
								citiesCounts[newCityID] = 0;							// Erase the number of clients from the new city's old spot
							}
							cout << "\nClient's current address is:" << endl;	// Print client's new address
							cout << endl << client[id].number << ", " << client[id].street << ", " << client[id].neighborhood << ", " << client[id].city << endl;
						}
						else {
							cout << "\nClient's address not changed" << endl;
						}
						break;
					}
					case 3: {	// Change client's transactions
						EditTransactions(id);
						break;
					}
					case 4: {	// Change client's balance
						cout << "\nPlease enter " << client[id].name << "'s new balance: " << endl;
						char preBALANCE[MAX_TEMP] = {};	// Temp variable to store client's balance
						while (true) {
							cin.getline(preBALANCE, MAX_TEMP);
							if (CharToInt(preBALANCE) != -1) {	// As long as the user's input is a positive number
								client[id].balance = CharToInt(preBALANCE);	// Convert user's input to a number and store it
								cout << endl << client[id].name << "'s current balance is " << client[id].balance << endl;
								for (int i = 0; i < userMaxTransactions; i++) {	// Reset all transactions
									client[id].tTypes[i] = '\0';
									client[id].tAmounts[i] = '\0';
									client[id].lastTransaction = 0;
								}
								cout << "\nAll transactions were reset" << endl;
								break;
							}
							else cout << "\nPlease enter a positive integer number" << endl;
						}
						break;
					}
					default: {break; }
					}
				}
				else break;	// If the choice is (0) Exit
			}
			else cout << "\nPlease enter a positive integer number" << endl;	// If the choice is not a number
		}
	}
}

void ClientsInCities()	// Print number of clients in cities
{
	char tempCHOICE[MAX_TEMP] = {};	// Temp variable to store user's choice
	while (true) {
		cout << "\n(0) Exit" << endl;									//
		cout << "(1) Numbers of clients in all cities" << endl;			// Print the user's choice
		cout << "(2) Numbers of clients in a specific city" << endl;	//
		cin.getline(tempCHOICE, MAX_TEMP, '\n');
		if (CharToInt(tempCHOICE) != -1) {	// As long as the user's input can be converted to a number
			int choice = CharToInt(tempCHOICE);	// Set the actual variable of choice to user's converted input
			if (choice > 2) cout << "\nInvalid choice, please try again" << endl;	// If the input is greater than what's available
			else {
				if (choice != 0) {	// As long as the choice is not (0) Exit
					switch (choice) {
					case 1: {	// Print numbers of clients in all cities
						for (int i = 0; cities[i][0] != '\0'; i++)
							cout << "\nThe number of clients from " << cities[i] << " is " << citiesCounts[i] << " clients";
						break;
					}
					case 2: {	// Print number of clients in a specific city
						cout << "\nAvailable cities:" << endl;											// Print all available
						for (int i = 0; cities[i][0] != '\0'; i++) cout << '-' << cities[i] << endl;	// cities to the user
						cout << "\n\nEnter a city's name: " << endl;
						int id = -1;			// ID of the city
						char city[MAX_ADDRESS];	// Name of the city
						while (true) {
							cin.getline(city, MAX_ADDRESS, '\n');
							int validCity = CheckString(city, MIN_ADDRESS, MAX_ADDRESS);	// Check that the entered city name
							if (validCity != 2) {	// As long as the input is not BREAK	// has nothing other than letters
								SetFormat(city);
								if (validCity == 0) {	// If the entered city has anything other than letters
									cout << "\nInvalid city name, please try again.\n\n(Enter '" << BREAK << "' To Stop)" << endl;
								}
								else {
									id = FindCity(city);	// Store the city's ID
									if (id == -1) {			// If the city can't be found in "cities"
										cout << "\nCity not found, please try again.\n\n(Enter '" << BREAK << "' To Stop)" << endl;
									}
									else {					// If the city is found in "cities"
										cout << "\nThe number of clients from " << cities[id] << " is " << citiesCounts[id] << " clients" << endl;
										break;
									}
								}
							}
							else break;	// If the first letter of the entered city is BREAK character
						}
						break;
					}
					}
				}
				else break;	// If the choice is (0) Exit
			}
		}
		else cout << "\nPlease enter a positive integer number" << endl;	// If the user's choice can't be converted to a number
		cout << endl;
	}
}

void PrintBalanceOf()	// Print customer's balance
{
	cout << "\nWhose balance would you like to see?" << endl;
	cout << "\nAvailable clients:" << endl;	// Print names of available clients to choose from
	for (int i = 0; i < lastClient; i++) cout << '-' << client[i].name << endl;
	cout << endl;
	cout << "\n(Type '" << BREAK << "' To Stop)" << endl;
	int id = 0;	// ID of the chosen client
	char name[MAX_NAME] = {};	// Name of the chosen client
	while (true) {
		cin.getline(name, MAX_NAME, '\n');
		if (CheckString(name, MIN_NAME, MAX_NAME) == 0) {	// If the entered name has anything other than letters
			cout << "\nInvalid name, please try again." << endl;
			cout << "\n(Type '" << BREAK << "' To Stop)" << endl;
		}
		else if (CheckString(name, MIN_NAME, MAX_NAME) == 1) {	// As long as the first character of the entered name is not BREAK character
			SetFormat(name);
			id = FindClient(name);	// Find the ID of this client
			if (id == -1) {	// If the client can't be found
				cout << "\nClient Not Found. Please Try Again." << endl;
				cout << "\n(Type '" << BREAK << "' To Stop)" << endl;
			}
			else {
				cout << endl << client[id].name << "'s balance is " << client[id].balance << endl;
				break;
			}
		}
		else break;	// If the first character of the entered name is BREAK character
	}
}

void PrintAllBalances()	// Print all customer names and their balances
{
	cout << endl << setw(MAX_NAME) << "Name" << setw(MAX_NAME) << "Balance" << endl;
	for (int i = 0; i < lastClient; i++)
		cout << endl << setw(MAX_NAME) << client[i].name << setw(MAX_NAME) << client[i].balance << endl;
}

void PrintGreatestBalance()	// Print the greatest balance of all clients
{
	int id = 0, maxBalance = client[id].balance;	// ID of the client with greatest balance, Current highest balance
	for (int i = 0; i < lastClient; i++) {	// Go over balances of all clients
		if (client[i].balance > maxBalance) {	// If current client's balance is greatest than current highest balance:
			maxBalance = client[i].balance;		// -Set current highest balance to current client's balance
			id = i;								// -Store current client's ID
		}
	}
	cout << "\nThe greatest balance is " << client[id].name << "'s account with a balance of " << maxBalance << endl;
}

// Check a string for characters other than letters or spaces (string's variable, its minimum length, its maximum length)
int CheckString(char variable[], unsigned int MIN, unsigned int MAX) {
	SetFormat(variable);	// Set format of variable to remove space at the begining of the string
	if (variable[0] == BREAK) return 2;	// Return 2 if the first character is BREAK character
	else {
		int notFaulty = 1, correctSize = 1;	// Flags for not having characters other than letters and not being of wrong sizes
		for (int i = 0; variable[i] != '\0'; i++) {
			bool notUpperCase = (variable[i] < 65 || variable[i] > 90);		// Check for anything other than uppercase letters
			bool notLowerCase = (variable[i] < 97 || variable[i] > 122);	// Check for anything other than lowercase letters
			bool notSpace = (variable[i] != ' ');							// Check for anything other than space
			if (notLowerCase && notUpperCase && notSpace) {
				return 0;
			}
		}
		if (strlen(variable) < MIN || strlen(variable) > MAX) return 0;		// Check if variable's length is less than given minimum
		return 1;															// or higher than given maximum
	}
}

int isNumeric(char variable[])	// Check if a string contains only numbers (string's variable)
{
	int numeric = 1;	// Flag for whether the current character is a number
	for (int i = 0; variable[i] != '\0'; i++) {
		if (variable[i] < 48 || variable[i] > 57) numeric = 0;	// If the ASCII of current character doesn't correspond to a number
	}
	return numeric;	// Return the value of the flag
}

int FindClient(char name[])	// Find a client's ID (name's variable)
{
	for (int i = 0; i < lastClient; i++) {
		if (strcmp(name, client[i].name) == 0) return i;	// Return the index of the client if the name is matching
	}
	return -1;
}

int FindCity(char city[])	// Find a city's index in "cities"(city's variable)
{
	for (int i = 0; i < userMaxClients; i++) {
		if (strcmp(city, cities[i]) == 0) return i;		// Return the index of the city if they're matching
	}
	return -1;
}

int CharToInt(char variable[])	// Cast an array of numbers to an integer(Numbers' array)
{
	int num = 0, power = 1, length = strlen(variable);	// Converted value, Length of the array of numbers
	if (isNumeric(variable) == 0 || length < 1) return -1;	// If the array isn't numbers only or if it's length is less than 1, return -1
	for (int i = 0; i < length; i++) {
		power = 1;
		for (int j = 0; j < length - (i + 1); j++) {	// Get the multiplier of the current place
			power *= 10;
		}
		num += (variable[i] - '0') * power;	// Add current's place value
	}
	return num;	// Return the converted value
}

void EditTransactions(int id)	// Edit client's transactions (Client's ID)
{
	int choice = 0, offset = 1;	// User's choice, Offset of displayed option numbers
	int maximumChoice = client[id].lastTransaction + offset;	// Maximum and Minimum numbers user's choice can be
	int exitTransaction = 0;	// Flag to stop entering transactions
	char preCHOICE[MAX_TEMP] = {};	// Temp variable to store user's choice
	cout << endl << client[id].name << "'s Transactions" << endl;
	cout << "\n(0) Exit" << endl;
	for (int i = 0; i < client[id].tTypes[i] != '\0'; i++)	// Print all client's transactions
		cout << '(' << i + 1 << ") " << client[id].tTypes[i] << ' ' << client[id].tAmounts[i] << endl;
	cout << '(' << client[id].lastTransaction + offset << ") Change All Transactions" << endl;	// Option to change all client's transactions
	if (client[id].lastTransaction < userMaxTransactions) {	// If the client hasn't used up all their transactions, show an option to add to them
		maximumChoice = client[id].lastTransaction + (offset + 1);	// Extend the maximum choice the user can enter by 1
		cout << '(' << maximumChoice << ") Add New Transactions" << endl;
	}
	cout << "\nEnter the number of your choice:" << endl;
	while (true) {
		cin.getline(preCHOICE, MAX_TEMP);
		if (CharToInt(preCHOICE) != -1) {	// If user's choice can be converted to a number
			choice = CharToInt(preCHOICE);	// Convert and store it in the actual choice variable
			if (choice > maximumChoice) {	// If the choice exceeds the maximum choice
				cout << "\nInvalid choice. Please try again." << endl;
				continue;
			}
			else {
				if (choice == 0) cout << "\nNo changes were made" << endl;	// If the user chooses to Exit
				else if (choice == client[id].lastTransaction + offset) {	// If the user chooses to edit all transactions
					for (int i = 0; i < client[id].lastTransaction; i++) {	// Clear all previous transactions
						client[id].tTypes[i] = '\0';
						client[id].tAmounts[i] = '\0';
					}
					client[id].lastTransaction = 0;	// Reset client's transactions counter
					cout << "\n All previous transactions were cleared" << endl;
					cout << "\nRe-Enter all transactions" << endl;
					cout << "\n(Maximum " << userMaxTransactions << " Transactions)" << endl;
					client[id].balance = BASEBALANCE;	// Reset client's balance
					ClientTransactions(id, 0, userMaxTransactions, exitTransaction);	//	 Ask for new transactions
					cin.ignore();
					cout << "\nClient's current balance is " << client[id].balance << endl;	// Print client's new balance
				}
				else if (choice == client[id].lastTransaction + (offset + 1)) {	// If the user chooses add more transactions
					cout << "\nEnter Transactions. Maximum " << userMaxTransactions - client[id].lastTransaction << " Transactions" << endl;
					ClientTransactions(id, client[id].lastTransaction, userMaxTransactions, exitTransaction);	// Ask for transactions
					cin.ignore();
					cout << "\nClient's current balance is " << client[id].balance << endl;	// Print client's new balance
				}
				else {
					cout << "\nEnter the new transaction:" << endl;	// If the user chooses to edit a specific transaction
					int originalAmount = client[id].tAmounts[choice - 1];	// Store the original transaction amount
					char originalType = client[id].tTypes[choice - 1];		// Store the original transaction type
					ClientTransactions(id, choice - 1, choice, exitTransaction);	// Ask for replacement transaction
					cin.ignore();
					if (exitTransaction == 1) {	// If the user stopped entering the transaction
						cout << "\nTransaction not changed" << endl;
					}
					else {
						if (originalType == 'W') client[id].balance += originalAmount;	// Reverse the original transaction
						else client[id].balance -= originalAmount;
						cout << "\nClient's current balance is " << client[id].balance << endl;	// Print client's new balance
					}
				}
			}
			break;
		}
		else cout << "\nPlease enter a positive integer number" << endl;	// If user's choice can't be converted to a number or is negative
	}
}

void SetFormat(char variable[])	// Set the first letter and each letter after a space to uppercase (string's varible)
{
	for (unsigned int i = 0; i < strlen(variable); i++) {	// Remove all spaces at the begining of the string
		if (variable[i] == ' ') {
			for (unsigned int j = 0; j < strlen(variable); j++) variable[j] = variable[j + 1];
			i--;
		}
		else break;
	}
	for (int i = strlen(variable)-1; i > 0; i--) {	// Remove all spaces at the end of the string
		if (variable[i] == ' ') {
			variable[i] = '\0';
		}
		else break;
	}
	int start = 0;	// The start of a new word
	for (int i = 0; variable[i] != '\0'; i++) {
		if (variable[start] >= 97 && variable[start] <= 122) variable[start] -= 32;	// If the start of a word is lowercase, set it to uppercase
		if (variable[i] != ' ') {
			if (variable[i] >= 65 && variable[i] <= 90) variable[i] += 32;	// If the rest of a word is uppercase, set it to lowercase
		}
		else start = i + 1;	// If the current character is a space, skip to the next letter
	}
}

// Calculate client's balance using their transactions and BASEBALANCE (client's ID, transaction type, transaction amount)
void CalculateBalance(int id, char transaction, int amount)
{
	if (transaction == 'W' || transaction == 'w') client[id].balance -= amount;	// If transaction type is W (Withdraw), deduct the amount
	else client[id].balance += amount;											// Otherwise, add the amount
}
// End of Function Definitions
/////////////////////////////////////////////////////////////