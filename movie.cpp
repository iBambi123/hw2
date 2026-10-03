#include <sstream>
#include "movie.h"
#include "util.h"

using namespace std;

Movie::Movie(const string category, const string name, double price, int qty,
             const string genre, const string rating)
    : Product(category, name, price, qty), genre_(genre), rating_(rating)
{
}

Movie::~Movie()
{
}

set<string> Movie::keywords() const
{
    set<string> result = parseStringToWords(name_);
    result.insert(convToLower(genre_));
    return result;
}

string Movie::displayString() const
{
    stringstream ss;
    ss << name_ << "\n"
       << "Genre: " << genre_ << " Rating: " << rating_ << "\n"
       << price_ << " " << qty_ << " left.";
    return ss.str();
}

void Movie::dump(ostream& os) const
{
    Product::dump(os);
    os << genre_ << "\n" << rating_ << endl;
}