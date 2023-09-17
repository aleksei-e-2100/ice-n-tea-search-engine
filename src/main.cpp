#include <iostream>
#include <fstream>
#include <string>

#include "nlohmann/json.hpp"

using namespace std;

string projectName = "iCE && tEA Search Engine";
string projectVersion = "1.0";

int main()
{
    cout << projectName << " " << projectVersion << endl;

    ofstream file ("output.json");
    nlohmann::json dict;

    dict["name"] = "My name";
    dict["surname"] = "My surname";
   
    file << dict;

    file.close();
}
