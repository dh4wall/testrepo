#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip> 

using namespace std;

class Symbol {
public:
    string name;
    int address;
};

class Literal { 
public:
    string name;
    int address;
};

class AssemblerPass2 {
    Symbol symtab[50];
    Literal littab[50];
    int symCount;
    int litCount;

    string pad(string s, int len) {
        stringstream ss;
        ss << setw(len) << setfill('0') << s;
        return ss.str();
    }

    string extractCode(string part) {
        size_t comma = part.find(',');
        size_t closeP = part.find(')');
        if (comma == string::npos || closeP == string::npos) {
            return "";
        }
        return part.substr(comma + 1, closeP - comma - 1);
    }

    string getOperandAddress(string part) {
        char type = part[1]; 
        int index = 0;
        try {
            index = stoi(part.substr(3, part.length() - 4)); 
        } catch (...) {
            cerr << "Error parsing index: " << part << endl;
            return "000";
        }

        if (type == 'S') {
            if (index >= symCount) {
                cerr << "Error: Invalid symbol index " << index << endl;
                return "000";
            }
            return pad(to_string(symtab[index].address), 3);
        } 
        else if (type == 'L') {
            if (index >= litCount) {
                cerr << "Error: Invalid literal index " << index << endl;
                return "000";
            }
            return pad(to_string(littab[index].address), 3);
        }
        return "000";
    }


public:
    AssemblerPass2() {
        symCount = 0;
        litCount = 0;
    }
    void loadTables() {
        ifstream symFile("sym_table.txt");
        if (!symFile) {
            cerr << "Error: Cannot open sym_table.txt" << endl;
            return;
        }

        string line;
        getline(symFile, line); 
        getline(symFile, line);

        int index;
        string name;
        int address;
        while (symFile >> index >> name >> address) {
            symtab[index].name = name;
            symtab[index].address = address;
            symCount = max(symCount, index + 1);
        }
        symFile.close();
        cout << "Symbol table loaded. " << symCount << " entries." << endl;

        ifstream litFile("lit_table.txt");
        if (!litFile) {
            cerr << "Error: Cannot open lit_table.txt" << endl;
            return;
        }

        getline(litFile, line); 
        getline(litFile, line); 

        while (litFile >> index >> name >> address) {
            littab[index].name = name;
            littab[index].address = address;
            litCount = max(litCount, index + 1);
        }
        litFile.close();
        cout << "Literal table loaded. " << litCount << " entries." << endl;
    }
    string parseIC(string line) {
        if (line.empty()) {
            return "";
        }
        string address = line.substr(1, line.find(')') - 1);
        string icPart;
        size_t firstParen = line.find('(');
        size_t firstCloseParen = line.find(')');
        if (firstCloseParen != string::npos && firstCloseParen + 2 < line.length()) {
            icPart = line.substr(firstCloseParen + 2);
        } else {
            return ""; 
        }
        
        
        stringstream ss(icPart);
        string part1, part2, part3;
        ss >> part1 >> part2 >> part3; 
        
        if (part1.empty()) {
            return "";
        }
        
        cout<<address<<endl;
        string opClass = part1.substr(1, 2); 
        string opCode = extractCode(part1);

       
        if (opClass == "AD") {
            return ""; 
        }

     
        if (opClass == "DL") {
            if (opCode == "02") { 
                return ""; 
            }
            if (opCode == "01") { 
                string value = extractCode(part2); 
                return address + ") + 00 0 " + pad(value, 3);
            }
        }

        
        if (opClass == "IS") {
            string regCode = "0";
            string operandAddr = "000";

            if (opCode == "00") { 
                return address + ") + 00 0 000";
            }
            
            if (opCode == "09" || opCode == "10") { 
                operandAddr = getOperandAddress(part2);
            } 
            else if (opCode == "07") { 
               
                regCode = extractCode(part2);
                operandAddr = getOperandAddress(part3);
            } 
            else { 
                regCode = extractCode(part2); 
                operandAddr = getOperandAddress(part3);
            }
            
            return address + ") + " + opCode + " " + regCode + " " + operandAddr;
        }

        return ""; 
    }

    void assemble() {
        cout << "Starting Pass 2..." << endl;
        loadTables();

        ifstream in("intermediate.txt");
        if (!in) {
            cerr << "Error: Cannot open intermediate.txt" << endl;
            return;
        }

        ofstream out("machine_code.txt");
        if (!out) {
            cerr << "Error: Cannot create machine_code.txt" << endl;
            return;
        }

        string line;
        while (getline(in, line)) {
            string machineCode = parseIC(line);
            if (!machineCode.empty()) {
                out << machineCode << endl;
            }
        }

        in.close();
        out.close();

        cout << "Pass-2 completed. Output written to machine_code.txt" << endl;
    }
};

int main() {
    // Make sure intermediate.txt, sym_table.txt, and lit_table.txt
    // are in the same directory.
    AssemblerPass2 a;
    a.assemble();
    return 0;
}