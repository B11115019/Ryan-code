#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

using namespace std;

struct state
{
    string curr_State = "";
    vector<string> next_State;
    vector<string> output;
};

void readFileName(string& programName, string& KISS_IN, string& KISS_OUT, string& DOT_FILE, char* argv[]);
void readFile(string& KISS_IN, vector<state>& store);
void inOrder(vector<state>& store);
void inputKissFile(vector<state>& store);
int binToDec(string& read);
void checkMatch(vector<state>& store, bool** match, map<string, int>& charToInt);
void combine(vector<state>& store, bool** match, int& totalState);
void replace(string replaceState, string remainState, vector<state>& store);
void outputKissFile(vector<state>& store, string& KISS_OUT, int totalState);
string DecToBin(int num);
void outputDotFile(vector<state>& store, string& DOT_FILE);

string orginalStatus_String = "", inputAmount_String = "", outputAmount = "";

int main(int argc, char* argv[])
{
    if (argc != 4) return 0;

    string programName = "", KISS_IN = "", KISS_OUT = "", DOT_FILE = "";
    vector<state> store;
    map<string, int> charToInt;
    map<int, string> intToChar;

    readFileName(programName, KISS_IN, KISS_OUT, DOT_FILE, argv);
    readFile(KISS_IN, store);
    //for (int i = 0; i < store.size(); ++i)
    //{
    //    cout << "index = " << i << '\n';
    //    cout << "current state = " << store[i].curr_State << '\n';
    //    cout << "next state = ";
    //    for (int j = 0; j < store[i].next_State.size(); ++j)
    //    {
    //        for (int k = 0; k < store[i].next_State[j].size(); ++k)
    //        {
    //            cout << store[i].next_State[j][k] << ' ';
    //        }
    //    }
    //    cout << "\noutput = ";
    //    for (int j = 0; j < store[i].output.size(); ++j)
    //    {
    //        for (int k = 0; k < store[i].next_State[j].size(); ++k)
    //        {
    //            cout << store[i].output[j][k] << ' ';
    //        }
    //    }
    //    cout << '\n';
    //}
    //cout << "\n\n";
    inOrder(store);
    //for (int i = 0; i < store.size(); ++i)
    //{
    //    cout << "index = " << i << '\n';
    //    cout << "current state = " << store[i].curr_State << '\n';
    //    cout << "next state = ";
    //    for (int j = 0; j < store[i].next_State.size(); ++j)
    //    {
    //        for (int k = 0; k < store[i].next_State[j].size(); ++k)
    //        {
    //            cout << store[i].next_State[j][k] << ' ';
    //        }
    //    }
    //    cout << "\noutput = ";
    //    for (int j = 0; j < store[i].output.size(); ++j)
    //    {
    //        for (int k = 0; k < store[i].next_State[j].size(); ++k)
    //        {
    //            cout << store[i].output[j][k] << ' ';
    //        }
    //    }
    //    cout << '\n';
    //}
    //return 0;
    inputKissFile(store);
    int totalState = store.size();
    

    for (int i = 0; i < store.size(); ++i)
        charToInt[store[i].curr_State] = i;

    //map<int, string>::iterator I;

    //for (I = charToInt.begin(); I != charToInt.end(); ++I)
    //    cout << I->first << ' ' << I->second << '\n';

    //return 0;
    bool** match;
    match = new bool* [store.size()];
    for (int i = 0; i < store.size(); ++i)
    {
        match[i] = new bool[store.size()];
        for (int j = 0; j < store.size(); ++j)
        {
            if (i == j) match[i][j] = true;
            else match[i][j] = false;
        }
    }

    checkMatch(store, match, charToInt);
    //for (int i = 0; i < store.size(); ++i)
    //{
    //    for (int j = 0; j < store.size(); ++j)
    //    {
    //        cout << match[i][j] << ' ';
    //    }
    //    cout << '\n';
    //}
    //return 0;
    combine(store, match, totalState);
    //for (int i = 0; i < store.size(); ++i)
    //{
    //    if (store[i].curr_State != "")
    //    {
    //        cout << "index = " << i << '\n';
    //        cout << "current state = " << store[i].curr_State << '\n';
    //        cout << "next state = ";
    //        for (int j = 0; j < store[i].next_State.size(); ++j)
    //        {
    //            for (int k = 0; k < store[i].next_State[j].size(); ++k)
    //            {
    //                cout << store[i].next_State[j][k] << ' ';
    //            }
    //        }
    //        cout << "\noutput = ";
    //        for (int j = 0; j < store[i].output.size(); ++j)
    //        {
    //            for (int k = 0; k < store[i].next_State[j].size(); ++k)
    //            {
    //                cout << store[i].output[j][k] << ' ';
    //            }
    //        }
    //        cout << '\n';
    //    }
    //}
    //return 0;
    //int count = 0;
    //for (int i = 0; i < store.size(); ++i)
    //{
    //    if (store[i].curr_State == "")
    //    {
    //        ++count;
    //        for (int j = i + 1; j < store.size(); ++j)
    //        {
    //            store[j - 1] = store[j];
    //        }
    //    }
    //}
    //store.resize(store.size() - count);
    //cout << store.size();
    //return 0;

    /*for (int i = 0; i < store.size(); ++i)
    {
        if (store[i].curr_State != "")
        {
            cout << "index = " << i << '\n';
            cout << "current state = " << store[i].curr_State << '\n';
            cout << "next state = ";
            for (int j = 0; j < store[i].next_State.size(); ++j)
            {
                for (int k = 0; k < store[i].next_State[j].size(); ++k)
                {
                    cout << store[i].next_State[j][k] << ' ';
                }
            }
            cout << "\noutput = ";
            for (int j = 0; j < store[i].output.size(); ++j)
            {
                for (int k = 0; k < store[i].next_State[j].size(); ++k)
                {
                    cout << store[i].output[j][k] << ' ';
                }
            }
            cout << '\n';
        }
    }*/

    outputKissFile(store, KISS_OUT, totalState);
    outputDotFile(store, DOT_FILE);
    return 0;
}

void readFileName(string& programName, string& KISS_IN, string& KISS_OUT, string& DOT_FILE, char* argv[])
{
    programName = argv[0];
    KISS_IN = argv[1];
    KISS_OUT = argv[2];
    DOT_FILE = argv[3];
    return;
}

void readFile(string& KISS_IN, vector<state>& store)
{
    ifstream input;
    string statusAmount_String = "";
    string read = "", case_String = "";
    int case_Int = 0, statusAmount = 0;

    input.open(KISS_IN);

    while (!input.eof())
    {
        input >> read;
        if (read == ".start_kiss") continue;
        else if (read == ".i")
        {
            input >> read;
            inputAmount_String = read;
        }
        else if (read == ".o")
        {
            input >> read;
            outputAmount = read;
        }
        else if (read == ".p")
        {
            input >> case_String;
            case_Int = stoi(case_String);
        }
        else if (read[0] == '1' || read[0] == '0')
        {
            int index = binToDec(read);
            input >> read;

            bool add = 0;
            if (statusAmount_String == "")
            {
                if (store.size() == 0) store.resize(1);
                else
                {
                    for (int i = 0; i < store.size(); ++i)
                        if (store[i].curr_State == read) add = 1;

                    if (!add) store.resize(store.size() + 1);
                }
            }

            for (int i = 0; i < store.size(); ++i)
            {
                if (index >= store[i].next_State.size())
                    store[i].next_State.resize(index + 1);

                if (index >= store[i].output.size())
                    store[i].output.resize(index + 1);

                if (store[i].curr_State == read)
                {
                    input >> read;
                    store[i].next_State[index] = read;

                    input >> read;
                    store[i].output[index] = read;
                    break;
                }
                else if (store[i].curr_State == "")
                {
                    store[i].curr_State = read;

                    input >> read;

                    store[i].next_State[index] = read;
                    input >> read;

                    store[i].output[index] = read;
                    break;
                }
            }
        }
        else if (read == ".s")
        {
            input >> statusAmount_String;
            statusAmount = stoi(statusAmount_String);
            store.resize(statusAmount);
        }
        else if (read == ".r")
        {
            input >> orginalStatus_String;
        }
        else if (read == ".end_kiss")
        {
            input.close();
            return;
        }
        else
        {
            cout << "your pla file may have wrong format, please revise it and excute again.\n";
            return;
        }
    }

    return;
}

void inOrder(vector<state>& store)
{
    map<string, state> temp;
    for (int i = 0; i < store.size(); ++i)
    {
        temp[store[i].curr_State] = store[i];
    }

    map<string, state>::iterator I;
    int i = 0;

    for (I = temp.begin(), i = 0; I != temp.end(); ++I, ++i)
    {
        store[i] = I->second;
    }
}

void inputKissFile(vector<state>& store)
{
    ofstream out;
    out.open("Myinput.dot");

    out << "digraph STG {\n   rankdir=LR;\n\n   INIT [shape=point];\n";

    for (int i = 0; i < store.size(); ++i)
    {
        out << "   " << store[i].curr_State << ' ' << "[label=\"" << store[i].curr_State << "\"];\n";
    }
    out << '\n';
    out << "   INIT -> " << orginalStatus_String << ";\n";

    for (int i = 0; i < store.size(); ++i)
    {
        for (int j = 0; j < store[i].next_State.size(); ++j)
        {
            out << "   " << store[i].curr_State << " -> " << store[i].next_State[j] << " [label=\"" << DecToBin(j) << '/' << store[i].output[j] << "\"];\n";
        }
    }
    out << "}\n";
}

int binToDec(string& read)
{
    int total = 0;
    for (int i = read.size() - 1; i >= 0; --i)
        total = total + (read[i] - '0') * pow(2, read.size() - 1 - i);
    return total;
}

void checkMatch(vector<state>& store, bool** match, map<string, int>& charToInt)
{
    for (int i = 0; i < store.size(); ++i)
    {
        for (int j = i + 1; j < store.size(); ++j)
        {
            if (store[i].output == store[j].output)
            {
                match[i][j] = true;
                match[j][i] = true;
            }
        }
    }

    //for (int i = 0; i < store.size(); ++i)
    //{
    //    for (int j = 0; j < store.size(); ++j)
    //    {
    //        cout << match[i][j] << ' ';
    //    }
    //    cout << '\n';
    //}
    //cout << '\n';

    bool flag = 0;
    while(!flag)
    {
        for (int i = 0; i < store.size(); ++i)
        {
            for (int j = i + 1; j < store.size(); ++j)
            {
                if (match[charToInt[store[i].curr_State]][charToInt[store[j].curr_State]])
                {
                    for (int k = 0; k < store[i].next_State.size(); ++k)
                    {
                        if (!match[charToInt[store[i].next_State[k]]][charToInt[store[j].next_State[k]]])
                        {
                            flag = 1;
                            match[charToInt[store[i].curr_State]][charToInt[store[j].curr_State]] = false;
                            match[charToInt[store[j].curr_State]][charToInt[store[i].curr_State]] = false;
                        }
                    }
                }
            }
        }
        if (flag) flag = 0;
        else break;
    }
}

void combine(vector<state>& store, bool** match, int& totalState)
{
    for (int i = 0; i < store.size(); ++i)
    {
        for (int j = 0; j < store.size(); ++j)
        {
            if (i != j && match[i][j])
            {
                replace(store[j].curr_State, store[i].curr_State, store);
                store[j].curr_State = "";
                --totalState;
                for (int k = 0; k < store.size(); ++k)
                {
                    match[j][k] = false;
                    match[k][j] = false;
                }
            }
        }
    }
}

void replace(string replaceState, string remainState, vector<state>& store)
{
    for (int i = 0; i < store.size(); ++i)
    {
        for (int j = 0; j < store[i].next_State.size(); ++j)
        {
            if (store[i].next_State[j] == replaceState)
            {
                store[i].next_State[j] = remainState;
            }
        }
    }
}

void outputKissFile(vector<state>& store, string& KISS_OUT, int totalState)
{
    ofstream out;
    out.open(KISS_OUT);

    out << ".start_kiss\n";
    out << ".i " << inputAmount_String << '\n';
    out << ".o " << outputAmount << '\n';
    out << ".p " << totalState * pow(2, stoi(inputAmount_String)) << '\n';
    out << ".s " << totalState << '\n';
    out << ".r " << orginalStatus_String << '\n';

    for (int i = 0; i < store.size(); ++i)
    {
        if (store[i].curr_State != "")
        {
            for (int j = 0; j < store[i].next_State.size(); ++j)
            {
                
                out << DecToBin(j) << ' ' << store[i].curr_State << ' ' << store[i].next_State[j] << ' ' << store[i].output[j] << '\n';
            }
        }
    }

    out << ".end_kiss\n";
}

string DecToBin(int num)
{
    string result;
    int input = stoi(inputAmount_String);

    while (input--)
    {
        result.push_back((num % 2) + '0');
        num /= 2;
    }

    reverse(result.begin(), result.end());
    return result;
}

void outputDotFile(vector<state>& store, string& DOT_FILE)
{
    ofstream out;
    out.open(DOT_FILE);

    out << "digraph STG {\n   rankdir=LR;\n\n   INIT [shape=point];\n";

    for (int i = 0; i < store.size(); ++i)
    {
        if (store[i].curr_State != "")
            out << "   " << store[i].curr_State << ' ' << "[label=\"" << store[i].curr_State << "\"];\n";
    }
    out << '\n';
    out << "   INIT -> " << orginalStatus_String << ";\n";

    for (int i = 0; i < store.size(); ++i)
    {
        if (store[i].curr_State != "")
        {
            for (int j = 0; j < store[i].next_State.size(); ++j)
            {
                out << "   " << store[i].curr_State << " -> " << store[i].next_State[j] << " [label=\"" << DecToBin(j) << '/' << store[i].output[j] << "\"];\n";
            }
        }
    }
    out << "}\n";
}
