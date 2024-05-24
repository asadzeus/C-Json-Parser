#include <iostream>
#include <string>
#include <map>
#include <cctype>

#define maks 10
using namespace std;

struct jsonvalue
{
    map<string, string> strs;
    map<string, int> numbers;
    map<string, bool> bools;
};

class myjsonparser
{
private:
    int currindex;
    int startindexes[maks];
    bool line = true;

    void skipWhitespace()
    {
        while (isspace(json[currindex]))
        {
            currindex++;
        }
    }

    int objCount()
    {
        currindex = 0;
        int count = 0;
        while (json[currindex] != ']')
        {
            currindex++;
            skipWhitespace();
            if (json[currindex] == '}' && json[currindex + 1] == ',')
            {
                count++;
            }
        }
        count++;

        return count;
    }

    void getStartingIndexes()
    {
        currindex = 0;

        for (int i = 0; i < objcount; i++)
        {
            while (json[currindex] != '{')
            {
                currindex++;
                skipWhitespace();
            }
            startindexes[i] = currindex;
            currindex++;
            skipWhitespace();
        }
    }

    bool parseValue()
    {
        objcount = objCount();

        getStartingIndexes();
        all();

        return true;
    }

    string all()
    {
        string str;
        string label;

        for (int i = 0; i < objcount; i++)
        {
          //  cout << "******** New Object ********" << endl;

            line = true;
            currindex = startindexes[i];

            while (json[currindex] != '}')
            {
                currindex++;
                skipWhitespace();

                if (json[currindex] == '"' && line)
                {        
                    label = getLabel(json, i);
                //    cout << "---------------------" << endl;
                }

                if (json[currindex] == ',')
                {
                    line = true;
                }
            }
        }

        return str;
    }

    void check(string label, int i)
    {
        string str;
        int number;
        bool bl;

        if (json[currindex] == '"')
        {
            str = getString();
            objects[i].strs.insert({label, str});
        }

        else if (isdigit(json[currindex]))
        {
            number = getInt(currindex);
            objects[i].numbers.insert({label, number});
        }

        else if (json.substr(currindex, 5) == "false" || json.substr(currindex, 4) == "true")
        {
            if (json.substr(currindex, 5) == "false")
            {
                bl = false;
                currindex += 5;
            }
            else if (json.substr(currindex, 4) == "true")
            {
                bl = true;
                currindex += 4;
            }

            objects[i].bools.insert({label, bl});
        }
    }

    string getLabel(const string &json, int i)
    {
        line = false;
        string label;

        currindex++;
        skipWhitespace();

        while (json[currindex] != '"')
        {
            label += json[currindex];
            currindex++;
        }

        // skip "
        currindex++;
        skipWhitespace();

        // skip :
        currindex++;
        skipWhitespace();

      //  cout << "Label: " << label << endl;
        check(label, i);
        return label;
    }

    string getString()
    {
        string item;

        while (!isalpha(json[currindex]))
        {
            currindex++;
            skipWhitespace();
        }

        while (json[currindex] != '"')
        {
            item += json[currindex];
            currindex++;
        }

        return item;
    }

    int getInt(int start)
    {

        string item;
      //  currindex = start;

        while (!isdigit(json[currindex]))
        {
            currindex++;
            skipWhitespace();
        }

        while (isdigit(json[currindex]))
        {
            item += json[currindex];
            currindex++;
        }

        skipWhitespace();

        return stoi(item);
    }

public:
    int objcount;
    jsonvalue objects[maks];
    string json;

    void parseFile(const string &jsontext)
    {
        json = jsontext;
        currindex = 0;
        parseValue();
    }
};
