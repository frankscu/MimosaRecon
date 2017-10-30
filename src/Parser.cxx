#include "Parser.h"
//#include "AnalysisManager.h"
#include "time.h"
#include "float.h"

Parser* Parser::m_parser = NULL;

Parser* Parser::Instance(){
  if(m_parser == NULL){
    m_parser = new Parser();
  }
  return m_parser;
}

Parser::Parser(){
  if(m_parser) cout<<"ERROR::The parser has been created twice!!!"<<endl;
  else m_initFlag = false;
}

Parser::~Parser(){
  m_parserMap.clear();
  if(m_parser) delete m_parser;
}

void Parser::Initialize(string confile){

  if(m_initFlag){
    cout<<"WARNING::The Parser has been initialized!!!"<<endl;
    return;
  }
  m_initFlag = true;

  m_par="";
  m_val="";
  string line,lineTmp;
  fstream* m_finConf = new fstream(confile.c_str(),ios::in);

  cout<<"############# JobOptionFile:: "<<confile<<" ################"<<endl;
  cout<<""<<endl;
  int _nItem=0;
  while(m_finConf->good()){
    std::getline(*m_finConf,lineTmp);
    if(!m_finConf->good()){
      break;
    }

    lineTmp.erase(0,lineTmp.find_first_not_of(" "));
    if(lineTmp.size()<1) continue;
    if(lineTmp.substr(0,1)=="#") continue;

    //Process command with multi-line
    if(lineTmp.find(";") == string::npos){
      //lineTmp.erase(lineTmp.find_last_of("\n"),1);
      line.append(lineTmp); 
      continue;
    }else{
      lineTmp.erase(0,lineTmp.find_first_not_of(" "));
      line.append(lineTmp);
    }
    
    //Process the command
    string cmdStr = line.substr(line.find_first_not_of(" "),line.find_last_of(";"));
    if(cmdStr.size()<1) continue;
    if(cmdStr.substr(0,1)=="#") continue;
    unsigned int cmdEqFound = cmdStr.find("=");
    string parRaw = cmdStr.substr(0,cmdEqFound);
    string par = parRaw.substr(0,parRaw.find_last_not_of(" ")+1);
    string valRaw = cmdStr.substr(cmdEqFound+1); 
    string valRaw2 = valRaw.substr(valRaw.find_first_not_of(" "));
    string val;
    unsigned int lastChar = valRaw2.find_last_not_of("\"");
    if(lastChar == valRaw2.size()-1){
      val = valRaw2.substr(valRaw2.find_first_not_of("\""),lastChar+1);
    }else{
      val = valRaw2.substr(valRaw2.find_first_not_of("\""),lastChar); 
    }


    cout<<par<<" = "<<val<<endl;
    m_parserMap.insert(ParserPair(par,val));
    _nItem++;

    line.clear();
  }
 
  cout<<""<<endl;
  cout<<"Parser:: "<<_nItem<<" parameters are initialized!!!"<<endl;
  cout<<"#####################################"<<endl;
  cout<<""<<endl;

  delete m_finConf;
}

bool Parser::ParExist(string parName){
  ParserMap::iterator it_par = m_parserMap.find(parName);
  if(it_par != m_parserMap.end()){
    m_par=parName;
    m_val=m_parserMap[parName];
    return true;
  }
  return false;
}

int Parser::GetIntPar(string parName){
  if(m_par != parName){
    if(!ParExist(parName)){
      cout<<"WARNING::Parameter "<<parName<<" doesn't exist!!!"<<endl;
      return 0;
    }
  }
  return atoi(m_val.c_str());
}

double Parser::GetDoublePar(string parName){
  if(m_par != parName){
    if(!ParExist(parName)){
      cout<<"WARNING::Parameter "<<parName<<" doesn't exist!!!"<<endl;
      return DBL_MAX;
    }
  }
  return atof(m_val.c_str());
}

string Parser::GetStringPar(string parName){
  if(m_par != parName){
    if(!ParExist(parName)){
      cout<<"WARNING::Parameter "<<parName<<" doesn't exist!!!"<<endl;
      return "";
    }
  }
  return m_val;
}

bool Parser::GetIntVector(string parName, int vecSize, vector<int>& dbParVec){
  dbParVec.clear();
  if(!ParExist(parName)){
    cout<<"WARNING::Parameter "<<parName<<" doesn't exist!!!"<<endl;
    return false;
  }
  
  vector<string> arrayStrVec;
 
  if(!GetVectorCore(vecSize,arrayStrVec)) return false;
  
  for(int i=0;i<vecSize;i++){
    string valStrTmp = arrayStrVec[i];
    string valStr = valStrTmp.substr(valStrTmp.find_first_not_of(" "),valStrTmp.find_last_not_of(" ")+1);
    dbParVec.push_back(atoi(valStr.c_str()));
  }
  return true;
}

bool Parser::GetDoubleVector(string parName, int vecSize, vector<double>& dbParVec){
  dbParVec.clear();
  if(!ParExist(parName)){
    cout<<"WARNING::Parameter "<<parName<<" doesn't exist!!!"<<endl;
    return false;
  }
  
  vector<string> arrayStrVec;
 
  if(!GetVectorCore(vecSize,arrayStrVec)) return false;
  
  for(int i=0;i<vecSize;i++){
    string valStrTmp = arrayStrVec[i];
    string valStr = valStrTmp.substr(valStrTmp.find_first_not_of(" "),valStrTmp.find_last_not_of(" ")+1);
    dbParVec.push_back(atof(valStr.c_str()));
  }
  return true;
}

bool Parser::GetStringVector(string parName, int vecSize, vector<string>& dbParVec){
  dbParVec.clear();
  if(!ParExist(parName)){
    cout<<"WARNING::Parameter "<<parName<<" doesn't exist!!!"<<endl;
    return false;
  }
  
  vector<string> arrayStrVec;
 
  if(!GetVectorCore(vecSize,arrayStrVec)) return false;
  
  for(int i=0;i<vecSize;i++){
    string valStrTmp = arrayStrVec[i];
    string valStrRaw = valStrTmp.substr(valStrTmp.find_first_not_of(" "),valStrTmp.find_last_not_of(" ")+1);
    string valStr;
    unsigned int lastChar = valStrRaw.find_last_not_of("\"");
    if(lastChar == valStrRaw.size()-1){
      valStr = valStrRaw.substr(valStrRaw.find_first_not_of("\""),lastChar+1);
    }else{
      valStr = valStrRaw.substr(valStrRaw.find_first_not_of("\""),lastChar); 
    }
    dbParVec.push_back(valStr.c_str());
  }
  return true;
}


bool Parser::GetVectorCore(int vecSize, vector<string>& arrayStrVec){

  string arrayRawStr = m_val.substr(m_val.find_first_not_of("{"),m_val.find_last_of("}")-1);
  //cout<<arrayRawStr<<endl;
  string arrayStr = arrayRawStr.substr(arrayRawStr.find_first_not_of(" "),arrayRawStr.find_last_not_of(" ")+1);
  //cout<<arrayStr<<endl;
  
  SplitString(arrayStr,",",arrayStrVec);

  int size_tmp = arrayStrVec.size();
  if(size_tmp != vecSize){
    cout<<"Error::Vector Size: "<<size_tmp<<", Required Size: "<<vecSize<<endl;
    return false;
  }

  return true;
}

void Parser::SplitString(std::string& str,std::string pattern,std::vector<std::string>& result)
{
  result.clear();
  std::string::size_type pos;
  str+=pattern;
  unsigned int size=str.size();

  for(unsigned int i=0; i<size; i++)
  {
    pos=str.find(pattern,i);
    if(pos<size)
    {
      std::string s=str.substr(i,pos-i);
      result.push_back(s);
      i=pos+pattern.size()-1;
    }
  }
}


void Parser::SaveConfig(){
  char fName[200];
  string outputPath = GetStringPar("OutputPath");
  string fileNamePattern = GetStringPar("OutputFileNamePattern");
  int startJob = GetIntPar("StartJob");
  int finalJob = GetIntPar("FinalJob");
  sprintf(fName,"%s/%s_%d_%d_Config.txt",outputPath.c_str(),fileNamePattern.c_str(),startJob,finalJob);
  fstream foutConf(fName,ios::out);
  
  time_t timep;
  time(&timep);
  //foutConf<<"Run Time: "<<asctime(gmtime(&timep))<<endl;
  foutConf<<"Run Time: "<<asctime(localtime(&timep))<<endl;
 
  foutConf<<"*********** Result Files ************ "<<endl;
  //AnalysisManager::Instance()->SaveConfig(foutConf);

  foutConf<<"*********** Config Information ************ "<<endl;
  ParserMap::iterator itPar;
  for(itPar=m_parserMap.begin();itPar!=m_parserMap.end();itPar++){
    foutConf<<itPar->first<<" = "<<itPar->second<<endl;
  }
  foutConf.close();
}


