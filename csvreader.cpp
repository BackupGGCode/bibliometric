
#include "csvreader.h"


using namespace std;

vector<string> break_string(
        const char *str,
        const char *separators ,
        const char *delimiters ){

    vector<string> ans;
    string tmp, sep(separators), enc(delimiters);
    int s = strlen(str);
    bool infield = false;
    
    sep+= "\0";

    for(int i = 0; i <= s; ++i){
        //if(str[i] == delimiter){
        if(enc.find(str[i]) != string::npos){
            infield = !infield;
        }
        else{
            if(infield){
                tmp.push_back(str[i]);
            }
            else{
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
    if(tmp.size() > 0){
        ans.push_back(tmp);
        tmp.clear();
    }
    return ans;
}

int fread_a_line(FILE* fp, string &ans){
    char tmp;
    int count = 0;
    ans.clear();
    while(!feof(fp)){
        tmp = fgetc(fp);
        if(tmp == '\r' || tmp == '\n'){
            //cout << ans << endl;
            if(count > 0)
                return 1;
            else{
                continue;
            }
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

csvreader::csvreader():
            indexed_column_id(0)
        {
            
        }

        vector<string>& csvreader::operator[](int i){
            return data[i];
        }
        vector<string>& csvreader::operator[](const string &str){
            return data[row_index[str]];
        }
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
        bool csvreader::read(const char *filename,
                const char *separator,
                const char *delimiter,
                bool hasHeader
                ){
            FILE *fp = fopen(filename, "r");
            string buf;
            if(hasHeader){
                fread_a_line(fp, buf);
                header = break_string(buf.c_str(), separator, delimiter);
            }
            rows = 0;
            while(fread_a_line(fp, buf)){
                ++rows;
                data.push_back(break_string(buf.c_str(), separator, delimiter));
            }
        }
        bool csvreader::write(const char * filename,
                   const char   separator = ',',
                   const char   delimiter = '\"',
                   bool hasHeader = true
                ){
            write(fopen(filename, "w"), separator, delimiter, hasHeader);
            return true;
        }
        bool csvreader::write(FILE * fp,
                   const char   separator = ',',
                   const char   delimiter = '\"',
                   bool hasHeader = true
                ){
            if(hasHeader){
                for(int i = 0; i < header.size(); ++i){
                    if(i) fprintf(fp, "%c", separator);
                    if(delimiter != '\0')fprintf(fp, "%c", delimiter);
                    fprintf(fp, "%s", header[i].c_str());
                    if(delimiter != '\0')fprintf(fp, "%c", delimiter);
                }
                fprintf(fp, "\n");
            }
            for( int i = 0 ; i < data.size(); ++i )
            {
                for( int j = 0 ; j < data[i].size(); ++j )
                {
                    if(j) fprintf(fp, "%c", separator);
                    if(delimiter != '\0')fprintf(fp, "%c", delimiter);
                    fprintf(fp, "%s", data[i][j].c_str());
                    if(delimiter != '\0')fprintf(fp, "%c", delimiter);
                }
                fprintf(fp, "\n");
            }
            return true;
        }

        /**
         * @brief 
         * show the csv table
         */
        void csvreader::show(){
            int s1 = header.size();
            for( int i = 0 ; i < s1 ; ++i )
            {
                cout << header[i] << '|';
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
        int csvreader::getFieldIndex(const char* field)const{
            int s = header.size();
            for( int i = 0 ; i < s ; ++i )
            {
                if(strcmp(header[i].c_str(), field) == 0)
                    return i;
                else{
                    //printf("%s(%d)!=%s(%d)\n", header[i].c_str(), strlen(head[i].c_str()), field,strlen(field) );
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
        vector<string> csvreader::getCol(int index)const{
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
        vector<string> csvreader::getCol(const char* field)const{
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
        vector<string> csvreader::getRow(int index){
            return data[index];
        }
        /**
         * @brief 
         * get the row count
         *
         * @return 
         * the row count
         */
        int csvreader::rowCount(){
            return rows;
        }


        void csvreader::rebuild_row_index(){
            for(int i = 0; i < data.size(); ++i){
                //row_index[data[indexed_column_id]] = i;
            }
        }
