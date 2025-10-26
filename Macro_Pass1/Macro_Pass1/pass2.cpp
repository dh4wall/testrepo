#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#define MAX 50

class MNTEntry {
public:
    string name;
    int mdtIndex;
    int pntIndex;
    int kpdtIndex;
};

class KPDTABEntry {
public:
    string param;
    string defValue;
};

string trim(string s) {
    int i = 0;
    while (i < (int)s.size() && (s[i] == ' ' || s[i] == '\t'))
        i++;
    int j = (int)s.size() - 1;
    while (j >= 0 && (s[j] == ' ' || s[j] == '\t' || s[j] == '\r'))
        j--;
    if (i > j) return "";
    return s.substr(i, j - i + 1);
}

int split(string s, string arr[], char delim) {
    int count = 0;
    string word = "";
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == delim) {
            if (word != "") arr[count++] = trim(word);
            word = "";
        } else word += s[i];
    }
    if (word != "") arr[count++] = trim(word);
    return count;
}

int main() {
    ifstream fin("source.asm");
    if (!fin.is_open()) {
        cout << "Cannot open source.asm\n";
        return 0;
    }

    ofstream fMNT("MNT.txt"), fMDT("MDT.txt"), fPNT("PNTAB.txt"),fKPD("KPDTAB.txt"), fInter("intermediate.txt");

    MNTEntry MNT[MAX];
    string MDT[MAX];
    string PNTAB[MAX][MAX];
    KPDTABEntry KPDTAB[MAX];

    int mntc = 0, mdtc = 0, pntCount = 0, kpdtCount = 0;
    bool inMacro = false;

    string line, macroName, params;
    int kpStart = 0;

    while (getline(fin, line)) {
        string t = trim(line);
        if (t == "") continue;

        if (t == "MACRO") {
            // Header next line
            getline(fin, line);
            line = trim(line);
            stringstream ss(line);
            ss >> macroName;
            getline(ss, params);
            params = trim(params);

            MNT[mntc].name = macroName;
            MNT[mntc].mdtIndex = mdtc + 1;
            MNT[mntc].pntIndex = pntCount + 1;
            MNT[mntc].kpdtIndex = kpdtCount + 1;

            // Parse parameters
            if (params != "") {
                string arr[MAX];
                int cnt = split(params, arr, ',');
                for (int i = 0; i < cnt; i++) {
                    string arg = arr[i];
                    if (arg[0] == '&') arg = arg.substr(1);

                    int eq = arg.find('=');
                    if (eq == -1) {
                        PNTAB[mntc][pntCount] = arg;
                        pntCount++;
                    } else {
                        string pname = arg.substr(0, eq);
                        string defv = arg.substr(eq + 1);
                        PNTAB[mntc][pntCount] = pname;
                        KPDTAB[kpdtCount].param = pname;
                        KPDTAB[kpdtCount].defValue = defv;
                        kpdtCount++;
                        pntCount++;
                    }
                }
            }
            // Read body until MEND
            while (getline(fin, line)) {
                line = trim(line);
                MDT[mdtc++] = line;
                if (line == "MEND") break;
            }
            mntc++;
        } 
        else {
            fInter << line << "\n"; // Non-macro line
        }
    }

    // ----- OUTPUT FILES -----
    fMNT << "INDEX\tNAME\tMDT_PTR\tPNT_PTR\tKPDT_PTR\n";
    for (int i = 0; i < mntc; i++)
        fMNT << (i + 1) << "\t" << MNT[i].name << "\t"
             << MNT[i].mdtIndex << "\t" << MNT[i].pntIndex << "\t"
             << MNT[i].kpdtIndex << "\n";

    fMDT << "INDEX\tINSTRUCTION\n";
    for (int i = 0; i < mdtc; i++)
        fMDT << (i + 1) << "\t" << MDT[i] << "\n";

    fPNT << "MACRO\tPARAMETERS\n";
    for (int i = 0; i < mntc; i++) {
        fPNT << MNT[i].name << "\t";

        int j = MNT[i].pntIndex - 1;
        while (PNTAB[i][j] != "") {
            fPNT << PNTAB[i][j] << " ";
            j++;
        }
        fPNT << "\n";
    }

    fKPD << "INDEX\tPARAM\tDEFAULT\n";
    for (int i = 0; i < kpdtCount; i++)
        fKPD << (i + 1) << "\t" << KPDTAB[i].param << "\t" << KPDTAB[i].defValue << "\n";

    fin.close();
    fMNT.close();
    fMDT.close();
    fPNT.close();
    fKPD.close();
    fInter.close();
    return 0;
}
