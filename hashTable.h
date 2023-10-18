#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <fstream>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <string>
#include <time.h>
using std::string;

class hashTable
{
private:
    /* CLASS ATTRIBUTES */
    class entry;
    int tableSize;

    /* HASH TABLE */
    entry **hashArray;

    /* FUNCTION DECLARATIONS */
    int hash(string);
    entry *getEntry(string);

public:
    /* FUNCTION DECLARATIONS */
    /* CONSTRUCTOR AND DESTRUCTOR */
    hashTable(int);
    ~hashTable();

    /* SALT FUNCTIONS */
    string generateSalt();
    string getSalt(string);

    /* TABLE MUTATOR */
    void addEntry(string, string, string);

    /* VALIDATORS */
    bool validateLogin(string, string);
    bool removeUser(string, string);
};

class hashTable::entry
{
private:
    /* CLASS ATTRIBUTES */
    string uname;
    string passHash;
    string salt;

public:
    /* CLASS ATTRIBUTES */
    entry *next;

    /* FUNCTION DECLARATIONS */
    /* CONSTRUCTOR */
    entry(string u, string p, string s)
        : uname(u), passHash(p), salt(s), next(nullptr){};

    /* ACCESSORS */
    string getUsername() { return uname; };
    string getHashedPwd() { return passHash; };
    string getSalt() { return salt; };
};

#endif