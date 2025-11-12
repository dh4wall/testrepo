#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip> 

using namespace std;

struct MNTEntry {
    string name;
    int pp;          
    int kp;          
    int mdtp;       
    int kpdptr;      
    int pntabptr;    
};

int main() {
    vector<MNTEntry> MNT;
    vector<string> MDT;
    vector<pair<string, string>> KPDTAB; 
    vector<string> PNTAB;                
    
    ifstream fin("input.asm");
    if (!fin.is_open()) {
        cerr << "Error: Could not open input.asm" << endl;
        return 1;
    }
    
    ofstream f_mnt("mnt.txt");
    ofstream f_pntab("pntab.txt");
    ofstream f_kpdtab("kpdtab.txt");
    ofstream f_mdt("mdt.txt");
    
    ofstream f_ic("ic.txt"); 

    bool inMacro = false;
    MNTEntry curr;
    vector<string> currParamOrder; 

    string raw;
    while (getline(fin, raw)) {
        string line = raw;
        stringstream ss(line);
        string first;
        ss >> first;

        if (first == "MACRO") {
            inMacro = true;
            curr = {"", 0, 0, 0, 0, 0};
            currParamOrder.clear();
            continue; 
        }

        if (first == "MEND") {
            MDT.push_back("MEND");
            inMacro = false;
            MNT.push_back(curr);
            continue;
        }   
        
        if (!inMacro) {
            f_ic << raw << "\n";
            continue;
        }

        if (curr.name.empty()) {
            curr.name = first;
            curr.mdtp = MDT.size() + 1;     
            curr.pntabptr = PNTAB.size() + 1; 
            curr.kpdptr = KPDTAB.size() + 1;  

            string token;
            vector<pair<string, string>> header_kv;
            vector<string> header_pos;

            while (ss >> token) {
                if (token.empty()) continue;
                if (token[0] == '&') {
                    size_t eq = token.find('=');
                    if (eq != string::npos) {
                        string key = token.substr(0, eq);
                        string val = (eq + 1 < token.size()) ? token.substr(eq + 1) : ""; 
                        header_kv.push_back({key, val});
                        curr.kp++;
                    } else {
                        header_pos.push_back(token);
                        curr.pp++;
                    }
                }
            }

            for (auto& p : header_pos) {
                PNTAB.push_back(p);
                currParamOrder.push_back(p);
            }
            for (auto& kv : header_kv) {
                KPDTAB.push_back(kv);
                PNTAB.push_back(kv.first);
                currParamOrder.push_back(kv.first);
            }

            continue; 
        }

        vector<string> parts;
        string t;
        stringstream s2(raw); 
        while (s2 >> t) parts.push_back(t);

        string outLine;
        for (auto& tok : parts) {
            string replaced = tok;
           
            if (tok.size() && tok[0] == '&') {
                int foundIndex = -1;
                for (int i = 0; i < (int)currParamOrder.size(); ++i) {
                    if (currParamOrder[i] == tok) {
                        foundIndex = i + 1; 
                        break;
                    }
                }
                if (foundIndex != -1) {
                    replaced = "#" + to_string(foundIndex);
                }
            }
            outLine += replaced + " ";
        }
        MDT.push_back(outLine);
    }

    cout << "=== MNT ===\n";
    cout << "Name\t#PP\t#KP\tMDTP\tKPDTP\tPNTABP\n";
    f_mnt << "Name\t#PP\t#KP\tMDTP\tKPDTP\tPNTABP\n";
    for (auto& e : MNT) {
        cout << e.name << "\t" << e.pp << "\t" << e.kp << "\t" << e.mdtp << "\t" << e.kpdptr << "\t" << e.pntabptr << "\n";
        f_mnt << e.name << "\t" << e.pp << "\t" << e.kp << "\t" << e.mdtp << "\t" << e.kpdptr << "\t" << e.pntabptr << "\n";
    }

    cout << "\n=== PNTAB ===\n";
    f_pntab << "Index\tParam\n";
    for (int i = 0; i < (int)PNTAB.size(); ++i) {
        cout << (i + 1) << "\t" << PNTAB[i] << "\n";
        f_pntab << (i + 1) << "\t" << PNTAB[i] << "\n";
    }

    cout << "\n=== KPDTAB ===\n";
    f_kpdtab << "Index\tParam\tDefault\n";
    for (int i = 0; i < (int)KPDTAB.size(); ++i) {
        cout << (i + 1) << "\t" << KPDTAB[i].first << "\t" << KPDTAB[i].second << "\n";
        f_kpdtab << (i+1) << "\t" << KPDTAB[i].first << "\t" << KPDTAB[i].second << "\n";
    }

    cout << "\n=== MDT ===\n";
    for (int i = 0; i < (int)MDT.size(); ++i) {
        cout << (i + 1) << "\t" << MDT[i] << "\n";
        f_mdt << MDT[i] << "\n";
    }

    fin.close();
    f_mnt.close();
    f_pntab.close();
    f_kpdtab.close();
    f_mdt.close();
    f_ic.close();

    cout << "\nSaved: mnt.txt, pntab.txt, kpdtab.txt, mdt.txt, ic.txt\n";
    cout << "Pass 1 complete. 'ic.txt' is ready for Pass 2.\n";
    return 0;
}