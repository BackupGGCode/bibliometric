/*
 * $Id$
 */
#ifndef CSVREADER_H
#  define CSVREADER_H

#include    "iostream"
#include    "cstdio"
#include    "cstdlib"
#include    "cstring"
#include    "string"
#include    "vector"
#include <map>

//#include    "common.h"


using namespace std;

vector<string> break_string(
        const char *str,
        const char *separators = ",",
        const char *delimiters = "\"");
int fread_a_line(FILE* fp, string &ans);
class csvreader{
    public:
        vector<string> header;
        vector<vector<string> >data;

        csvreader();

        vector<string>& operator[](int i);
        vector<string>& operator[](const string &str);
        /**
         * @brief 
         * read a csv file
         *
         * @param filename
         * just filename
         * @param separator
         * the separator between fields
         * @param delimiter
         *
         * @return 
         */
        bool read(const char *filename,
                const char *separator = ",",
                const char *delimiter = "\"",
                bool hasHeader = true
                );
        bool write(const char * filename,
                   const char   separator,
                   const char   delimiter,
                   bool hasHeader
                );
        bool write(FILE * fp,
                   const char   separator,
                   const char   delimiter,
                   bool hasHeader
                );
        /**
         * @brief 
         * show the csv table
         */
        void show();        /**
         * @brief 
         *
         * @param field
         *
         * @return 
         * get a field name's index
         * -1 if not found the field name
         */
        int getFieldIndex(const char* field)const;
        /**
         * @brief 
         *
         * @param index
         *
         * @return 
         * the index-th column
         */
        vector<string> getCol(int index)const;
        /**
         * @brief 
         *
         * @param field
         *
         * the field name
         *
         * @return 
         *
         * the col with the name of 'field'
         */
        vector<string> getCol(const char* field)const;
        /**
         * @brief 
         *
         * @param index
         *
         * @return 
         * the index-th row of the data
         */
        vector<string> getRow(int index);
        /**
         * @brief 
         * get the row count
         *
         * @return 
         * the row count
         */
        int rowCount();
    private:
        int rows;
        int indexed_column_id;
        map<string, int> row_index;

        void rebuild_row_index();
};


#endif /* ifndef CSVREADER_H */

