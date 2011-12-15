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

//#include    "common.h"


using namespace std;

vector<string> break_string(
        const char *str,
        const char *separates = ",",
        const char *encloses = "\""){

    vector<string> ans;
    string tmp, sep(separates), enc(encloses);
    int s = strlen(str);
    bool infield = false;
    
    sep+= "\0";

    for(int i = 0; i <= s; ++i){
        //if(str[i] == enclose){
        if(enc.find(str[i]) != string::npos){
            infield = !infield;
        }
        else{
            if(infield){
                tmp.push_back(str[i]);
            }
            else{
                //if(str[i] != separate && str[i] != '\0'){
                if(sep.find(str[i]) == string::npos && str[i] != '\0'){
                    tmp.push_back(str[i]);
                }
                else{
                    ans.push_back(tmp);
                    tmp.clear();
                }
            }
        }
    }
    return ans;
}

int fread_a_line(FILE* fp, string &ans){
    char tmp;
    int count = 0;
    ans.clear();
    while((tmp = fgetc(fp)) != EOF){
        if(tmp == '\r' || tmp == '\n'){
            //cout << ans << endl;
            if(count > 0)
                return 1;
        }
        if(tmp == -17 || tmp == -69 || tmp == -65){
            //printf("got ctrl :%c[%d]\n", tmp, tmp);
            continue;
        }
        ans.push_back(tmp);
        count ++;
    }
    return 0;
}

class csvreader{
    public:
        vector<string> head;
        vector<vector<string> >data;
        /**
         * @brief 
         * read a csv file
         *
         * @param filename
         * just filename
         * @param separate
         * the separator between fields
         * @param enclose
         *
         * @return 
         */
        bool read(const char *filename,
                const char *separate = ",",
                const char *enclose = ""){
            FILE *fp = fopen(filename, "r");
            string buf;
            fread_a_line(fp, buf);
            head = break_string(buf.c_str(), separate, enclose);
            rows = 0;
            while(fread_a_line(fp, buf)){
                ++rows;
                data.push_back(break_string(buf.c_str(), separate, enclose));
            }
        }
        /**
         * @brief 
         * show the csv table
         */
        void show(){
            int s1 = head.size();
            for( int i = 0 ; i < s1 ; ++i )
            {
                cout << head[i] << '|';
            }
            cout << "\n--------------------------\n";

            s1 = data.size();
            for( int i = 0 ; i < s1 ; ++i )
            {
                int s2 = data[i].size();
                for( int j = 0 ; j < s2 ; ++j )
                {
                    cout << data[i][j] << "|";
                }
                cout << "\n--------------------------\n";
            }
        }
        /**
         * @brief 
         *
         * @param field
         *
         * @return 
         * get a field name's index
         * -1 if not found the field name
         */
        int getFieldIndex(const char* field)const{
            int s = head.size();
            for( int i = 0 ; i < s ; ++i )
            {
                if(strcmp(head[i].c_str(), field) == 0)
                    return i;
                else{
                    //printf("%s(%d)!=%s(%d)\n", head[i].c_str(), strlen(head[i].c_str()), field,strlen(field) );
                }
            }
            return -1;
        }
        /**
         * @brief 
         *
         * @param index
         *
         * @return 
         * the index-th column
         */
        vector<string> getCol(int index)const{
            vector<string>ans;
            for(int i = 0; i < rows; ++i){
                ans.push_back(data[i][index]);
            }
            return ans;
        }
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
        vector<string> getCol(const char* field)const{
            int index = getFieldIndex(field);
            if(index > -1)
                return getCol(index);
            else return vector<string>(0);
        }
        /**
         * @brief 
         *
         * @param index
         *
         * @return 
         * the index-th row of the data
         */
        vector<string> getRow(int index){
            return data[index];
        }
        /**
         * @brief 
         * get the row count
         *
         * @return 
         * the row count
         */
        int rowCount(){
            return rows;
        }
    private:
        int rows;
};


#endif /* ifndef CSVREADER_H */

