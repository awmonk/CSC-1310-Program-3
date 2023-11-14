#include "hashTable.h"
using std::cout;
using std::string;

/* HASHTABLE FUNCTIONS */
/* Constructor */
/* Set private variable tableSize to size passed through as argument */
hashTable::hashTable(int size) : tableSize(size)
{
    /* Dynamically allocate an array of pointers */
    hashArray = new entry *[tableSize];

    /* Initialize each element in the array to a nullptr */
    std::fill(hashArray, hashArray + size, nullptr);

    srand(time(0));
};

/* Destructor */
hashTable::~hashTable()
{
    /* Deallocating elements from the front of the list */
    for (int i = 0; i < tableSize; i++)
    {
        if (hashArray[i])
        {
            /* Initialize traversal pointers */
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

/* Accessor */
string hashTable::getSalt(string username)
{
    entry *curr = getEntry(username);
    return curr ? curr->getSalt() : "";
};

/* Add Entry */
void hashTable::addEntry(string user, string pwd, string salt)
{
    /* Dynamically allocate a new entry */
    entry *newEntry = new entry(user, pwd, salt);

    /* Hash the entry using the username as a key */
    int index = hash(newEntry->getUsername());

    /* List traversal */
    /* Collision detection: If current index doesn't exist, add entry to index */
    if (!hashArray[index])
        hashArray[index] = newEntry;
    /* Otherwise, traverse the list for a free address */
    else
    {
        /* Initialize a traversal pointer */
        entry *curr = hashArray[index];

        /* While traversal pointer is pointing to a valid memory address */
        while (curr)
        {
            /* Handle case for existing username */
            if (curr->getUsername() == user)
            {
                cout << "\nThis username already exists, "
                     << "please choose a different username.\n";

                /* Deallocate newEntry and return to main */
                delete newEntry;
                return;
            }

            /* Check to see if the end of the list has been reached */
            if (!curr->next)
            {
                /* Assign newEntry to the end of the list */
                curr->next = newEntry;
                return;
            }

            /* Continue traversal */
            curr = curr->next;
        }
    }
};

/* VALIDATORS */
/* Login Validator */
bool hashTable::validateLogin(string user, string key)
{
    /* Initialize traversal pointer using the username as a key */
    entry *curr = getEntry(user);

    /* Handle invalid pointer */
    if (!curr)
        return false;

    /* Validation output */
    cout << "\nPassword hash being validated:\t" << key
         << "\nPassword hash provided by user:\t" << curr->getHashedPwd()
         << "\n";

    /* Return boolean for whether the hashed password for the traversal pointer
       matches the argument passed through to the function */
    return (curr->getHashedPwd() == key);
}

/* Entry removal */
bool hashTable::removeUser(string username, string key)
{
    int index = hash(username);
    entry *curr = getEntry(username);

    /* Check to see if the entry exists */
    if (!curr)
        return false;

    /* Validation output */
    cout << "\nPassword hash being validated:\t" << key
         << "\nPassword hash provided by user:\t" << curr->getHashedPwd()
         << "\n";

    /* Validate the user input against the table */
    if (curr->getHashedPwd() == key)
    {
        /* If user's entry is the first in the list, update the array */
        if (hashArray[index] == curr)
            hashArray[index] = curr->next;
        /* Otherwise, traverse the list for the user's entry */
        else
        {
            /* Initialize a prev traversal pointer */
            entry *prev = hashArray[index];

            /* While prev pointer has not caught up with the current pointer,
               continue traversal */
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

/* Accessor */
hashTable::entry *hashTable::getEntry(string key)
{
    /* Hash the key before initializing a traversal pointer assigned to its respective
       bucket */
    int index = hash(key);
    entry *curr = hashArray[index];

    /* Traverse the list using the username as the key */
    while (curr)
    {
        if (curr->getUsername() == key)
            return curr;

        curr = curr->next;
    }

    /* Handle case for invalid entry */
    return nullptr;
};