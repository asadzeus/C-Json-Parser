#include <iostream>
#include <string>
#include <map>
#include <cctype>

#define max 10
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
    int startindexes[max];
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
            if (json[currindex] == '}')
            {
                count++;
            }
        }

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

    void parseObjects()
    {
        string label;

        for (int i = 0; i < objcount; i++)
        {
            line = true;
            currindex = startindexes[i];

            while (json[currindex] != '}')
            {
                currindex++;
                skipWhitespace();

                if (json[currindex] == '"' && line)
                {
                    label = getLabel(json, i);
                }

                if (json[currindex] == ',')
                {
                    line = true;
                }
            }
        }
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
            bl = getBool();
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

    bool getBool()
    {
        bool bl;

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

        return bl;
    }

public:
    int objcount;
    jsonvalue objects[max];
    string json;

    void parseFile(const string &jsontext)
    {
        json = jsontext;
        currindex = 0;
        objcount = objCount();
        getStartingIndexes();
        parseObjects();
    }
};
