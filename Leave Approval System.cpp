#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

struct Employee
{
    string id;
    string name;
    string password;
};

struct Leave
{
    int requestId;
    string employeeId;
    string employeeName;
    string leaveType;
    string startDate;
    string endDate;
    int days;
    string reason;
    string status;
    string remark;
};

class LeaveSystem
{
private:

    vector<Employee> employees;
    vector<Leave> leaves;

    int nextRequestId;

    string adminUsername = "admin";
    string adminPassword = "admin123";

    const int casualLimit = 12;
    const int sickLimit = 10;
    const int emergencyLimit = 5;
    const int earnedLimit = 15;

public:

    LeaveSystem()
    {
        nextRequestId = 1;

        loadEmployees();
        loadLeaves();
    }

    void loadEmployees()
    {
        ifstream file("employees.txt");

        Employee e;

        while (file >> quoted(e.id)
                    >> quoted(e.name)
                    >> quoted(e.password))
        {
            employees.push_back(e);
        }

        file.close();
    }

    void loadLeaves()
    {
        ifstream file("leaves.txt");

        Leave l;

        while (file >> l.requestId
                    >> quoted(l.employeeId)
                    >> quoted(l.employeeName)
                    >> quoted(l.leaveType)
                    >> quoted(l.startDate)
                    >> quoted(l.endDate)
                    >> l.days
                    >> quoted(l.reason)
                    >> quoted(l.status)
                    >> quoted(l.remark))
        {
            leaves.push_back(l);

            if (l.requestId >= nextRequestId)
            {
                nextRequestId = l.requestId + 1;
            }
        }

        file.close();
    }

    void saveEmployees()
    {
        ofstream file("employees.txt");

        for (int i = 0; i < employees.size(); i++)
        {
            file << quoted(employees[i].id) << " "
                 << quoted(employees[i].name) << " "
                 << quoted(employees[i].password) << endl;
        }

        file.close();
    }

    void saveLeaves()
    {
        ofstream file("leaves.txt");

        for (int i = 0; i < leaves.size(); i++)
        {
            file << leaves[i].requestId << " "
                 << quoted(leaves[i].employeeId) << " "
                 << quoted(leaves[i].employeeName) << " "
                 << quoted(leaves[i].leaveType) << " "
                 << quoted(leaves[i].startDate) << " "
                 << quoted(leaves[i].endDate) << " "
                 << leaves[i].days << " "
                 << quoted(leaves[i].reason) << " "
                 << quoted(leaves[i].status) << " "
                 << quoted(leaves[i].remark) << endl;
        }

        file.close();
    }

    int findEmployee(string id)
    {
        for (int i = 0; i < employees.size(); i++)
        {
            if (employees[i].id == id)
            {
                return i;
            }
        }

        return -1;
    }

    void registerEmployee()
    {
        Employee e;

        cout << "\n========== EMPLOYEE REGISTRATION ==========\n";

        cout << "Enter Employee ID: ";
        cin >> e.id;

        if (findEmployee(e.id) != -1)
        {
            cout << "\nEmployee ID already exists!\n";
            return;
        }

        cout << "Enter Full Name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, e.name);

        cout << "Create Password: ";
        getline(cin, e.password);

        if (e.name.empty() || e.password.empty())
        {
            cout << "\nName and password cannot be empty!\n";
            return;
        }

        employees.push_back(e);

        saveEmployees();

        cout << "\nRegistration Successful!\n";
        cout << "You can now login using your Employee ID.\n";
    }

    void employeeLogin()
    {
        string id, password;

        cout << "\n========== EMPLOYEE LOGIN ==========\n";

        cout << "Employee ID: ";
        cin >> id;

        cout << "Password: ";
        cin >> password;

        int index = findEmployee(id);

        if (index == -1 || employees[index].password != password)
        {
            cout << "\nInvalid Employee ID or Password!\n";
            return;
        }

        cout << "\nWelcome, " << employees[index].name << "!\n";

        employeeMenu(index);
    }

    void adminLogin()
    {
        string username, password;

        cout << "\n========== ADMIN LOGIN ==========\n";

        cout << "Username: ";
        cin >> username;

        cout << "Password: ";
        cin >> password;

        if (username == adminUsername &&
            password == adminPassword)
        {
            cout << "\nAdmin Login Successful!\n";

            adminMenu();
        }
        else
        {
            cout << "\nInvalid Admin Credentials!\n";
        }
    }

    int getLeaveBalance(string id, string type)
    {
        int used = 0;
        int total = 0;

        if (type == "Casual")
            total = casualLimit;

        else if (type == "Sick")
            total = sickLimit;

        else if (type == "Emergency")
            total = emergencyLimit;

        else if (type == "Earned")
            total = earnedLimit;

        for (int i = 0; i < leaves.size(); i++)
        {
            if (leaves[i].employeeId == id &&
                leaves[i].leaveType == type &&
                leaves[i].status == "Approved")
            {
                used += leaves[i].days;
            }
        }

        return total - used;
    }

    void showLeaveBalance(string id)
    {
        cout << "\n========== LEAVE BALANCE ==========\n";

        cout << "Casual Leave: "
             << getLeaveBalance(id, "Casual") << "/"
             << casualLimit << endl;

        cout << "Sick Leave: "
             << getLeaveBalance(id, "Sick") << "/"
             << sickLimit << endl;

        cout << "Emergency Leave: "
             << getLeaveBalance(id, "Emergency") << "/"
             << emergencyLimit << endl;

        cout << "Earned Leave: "
             << getLeaveBalance(id, "Earned") << "/"
             << earnedLimit << endl;
    }

    void applyLeave(int index)
    {
        Leave l;

        l.requestId = nextRequestId++;

        l.employeeId = employees[index].id;
        l.employeeName = employees[index].name;

        cout << "\n========== APPLY FOR LEAVE ==========\n";

        cout << "\n1. Casual Leave";
        cout << "\n2. Sick Leave";
        cout << "\n3. Emergency Leave";
        cout << "\n4. Earned Leave";

        int choice;

        cout << "\n\nSelect Leave Type: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                l.leaveType = "Casual";
                break;

            case 2:
                l.leaveType = "Sick";
                break;

            case 3:
                l.leaveType = "Emergency";
                break;

            case 4:
                l.leaveType = "Earned";
                break;

            default:
                cout << "\nInvalid Leave Type!\n";
                nextRequestId--;
                return;
        }

        cout << "Starting Date (DD-MM-YYYY): ";
        cin >> l.startDate;

        cout << "Ending Date (DD-MM-YYYY): ";
        cin >> l.endDate;

        cout << "Number of Days: ";
        cin >> l.days;

        if (l.days <= 0)
        {
            cout << "\nInvalid number of days!\n";
            nextRequestId--;
            return;
        }

        if (l.days > getLeaveBalance(l.employeeId, l.leaveType))
        {
            cout << "\nInsufficient Leave Balance!\n";
            nextRequestId--;
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Reason for Leave: ";
        getline(cin, l.reason);

        if (l.reason.empty())
        {
            cout << "\nPlease enter a reason for leave!\n";
            nextRequestId--;
            return;
        }

        l.status = "Pending";
        l.remark = "Not reviewed yet";

        leaves.push_back(l);

        saveLeaves();

        cout << "\nLeave Application Submitted Successfully!\n";

        cout << "Your Request ID: " << l.requestId;
        cout << "\nStatus: Pending\n";
    }

    void displayLeave(Leave l)
    {
        cout << "\nRequest ID: " << l.requestId;
        cout << "\nEmployee ID: " << l.employeeId;
        cout << "\nEmployee Name: " << l.employeeName;
        cout << "\nLeave Type: " << l.leaveType;
        cout << "\nStart Date: " << l.startDate;
        cout << "\nEnd Date: " << l.endDate;
        cout << "\nTotal Days: " << l.days;
        cout << "\nReason: " << l.reason;
        cout << "\nStatus: " << l.status;
        cout << "\nAdmin Remark: " << l.remark;

        cout << "\n----------------------------------------\n";
    }

    void employeeHistory(string id)
    {
        bool found = false;

        cout << "\n========== MY LEAVE HISTORY ==========\n";

        for (int i = 0; i < leaves.size(); i++)
        {
            if (leaves[i].employeeId == id)
            {
                displayLeave(leaves[i]);
                found = true;
            }
        }

        if (!found)
        {
            cout << "\nNo leave history found!\n";
        }
    }

    void viewAllRequests()
    {
        cout << "\n========== ALL LEAVE REQUESTS ==========\n";

        if (leaves.empty())
        {
            cout << "\nNo applications available!\n";
            return;
        }

        for (int i = 0; i < leaves.size(); i++)
        {
            displayLeave(leaves[i]);
        }
    }

    void pendingRequests()
    {
        bool found = false;

        cout << "\n========== PENDING REQUESTS ==========\n";

        for (int i = 0; i < leaves.size(); i++)
        {
            if (leaves[i].status == "Pending")
            {
                displayLeave(leaves[i]);
                found = true;
            }
        }

        if (!found)
        {
            cout << "\nNo pending applications!\n";
        }
    }

    void processRequest()
    {
        int id, choice;

        cout << "\nEnter Request ID: ";
        cin >> id;

        for (int i = 0; i < leaves.size(); i++)
        {
            if (leaves[i].requestId == id)
            {
                if (leaves[i].status != "Pending")
                {
                    cout << "\nThis request is already processed!\n";
                    return;
                }

                cout << "\n1. Approve Leave";
                cout << "\n2. Reject Leave";

                cout << "\nEnter choice: ";
                cin >> choice;

                if (choice == 1)
                {
                    leaves[i].status = "Approved";
                    leaves[i].remark = "Leave approved by admin";

                    cout << "\nLeave Approved Successfully!\n";
                }
                else if (choice == 2)
                {
                    leaves[i].status = "Rejected";

                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Enter Rejection Remark: ";
                    getline(cin, leaves[i].remark);

                    if (leaves[i].remark.empty())
                    {
                        leaves[i].remark = "Rejected by admin";
                    }

                    cout << "\nLeave Rejected Successfully!\n";
                }
                else
                {
                    cout << "\nInvalid Choice!\n";
                    return;
                }

                saveLeaves();

                return;
            }
        }

        cout << "\nRequest ID Not Found!\n";
    }

    void searchRequest()
    {
        int id;

        cout << "\nEnter Request ID: ";
        cin >> id;

        for (int i = 0; i < leaves.size(); i++)
        {
            if (leaves[i].requestId == id)
            {
                displayLeave(leaves[i]);
                return;
            }
        }

        cout << "\nRequest Not Found!\n";
    }

    void leaveReport()
    {
        int approved = 0;
        int rejected = 0;
        int pending = 0;

        for (int i = 0; i < leaves.size(); i++)
        {
            if (leaves[i].status == "Approved")
                approved++;

            else if (leaves[i].status == "Rejected")
                rejected++;

            else if (leaves[i].status == "Pending")
                pending++;
        }

        cout << "\n========== LEAVE REPORT ==========\n";

        cout << "\nTotal Employees: " << employees.size();
        cout << "\nTotal Applications: " << leaves.size();

        cout << "\nApproved Requests: " << approved;
        cout << "\nRejected Requests: " << rejected;
        cout << "\nPending Requests: " << pending;

        cout << "\n==================================\n";
    }

    void employeeMenu(int index)
    {
        int choice;

        do
        {
            cout << "\n\n================================";
            cout << "\n         EMPLOYEE PANEL";
            cout << "\n================================";

            cout << "\n1. Apply for Leave";
            cout << "\n2. Check Leave Balance";
            cout << "\n3. My Leave History";
            cout << "\n4. Logout";

            cout << "\n\nEnter Choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:
                    applyLeave(index);
                    break;

                case 2:
                    showLeaveBalance(employees[index].id);
                    break;

                case 3:
                    employeeHistory(employees[index].id);
                    break;

                case 4:
                    cout << "\nLogged Out Successfully!\n";
                    break;

                default:
                    cout << "\nInvalid Choice!\n";
            }

        } while (choice != 4);
    }

    void adminMenu()
    {
        int choice;

        do
        {
            cout << "\n\n================================";
            cout << "\n           ADMIN PANEL";
            cout << "\n================================";

            cout << "\n1. View All Requests";
            cout << "\n2. View Pending Requests";
            cout << "\n3. Approve / Reject Leave";
            cout << "\n4. Search Leave Request";
            cout << "\n5. Leave Reports";
            cout << "\n6. Logout";

            cout << "\n\nEnter Choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:
                    viewAllRequests();
                    break;

                case 2:
                    pendingRequests();
                    break;

                case 3:
                    processRequest();
                    break;

                case 4:
                    searchRequest();
                    break;

                case 5:
                    leaveReport();
                    break;

                case 6:
                    cout << "\nAdmin Logged Out!\n";
                    break;

                default:
                    cout << "\nInvalid Choice!\n";
            }

        } while (choice != 6);
    }

    void mainMenu()
    {
        int choice;

        do
        {
            cout << "\n\n";
            cout << "========================================\n";
            cout << "          LEAVE APPROVAL SYSTEM         \n";
            cout << "========================================\n";

            cout << "\n1. Employee Registration";
            cout << "\n2. Employee Login";
            cout << "\n3. Admin Login";
            cout << "\n4. Exit";

            cout << "\n\nEnter Your Choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:
                    registerEmployee();
                    break;

                case 2:
                    employeeLogin();
                    break;

                case 3:
                    adminLogin();
                    break;

                case 4:
                    cout << "\nThank you for using Leave Approval System!\n";
                    break;

                default:
                    cout << "\nInvalid Choice! Try Again.\n";
            }

        } while (choice != 4);
    }
};

int main()
{
    LeaveSystem system;

    system.mainMenu();

    return 0;
}
