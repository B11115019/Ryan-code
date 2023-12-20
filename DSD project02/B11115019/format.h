#pragma once
#include <string>
#include <vector>

using namespace std;

class format
{
public:
    string bin;
    bool used;
    short dontCare;
    int dec;
    vector<int> usedMinTerm;
    format()
    {
        used = 0;
        dontCare = 0;
        dec = 0;
    }

    format(string bin, bool used, short dontCare)
    {
        this->bin = bin;
        this->used = used;
        this->dontCare = dontCare;
    }

    format(string bin, bool used, short dontCare, int dec)
    {
        this->bin = bin;
        this->used = used;
        this->dontCare = dontCare;
        this->dec = dec;
    }

    format(string bin, bool used, vector<int> usedMinTerm)
    {
        this->bin = bin;
        this->used = used;
        this->usedMinTerm = usedMinTerm;
    }

    format(string bin, bool used, short dontCare, vector<int> usedMinTerm)
    {
        this->bin = bin;
        this->used = used;
        this->dontCare = dontCare;
        this->usedMinTerm = usedMinTerm;
    }
};