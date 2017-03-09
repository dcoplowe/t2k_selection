#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <iostream>
#include <fstream>
#include <limits>

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
    
    std::string GetFileName(int num);
    
};

FindFiles::FindFiles(std::string infilename, std::string oa_list){
    
    m_infile = new TFile(infilename.c_str(), "READ");
    m_intree = (TTree*)m_infile->Get("Truth");
    m_intree->SetBranchAddress("evt", &m_evt, &m_b_evt);
    m_entries = m_intree->GetEntries();
    
//    if(m_infile){
//        cout << "ERROR : Could not open file (NULL): " << infilename << endl;
//        exit(0);
//    }
    
    if(!m_infile->IsOpen()){
        cout << "ERROR : Could not open file : " << infilename << endl;
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

    int number_of_lines = 0;
    std::string line;
    std::ifstream linecount(m_oalistname.c_str());
    while (std::getline(linecount, line)) number_of_lines++;
    
    for(int i = 0; i < 11; i++){
        string line1 = GetFileName(i);
        cout << "A1)" << line1 << endl;
    }
    
//    for(int i = 0; i < m_entries; i++){
//     
//    
//        
//        
//        
//    }
    
    
    string line;
    ifstream myfile (m_oalistname.c_str());
    if (myfile.is_open())
    {
        int counter = 0;
        while ( getline (myfile,line) )
        {
            cout << "B1)" << line << endl;
            counter++;
            if(counter == 10) break;
        }
        myfile.close();
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