#include "hashTable.h"
using std::cout;
using std::string;

/* HASHTABLE FUNCTIONS */
hashTable::hashTable(int size) : tableSize(size)
{
    /* Dynamically allocate an array of pointers */
    hashArray = new entry *[tableSize];

    /* Initialize each element in the array to a nullptr */
    std::fill(hashArray, hashArray + size, nullptr);

    srand(time(0));
};

hashTable::~hashTable()
{
    /* Deallocating elements from the front of the list */
    for (int i = 0; i < tableSize; i++)
    {
        if (hashArray[i])
        {
            entry *prev = nullptr;
            entry *curr = hashArray[i];

            while (curr)
            {
                prev = curr;
                curr = curr->next;
                delete prev;
            }
        }
    }

    /* Deallocating the array itself */
    delete[] hashArray;
};

// DO NOT TOUCH THIS
string hashTable::generateSalt()
{
    string s = "";

    for (int i = 0; i < tableSize; i++)
        s = s + char('!' + (rand() % 93));

    return s;
};

string hashTable::getSalt(string username)
{
    entry *curr = getEntry(username);
    return curr ? curr->getSalt() : "";
};

void hashTable::addEntry(string user, string pwd, string salt)
{
    /* Dynamically allocate a new entry */
    entry *newEntry = new entry(user, pwd, salt);

    /* Hash the entry using the username as a key */
    int index = hash(newEntry->getUsername());

    if (!hashArray[index])
        hashArray[index] = newEntry;
    else
    {
        entry *curr = hashArray[index];

        while (curr)
        {
            if (curr->getUsername() == user)
            {
                cout << "\nThis username already exists, ";
                cout << "please choose a different username.\n";

                delete newEntry;
                return;
            }

            if (!curr->next)
            {
                curr->next = newEntry;
                return;
            }

            curr = curr->next;
        }
    }
};

/* VALIDATORS */
bool hashTable::validateLogin(string user, string key)
{
    entry *curr = getEntry(user);

    if (!curr)
        return false;

    cout << "\nPassword hash being validated:\t" << key;
    cout << "\nPassword hash provided by user:\t" << curr->getHashedPwd();
    cout << "\n";

    return (curr->getHashedPwd() == key);
}

bool hashTable::removeUser(string username, string key)
{
    int index = hash(username);
    entry *curr = getEntry(username);

    /* Check to see if entry exists */
    if (!curr)
        return false;

    cout << "\nPassword hash being validated:\t" << key;
    cout << "\nPassword hash provided by user:\t" << curr->getHashedPwd();
    cout << "\n";

    /* Validate the user input against the table */
    if (curr->getHashedPwd() == key)
    {
        /* If user's entry is the first in the list, update the array */
        if (hashArray[index] == curr)
            hashArray[index] = curr->next;
        /* Otherwise, traverse the list for the user's entry */
        else
        {
            entry *prev = hashArray[index];

            while (prev->next != curr)
                prev = prev->next;

            prev->next = curr->next;
        }

        /* Deallocate memory for the entry */
        delete curr;
        /* User removed successfully */
        return true;
    }

    /* Credentials not validated */
    return false;
}

/* PRIVATE FUNCTIONS */
// DO NOT TOUCH THIS
int hashTable::hash(string key)
{
    int sum = 0;

    for (int i = 0; i < key.size(); i++)
        sum += key[i];

    return sum % tableSize;
};

hashTable::entry *hashTable::getEntry(string key)
{
    int index = hash(key);
    entry *curr = hashArray[index];

    while (curr)
    {
        if (curr->getUsername() == key)
            return curr;

        curr = curr->next;
    }

    return nullptr;
};