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

using namespace std;

class FindFiles {
public:
    FindFiles(std::string infilename, std::string oa_listname);
    ~FindFiles();
    
    void Run(Int_t event_no = -1);
    
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
    
//    m_outfilename += "_goodoa_list.txt";
    m_outfilename = outdir + m_outfilename;
//    cout << "outdir = " << outdir << endl;
//    cout << "m_outfilename = " << m_outfilename << endl;
}

FindFiles::~FindFiles(){
    if(m_infile->IsOpen()) m_infile->Close();
    if(m_infile) delete m_infile;
}

void FindFiles::Run(Int_t event_no){

    int n_lines = GetNoLines();
 
    TChain * header = new TChain("HeaderDir/BasicHeader");
    
    for(int i = 1; i < n_lines; i++){
        header->Add(GetFileName(i).c_str());
        if(i == 20) break;
    }
    
    Int_t header_evt;
    header->SetBranchAddress("EventID", &header_evt);
    Int_t header_entries = header->GetEntries();
    
    cout << "m_entries = " << m_entries << endl;
    
    int first = 0;
    int last = m_entries;
    
    if(event_no != -1){
        
        if(event_no > m_entries - 1){
            cout << "ERROR : Event no out of range " << event_no << " (max = " << m_entries << ")" << endl;
            exit(0);
        }
        first = event_no;
        last = event_no + 1;
    }
    
    //Open the file here:
    
    stringstream ss_first, ss_last;
    ss_first << first;
    ss_last << (last - 1);
    
    string savename = m_outfilename + "_range" + ss_first.str() + "-" + ss_last.str() + ".txt";
    cout << "Outfile: " << savename << endl;
    
    ofstream outfile(savename.c_str());
    if(!outfile.is_open()){
        cout << "ERROR : Could not open file: " << savename << endl;
        exit(0);
    }
    
    for(int entry = first; entry < last; entry++){
        m_intree->GetEntry(entry);
        cout << "Entry " << entry << ") evt = " << m_evt << endl;
        
        for (int head_evt = 0; head_evt < header_entries; head_evt++) {
            header->GetEntry(head_evt);
            if(header_evt == 40/*m_evt*/){
                cout << "Found Event = " << header_evt << endl;
                cout << header->GetFile()->GetName() << " " << header_evt << endl;
                outfile << header->GetFile()->GetName() << " " << header_evt << endl;
                break;
            }
        }
    }
    if(outfile.is_open()) outfile.close();
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
    Int_t event_no = -1;
    
    char cc;
    while ((cc = getopt(argc, argv, "i:l:n:")) != -1) {
        switch (cc) {
            case 'i': infilename = string(optarg); break;
            case 'l': oa_list = string(optarg); break;
            case 'n': event_no = atoi(optarg); break;
            default: break;
        }
    }
    
    FindFiles * find = new FindFiles(infilename, oa_list);
    find->Run(event_no);
    
    delete find;
    
    return 1;
}