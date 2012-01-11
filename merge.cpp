/* *************************
 * Author: xg1990
 * Created Time:  
 * LastModified:  Wed 11 Jan 2012 02:48:29 PM CST
 * C File Name: 
 * ************************/
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "csvreader.h"
using namespace std;
#define out(v) cerr << #v << ": " << (v) << endl

int main(int argc, char *argv[])
{
    string output, buf;
    vector<string> input;
    bool header=false;
    FILE *fi, *fo;
    for( int i = 1 ; i < argc ; ++i )
    {
        if(strcmp(argv[i], "-o") == 0){
            output = argv[i + 1];
            ++i;
        }
        else{
            input.push_back(argv[i]);
        }
    }
    while(input.size() == 0){
        printf("Please give me the input file names:\
                \n(type \"END\" to stop input; filename cannot contain SPACE)\n");
        while(1){
            cin >> buf;
            if(buf == "END")break;
            input.push_back(buf);
        }
    }
    if(output.size() == 0){
        printf("Please give me the output file name:\n");
        cin >> output;
    }
/* start merging! */
    fo = fopen(output.c_str(), "w");
    printf("output to %s\n", output.c_str());
    if(!fo){
        printf("cannot open file %s\n", output.c_str());
        getchar();
        exit(0);
    }
    for( int i = 0 ; i < input.size() ; ++i )
    {
        printf("reading %s\n", input[i].c_str());
        fi = fopen(input[i].c_str(), "r");
        if(!fi){
            printf("cannot open file %s\n", input[i].c_str());
            getchar();
            exit(0);
        }
        fread_a_line(fi, buf);
        if(!header){
            fprintf(fo, "%s\n", buf.c_str());
            header = true;
        }
        while(fread_a_line(fi, buf)){
            fprintf(fo, "%s\n", buf.c_str());
        }
        fclose(fi);
    }
    fclose(fo);
    printf("finished\n");
    return 0;
}
