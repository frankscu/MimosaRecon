#ifndef Parser_H
#define Parser_H 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include <algorithm>
#include <vector>
#include <map>

using namespace std;

typedef pair<string,string> ParserPair;
typedef map<string,string> ParserMap;

class Parser{
  public:
    static Parser* Instance();
    ~Parser();

    void Initialize(string confile);
    string GetParName() const {return m_par;};
    bool ParExist(string parName);
    int GetIntPar(string parName);
    double GetDoublePar(string parName);
    string GetStringPar(string parName);

    bool GetIntVector(string parName, int vecSize, vector<int>& dbParVec);
    bool GetDoubleVector(string parName, int vecSize, vector<double>& dbParVec);
    bool GetStringVector(string parName, int vecSize, vector<string>& dbParVec);
    
    void SplitString(std::string& str,std::string pattern,std::vector<std::string>& result);

    void SaveConfig();
  private:
    Parser();

    bool GetVectorCore(int vecSize, vector<string>& dbParVec);
    
    ParserMap m_parserMap;
    string m_par;
    string m_val;

    static Parser* m_parser;
    bool m_initFlag;
};

#endif
