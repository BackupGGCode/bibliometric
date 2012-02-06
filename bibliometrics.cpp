#include <iostream>
#include <cstdlib> 
#include <cstdio>
#include "csvreader.h"
#include    <string>

#include    "bibliometrics.h"

using namespace std;

int main(int argc, char *argv[])
{
    string datafile, out_directory("./");
    csvreader rr;

    if(argc > 1){
        datafile = argv[1];
    }
    else{
        printf("Please tell me the input file name:\n");
        cin >> datafile;
    }
    if(argc > 2){
        out_directory = argv[2];
    }


    rr.read(datafile.c_str(), "\t", "");
    if(out_directory[out_directory.size() - 1] != '/')
        out_directory += "/";

    cout << "reading file :" << datafile << endl;
    cout << "output folder : " << out_directory << endl;

    string  author_tc_filename                = out_directory+"author_tc.txt";
    string  author_count_filename             = out_directory+"author_count.txt";
    string  year_count_filename               = out_directory+"year_count.txt";
    string  document_type_filename            = out_directory+"document_type.txt";
    string  year_pages_filename               = out_directory+"year_pages.txt";
    string  year_tc_filename                  = out_directory+"year_tc.txt";
    string  country_tc_filename               = out_directory+"country_tc.txt";
    string  year_nr_filename                  = out_directory+"year_nr.txt";
    string  year_subject_count_filename       = out_directory+"year_subject_count.txt";
    string  year_publication_count_filename   = out_directory+"year_publication_count.txt";
    string  publication_tc_count_filename     = out_directory+"publication_tc_count.txt";
    string  year_country_count_filename       = out_directory+"year_country_count.txt";
    string  year_cooperation_count_filename   = out_directory+"year_cooperation_count.txt";
    string  country_relationship_filename     = out_directory+"country_relationship.txt";
    string  author_relationship_filename      = out_directory+"author_relationship.txt";
    string  institute_tc_filename             = out_directory+"institute_tc.txt";
    string  institute_relationship_filename   = out_directory+"institute_relationship.txt";
    string  year_institute_count_filename     = out_directory+"year_institute_count.txt";
    string  year_keyword_DE_filename          = out_directory+"year_keyword_DE.txt";
    string  year_keyword_ID_filename          = out_directory+"year_keyword_ID.txt";
    string  year_keyword_DE_and_ID_filename   = out_directory+"year_keyword_DE_and_ID.txt";
    string  year_title_word_filename          = out_directory+"year_title_word.txt";

    author_cited(rr,"AU", ";","TC",fopen(author_tc_filename.c_str(),"w"));
    year_author_count(rr,"PY", "AU", ";", fopen(author_count_filename.c_str(), "w"));

    /*总量分析*/
    year_count(rr, "PY", fopen(year_count_filename.c_str(),"w"));
    year_document_type(rr,"PY","DT",fopen(document_type_filename.c_str(),"w"));
    year_pages(rr,"PY","PG",fopen(year_pages_filename.c_str(),"w"));
    year_pages(rr,"PY","TC",fopen(year_tc_filename.c_str(),"w"));
    year_pages(rr,"PY","NR",fopen(year_nr_filename.c_str(),"w"));

    /*领域分析*///same as publications
    /*year_subject == year_keyword*/
    year_subject(rr,"PY","SC", ";",fopen(year_subject_count_filename.c_str(), "w"));


    /*期刊分析*/
    year_publication_count(rr,"PY","SO",fopen(year_publication_count_filename.c_str(), "w"));
    publication_tc_count(rr,"SO","TC",fopen(publication_tc_count_filename.c_str(),"w"));

    /*地域分布及国际合作*/
    year_country_count(rr,"PY","C1", ";", "[]",fopen(year_country_count_filename.c_str(),"w"));
    year_cooperation_count(rr,"PY","C1", ";", "[]", fopen(year_cooperation_count_filename.c_str(),"w"));
    country_cited(rr,"C1", ";", "[]","TC", fopen(country_tc_filename.c_str(),"w"));
    country_relationship(rr,"C1", ";", "[]", fopen(country_relationship_filename.c_str(),"w"));
    author_relationship(rr,"AU", ";", fopen(author_relationship_filename.c_str(),"w"));

    /* 机构与合作*/
    institute_cited(rr,"C1",";","[]","TC",fopen(institute_tc_filename.c_str(),"w"));
    institute_relationship(rr,"C1", ";", "[]", fopen(institute_relationship_filename.c_str(),"w"));
    year_institute_count(rr,"PY","C1",";","[]",fopen(year_institute_count_filename.c_str(), "w"));

    /*关键词分析*/
    year_keyword(rr,"PY","DE", ";",fopen(year_keyword_DE_filename.c_str(),"w"));
    year_keyword(rr,"PY","ID", ";",fopen(year_keyword_ID_filename.c_str(),"w"));
    year_dual_keyword(rr,"PY", "DE", ";", "ID", ";", fopen(year_keyword_DE_and_ID_filename.c_str(),"w"));

    year_keyword(rr,"PY","TI", "\'()\",. ;",fopen(year_title_word_filename.c_str(),"w"));

    //year_country_count(rr,"PY","C1", ";", "[]",stdout);
    //year_field_count(rr,"PY","SO",fp);
    //fclose(fp);
    return 0;
} 
