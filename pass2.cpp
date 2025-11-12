#include <bits/stdc++.h>
using namespace std;

struct MNT_E {
    string nm;
    int pp, kp, mdtp, kpdptr, pntabptr;
};

vector<MNT_E> MNT = {
    {"ONE", 2, 1, 1, 1, 1},
    {"TWO", 2, 1, 5, 2, 4}
};

vector<string> MDT = {
    "MOVER #3 #1",
    "ADD #3 #2",
    "MOVEM #3 #1",
    "MEND",
    "MOVER #3 #1",
    "ADD #3 #2",
    "MOVEM #3 #1",
    "MEND"
};

vector<pair<string,string>> KPDTAB = {
    {"&E", "AREG"},
    {"&O", "DREG"}
};

vector<string> PNTAB = {
    "&O", "&N", "&E", "&T", "&W", "&O"
};

int findmacro(string name){
    int ans=-1;
    for(int i=0;i<MNT.size();i++){
        if(MNT[i].nm==name)ans=i;
    }
    return ans;
}

void expandmacro(int idx, vector<string> args, ofstream& out) {
    int totalparams = MNT[idx].pp + MNT[idx].kp;
    vector<string> vals(totalparams); 

    int pntabidx = MNT[idx].pntabptr - 1;
    int kpdtabidx = MNT[idx].kpdptr - 1;

   
    for (int i = 0; i < MNT[idx].kp; i++) {
        string pn = KPDTAB[kpdtabidx + i].first; 
        string pv = KPDTAB[kpdtabidx + i].second; 
        
        for (int j = 0; j < totalparams; j++) {
            if (PNTAB[pntabidx + j] == pn) {
                vals[j] = pv;
                break;
            }
        }
    }

 
    for (int i = 0; i < args.size(); i++) {
        if (i < MNT[idx].pp) { 
            vals[i] = args[i];
        }
    }

    int strt = MNT[idx].mdtp;
    strt--;
    while (MDT[strt] != "MEND") {
        string line = MDT[strt];
        string op = "";
        stringstream s1(line);
        string l;
        while (s1 >> l) {
            if (l[0] == '#') {
                int id = l[1] - '0'; 
                id--;
                op += vals[id]; 
            } else {
                op += l;
            }
            op += " ";
        }
        out << op << endl;
        strt++;
    }
}

int main(){
    fstream f("ic.txt");
    string line;

    ofstream out("expandedasm.asm");
    while(getline(f,line)){
        stringstream ss(line);
        string first;
        ss>>first;

        int idx=findmacro(first);
        if(idx!=-1){
            vector<string>args;
            string line;
            while(ss>>line){
                args.push_back(line);
            }

            expandmacro(idx,args,out);

        }else{
            out<<line<<endl;
        }
    }

    return 0;
}