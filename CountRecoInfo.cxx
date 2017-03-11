#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <iostream>
#include <fstream>
#include <limits>
#include "TChain.h"
#include <sstream>
#include "BaseClasses.h"

using namespace std;

struct EventInfo {
    std::string filename;
    Int_t EventID;
//    Int_t FGD;
};

class CountRecoInfo {
public:
    CountRecoInfo(std::string infilename);
    ~CountRecoInfo();
    
    void Run();
    
private:
    std::vector<EventInfo> m_info;
};

CountRecoInfo::CountRecoInfo(std::string infilename){
    
    ifstream infile(infilename.c_str());
    if(!infile.is_open()){
        cout << "ERROR : Could not open file " << infilename << endl;
        exit(0);
    }
    
    string line;
    while (std::getline(infile, line)){
//        cout << "Line = " <<  line << endl;
        string tmp_file;
        Int_t tmp_event;
        stringstream ss(line);
        if( !(ss >> tmp_file >> tmp_event) ){
            cerr<<"Bad line format: "<<endl
            <<"     "<<line<<endl;
            continue;
        }
//        cout << "tmp_file = " << tmp_file << " tmp_event = " << tmp_event << endl;
        EventInfo tmp_info;
        tmp_info.filename = tmp_file;
        tmp_info.EventID = tmp_event;
        m_info.push_back(tmp_info);
    }
    infile.close();
    
}

CountRecoInfo::~CountRecoInfo(){
    m_info.clear();
}

void CountRecoInfo::Run(){
    
    for(int i = 0; i < (int)m_info.size(); i++){//Iterate through events
        
        TChain * global = new TChain("ReconDir/Global");
        //    TChain * tracker = new TChain("ReconDir/Tracker");
        global->Add(m_info[i].filename.c_str());
        
        Int_t EventID;
        global->SetBranchAddress("EventID", &EventID);
        
        Int_t NPIDs;
        global->SetBranchAddress("NPIDs", &NPIDs);
        global->SetBranchAddress("PIDs", &PIDs);
        
        
        
    }
    

}

int main(int argc, char *argv[]){

    string infilename;
    
    char cc;
    while ((cc = getopt(argc, argv, "i:")) != -1) {
        switch (cc) {
            case 'i': infilename = string(optarg); break;
            default: break;
        }
    }
    
    CountRecoInfo * find = new CountRecoInfo(infilename);
//    find->Run();
    
    delete find;
    
    return 1;
}