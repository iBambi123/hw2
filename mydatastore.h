#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <map>
#include <deque>
#include <vector>
#include "datastore.h"

class MyDataStore : public DataStore {
public:
    MyDataStore();
    ~MyDataStore();

    void addProduct(Product* p);
    void addUser(User* u);
    std::vector<Product*> search(std::vector<std::string>& terms, int type);
    void dump(std::ostream& ofile);

    // Cart operations (return false if the username is invalid)
    bool addToCart(std::string username, Product* p);
    bool viewCart(std::string username);
    bool buyCart(std::string username);

private:
    std::set<Product*> products_;
    std::map<std::string, User*> users_;
    std::map<std::string, std::set<Product*>> keywordMap_;
    std::map<std::string, std::deque<Product*>> carts_;
};
#endif