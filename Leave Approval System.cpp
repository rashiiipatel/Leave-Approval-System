
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Leave
{
    int id;
    string name;
    string reason;
    int days;
    string status;
};

vector<Leave> leaves;
int nextId = 1;

void applyLeave()
{
    Leave l;

    l.id = nextId++;

    cout << "\nEnter your name: ";
    cin.ignore();
    getline(cin, l.name);

    cout << "Enter reason for leave: ";
    getline(cin, l.reason);

    cout << "Enter number of leave days: ";
    cin >> l.days;

    if (l.days <= 0)
    {
        cout << "Invalid number of days!\n";
        nextId--;
        return;
    }

    l.status = "Pending";

    leaves.push_back(l);

    cout << "\nLeave applied successfully!";
    cout << "\nYour Request ID is: " << l.id << endl;
}

void viewLeaves()
{
    if (leaves.empty())
    {
        cout << "\nNo leave requests found.\n";
        return;
    }

    cout << "\n---------- ALL LEAVE REQUESTS ----------\n";

    for (int i = 0; i < leaves.size(); i++)
    {
        cout << "\nRequest ID: " << leaves[i].id;
        cout << "\nName: " << leaves[i].name;
        cout << "\nReason: " << leaves[i].reason;
        cout << "\nDays: " << leaves[i].days;
        cout << "\nStatus: " << leaves[i].status;
        cout << "\n----------------------------------------\n";
    }
}

void pendingLeaves()
{
    bool found = false;

    cout << "\n---------- PENDING REQUESTS ----------\n";

    for (int i = 0; i < leaves.size(); i++)
    {
        if (leaves[i].status == "Pending")
        {
            cout << "\nID: " << leaves[i].id;
            cout << "\nName: " << leaves[i].name;
            cout << "\nReason: " << leaves[i].reason;
            cout << "\nDays: " << leaves[i].days << endl;

            found = true;
        }
    }

    if (!found)
        cout << "\nNo pending requests.\n";
}

void updateLeave()
{
    int id, choice;
    bool found = false;

    cout << "\nEnter Request ID: ";
    cin >> id;

    for (int i = 0; i < leaves.size(); i++)
    {
        if (leaves[i].id == id)
        {
            found = true;

            if (leaves[i].status != "Pending")
            {
                cout << "\nThis request has already been processed.\n";
                return;
            }

            cout << "\n1. Approve Leave";
            cout << "\n2. Reject Leave";
            cout << "\nEnter choice: ";
            cin >> choice;

            if (choice == 1)
            {
                leaves[i].status = "Approved";
                cout << "\nLeave Approved Successfully!\n";
            }
            else if (choice == 2)
            {
                leaves[i].status = "Rejected";
                cout << "\nLeave Rejected Successfully!\n";
            }
            else
            {
                cout << "\nInvalid choice!\n";
            }

            break;
        }
    }

    if (!found)
        cout << "\nRequest ID not found.\n";
}

void searchLeave()
{
    int id;
    bool found = false;

    cout << "\nEnter Request ID to search: ";
    cin >> id;

    for (int i = 0; i < leaves.size(); i++)
    {
        if (leaves[i].id == id)
        {
            cout << "\nName: " << leaves[i].name;
            cout << "\nReason: " << leaves[i].reason;
            cout << "\nDays: " << leaves[i].days;
            cout << "\nStatus: " << leaves[i].status << endl;

            found = true;
            break;
        }
    }

    if (!found)
        cout << "\nRequest not found.\n";
}

void leaveSummary()
{
    int approved = 0, rejected = 0, pending = 0;

    for (int i = 0; i < leaves.size(); i++)
    {
        if (leaves[i].status == "Approved")
            approved++;

        else if (leaves[i].status == "Rejected")
            rejected++;

        else
            pending++;
    }

    cout << "\n---------- LEAVE SUMMARY ----------";
    cout << "\nTotal Requests: " << leaves.size();
    cout << "\nApproved: " << approved;
    cout << "\nRejected: " << rejected;
    cout << "\nPending: " << pending << endl;
}

int main()
{
    int choice;

    do
    {
        cout << "\n\n================================";
        cout << "\n       LEAVE APPROVAL SYSTEM";
        cout << "\n================================";
        cout << "\n1. Apply for Leave";
        cout << "\n2. View All Requests";
        cout << "\n3. View Pending Requests";
        cout << "\n4. Approve / Reject Leave";
        cout << "\n5. Search Leave";
        cout << "\n6. Leave Summary";
        cout << "\n7. Exit";

        cout << "\n\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                applyLeave();
                break;

            case 2:
                viewLeaves();
                break;

            case 3:
                pendingLeaves();
                break;

            case 4:
                updateLeave();
                break;

            case 5:
                searchLeave();
                break;

            case 6:
                leaveSummary();
                break;

            case 7:
                cout << "\nThank you for using the system!\n";
                break;

            default:
                cout << "\nInvalid choice!\n";
        }

    } while (choice != 7);

    return 0;
}