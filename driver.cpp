#include "sha256.h"
#include "hashTable.h"
using namespace std;

int main()
{
    ifstream file("testlist.txt");
    int menuChoice;
    int numUsers;
    string user, pwd, salt;
    string username, password;

    /* Declare hash table in scope */
    hashTable *table;

    if (file.is_open())
    {
        file >> numUsers;
        file.ignore(1, '\n');

        /* Dynamically allocate a new hash table */
        table = new hashTable(numUsers);

        while (getline(file, user))
        {
            getline(file, pwd);

            /* Generate salt and password based on that salt */
            salt = table->generateSalt();
            password = sha256(pwd + salt);

            /* Add user to table for each line */
            table->addEntry(user, password, salt);
        }
    }

    do
    {
        try
        {
            cout << "\nWhat would you like to do?\n";
            cout << "\t1. Login.\n";
            cout << "\t2. Create account.\n";
            cout << "\t3. Delete account.\n";
            cout << "\t4. Exit.";
            cout << "\nCHOOSE 1-4: ";
            cin >> menuChoice;

            /* Catch for invalid menu input */
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw std::invalid_argument("Not an integer.");
            }

            switch (menuChoice)
            {
            case 1:
                cout << "\nEnter your username:\t";
                cin >> username;
                cout << "Enter your password:\t";
                cin >> password;

                /* Encrypt the password */
                salt = table->getSalt(username);
                password = sha256(password + salt);

                /* Ternary if statements. Pretty and fun! */
                table->validateLogin(username, password)
                    ? cout << "\nLogin successful\n"
                    : cout << "\nInvalid credentials\n";
                break;

            case 2:
                cout << "\nEnter your new username:\t";
                cin >> username;
                cout << "Enter your new password:\t";
                cin >> password;

                /* Encrypt the password */
                salt = table->getSalt(username);
                password = sha256(password + salt);

                cout << "\nUser added successfully.\n";
                break;

            case 3:
                cout << "\nEnter your username:\t";
                cin >> username;
                cout << "Enter your password:\t";
                cin >> password;

                table->removeUser(username, password)
                    ? cout << "\nUser removed successfully.\n"
                    : cout << "\nInvalid credentials, cannot remove user.\n";
                break;

            case 4:
                cout << "\nGOODBYE!\n";
                cout << endl;

                delete table;
                break;

            default:
                cout << "\nThat is not a valid choice.\n";
            }
        }
        catch (const std::invalid_argument &e)
        {
            /* Throw the error declared earlier */
            cout << "\nError: " << e.what() << endl;
        }
    } while (menuChoice != 4);

    return 0;
}