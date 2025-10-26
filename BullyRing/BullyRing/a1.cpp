#include <iostream>
using namespace std;

class Bully
{
    int n;              // number of processes
    int processes[10];  // process IDs
    int coordinator;    // current coordinator

public:
    void input()
    {
        cout << "Enter number of processes: ";
        cin >> n;
        cout << "Enter process IDs: ";
        for (int i = 0; i < n; i++)
            cin >> processes[i];
        coordinator = processes[n - 1]; // assume last one is highest
    }

    void displayCoordinator()
    {
        cout << "Current Coordinator: Process " << coordinator << endl;
    }

    void startElection()
    {
        int failed;
        cout << "Enter process ID which failed: ";
        cin >> failed;

        cout << "Process " << failed << " (Coordinator) has failed!\n";

        int initiator;
        cout << "Enter the process which initiates election: ";
        cin >> initiator;

        cout << "\nElection started by Process " << initiator << endl;
        bool foundHigher = false;
        int newCoordinator = initiator;

        for (int i = 0; i < n; i++)
        {
            if (processes[i] > initiator && processes[i] != failed)
            {
                cout << "Process " << initiator << " sends message to Process " << processes[i] << endl;
                cout << "Process " << processes[i] << " responds OK\n";
                foundHigher = true;
                if (processes[i] > newCoordinator)
                    newCoordinator = processes[i];
            }
        }

        if (foundHigher)
            coordinator = newCoordinator;
        else
            coordinator = initiator;

        cout << "\nNew Coordinator is Process " << coordinator << endl;
    }
};

class Ring
{
    int n;
    int processes[10];
    int coordinator;

public:
    void input()
    {
        cout << "\nEnter number of processes: ";
        cin >> n;
        cout << "Enter process IDs (in ring order): ";
        for (int i = 0; i < n; i++)
            cin >> processes[i];
        coordinator = processes[n - 1];
    }

    void displayCoordinator()
    {
        cout << "Current Coordinator: Process " << coordinator << endl;
    }

    void startElection()
    {
        int failed;
        cout << "Enter process ID which failed: ";
        cin >> failed;
        cout << "Process " << failed << " (Coordinator) has failed!\n";

        int initiator;
        cout << "Enter the process which initiates election: ";
        cin >> initiator;

        cout << "\nElection started by Process " << initiator << endl;

        int startIndex = -1;
        for (int i = 0; i < n; i++)
        {
            if (processes[i] == initiator)
            {
                startIndex = i;
                break;
            }
        }

        int highest = initiator;
        int index = (startIndex + 1) % n;

        cout << "Message passing in ring: ";
        cout << processes[startIndex] << " -> ";
        while (processes[index] != initiator)
        {
            if (processes[index] != failed)
            {
                cout << processes[index] << " -> ";
                if (processes[index] > highest)
                    highest = processes[index];
            }
            index = (index + 1) % n;
        }
        cout << processes[startIndex] << " (back to start)" << endl;

        coordinator = highest;
        cout << "\nNew Coordinator is Process " << coordinator << endl;
    }
};

int main()
{
    Bully bully;
    cout << "=== Bully Election Algorithm ===" << endl;
    bully.input();
    bully.displayCoordinator();
    bully.startElection();
    bully.displayCoordinator();

    Ring ring;
    cout << "\n=== Ring Election Algorithm ===" << endl;
    ring.input();
    ring.displayCoordinator();
    ring.startElection();
    ring.displayCoordinator();

    return 0;
}
