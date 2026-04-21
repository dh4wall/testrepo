#include <bits/stdc++.h>
using namespace std;

class HealthcareExpert {
private:
    void toLowerCase(string& str) {
        for (char& c : str) {
            c = tolower(c);
        }
    }

public:
    void diagnose(string input) {
        toLowerCase(input);

        bool fever = input.find("fever") != string::npos;
        bool cough = input.find("cough") != string::npos;
        bool headache = input.find("headache") != string::npos;
        bool stomach = input.find("stomach") != string::npos;
        bool rash = input.find("rash") != string::npos;
        bool pain = input.find("pain") != string::npos;
        bool fatigue = input.find("fatigue") != string::npos;

        cout << "\n--- Diagnosis Report ---\n";

        if (fever && cough) {
            cout << "Possible Disease: Flu / Viral Infection\n";
            cout << "Medicine: Paracetamol, Cough Syrup\n";
            cout << "Tips: Get plenty of rest and drink warm fluids.\n";
        } 
        else if (stomach && pain) {
            cout << "Possible Disease: Food Poisoning / Gastric Issue\n";
            cout << "Medicine: ORS, Antacids\n";
            cout << "Tips: Avoid solid and spicy foods. Stay hydrated.\n";
        } 
        else if (headache && fatigue) {
            cout << "Possible Disease: Stress / Migraine\n";
            cout << "Medicine: Ibuprofen\n";
            cout << "Tips: Rest in a dark, quiet room and get enough sleep.\n";
        }
        else if (rash) {
            cout << "Possible Disease: Allergic Reaction\n";
            cout << "Medicine: Antihistamines, Calamine Lotion\n";
            cout << "Tips: Wash the area with cold water. Avoid scratching.\n";
        }
        else if (pain && fatigue) {
            cout << "Possible Disease: Muscle Fatigue / Body Ache\n";
            cout << "Medicine: Pain relievers\n";
            cout << "Tips: Take complete rest and apply a warm compress.\n";
        }
        else if (fever) {
            cout << "Possible Disease: Mild Fever\n";
            cout << "Medicine: Paracetamol\n";
            cout << "Tips: Monitor your temperature regularly.\n";
        }
        else {
            cout << "Possible Disease: Unclear / Unknown from given symptoms\n";
            cout << "Medicine: None prescribed.\n";
            cout << "Tips: Please consult a registered doctor immediately.\n";
        }
        
        cout << "------------------------\n";
    }
};

int main() {
    HealthcareExpert expert;
    string condition;

    cout << "Welcome to the Hospital Healthcare Expert System\n";
    
    while (true) {
        cout << "\nDescribe your symptoms (or type 'exit' to quit):\n> ";
        getline(cin, condition);

        string checkExit = condition;
        for (char& c : checkExit) c = tolower(c);
        if (checkExit == "exit") {
            break;
        }

        if (condition.empty()) {
            continue;
        }

        expert.diagnose(condition);
    }

    return 0;
}