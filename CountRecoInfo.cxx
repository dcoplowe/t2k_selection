#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <iostream>
#include <fstream>
#include <limits>
#include "TChain.h"

using namespace std;

struct EventInfo {
    std::string filename;
    Int_t EventID;
};

class CountRecoInfo {
public:
    CountRecoInfo(std::string infilename);
    ~CountRecoInfo();
    
    void Run();
    
private:
    
    int GetNoLines();
    std::string GetFileName(int num);
    
};

CountRecoInfo::CountRecoInfo(std::string infilename){
    
    
    
    
}

CountRecoInfo::~CountRecoInfo(){
    if(m_infile->IsOpen()) m_infile->Close();
    if(m_infile) delete m_infile;
}

void CountRecoInfo::Run(Int_t event_no){

    int n_lines = GetNoLines();
    
//    TChain * global = new TChain("ReconDir/Global");
//    TChain * tracker = new TChain("ReconDir/Tracker");
    TChain * header = new TChain("HeaderDir/BasicHeader");
//    string common_base = GetFileName(1);
    
    for(int i = 1; i < n_lines; i++){
//        global->Add(GetFileName(i).c_str());
//        tracker->Add(GetFileName(i).c_str());
        header->Add(GetFileName(i).c_str());
        if(i == 20) break;
    }
    
//    Int_t global_evt;
//    Int_t global_run;
//    Int_t tracker_evt;

    Int_t header_evt;
    
//    global->SetBranchAddress("EventID", &global_evt);
//    global->SetBranchAddress("RunID", &global_run);
    header->SetBranchAddress("EventID", &header_evt);
    
//    tracker->SetBranchAddress("EventID", &tracker_evt);
    
//    Int_t glob_entries = global->GetEntries();
//    Int_t trac_entries = tracker->GetEntries();
    
    cout << "m_entries = " << m_entries << endl;
//    cout << "glob_entries = " << glob_entries << endl;
//    cout << "trac_entries = " << trac_entries << endl;
    
    int first = 0;
    int last = m_entries;
    
    if(event_no != -1){
        first = event_no;
        last = event_no + 1;
    }
    
    for(int first = 0; entry < last; entry++){
        
        m_intree->GetEntry(entry);
        cout << "Entry " << entry << ") evt = " << m_evt << endl;
        
        for (int glob_evt = 0; glob_evt < glob_entries; glob_evt++) {
//            global->GetEntry(glob_evt);
//            tracker->GetEntry(glob_evt);
            
            header->GetEntry(glob_evt);
            
//            cout << glob_evt + 1 << "/" << glob_entries << " m_evt = " << m_evt << ": Global evt = " << global_evt << " run = " << global_run << " Diff = " << m_evt - global_evt << endl;
            
            if(glob_evt == m_evt){
                cout << "Found Event = " << global_evt << endl;
                break;
            }
            
//            if(glob_evt == 100) break;
        }
//
//        for (int trac_evt = 0; trac_evt < trac_entries; trac_evt++) {
//            tracker->GetEntry(trac_evt);
//            cout << "Tracker evt = " << tracker_evt << endl;
//            if(trac_evt == 100) break;
//        }
        
        if(entry == 200) break;
    }

}

std::string CountRecoInfo::GetFileName(int num){
    ifstream file (m_oalistname.c_str());
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    string line;
    file >> line;
    return line;
}

int CountRecoInfo::GetNoLines(){
    int number_of_lines = 0;
    std::string line;
    std::ifstream linecount(m_oalistname.c_str());
    while (std::getline(linecount, line)) number_of_lines++;
//    cout << "number_of_lines = " << number_of_lines << endl;
    return number_of_lines;
}


int main(int argc, char *argv[]){

    string infilename;
    string oa_list;
    Int_t event_no = -1;
    
    char cc;
    while ((cc = getopt(argc, argv, "i:l:n:")) != -1) {
        switch (cc) {
            case 'i': infilename = string(optarg); break;
            case 'l': oa_list = string(optarg); break;
            case 'n': event_no = atio(optarg); break;
            default: break;
        }
    }
    
    CountRecoInfo * find = new CountRecoInfo(infilename, oa_list);
    find->Run(event_no);
    
    delete find;
    
    return 1;
}