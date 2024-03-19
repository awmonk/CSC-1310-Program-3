#include "hashTable.h"
#include "sha256.h"
#include <conio.h>
using namespace std;

string getPassword()
{
    /* A simple function to obscure passwords while typing. Uses an empty string and
       C++'s embedded i/o and stack methods */
    string password = "";
    char ch;

    /* getch, or "get char" function captures the user's input. '13' is the ASCII value
       for ENTER; the loop will complete once the user hits the enter key */
    while ((ch = getch()) != 13)
    {
        /* If the character is '\b'; the BACKSPACE key */
        if (ch == '\b')
        {
            /* Treating the array of characters as a stack and checking to see if any
               characters have been typed */
            if (!password.empty())
            {
                /* BACKSPACE twice, once for the newchar at the end of the array and once
                   for the char the user is trying to remove. Using the stack method
                   "pop" to remove the top char from the stack */
                cout << "\b \b";
                password.pop_back();
            }
        }
        else
        {
            /* Display an asterisk to let the user know a character has been added to
               the string. Then add the character to the array */
            cout << '*';
            password += ch;
        }
    }

    cout << "\n";
    return password;
}

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
            pwd = sha256(pwd + salt);

            /* Add user to table for each line */
            table->addEntry(user, pwd, salt);
        }
    }

    do
    {
        try
        {
            cout << "\nWhat would you like to do?\n"
                 << "\t1. Login.\n"
                 << "\t2. Create account.\n"
                 << "\t3. Delete account.\n"
                 << "\t4. Exit."
                 << "\nCHOOSE 1-4: ";
            cin >> menuChoice;

            /* Catch for invalid menu input */
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Not an integer.");
            }

            switch (menuChoice)
            {
            case 1:
                cout << "\nEnter your username:\t";
                cin >> username;

                cout << "Enter your password:\t";
                password = getPassword();

                /* Check the table for the salted hash value */
                salt = table->getSalt(username);
                password = sha256(password + salt);

                table->validateLogin(username, password)
                    ? cout << "\nLogin successful.\n"
                    : cout << "\nInvalid credentials.\n";
                break;

            case 2:
                cout << "\nEnter your new username:\t";
                cin >> username;

                cout << "Enter your new password:\t";
                password = getPassword();

                /* Encrypt the password */
                salt = table->generateSalt();
                password = sha256(password + salt);

                /* Add user */
                table->addEntry(username, password, salt);

                cout << "\nUser added successfully.\n";
                break;

            case 3:
                cout << "\nEnter your username:\t";
                cin >> username;

                cout << "Enter your password:\t";
                password = getPassword();

                /* Check the table for the salted hash value */
                salt = table->getSalt(username);
                password = sha256(password + salt);

                table->removeUser(username, password)
                    ? cout << "\nUser removed successfully.\n"
                    : cout << "\nInvalid credentials, cannot remove user.\n";
                break;

            case 4:
                cout << "\nGOODBYE!\n\n";

                delete table;
                break;

            default:
                cout << "\nThat is not a valid choice.\n";
                break;
            }
        }
        catch (const invalid_argument &e)
        {
            /* Throw the error declared earlier */
            cout << "\nError: " << e.what() << "\n";
        }
    } while (menuChoice != 4);

    return 0;
}