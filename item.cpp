#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip> 

using namespace std;

string item = "items.csv";

ofstream openOutputFile(const string filename, const string mode = "a") {
    ofstream file;

    if (mode == "a") {
        file.open(filename, ios::app);
    }
    else {
        file.open(filename, ios::out);
    }

    if (!file.is_open()) {
        cerr << "Couldn't open the file: " << filename << endl;
    }

    return file;
}

ifstream openInputFile(string filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Couldn't open the file: " << filename << endl;
    }

    return file;
}

vector<string> separateSentence(const string& sentence) {
    vector<string> words;
    istringstream iss(sentence);
    string word;

    while (getline(iss, word, ',')) {
        words.push_back(word);
    }
    return words;
}

#include <iomanip>  // Include the <iomanip> header for setw()

// ...

void readFromFile(ifstream& file) {
    string line;
    vector<string> itemNames;

    int maxItemNameLength = 0;  // Track the maximum length of item names

    while (getline(file, line)) {
        vector<string> words = separateSentence(line);
        itemNames.push_back(words[1]);
        maxItemNameLength = max(maxItemNameLength, static_cast<int>(words[1].length()));
    }

    sort(itemNames.begin(), itemNames.end());

    for (const string& itemName : itemNames) {
        file.clear();  // Reset file stream state to read from the beginning
        file.seekg(0, ios::beg);

        while (getline(file, line)) {
            vector<string> words = separateSentence(line);
            if (words[1] == itemName) {
                cout << "Item ID: " << setw(8) << left << words[0]
                     << "Item Name: " << setw(maxItemNameLength + 4) << left << words[1]
                     << "Quantity: " << setw(8) << left << words[2]
                     << "Reg Date: " << words[3] << endl;
            }
        }
    }

    file.close();
}



string findItemByName(const string& item_name) {
    ifstream inputFile = openInputFile(item);
    string line;
    string item_found = "";

    string upper_item_name = item_name;
    for (char& c : upper_item_name) {
        c = toupper(c);
    }

    while (getline(inputFile, line)) {
        vector<string> words = separateSentence(line);
        string upper_word = words[1];
        for (char& c : upper_word) {
            c = toupper(c);
        }
        if (upper_word == upper_item_name) {
            item_found += line;
        }
    }

    return item_found;
}

bool isValidNumber(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool isValidDate(const string& str) {
    if (str.length() != 10) {
        return false;
    }

    for (char c : str) {
        if (!isdigit(c) && c != '-') {
            return false;
        }
    }
    return true;
}

void addItem(const string& item_id, const string& item_name, const string& item_quantity, const string& item_registration_date) {
    cout << "Creating item: " << item_name << endl;
    if (findItemByName(item_name) != "") {
        cout << "Item already exists!" << endl;
    }
    else {
        if (!isValidNumber(item_id)) {
            cout << "Invalid item ID!" << endl;
            return;
        }
        if (item_name.empty()) {
            cout << "Item name is required!" << endl;
            return;
        }
        if (!isValidNumber(item_quantity)) {
            cout << "Invalid item quantity!" << endl;
            return;
        }
        if (!isValidDate(item_registration_date)) {
            cout << "Invalid registration date!" << endl;
            return;
        }

        ofstream item_file = openOutputFile(item);
        item_file << item_id + "," + item_name + "," + item_quantity + "," + item_registration_date << endl;
        cout << "Item saved successfully!" << endl;
    }
}

void listItems() {
    cout << "Loading ........" << endl;
    ifstream itemFile = openInputFile(item);
    readFromFile(itemFile);
}

void chooseAction() {
    while (true) {
        cout << endl;
        cout << "Console > ";

        string request;
        getline(cin, request);

        for (size_t i = 0; i < request.length(); i++) {
            request[i] = toupper(request[i]);
        }
        istringstream iss(request);
        string category;
        iss >> category;

        if (category == "exit") {
            cout << "-----------------------------------------------" << endl;
            cout << "*                SYSTEM CLOSED                *" << endl;
            cout << "*                BYE BYE !!!!                 *" << endl;
            cout << "-----------------------------------------------" << endl;
            break;
        }
        else if (category == "itemslist") {
            listItems();
        }
        else if (category == "itemadd") {
            string id;
            iss >> id;
            string name;
            iss >> name;
            string quantity;
            iss >> quantity;
            string date;
            iss >> date;
            addItem(id, name, quantity, date);
        }
        else if (category == "help") {
            cout << endl;
            cout << "------------------------------------------------------------------" << endl;
            cout << "*                        Commands syntaxes                       *" << endl;
            cout << "------------------------------------------------------------------" << endl;
            cout << "      Itemadd <item_id> <item_name> <quantity> <registration_date>" << endl;
            cout << "      Itemslist" << endl;
            cout << "      Exit" << endl;
            cout << "------------------------------------------------------------------" << endl;
        }
        else {
            cout << "Invalid Command" << endl;
        }
    }
}

int main() {
    cout << "------------------------------------------------------------------" << endl;
    cout << "*                                Welcome!                        *" << endl;
    cout << "*                       RCA Inventory Management System          *" << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "If you need help, enter 'help' command" << endl;
    

    chooseAction();

    return 0;
}
