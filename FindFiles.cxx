#include <iostream>
#include <string>
#include "TFile.h"

using namespace std;

class FindFiles {
public:
    FindFiles(std::string infilename, std::string oa_listname);
    ~FindFiles();
    
    void Run(){;}
    
private:
    TFile * m_infile;
    std::string m_oalistname;
    std::string m_outfilename;
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
    cout << "outdir = " << outdir << endl;
    cout << "m_outfilename = " << m_outfilename << endl;
//    m_outfilename
    
//    m_outfilename
    
}

FindFiles::~FindFiles(){
    if(m_infile->IsOpen()) m_infile->Close();
    if(m_infile) delete m_infile;
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
    
    delete find;
    
    return 1;
}