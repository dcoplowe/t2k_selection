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

class FindFiles {
public:
    FindFiles(std::string infilename, std::string oa_listname);
    ~FindFiles();
    
    void Run();
    
private:
    TFile * m_infile;
    TTree * m_intree;
    
    Int_t m_evt;
    TBranch * m_b_evt;
    Int_t m_entries;
    
    std::string m_oalistname;
    std::string m_outfilename;
    
    int GetNoLines();
    std::string GetFileName(int num);
    
};

FindFiles::FindFiles(std::string infilename, std::string oa_list){
    
    m_infile = new TFile(infilename.c_str(), "READ");
    
//    if(m_infile){
//        cout << "ERROR : Could not open file (NULL): " << infilename << endl;
//        exit(0);
//    }
    
    if(!m_infile->IsOpen()){
        cout << "ERROR : Could not open file : " << infilename << endl;
        exit(0);
    }
    
    m_intree = (TTree*)m_infile->Get("truth");

    if(m_intree){
    m_intree->SetBranchAddress("evt", &m_evt, &m_b_evt);
    m_entries = m_intree->GetEntries();
    }
    else {
        cout << "ERROR : Could not get tree" << endl;
        exit(0);
    }
    
    m_oalistname = oa_list;
    
    //Now set the out file name:
    m_outfilename= infilename;
    //1) Remove any path info.
    //2) Remove .root type.
    //3) Append _analysis_list.txt
    
    size_t point;// = 0;
    while( (point = m_outfilename.find("/")) != std::string::npos){
        m_outfilename = m_outfilename.substr( (point + 1) );
//        cout << "Filename : " << m_outfilename << endl;
    }
    
    if( (point = m_outfilename.find(".root")) != std::string::npos){
        m_outfilename = m_outfilename.substr(0, point);
//                cout << "Filename : " << m_outfilename << endl;
    }
    
    string outdir = infilename;
    if( (point = outdir.find(m_outfilename.c_str()) ) != std::string::npos){
//        cout << "Point = " << point << endl;
        outdir = outdir.substr(0,point);
    }
    
    if(outdir.empty()) outdir = "~/";
    
    m_outfilename += "_goodoa_list.txt";
    m_outfilename = outdir + m_outfilename;
//    cout << "outdir = " << outdir << endl;
//    cout << "m_outfilename = " << m_outfilename << endl;
}

FindFiles::~FindFiles(){
    if(m_infile->IsOpen()) m_infile->Close();
    if(m_infile) delete m_infile;
}

void FindFiles::Run(){

    int n_lines = GetNoLines();
    
    TChain * global = new TChain("ReconDir/Global");
    TChain * tracker = new TChain("ReconDir/Tracker");
    TChain * header = new TChain("HeaderDir/BasicHeader");
//    string common_base = GetFileName(1);
    
    for(int i = 1; i < n_lines; i++){
        global->Add(GetFileName(i).c_str());
        tracker->Add(GetFileName(i).c_str());
        header->Add(GetFileName(i).c_str());
//        if(i == 20) break;
    }
    
    Int_t global_evt;
    Int_t global_run;
    Int_t tracker_evt;

    Int_t header_evt;
    
    global->SetBranchAddress("EventID", &global_evt);
//    global->SetBranchAddress("RunID", &global_run);

//    header->SetBranchAddress("EventID", &header_evt);
    
//    tracker->SetBranchAddress("EventID", &tracker_evt);
    
    Int_t glob_entries = global->GetEntries();
    Int_t trac_entries = tracker->GetEntries();
//    Int_t trac_entries = tracker->GetEntries();
    
    cout << "m_entries = " << m_entries << endl;
    cout << "glob_entries = " << glob_entries << endl;
    cout << "trac_entries = " << trac_entries << endl;
    
    for(int entry = 0; entry < m_entries; entry++){
        
        m_intree->GetEntry(entry);
        cout << "Entry " << entry << ") evt = " << m_evt << endl;
        
        for (int glob_evt = 0; glob_evt < glob_entries; glob_evt++) {
            global->GetEntry(glob_evt);
            cout << glob_evt + 1 << "/" << glob_entries << " m_evt = " << m_evt << ": Global evt = " << global_evt << " run = " << global_run << " Diff = " << m_evt - global_evt << endl;
            
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
        
//        if(entry == 200) break;
    }

}

std::string FindFiles::GetFileName(int num){
    ifstream file (m_oalistname.c_str());
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    string line;
    file >> line;
    return line;
}

int FindFiles::GetNoLines(){
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
    
    char cc;
    while ((cc = getopt(argc, argv, "i:l:")) != -1) {
        switch (cc) {
            case 'i': infilename = string(optarg); break;
            case 'l': oa_list = string(optarg); break;
            default: break;
        }
    }
    
    FindFiles * find = new FindFiles(infilename, oa_list);
    find->Run();
    
    delete find;
    
    return 1;
}