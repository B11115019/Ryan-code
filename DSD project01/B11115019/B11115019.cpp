#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct format
{
    string val;
    int ELSE_EDGE = -1;
    int THEN_EDGE = -1;
};

void readFileName(string& programName, string& PLA_FILE, string& DOT_FILE, char* argv[]);
int readFile(string& PLA_FILE);
void recursion(char ch, int j, int index, char temp, string& case_String, int inputAmount_Int);
void reduce(int inputAmount);
void DOT_Format(string& DOT_FILE, int inputAmount);

format* tree = NULL;

int main(int argc, char* argv[])
{
    if (argc != 3) return 0;

    string programName = "", PLA_FILE = "", DOT_FILE = "";
    readFileName(programName, PLA_FILE, DOT_FILE, argv);
    int inputAmount = readFile(PLA_FILE);

    reduce(inputAmount);
    DOT_Format(DOT_FILE, inputAmount);
    return 0;
}

void readFileName(string& programName, string& PLA_FILE, string& DOT_FILE, char* argv[])
{
    programName = argv[0];
    PLA_FILE = argv[1];
    DOT_FILE = argv[2];
    return;
}

int readFile(string& PLA_FILE)
{
    ifstream input;
    string inputAmount_String = "", outputAmount = "", output = "";
    string read = "", case_String = "";
    int inputAmount_Int = 0, case_Int = 0;

    input.open(PLA_FILE);

    while (!input.eof())
    {
        input >> read;
        if (read == ".i")
        {
            input >> inputAmount_String;
            inputAmount_Int = stoi(inputAmount_String);

            int length = pow(2, inputAmount_Int);
            tree = new format[(int)pow(2, inputAmount_Int) + 1];
            tree[0].val = '0';
            tree[length].val = '1';
        }
        else if (read == ".o") input >> outputAmount;
        else if (read == ".ilb")
        {
            int index = 1;

            for (int i = 0; i < inputAmount_Int; ++i)
            {
                string temp;
                input >> temp;

                int count = pow(2, i);
                while (count--)
                {
                    tree[index++].val = temp;
                }
            }

            for (int i = 1; i < pow(2, inputAmount_Int - 1); ++i)
            {
                tree[i].ELSE_EDGE = 2 * i;
                tree[i].THEN_EDGE = 2 * i + 1;
            }
        }
        else if (read == ".ob") input >> output;
        else if (read == ".p")
        {
            input >> case_String;
            case_Int = stoi(case_String);

            for (int i = 0; i < case_Int; ++i)
            {
                int index = 1;
                input >> case_String;
                char temp;
                input >> temp;
                recursion(case_String[0], 0, index, temp, case_String, inputAmount_Int);
            }

            for (int i = 1; i < pow(2, inputAmount_Int); ++i)
            {
                if (tree[i].ELSE_EDGE == -1 ) tree[i].ELSE_EDGE = 0;
                if(tree[i].THEN_EDGE == -1) tree[i].THEN_EDGE = 0;
            }
        }
        else if (read[0] == '1' || read[0] == '0' || read[0] == '-')
        {
            case_String = read;
            int index = 1;
            char temp;
            input >> temp;
            recursion(case_String[0], 0, index, temp, case_String, inputAmount_Int);

            for (int i = 1; i < pow(2, inputAmount_Int); ++i)
            {
                if (tree[i].ELSE_EDGE == -1) tree[i].ELSE_EDGE = 0;
                if (tree[i].THEN_EDGE == -1) tree[i].THEN_EDGE = 0;
            }
        }
        else if (read == ".e")
        {
            input.close();
            return inputAmount_Int;
        }
        else
        {
            cout << "your pla file may have wrong format, please revise it and excute again.\n";
            return -3;
        }
    }

    return 0;
}

void recursion(char ch, int j, int index, char temp, string& case_String, int inputAmount_Int)
{
    if (j == inputAmount_Int - 1)
    {
        if (ch == '0' && temp == '1') tree[index].ELSE_EDGE = pow(2, inputAmount_Int);
        else if (ch == '1' && temp == '1') tree[index].THEN_EDGE = pow(2, inputAmount_Int);
        else if (ch == '0' && temp == '0') tree[index].ELSE_EDGE = 0;
        else if (ch == '1' && temp == '0') tree[index].THEN_EDGE = 0;
        else if (ch == '-' && temp == '1') tree[index].THEN_EDGE = pow(2, inputAmount_Int), tree[index].ELSE_EDGE = pow(2, inputAmount_Int);
        else if (ch == '-' && temp == '0') tree[index].THEN_EDGE = 0, tree[index].ELSE_EDGE = 0;
        return;
    }

    if (ch == '1')
    {
        recursion(case_String[j + 1], j + 1, tree[index].THEN_EDGE, temp, case_String, inputAmount_Int);
    }
    else if (ch == '0')
    {
        recursion(case_String[j + 1], j + 1, tree[index].ELSE_EDGE, temp, case_String, inputAmount_Int);
    }
    else if (ch == '-')
    {
        recursion(case_String[j + 1], j + 1, tree[index].THEN_EDGE, temp, case_String, inputAmount_Int);
        recursion(case_String[j + 1], j + 1, tree[index].ELSE_EDGE, temp, case_String, inputAmount_Int);
    }
}

void reduce(int inputAmount)
{
    for (int i = inputAmount; i > 0; --i)
    {
        for (int j = pow(2, i - 1); j < pow(2, i); ++j)
        {
            for (int k = j + 1; k < pow(2, i); ++k)
            {
                if (tree[k].THEN_EDGE == tree[j].THEN_EDGE && tree[k].ELSE_EDGE == tree[j].ELSE_EDGE && tree[k].val == tree[j].val)
                {
                    tree[k].THEN_EDGE = -1;
                    tree[k].ELSE_EDGE = -1;
                    for (int d = pow(2, i - 2); d < pow(2, i - 1); ++d)
                    {
                        if (tree[d].THEN_EDGE == k) tree[d].THEN_EDGE = j;
                        if (tree[d].ELSE_EDGE == k) tree[d].ELSE_EDGE = j;
                    }
                }
            }

            if (tree[j].THEN_EDGE == tree[j].ELSE_EDGE)
            {
                for (int d = pow(2, i - 2); d < pow(2, i - 1); ++d)
                {
                    if (tree[d].THEN_EDGE == j) tree[d].THEN_EDGE = tree[j].THEN_EDGE;
                    if (tree[d].ELSE_EDGE == j) tree[d].ELSE_EDGE = tree[j].ELSE_EDGE;
                }

                tree[j].THEN_EDGE = -1;
                tree[j].ELSE_EDGE = -1;
            }
        }
    }
}

void DOT_Format(string& DOT_FILE, int inputAmount)
{
    ofstream output;
    vector<int> storeIndex;
    output.open(DOT_FILE);

    output << "digraph ROBDD {\n";

    for (int k = 0; k < inputAmount; ++k)
    {
        bool flag = 1;
        for (int i = pow(2, k); i < pow(2, k + 1); ++i)
        {
            if (tree[i].THEN_EDGE != -1 && tree[i].ELSE_EDGE != -1)
            {
                if(flag) output << "{rank=same ", flag = 0;
                storeIndex.push_back(i);
                output << i << ' ';
            }
        }
        
        if(!flag) output << "}\n";
    }

    output << '\n';

    output << "0[label=0, shape=box];\n";
    for (int i = 0; i < storeIndex.size(); ++i)
    {
        output << storeIndex[i] << "[label=\"" << tree[storeIndex[i]].val << "\"]\n";
    }
    output << pow(2, inputAmount) << "[label=1, shape=box];\n";

    output << '\n';

    for (int i = 0; i < storeIndex.size(); ++i)
    {
        output << storeIndex[i] << "->" << tree[storeIndex[i]].ELSE_EDGE << "[label=\"0\", style=dotted]\n";
        output << storeIndex[i] << "->" << tree[storeIndex[i]].THEN_EDGE << "[label=\"1\", style=solid]\n";
    }
    output << '}';

    output.close();
}
