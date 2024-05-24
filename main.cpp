#include <iostream>
#include <fstream>
#include "./myjsonparser.cpp"

using namespace std;

int main()
{
    string filename = "details.json";
    fstream file;
    string line;
    string jsontext;

    file.open(filename);

    if (!file.is_open())
    {
        cout << "Error opening: " << filename << endl;
        return 1;
    }
    else
    {
        while (getline(file, line))
        {
            jsontext += line;
        }
    }
    myjsonparser parser;

    parser.parseFile(jsontext);

    for (int i = 0; i < parser.objcount; i++)
    {
        cout << "-----------------------" << endl;
        
        for (map<string, string>::iterator itr = parser.objects[i].strs.begin(); itr != parser.objects[i].strs.end(); itr++)
        {
            cout << itr->first << " : " << itr->second << endl;
        }
        for (map<string, int>::iterator itr = parser.objects[i].numbers.begin(); itr != parser.objects[i].numbers.end(); itr++)
        {
            cout << itr->first << " : " << to_string(itr->second) << endl;
        }
        for (map<string, bool>::iterator itr = parser.objects[i].bools.begin(); itr != parser.objects[i].bools.end(); itr++)
        {
            cout << itr->first << " : " << (itr->second ? "True" : "False") << endl;
        }
    }

    file.close();
}