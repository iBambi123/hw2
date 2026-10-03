#include <iostream>
#include "mydatastore.h"
#include "util.h"

using namespace std;

MyDataStore::MyDataStore()
{
}

MyDataStore::~MyDataStore()
{
    for (Product* p : products_) {
        delete p;
    }
    for (auto& entry : users_) {
        delete entry.second;
    }
}

void MyDataStore::addProduct(Product* p)
{
    products_.insert(p);
    set<string> keys = p->keywords();
    for (const string& k : keys) {
        keywordMap_[convToLower(k)].insert(p);
    }
}

void MyDataStore::addUser(User* u)
{
    string name = convToLower(u->getName());
    users_[name] = u;
    carts_[name];   // creates an empty cart for this user
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type)
{
    set<Product*> resultSet;
    bool first = true;

    for (const string& rawTerm : terms) {
        string term = convToLower(rawTerm);
        set<Product*> matches;
        auto it = keywordMap_.find(term);
        if (it != keywordMap_.end()) {
            matches = it->second;
        }

        if (first) {
            resultSet = matches;
            first = false;
        }
        else if (type == 0) {
            resultSet = setIntersection(resultSet, matches);
        }
        else {
            resultSet = setUnion(resultSet, matches);
        }
    }

    return vector<Product*>(resultSet.begin(), resultSet.end());
}

void MyDataStore::dump(ostream& ofile)
{
    ofile << "<products>" << endl;
    for (Product* p : products_) {
        p->dump(ofile);
    }
    ofile << "</products>" << endl;
    ofile << "<users>" << endl;
    for (auto& entry : users_) {
        entry.second->dump(ofile);
    }
    ofile << "</users>" << endl;
}

bool MyDataStore::addToCart(string username, Product* p)
{
    auto it = carts_.find(convToLower(username));
    if (it == carts_.end()) {
        return false;
    }
    it->second.push_back(p);
    return true;
}

bool MyDataStore::viewCart(string username)
{
    auto it = carts_.find(convToLower(username));
    if (it == carts_.end()) {
        return false;
    }
    int index = 1;
    for (Product* p : it->second) {
        cout << "Item " << index << endl;
        cout << p->displayString() << endl;
        cout << endl;
        index++;
    }
    return true;
}

bool MyDataStore::buyCart(string username)
{
    string name = convToLower(username);
    auto it = carts_.find(name);
    if (it == carts_.end()) {
        return false;
    }
    User* u = users_[name];
    deque<Product*> remaining;

    for (Product* p : it->second) {
        if (p->getQty() > 0 && u->getBalance() >= p->getPrice()) {
            p->subtractQty(1);
            u->deductAmount(p->getPrice());
        }
        else {
            remaining.push_back(p);
        }
    }
    it->second = remaining;
    return true;
}