#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <set>
#include <map>
#include <algorithm>
#include <unordered_map>
#include "format.h"

using namespace std;

void readFileName(string& programName, string& PLA_FILE, string& DOT_FILE, char* argv[]);
int readFile(string& PLA_FILE, vector<vector<format>>& literal, ofstream& out);
void recursive(vector<vector<format>>& literal, string case_String, char temp, int pos, set<int> &check);
int binToDec(string case_String);
void Quine_McClusky(vector<vector<format>>& literal, int maxSize);
void reduce(int maxSize, vector<vector<format>>& classified, vector<vector<format>>& literal);
vector<format> find_EPI(vector<vector<format>>& literal, vector<string>& EPI, vector<int>& remain_Minterm);
vector<string> petrick_Method(vector<format>& PI, vector<int>& remain_Minterm);
void output(vector<string> &EPI, vector<string>& possiblePI, ofstream& out);
int minimun = 0;

int main(int argc, char* argv[])
{
    if (argc != 3) return 0;

    vector<vector<format>> literal(0);
    vector<format> PI;
    vector<string> EPI, possiblePI;
    vector<int> remain_Minterm;
    string programName = "", PLA_FILE = "", DOT_FILE = "";
    ofstream out;

    readFileName(programName, PLA_FILE, DOT_FILE, argv);

    out.open(argv[2]);
    int maxSize = readFile(PLA_FILE, literal, out);

    Quine_McClusky(literal, maxSize);
    PI = find_EPI(literal, EPI, remain_Minterm);

    possiblePI = petrick_Method(PI, remain_Minterm);
    output(EPI, possiblePI, out);

    return 0;
}

void readFileName(string& programName, string& PLA_FILE, string& DOT_FILE, char* argv[])
{
    programName = argv[0];
    PLA_FILE = argv[1];
    DOT_FILE = argv[2];
}

int readFile(string& PLA_FILE, vector<vector<format>>& literal, ofstream& out)
{
    ifstream input;
    string inputAmount_String = "", outputAmount = "", output = "";
    string read = "", case_String = "";
    int inputAmount_Int = 0, case_Int = 0;
    int max = 0;
    set<int> check;
    input.open(PLA_FILE);

    while (!input.eof())
    {
        input >> read;
        if (read == ".i")
        {
            input >> inputAmount_String;
            inputAmount_Int = stoi(inputAmount_String);
            out << read << ' ' << inputAmount_Int << '\n';
        }
        else if (read == ".o")
        {
            input >> outputAmount;
            out << read << ' ' << outputAmount << '\n';
        }
        else if (read == ".ilb")
        {
            out << read << ' ';
            for (int i = 0; i < inputAmount_Int; ++i)
            {
                string temp;
                input >> temp;
                out << temp << ' ';
            }
            out << '\n';
        }
        else if (read == ".ob")
        {
            out << read << ' ';
            input >> output;
            out << output << '\n';
        }
        else if (read == ".p")
        {
            input >> case_String;
            case_Int = stoi(case_String);

            for (int i = 0; i < case_Int; ++i)
            {
                input >> case_String;
                char temp;
                input >> temp;
                
                literal.resize(case_String.size());
                recursive(literal, case_String, temp, 0, check);
                max = case_String.size();
            }
        }
        else if (read[0] == '1' || read[0] == '0' || read[0] == '-')
        {
            case_String = read;
            char temp;
            input >> temp;
            literal.resize(case_String.size());
            recursive(literal, case_String, temp, 0, check);
            max = case_String.size();
        }
        else if (read == ".e")
        {
            input.close();
        }
        else
        {
            cout << "your pla file may have wrong format, please revise it and excute again.\n";
            return 0;
        }
    }

    return max;
}

void recursive(vector<vector<format>>& literal, string case_String, char temp, int pos, set<int> &check)
{
    if (pos == case_String.size())
    {
        int num = binToDec(case_String);
        if (!check.count(num))
        {
            check.insert(num);
            vector<int> dec;
            dec.push_back(num);
            if (temp == '-') literal[case_String.size() - 1].push_back(format(case_String, 0, 1));
            else literal[case_String.size() - 1].push_back(format(case_String, 0, 0, dec));
        }
        return;
    }

    if (case_String[pos] == '-')
    {
        case_String[pos] = '0';
        recursive(literal, case_String, temp, pos + 1, check);
        case_String[pos] = '1';
        recursive(literal, case_String, temp, pos + 1, check);
    }
    else
    {
        recursive(literal, case_String, temp, pos + 1, check);
    }

    return;
}

int binToDec(string case_String)
{
    int num = 0;
    vector<int> store;
    for (int i = case_String.size() - 1; i >= 0; --i)
    {
        num = num + pow(2, case_String.size() - 1 - i) * (case_String[i] - '0');
    }
    return num;
}

void Quine_McClusky(vector<vector<format>>& literal, int maxSize)
{
    for (int i = maxSize - 1; i >= 0; --i)
    {
        vector<vector<format>> classified;
        classified.resize(maxSize + 1, vector<format>(0));
        for (int j = literal[i].size() - 1; j >= 0; --j)
        {
            int count = 0;
            for (int k = 0; k < literal[i][j].bin.size(); ++k)
            {
                if (literal[i][j].bin[k] == '1') ++count;
            }

            classified[count].push_back(format(literal[i][j].bin, 0, literal[i][j].dontCare, literal[i][j].usedMinTerm));
        }
        reduce(i, classified, literal);
    }
}

void reduce(int pos, vector<vector<format>>& classified, vector<vector<format>>& literal)
{
    int count = 0;
    int index = 0;
    vector<int> storeMinTerm;
    set<string> check;

    for (int i = 1; i < classified.size(); ++i)
    {
        for (int j = 0; j < classified[i - 1].size(); ++j)
        {
            for (int k = 0; k < classified[i].size(); ++k)
            {
                count = 0;
                index = 0;
                storeMinTerm.clear();

                for (int a = 0; a < classified[i][k].bin.size(); ++a)
                {
                    if (classified[i - 1][j].bin[a] != classified[i][k].bin[a])
                    {
                        count++;
                        index = a;
                        storeMinTerm = classified[i - 1][j].usedMinTerm;
                        for (int q = 0; q < classified[i][k].usedMinTerm.size(); ++q)
                            storeMinTerm.push_back(classified[i][k].usedMinTerm[q]);
                    }
                }

                if (count == 1)
                {
                    string temp = "";

                    for (int q = 0; q < literal[pos].size(); q++)
                    {
                        if (literal[pos][q].bin == classified[i - 1][j].bin || literal[pos][q].bin == classified[i][k].bin) literal[pos][q].used = 1;
                    }

                    temp = classified[i][k].bin;
                    temp[index] = '-';

                    minimun = pos - 1;
                    if (!check.count(temp)) literal[pos - 1].push_back(format(temp, 0, -1, storeMinTerm)), check.insert(temp);
                }
            }
        }
    }
}

vector<format> find_EPI(vector<vector<format>>& literal, vector<string>& EPI, vector<int>& remain_Minterm)
{
    map<int, int> store;
    vector<format> petrick;
    vector<format> possibleLiteral;

    for (int j = 0; j < literal.size(); ++j)
    {
        for (int c = 0; c < literal[j].size(); ++c)
        {
            if (literal[j][c].used == 0)
            {
                possibleLiteral.push_back(format(literal[j][c].bin, literal[j][c].used, literal[j][c].usedMinTerm));

                for (int i = 0; i < literal[j][c].usedMinTerm.size(); ++i)
                {
                    ++store[literal[j][c].usedMinTerm[i]];
                }
            }
        }
    }

    vector<int> total_Minterm;
    set<int> temp;
    for (int i = 0; i < possibleLiteral.size(); ++i)
    {
        for (int j = 0; j < possibleLiteral[i].usedMinTerm.size(); ++j)
        {
            if (!temp.count(possibleLiteral[i].usedMinTerm[j]))
            {
                temp.insert(possibleLiteral[i].usedMinTerm[j]);
                total_Minterm.push_back(possibleLiteral[i].usedMinTerm[j]);
            }
        }
    }

    sort(total_Minterm.begin(), total_Minterm.end());

    vector<int> appearOneMinterm;
    map<int, int>::iterator ptr;
    for (ptr = store.begin(); ptr != store.end(); ++ptr)
    {
        if (ptr->second == 1)
            appearOneMinterm.push_back(ptr->first);
    }
    
    int count = 0;
    bool flag = 0;
    temp.clear();
    vector<int> deleteMinterm;

    for (int c = 0; c < possibleLiteral.size(); ++c)
    {
        flag = 0;
        for (int i = 0; i < possibleLiteral[c].usedMinTerm.size(); ++i)
        {
            if (count < appearOneMinterm.size() && possibleLiteral[c].usedMinTerm[i] == appearOneMinterm[count])
            {
                EPI.push_back(possibleLiteral[c].bin);

                for (int k = 0; k < possibleLiteral[c].usedMinTerm.size(); ++k)
                {
                    deleteMinterm.push_back(possibleLiteral[c].usedMinTerm[k]);
                }
                ++count;
                flag = 1;
                break;
            }
        }

        if (!flag) petrick.push_back(format(possibleLiteral[c].bin, possibleLiteral[c].used, possibleLiteral[c].usedMinTerm));
    }

    for (int k = 0; k < total_Minterm.size(); ++k)
    {
        bool found = false;

        for (int j = 0; j < deleteMinterm.size(); ++j)
        {
            if (total_Minterm[k] == deleteMinterm[j])
            {
                found = true;
                break;
            }
        }

        if (!found && !temp.count(total_Minterm[k]))
        {
            temp.insert(total_Minterm[k]);
            remain_Minterm.push_back(total_Minterm[k]);
        }
    }

    return petrick;
}

vector<string> petrick_Method(vector<format>& PI, vector<int>& remain_Minterm)
{
    unordered_map<int, vector<string>> category;
    vector<string> result;
    for (int i = 0; i < PI.size(); ++i)
    {
        for (int j = 0; j < PI[i].usedMinTerm.size(); ++j)
        {
            category[PI[i].usedMinTerm[j]].push_back(PI[i].bin);
        }
    }

    vector<string> temp;
    set<string> check;
    for (int i = 0; i < remain_Minterm.size(); ++i)
    {
        if (i + 1 < remain_Minterm.size())
        {
            for (int j = 0; j < category[remain_Minterm[i]].size(); ++j)
            {
                for (int k = 0; k < category[remain_Minterm[i + 1]].size(); ++k)
                {
                    if (category[remain_Minterm[i]][j].find(category[remain_Minterm[i + 1]][k]) == string::npos)
                        temp.push_back(category[remain_Minterm[i]][j] + ' ' + category[remain_Minterm[i + 1]][k]);
                    else if (!check.count(category[remain_Minterm[i]][j]))
                    {
                        temp.push_back(category[remain_Minterm[i]][j]);
                        check.insert(category[remain_Minterm[i]][j]);
                    }
                }
            }

            category[remain_Minterm[i + 1]].clear();
            for (int j = 0; j < temp.size(); ++j) category[remain_Minterm[i + 1]].push_back(temp[j]);
            
            temp.clear();
            check.clear();
        }
    }

    map<string, int> record;
    int min = 0;
    for (int i = 0; i < category[remain_Minterm[remain_Minterm.size() - 1]].size(); ++i)
    {
        record[category[remain_Minterm[remain_Minterm.size() - 1]][i]];
        for (int j = 0; j < category[remain_Minterm[remain_Minterm.size() - 1]][i].size(); ++j)
        {
            if (category[remain_Minterm[remain_Minterm.size() - 1]][i][j] == ' ') 
                record[category[remain_Minterm[remain_Minterm.size() - 1]][i]]++;
        }
        if (min > record[category[remain_Minterm[remain_Minterm.size() - 1]][i]] || i == 0) 
            min = record[category[remain_Minterm[remain_Minterm.size() - 1]][i]];
    }

    map<string, int>::iterator ptr;
    for (ptr = record.begin(); ptr != record.end(); ++ptr)
        if (ptr->second == min) result.push_back(ptr->first);

    return result;
}

void output(vector<string>& EPI, vector<string>& possiblePI, ofstream& out)
{
    EPI.push_back(possiblePI[0]);

    int count1 = 0, count2 = 0;
    for (int i = 0; i < EPI.size(); ++i)
    {
        ++count1;
        for (int j = 0; j < EPI[i].size(); ++j)
        {
            if (EPI[i][j] == ' ') ++count1;
            else if (EPI[i][j] == '0' || EPI[i][j] == '1') ++count2;
        }
    }

    cout << "Total number of terms: " << count1 << '\n';
    cout << "Total number of terms: " << count2 << '\n';
    
    out << ".p " << count1 << '\n';

    for (int i = 0; i < EPI.size(); ++i)
    {
        for (int j = 0; j < EPI[i].size(); ++j)
        {
            if (EPI[i][j] == ' ') out << " 1\n";
            else out << EPI[i][j];
        }
        out << " 1\n";
    }
    out << ".e";
    out.close();
}
