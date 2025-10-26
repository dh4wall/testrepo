#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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

class AssemblerPass1 {
    string opcodeTable[10][3];
    string regTable[4][2];
    Symbol symtab[20];
    Literal littab[20];
    int pooltab[10];
    int symCount, litCount, poolCount;
    int LC;

public:
    AssemblerPass1() {
        // opcode table: mnemonic | class | code
        opcodeTable[0][0] = "START"; opcodeTable[0][1] = "AD"; opcodeTable[0][2] = "01";
        opcodeTable[1][0] = "END";   opcodeTable[1][1] = "AD"; opcodeTable[1][2] = "02";
        opcodeTable[2][0] = "LTORG"; opcodeTable[2][1] = "AD"; opcodeTable[2][2] = "05";
        opcodeTable[3][0] = "ADD";   opcodeTable[3][1] = "IS"; opcodeTable[3][2] = "01";
        opcodeTable[4][0] = "SUB";   opcodeTable[4][1] = "IS"; opcodeTable[4][2] = "02";
        opcodeTable[5][0] = "MULT";  opcodeTable[5][1] = "IS"; opcodeTable[5][2] = "03";
        opcodeTable[6][0] = "MOVER"; opcodeTable[6][1] = "IS"; opcodeTable[6][2] = "04";
        opcodeTable[7][0] = "MOVEM"; opcodeTable[7][1] = "IS"; opcodeTable[7][2] = "05";
        opcodeTable[8][0] = "DS";    opcodeTable[8][1] = "DL"; opcodeTable[8][2] = "01";
        opcodeTable[9][0] = "DC";    opcodeTable[9][1] = "DL"; opcodeTable[9][2] = "02";

        // register table: name | code
        regTable[0][0] = "AREG"; regTable[0][1] = "01";
        regTable[1][0] = "BREG"; regTable[1][1] = "02";
        regTable[2][0] = "CREG"; regTable[2][1] = "03";
        regTable[3][0] = "DREG"; regTable[3][1] = "04";

        symCount = litCount = 0;
        poolCount = 1;
        pooltab[0] = 0;  // start index 0
        LC = 0;
    }

    string getOpClass(string mnemonic) {
        for (int i = 0; i < 10; i++)
            if (opcodeTable[i][0] == mnemonic)
                return opcodeTable[i][1];
        return "";
    }

    string getOpCode(string mnemonic) {
        for (int i = 0; i < 10; i++)
            if (opcodeTable[i][0] == mnemonic)
                return opcodeTable[i][2];
        return "";
    }

    string getRegCode(string reg) {
        for (int i = 0; i < 4; i++)
            if (regTable[i][0] == reg)
                return regTable[i][1];
        return "00";
    }

    bool isOpcode(string word) {
        for (int i = 0; i < 10; i++)
            if (opcodeTable[i][0] == word)
                return true;
        return false;
    }

    void addSymbol(string name, int address) {
        for (int i = 0; i < symCount; i++) {
            if (symtab[i].name == name) {
                symtab[i].address = address;
                return;
            }
        }
        symtab[symCount].name = name;
        symtab[symCount].address = address;
        symCount++;
    }

    void addLiteral(string lit) {
        for (int i = 0; i < litCount; i++)
            if (littab[i].name == lit)
                return;
        littab[litCount].name = lit;
        littab[litCount].address = -1;
        litCount++;
    }

    void assignLiterals(ofstream &out) {
        for (int i = pooltab[poolCount - 1]; i < litCount; i++) {
            if (littab[i].address == -1) {
                littab[i].address = LC;
                out << "(" << LC << ") (DL,02) (C," << littab[i].name.substr(2, littab[i].name.length() - 3) << ")\n";
                LC++;
            }
        }
        pooltab[poolCount] = litCount;
        poolCount++;
    }

    void processLine(string line, ofstream &out) {
        if (line.empty())
            return;
        string label, opcode, op1, op2;
        stringstream ss(line);
        ss >> label;

        if (isOpcode(label)) {
            opcode = label;
            label = "";
        } else {
            ss >> opcode;
        }
        ss >> op1 >> op2;

        string cls = getOpClass(opcode);
        string code = getOpCode(opcode);

        if (opcode == "START") {
            LC = stoi(op1);
            out << "(AD," << code << ") (C," << LC << ")\n";
        } else if (opcode == "MOVER" || opcode == "MOVEM" || opcode == "ADD" ||
                   opcode == "SUB" || opcode == "MULT") {
            string regCode = getRegCode(op1.substr(0, op1.length() - 1));
            if (op2[0] == '=')
                addLiteral(op2);
            else
                addSymbol(op2, -1);

            out << "(" << LC << ") (IS," << code << ") (RG," << regCode << ")";
            if (op2[0] == '=')
                out << " (L," << (litCount - 1) << ")";
            else
                out << " (S," << (symCount - 1) << ")";
            out << "\n";
            LC++;
        } else if (opcode == "DC") {
            addSymbol(label, LC);
            out << "(" << LC << ") (DL," << code << ") (C," << op1 << ")\n";
            LC++;
        } else if (opcode == "DS") {
            addSymbol(label, LC);
            out << "(" << LC << ") (DL," << code << ") (C," << op1 << ")\n";
            LC += stoi(op1);
        } else if (opcode == "LTORG") {
            out << "(AD," << code << ")\n";
            assignLiterals(out);
        } else if (opcode == "END") {
            out << "(AD," << code << ")\n";
            assignLiterals(out);
        } else {
            if (!label.empty())
                addSymbol(label, LC);
        }
    }

    void displayTables(ofstream &out) {
        out << "\n=== SYMBOL TABLE ===\n";
        out << "Index\tName\tAddress\n";
        for (int i = 0; i < symCount; i++)
            out << i << "\t" << symtab[i].name << "\t" << symtab[i].address << "\n";

        out << "\n=== LITERAL TABLE ===\n";
        out << "Index\tName\tAddress\n";
        for (int i = 0; i < litCount; i++)
            out << i << "\t" << littab[i].name << "\t" << littab[i].address << "\n";

        out << "\n=== POOL TABLE ===\n";
        out << "Pool#\tStartIndex\n";
        for (int i = 0; i < poolCount; i++)
            out << i << "\t" << pooltab[i] << "\n";
    }

    void assemble(string inputFile, string outputFile) {
        ifstream in(inputFile);
        ofstream out(outputFile);

        string line;
        while (getline(in, line)) {
            processLine(line, out);
        }

        displayTables(out);
        cout << "Pass-1 completed. Output written to " << outputFile << endl;
    }
};
int main() {
    AssemblerPass1 a;
    a.assemble("input.asm", "intermediate.txt");
    return 0;
}