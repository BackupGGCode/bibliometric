#include "bibliometrics.h"

void trim(string & str){
    int i = 0, j = str.size() - 1;
    while(str[i] == ' ') ++i;
    while(str[j] == ' ') --j;
    str = str.substr(i,j - i + 1);
}
void to_lower(string & str){
    for(int i = 0; i < str.size(); ++i)
        if(str[i] <= 'Z' && str[i] >= 'A')
            str[i] -= 'A' - 'a';
}
void to_upper(string & str){
    for(int i = 0; i < str.size(); ++i)
        if(str[i] <= 'z' && str[i] >= 'a')
            str[i] += 'A' - 'a';
}


/**
 * @brief 
 *
 * convert string to int
 *
 * @param s
 *
 * @return 
 */
int string2int(const string &s){
    int ans;
    sscanf(s.c_str(), "%d", &ans);
    return ans;
}


void author_cited(
        const csvreader & csv,
        const char *author_field,
        const char * author_separator,
        const char * total_cited_field,
        FILE*out){
    /* 
     * check the csv data
     */
    int author_index = csv.getFieldIndex(author_field);
    int total_cited_index = csv.getFieldIndex(total_cited_field);

    if(author_index < 0 || total_cited_index < 0 || strlen(author_field) < 1 || strlen(total_cited_field) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector<string> authors = csv.getCol(author_index);
        vector<string> cited = csv.getCol(total_cited_index);

        map<string, int>author_total_cited;//record the answers;
        map<string, int>first_author_total_cited;//record the answers;
        map<string, double>author_mean_cited;//record the answers;
        map<string, double>first_author_mean_cited;//record the answers;


        int s = authors.size();
        for( int i = 0 ; i < s ; ++i )
        {
            vector<string>tmp = break_string(authors[i].c_str(),author_separator,"");
            trim(tmp[0]);
            to_upper(tmp[0]);

            first_author_total_cited[tmp[0]] += string2int(cited[i]);
            first_author_mean_cited[tmp[0]] += 1;

            for(int j = 0 ; j < tmp.size(); ++j){
                //fprintf(out, "%s ++", tmp[j].c_str());
                trim(tmp[j]);
                to_upper(tmp[j]);
                author_total_cited[tmp[j]] += string2int(cited[i]);
                author_mean_cited[tmp[j]] += 1;
            }
        }
        /********************
         *  start output
         *
         *******************/
        fprintf(out, "THIS INCLUDE :\n\tAUTHOR TOTAL CITED RANK\n\tFIRST AUTHOR TOTAL CITED RANK\n\tAUTHOR MEAN CITED RANK\n\tFIRST AUTHOR MEAN CITED RANK\n");

        /*
         * author_total_cited
         */
        fprintf(out, "AUTHOR TOTAL CITED RANK:\n");
        vector<pair<string, int> > ans_copy(author_total_cited.begin(), author_total_cited.end());
        sort(ans_copy.begin(), ans_copy.end(), greater_second<string, int>());
        for(int i = 0; i < ans_copy.size(); ++i){
            fprintf(out, "%s\t%d\n", ans_copy[i].first.c_str(), ans_copy[i].second);
        }
        /*
         * first_author_total_cited
         */
        fprintf(out, "FIRST AUTHOR TOTAL CITED RANK:\n");
        vector<pair<string, int> > ans_copy1(first_author_total_cited.begin(), first_author_total_cited.end());
        sort(ans_copy1.begin(), ans_copy1.end(), greater_second<string, int>());
        for(int i = 0; i < ans_copy1.size(); ++i){
            fprintf(out, "%s\t%d\n", ans_copy1[i].first.c_str(), ans_copy1[i].second);
        }
        /*
         * author_mean_cited
         */
        fprintf(out, "AUTHOR MEAN CITED RANK:\n");
        for(map<string, double>::iterator it = author_mean_cited.begin(); it != author_mean_cited.end(); ++it){
            it->second = author_total_cited[it->first] / it -> second;
        }
        vector<pair<string, double> >ans_copy2(author_mean_cited.begin(), author_mean_cited.end());
        sort(ans_copy2.begin(), ans_copy2.end(), greater_second<string, double>());
        for(int i = 0; i < ans_copy2.size(); ++i){
            fprintf(out, "%s\t%f\n", ans_copy2[i].first.c_str(), ans_copy2[i].second);
        }
        /*
         * first_author_mean_cited
         */
        fprintf(out, "FIRST AUTHOR MEAN CITED RANK:\n");
        for(map<string, double>::iterator it = first_author_mean_cited.begin(); it != first_author_mean_cited.end(); ++it){
            it->second = first_author_total_cited[it->first] / it -> second;
        }
        vector<pair<string, double> >ans_copy3(first_author_mean_cited.begin(), first_author_mean_cited.end());
        sort(ans_copy3.begin(), ans_copy3.end(), greater_second<string, double>());
        for(int i = 0; i < ans_copy3.size(); ++i){
            fprintf(out, "%s\t%f\n", ans_copy3[i].first.c_str(), ans_copy3[i].second);
        }
        fprintf(out, "END\n");
    }
    fclose(out);
}


string country_special_judge(const string & _cntry){
    string cntry = _cntry;
    to_upper(cntry);
    if(cntry.find("CHINA") != string::npos ||
            cntry.find("MACAO") != string::npos ||
            cntry.find("MACAU") != string::npos ||
            cntry.find("TAIWAN") != string::npos ||
            cntry.find("HONG KONG") != string::npos)
        return string("CHINA");
    else if(cntry.find("WALES") != string::npos ||
            cntry.find("ENGLAND") != string::npos ||
            cntry.find("NORTHERN IRELAND") != string::npos ||
            cntry.find("SCOTLAND") != string::npos)
        return string("UK");
    else if(cntry.find("BOSNIA") != string::npos)
        return string("BOSNIA & HERCEG");
    else if(cntry.find("SERBIA MONTENEG") != string::npos)
        return string("SERBIA MONTENEG");
    else if(cntry.find("VIETNAM") != string::npos)
        return string("VIETNAM");
    else if(cntry.find("USA") != string::npos ||
        cntry.find("CA ") != string::npos ||
        cntry.find("CO ") != string::npos ||
        cntry.find("CT ") != string::npos ||
        cntry.find("DC") != string::npos ||
        cntry.find("DE ") != string::npos ||
        cntry.find("FL ") != string::npos ||
        cntry.find("IA ") != string::npos ||
        cntry.find("IN ") != string::npos ||
        cntry.find("KY ") != string::npos ||
        cntry.find("LA ") != string::npos ||
        cntry.find("MA ") != string::npos ||
        cntry.find("MD ") != string::npos ||
        cntry.find("ME ") != string::npos ||
        cntry.find("MI ") != string::npos ||
        cntry.find("NC ") != string::npos ||
        cntry.find("NJ ") != string::npos ||
        cntry.find("NY ") != string::npos ||
        cntry.find("OH ") != string::npos ||
        cntry.find("PA ") != string::npos ||
        cntry.find("SC ") != string::npos ||
        cntry.find("TN") != string::npos ||
        cntry.find("TX ") != string::npos ||
        cntry.find("VA ") != string::npos ||
        cntry.find("VX ") != string::npos ||
        cntry.find("WA ") != string::npos)
        return string("USA");
    else
        return cntry;
}
/**
 * @brief 
 * fliter the characters within the delimiters 
 *
 * @param str
 * @param delimiters
 */
void fliter_string_by_delimiters(string & str, const string & delimiters){
    bool isin = false;
    int i, j;
    i = 0; while(i < str.size() && delimiters.find(str[i]) == string::npos) ++i;
    j = i + 1; while(j < str.size() && delimiters.find(str[j]) == string::npos) ++j;
    if(i >= str.size()) 
        return ;
    //cout << "fliter_string_by_delimiters(" << str << ", " << delimiters << "): (" << i << ", " << j - i + 1 << ")\n";
    str = str.replace(i,j - i + 1, "");
    fliter_string_by_delimiters(str,delimiters);
}

vector<string> get_institutes_from_address(
        const string & address,
        const char * address_separator, 
        const char * author_enclosor
        ){
    string buf = address;
    fliter_string_by_delimiters(buf, author_enclosor);
    vector<string> addresses = break_string(buf.c_str(),address_separator,author_enclosor);
    vector<string>ans ;
    //cout << buf <<endl << addresses.size() << endl;
    for(int i = 0; i < addresses.size(); ++i){
        //cout << "addresses[" << i << "]=" << addresses[i] << "\n";
        vector<string>tmp = break_string(addresses[i].c_str(), ",");
        //cout << "tmp[0]=" << tmp[0] << ":::::::::     \n";
        trim(tmp[0]);
        to_upper(tmp[0]);
        ans.push_back(tmp[0]);
    }
    return ans;
}

vector<string> get_countries_from_address(
        const string & address,
        const char * address_separator, 
        const char * address_enclosor
        ){
    vector<string> addresses = break_string(address.c_str(),address_separator,address_enclosor);
    vector<string>ans ;
//    qDebug() << address.c_str() <<endl << addresses.size() << endl;
    for(int i = 0; i < addresses.size(); ++i){
//        qDebug() << addresses[i].c_str() << ":::::::::::\n";
        vector<string>tmp = break_string(addresses[i].c_str(), ",");
        ans.push_back(country_special_judge(tmp[tmp.size() - 1]));
//        qDebug() << tmp[tmp.size() - 1].c_str() << ":::::::::     \n";
    }
//    qDebug()<< endl << endl;
    return ans;
}

string get_rp_name(string RP){
    int p = -1;
    for_i(i, RP.size()){
        if(RP[i] == '('){
            p = i;break;
        }
    }
    RP = RP.substr(0,p);
    trim(RP);
    to_upper(RP);
    return RP;
}

void showstrings(vector<string> str){
    int s = str.size();
    for( int i = 0 ; i < s ; ++i )
    {
        cout << str[i] << endl;
    }
    cout << endl;
}

/**
 * @brief 
 * compute the relationship between countries
 *
 * if author A and B from country X and Y(X != Y)
 * appeared in the same paper which finished by n authors,
 * the relationship index between X and Y will increase 1 / n
 *
 * @param csv
 * the csv data;
 *
 * @param address_field
 * @param address_separator
 * @param address_enclosor
 * @param 
 */
void country_relationship(
        const csvreader & csv, 
        const char *address_field, 
        const char *address_separator, 
        const char *address_enclosor, 
        FILE *out
        ){
    /* 
     * check the csv data
     */
    int address_index = csv.getFieldIndex(address_field);

    if(address_index < 0 
             || strlen(address_field) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector<string>  addresses = csv.getCol(address_index);
        vector<string>countries;
        map<pair<string, string>, double> rel;

        for(int i = 0; i < addresses.size(); ++i){

            if(addresses[i].size() < 1)
                continue;
            countries = get_countries_from_address(
                    addresses[i],address_separator,address_enclosor);
            /* trim each country    */
//            qDebug() << addresses[i].c_str() <<  countries.size();
            for(int j = 0; j < countries.size(); ++j)
            {
                trim(countries[j]);
//                qDebug() << countries[j].c_str();
            }

            /*
             * compute rel
             */
            for(int j = 0; j < countries.size(); ++j)
            {
                for( int k = j + 1 ; k < countries.size() ; ++k )
                {
                    if(countries[j] > countries[k])
                        rel[pair<string, string>(countries[j], countries[k])] += 1.00 / countries.size();
                    else if(countries[j] < countries[k])
                        rel[pair<string, string>(countries[k], countries[j])] += 1.00 / countries.size();
                }
            }
        }
        /*
         * print out
         */

        /* 
         * country count 
         * */
        vector<pair<pair<string,string> , double> > rel_copy(
                rel.begin(), rel.end());
        sort(rel_copy.begin(), rel_copy.end(), 
                greater_second<pair<string, string>, double>());

        /* header */

        /* content */
        for(int i = 0; i < rel_copy.size(); ++i){
            fprintf(out, "%s\t%s\t%f\n" 
                    ,rel_copy[i].first.first.c_str()
                    ,rel_copy[i].first.second.c_str()
                    ,rel_copy[i].second
                    );
        }
        fprintf(out, "OUT\n");
    }
}

void year_author_count(
        const csvreader & csv,
        const char *year_field, 
        const char *author_field,
        const char * author_separator,
        const char *rp_field,
        FILE *out
        ){
    /* 
     * check the csv data
     */
    int year_index = csv.getFieldIndex(year_field);
    int author_index = csv.getFieldIndex(author_field);
    int rp_index = csv.getFieldIndex(rp_field);

    if(     year_index < 0 ||
            author_index < 0 || 
            rp_index < 0 || 
            strlen(author_field) < 1 ||
            strlen(year_field) < 1||
            strlen(rp_field) < 1
            ){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector< string> years = csv.getCol(year_index);
        vector<string> authors = csv.getCol(author_index);
        vector<string> rp = csv.getCol(rp_index);

        map<string, int>author_total_pub;//record the answers;
        map<string, int>first_author_total_pub;//record the answers;

        map<string, int>author_total_ip_pub;//record the answers;
        map<string, int>author_total_cp_pub;//record the answers;

        map<string, map<int, int> > author_year_count;
        map<int, map<string, int> > year_author_count;

        map<int, int>year_author_per_paper;
        map<int, int>year_paper_count;
        map<int, int>year_author_net_count;

        map<string, int>rp_author_total_pub;
        map<int, map<string, int> > year_rp_author_count;

        set<string> author_names;


        int year, min_year, max_year;

        year = min_year = max_year = string2int(years[0]);
        for( int i = 0 ; i < years.size() ; ++i )
        {
            year = string2int(years[i]);
            min_year = min(min_year, year);
            max_year = max(max_year, year);

            vector<string>tmp = break_string(authors[i].c_str(),author_separator,"");
            string rp_au_name = get_rp_name(rp[i]);
            for(int j = 0 ; j < tmp.size(); ++j){
                trim(tmp[j]);
                to_upper(tmp[j]);
                author_names.insert(tmp[j]);
            }

            /* first author total publication */
            first_author_total_pub[tmp[0]] += 1;
            rp_author_total_pub[rp_au_name] += 1;

            /* author total publication */
            for(int j = 0 ; j < tmp.size(); ++j){
                author_total_pub[tmp[j]] += 1;
            }

            /* author's ip and cp*/
            if(tmp.size() > 1){
                for(int j = 0 ; j < tmp.size(); ++j){
                    author_total_cp_pub[tmp[j]] += 1;
                }
            }
            else{
                for(int j = 0 ; j < tmp.size(); ++j){
                    author_total_ip_pub[tmp[j]] += 1;
                }
            }
            year_rp_author_count[year][rp_au_name] += 1;

            /* author-year count */
            for(int j = 0 ; j < tmp.size(); ++j){
                author_year_count[tmp[j]][year] += 1;
                year_author_count[year][tmp[j]] += 1;
            }

            year_author_per_paper[year] += tmp.size();
            year_paper_count[year] += 1;

        }
        /********************
         *  start output
         *
         *******************/
        fprintf(out, "THIS INCLUDE :\n\tAnnual statistical\n\tAuthor statistical\n");
        /*
         * year's count
         */
        fprintf(out, "Annual STAT:\n");
        fprintf(out, "year\trp au count\tau count\tau per paper\n");
        for(int y = min_year; y <= max_year;++y){
            fprintf(out, "%d\t%d\t%d\t%f\n",
                    y,
                    year_rp_author_count[y].size(),
                    year_author_count[y].size(), 
                    1.0 * year_author_per_paper[y] / year_paper_count[y]);
        }
        fprintf(out, "\n\n\n");

        /*
         * author info
         */
        fprintf(out, "AUTHORS' statistical :\n");
        fprintf(out, "author\trp total public\ttotal public\tfirst au public\tip pub\tcp pub");
        for(int y = min_year; y <= max_year;++y){
            fprintf(out, "\t%d", y);
        }
        fprintf(out, "\n");

        for(set<string>::iterator it = author_names.begin();
                it != author_names.end();
                ++it){
            fprintf(out, "%s\t%d\t%d\t%d\t%d\t%d",
                    it->c_str(), 
                    rp_author_total_pub[*it],
                    author_total_pub[*it],
                    first_author_total_pub[*it],
                    author_total_ip_pub[*it],
                    author_total_cp_pub[*it]
                    );
            for(int y = min_year; y <= max_year;++y){
                fprintf(out, "\t%d", author_year_count[*it][y]);
            }
            fprintf(out, "\n");
        }
        /**************************/
        fprintf(out, "END\n");
    }
    fclose(out);
}

void author_relationship(
        const csvreader & csv, 
        const char *author_field, 
        const char *author_separator, 
        FILE *out
        ){
    /* 
     * check the csv data
     */
    int author_index = csv.getFieldIndex(author_field);

    if(author_index < 0 || strlen(author_field) < 1 ){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector<string> authors = csv.getCol(author_index);
        map<pair<string, string>, int> rel;
        map<pair<string, string>, int> first_rel;

        int s = authors.size();
        for( int i = 0 ; i < s ; ++i )
        {
            vector<string>tmp = break_string(authors[i].c_str(),author_separator,"");
            for(int j = 0 ; j < tmp.size(); ++j){
                trim(tmp[j]);
                to_upper(tmp[j]);
            }

            for(int j = 0 ; j < tmp.size(); ++j){
                if(tmp[j] > tmp[0])
                    first_rel[pair<string, string>(tmp[j], tmp[0])] += 1;
                else if(tmp[j] < tmp[0])
                    first_rel[pair<string, string>(tmp[0], tmp[j])] += 1;
                for( int k = j + 1 ; k < tmp.size() ; ++k )
                {
                    if(tmp[j] > tmp[k])
                        rel[pair<string, string>(tmp[j], tmp[k])] += 1;
                    else if(tmp[j] < tmp[k])
                        rel[pair<string, string>(tmp[k], tmp[j])] += 1;
                }
            }
        }
        /********************
         *  start output
         *
         *******************/
        vector<pair<pair<string,string> , int> > rel_copy(
                rel.begin(), rel.end());
        sort(rel_copy.begin(), rel_copy.end(), 
                greater_second<pair<string, string>, int>());

        /* header */

        /* content */
        for(int i = 0; i < rel_copy.size(); ++i){
            fprintf(out, "%s\t%s\t%d\n" 
                    ,rel_copy[i].first.first.c_str()
                    ,rel_copy[i].first.second.c_str()
                    ,rel_copy[i].second
                    );
        }
        fprintf(out, "=========================\n===  first rel ============\n==========================\n");

        vector<pair<pair<string,string> , int> > first_rel_copy(
                first_rel.begin(), first_rel.end());
        sort(first_rel_copy.begin(), first_rel_copy.end(), 
                greater_second<pair<string, string>, int>());

        /* header */

        /* content */
        for(int i = 0; i < first_rel_copy.size(); ++i){
            fprintf(out, "%s\t%s\t%d\n" 
                    ,first_rel_copy[i].first.first.c_str()
                    ,first_rel_copy[i].first.second.c_str()
                    ,first_rel_copy[i].second
                    );
        }
        fprintf(out, "END\n");
    }
    fclose(out);
}


void publication_tc_count(
        const csvreader & csv,
        const char* pub_name_field,
        const char* total_cited_field,
        FILE* out){
    /* 
     * check the csv data
     */
    int pub_name_index = csv.getFieldIndex(pub_name_field);
    int total_cited_index = csv.getFieldIndex(total_cited_field);

    if(pub_name_index < 0 || total_cited_index < 0 || strlen(pub_name_field) < 1 || strlen(total_cited_field) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector<string> pub_names = csv.getCol(pub_name_index);
        vector<string> cited = csv.getCol(total_cited_index);

        map<string, int>pub_name_total_cited;//record the answers;
        map<string, int>pub_name_count;//record the answers;
        map<string, double>pub_name_mean_cited;//record the answers;

        for( int i = 0 ; i < pub_names.size() ; ++i )
        {
            trim(pub_names[i]);
            pub_name_total_cited[pub_names[i]] += string2int(cited[i]);
            pub_name_count[pub_names[i]] += 1;
        }
        /********************
         *  start output
         *
         *******************/
        fprintf(out, "THIS INCLUDE :\n\tPUBLICATION TOTAL CITED RANK\n\tPUBLICATION MEAN CITED RANK\n");

        /*
         * pub_name_total_cited
         */
        fprintf(out, "PUBLICATION TOTAL CITED RANK:\n");
        vector<pair<string, int> > ans_copy(pub_name_total_cited.begin(), pub_name_total_cited.end());
        sort(ans_copy.begin(), ans_copy.end(), greater_second<string, int>());
        fprintf(out, "PUB\tTP\tTC\tTC/TP\n");
        for(int i = 0; i < ans_copy.size(); ++i){
            fprintf(out, "%s\t%d\t%d\t%f\n", 
                    ans_copy[i].first.c_str(),
                    pub_name_count[ans_copy[i].first],
                    ans_copy[i].second, 
                    1.0 * ans_copy[i].second / pub_name_count[ans_copy[i].first]
                    );
        }
        fprintf(out, "END\n");
    }
}

void country_cited(
        const csvreader & csv, 
        const char *address_field, 
        const char *address_separator, 
        const char *address_enclosor, 
        const char * total_cited_field,
        FILE *out
        ){
    /* 
     * check the csv data
     */
    int address_index = csv.getFieldIndex(address_field);
    int total_cited_index = csv.getFieldIndex(total_cited_field);

    if(address_index < 0 || total_cited_index < 0 || strlen(address_field) < 1 || strlen(total_cited_field) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector<string>  addresses = csv.getCol(address_index);
        vector<string> cited = csv.getCol(total_cited_index);
        set<string> country_names;

        vector<string>countries;
        set<string>country_set;

        map<string, int> country_cp_tc;
        map<string, int> country_ip_tc;
        map<string, int> country_cp_count;
        map<string, int> country_ip_count;

        map<string, int>country_total_cited;//record the answers;
        map<string, int>first_country_total_cited;//record the answers;
        map<string, int>country_pub_count;//record the answers;
        map<string, int>first_country_pub_count;//record the answers;

        /* start review each item   */
        for( int i = 0 ; i < addresses.size() ; ++i ){
            if(addresses[i].size() < 1)
                continue;
            countries = get_countries_from_address(
                    addresses[i],address_separator,address_enclosor);
            /* check if it is collaborative */
            country_set.clear();
            for(int j = 0; j < countries.size(); ++j){
                trim(countries[j]);
                country_set.insert(countries[j]);
            }

            first_country_total_cited[countries[0]] += string2int(cited[i]);
            first_country_pub_count[countries[0]] += 1;
            for(set<string>::iterator it = country_set.begin();it != country_set.end(); ++it){
                country_total_cited[*it] += string2int(cited[i]);
                country_pub_count[*it] += 1;
                country_names.insert(*it);
            }

            /********************************/
            /*  count country-year-cp/ip    */
            /********************************/
            if(country_set.size() == 1){/* is independent publication*/
                for(set<string>::iterator it = country_set.begin();it != country_set.end(); ++it)
                {
                    country_ip_count[*it] += 1;
                    country_ip_tc[*it] += string2int(cited[i]);
                }
            }
            else if(country_set.size() > 1){
                /* is international collaborative publication */
                for(set<string>::iterator it = country_set.begin();it != country_set.end(); ++it)
                {
                    country_cp_count[*it] += 1;
                    country_cp_tc[*it] += string2int(cited[i]);
                }
            }
        }
        /*
         * print out
         */
        fprintf(out, "Country\tTOTAL CITED\tFIRST- TOTAL CITED\tMEAN CITED\tFIRST- MEAN CITED RANK\tIP Count\tIP total cited\tIP mean cited\tCP Count\tCP total cited\tCP mean cited\tCP / IP\n");

        for(set<string>::iterator it = country_names.begin(); it != country_names.end(); ++it){
            fprintf(out, "%s\t%d\t%d\t%f\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%f\n", it->c_str(), 
                    country_total_cited[*it], first_country_total_cited[*it],
                    (country_pub_count[*it]) ? 1.0 * country_total_cited[*it] / country_pub_count[*it] : 0,
                    (first_country_pub_count[*it]) ? 1.0 * first_country_total_cited[*it] / first_country_pub_count[*it] : 0,
                    country_ip_count[*it],
                    country_ip_tc[*it],
                    (country_ip_count[*it]) ? 1.0 * country_ip_tc[*it] / country_ip_count[*it] : 0,
                    country_cp_count[*it],
                    country_cp_tc[*it],
                    (country_cp_count[*it]) ? 1.0 * country_cp_tc[*it] / country_cp_count[*it] : 0,
                    (country_ip_count[*it]) ? 1.0 * country_cp_count[*it] / country_ip_count[*it] : 0
                    );
        }                 

        fprintf(out, "END(ATTENTION: some items havs no address won't be counted)\n");
    }
}

void year_cooperation_count(
        const csvreader & csv, 
        const char *year_field, 
        const char *address_field, 
        const char *address_separator, 
        const char *address_enclosor, 
        FILE *out
        ){
    /* 
     * check the csv data
     */
    int year_index = csv.getFieldIndex(year_field);
    int address_index = csv.getFieldIndex(address_field);

    if(year_index < 0 || address_index < 0 
            || strlen(year_field) < 1 || strlen(address_field) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector< string> years = csv.getCol(year_index);
        vector<string>  addresses = csv.getCol(address_index);

        vector<string>countries;
        set<string>country_set;

        map<string, map<int, int> > country_year_cp;//collaborative
        map<string, int> country_cp;
        map<string, map<int, int> > country_year_ip;//independent
        map<string, int> country_ip;
        map<int, int>year_cp, year_ip;

        int year, min_year, max_year;

        year = min_year = max_year = string2int(years[0]);

        /* start review each item   */
        for(int i = 0; i < years.size(); ++i){
            year = string2int(years[i]);
            min_year = min(min_year, year);
            max_year = max(max_year, year);

            if(addresses[i].size() < 1)
                continue;
            countries = get_countries_from_address(
                    addresses[i],address_separator,address_enclosor);
            /* check if it is collaborative */
            country_set.clear();
            for(int i = 0; i < countries.size(); ++i){
                trim(countries[i]);
                country_set.insert(countries[i]);
            }

            /*  count country-year-cp/ip    */
            //cout << year << ":" << country_set.size() << ":";
            if(country_set.size() == 1){/* is independent publication*/
                year_ip[year] += 1;
                for(set<string>::iterator it = country_set.begin();it != country_set.end(); ++it)
                {
                    //cout << *it << ",";
                    country_year_ip[*it][year] += 1;
                    country_ip[*it] += 1;
                }
            }
            else if(country_set.size() > 1){
                /* is international collaborative publication */
                year_cp[year] += 1;
                for(set<string>::iterator it = country_set.begin();it != country_set.end(); ++it)
                {
                    //cout << *it << ",";
                    country_year_cp[*it][year] += 1;
                    country_cp[*it] += 1;
                }
            }
            //cout << endl;
            //fprintf(out, "%s\n", countries[0].c_str());
        }
        /*
         * print out
         */

        /* 
         * country year cp
         * */
        fprintf(out, "cooperation publication \n");
        vector<pair<string, int> > country_cp_copy(
                country_cp.begin(), country_cp.end());
        sort(country_cp_copy.begin(), country_cp_copy.end(), 
                greater_second<string, int>());

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < country_cp_copy.size(); ++i){
            fprintf(out, "%s\t", country_cp_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", country_year_cp[country_cp_copy[i].first][year]);
            }
            fprintf(out, "%d\n", country_cp_copy[i].second);
        }
        /* sum */
        fprintf(out, "year_sum\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t", year_cp[year]);
        }
        fprintf(out, "\n");

        /* 
         * country year ip
         * */
        fprintf(out, "independent publication \n");
        vector<pair<string, int> > country_ip_copy(
                country_ip.begin(), country_ip.end());
        sort(country_ip_copy.begin(), country_ip_copy.end(), 
                greater_second<string, int>());

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < country_ip_copy.size(); ++i){
            fprintf(out, "%s\t", country_ip_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", country_year_ip[country_ip_copy[i].first][year]);
            }
            fprintf(out, "%d\n", country_ip_copy[i].second);
        }
        /* sum */
        fprintf(out, "year_sum\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t", year_ip[year]);
        }
        fprintf(out, "\n");

        fprintf(out, "END(ATTENTION: some items havs no address won't be counted)\n");
    }
}


void year_count(
        const csvreader &csv,
        const char * year_field_name,
        FILE *out){
    int index = csv.getFieldIndex(year_field_name);
    if(index < 0 || strlen(year_field_name) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        int year, min_year, max_year;
        map<int, int> ans;
        vector<string> years = csv.getCol(index);
        int s = years.size();
        year = min_year = max_year = string2int(years[0]);
        for(int i = 0; i < s; ++i){
            year = string2int(years[i]);
            min_year = min(min_year, year);
            max_year = max(max_year, year);
            ans[year] += 1;
        }
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t%d\n", year, ans[year]);
        }
        fprintf(out, "END\n");
    }
}

void year_institute_count(
        const csvreader & csv, 
        const char *year_field, 
        const char *address_field, 
        const char *address_separator, 
        const char *author_enclosor, 
        FILE *out
        ){
    /* 
     * check the csv data
     */
    int year_index = csv.getFieldIndex(year_field);
    int address_index = csv.getFieldIndex(address_field);

    if(year_index < 0 || address_index < 0 
            || strlen(year_field) < 1 || strlen(address_field) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector< string> years = csv.getCol(year_index);
        vector<string>  addresses = csv.getCol(address_index);
        vector<string>institutes;
        map<string, map<int, int> > institute_year_count;
        map<string, int> institute_count;
        map<string, map<int, int> > first_institute_year_count;
        map<string, int> first_institute_count;
        int year, min_year, max_year;

        year = min_year = max_year = string2int(years[0]);
        for(int i = 0; i < years.size(); ++i){
            year = string2int(years[i]);
            min_year = min(min_year, year);
            max_year = max(max_year, year);

            if(addresses[i].size() < 1)
                continue;
            institutes = get_institutes_from_address(
                    addresses[i],address_separator,author_enclosor);
            removeDuplicates(institutes);

            //fprintf(out, "%s\n", countries[0].c_str());

            first_institute_year_count[institutes[0]][year] += 1;
            first_institute_count[institutes[0]] += 1;

            for(int j = 0; j < institutes.size(); ++j)
            {
                institute_year_count[institutes[j]][year] += 1;
                institute_count[institutes[j]] += 1;
            }
        }
        /*
         * print out
         */

        /* 
         * institute count 
         * */
        fprintf(out, "all institutes count \n");
        vector<pair<string, int> > institute_count_copy(
                institute_count.begin(), institute_count.end());
        sort(institute_count_copy.begin(), institute_count_copy.end(), 
                greater_second<string, int>());

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < institute_count_copy.size(); ++i){
            fprintf(out, "%s\t", institute_count_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", institute_year_count[institute_count_copy[i].first][year]);
            }
            fprintf(out, "%d\n", institute_count_copy[i].second);
        }
        /* 
         * first institutes count 
         */
        fprintf(out, " first author institute count \n");
        vector<pair<string, int> > first_institute_count_copy(
                first_institute_count.begin(), first_institute_count.end());
        sort(first_institute_count_copy.begin(), 
                first_institute_count_copy.end(), 
                greater_second<string, int>());

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < first_institute_count_copy.size(); ++i){
            fprintf(out, "%s\t", first_institute_count_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", first_institute_year_count[first_institute_count_copy[i].first][year]);
            }
            fprintf(out, "%d\n", first_institute_count_copy[i].second);
        }
        fprintf(out, "END\n");
    }
}

void year_country_count(
        const csvreader & csv, 
        const char *year_field, 
        const char *address_field, 
        const char *address_separator, 
        const char *address_enclosor, 
        FILE *out
        ){
    /* 
     * check the csv data
     */
    int year_index = csv.getFieldIndex(year_field);
    int address_index = csv.getFieldIndex(address_field);

    if(year_index < 0 || address_index < 0 
            || strlen(year_field) < 1 || strlen(address_field) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector< string> years = csv.getCol(year_index);
        vector<string>  addresses = csv.getCol(address_index);
        vector<string>countries;
        map<string, map<int, int> > country_year_count;
        map<string, int> country_count;
        map<string, map<int, int> > first_country_year_count;
        map<string, int> first_country_count;
        int year, min_year, max_year;

        year = min_year = max_year = string2int(years[0]);
        for(int i = 0; i < years.size(); ++i){
            year = string2int(years[i]);
            min_year = min(min_year, year);
            max_year = max(max_year, year);

            if(addresses[i].size() < 1)
                continue;
            countries = get_countries_from_address(
                    addresses[i],address_separator,address_enclosor);
            for(int j = 0; j < countries.size(); ++j)
            {
                trim(countries[j]);
            }
            removeDuplicates(countries);

            //fprintf(out, "%s\n", countries[0].c_str());

            first_country_year_count[countries[0]][year] += 1;
            first_country_count[countries[0]] += 1;

            for(int j = 0; j < countries.size(); ++j)
            {
                country_year_count[countries[j]][year] += 1;
                country_count[countries[j]] += 1;
            }
        }
        /*
         * print out
         */

        /* 
         * country count 
         * */
        fprintf(out, "all country count \n");
        vector<pair<string, int> > country_count_copy(
                country_count.begin(), country_count.end());
        sort(country_count_copy.begin(), country_count_copy.end(), 
                greater_second<string, int>());

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < country_count_copy.size(); ++i){
            fprintf(out, "%s\t", country_count_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", country_year_count[country_count_copy[i].first][year]);
            }
            fprintf(out, "%d\n", country_count_copy[i].second);
        }
        /* 
         * first country count 
         */
        fprintf(out, " first country count \n");
        vector<pair<string, int> > first_country_count_copy(
                first_country_count.begin(), first_country_count.end());
        sort(first_country_count_copy.begin(), 
                first_country_count_copy.end(), 
                greater_second<string, int>());

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < first_country_count_copy.size(); ++i){
            fprintf(out, "%s\t", first_country_count_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", first_country_year_count[first_country_count_copy[i].first][year]);
            }
            fprintf(out, "%d\n", first_country_count_copy[i].second);
        }
        fprintf(out, "END\n");
    }
}


void year_document_type(
        const csvreader & csv,
        const char *year_field_name,
        const char *document_type_name,
        FILE * out
        ){
    /* 
     * check the csv data
     */
    int year_index = csv.getFieldIndex(year_field_name);
    int document_type_index = csv.getFieldIndex(document_type_name);

    if(year_index < 0 || document_type_index < 0 
            || strlen(year_field_name) < 1 || strlen(document_type_name) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector< string> years = csv.getCol(year_index);
        vector<string>  types = csv.getCol(document_type_index);
        map<string, map<int, int> > type_year_count;
        map<string, int> type_count;
        vector<string>tmp;

        int year, min_year, max_year;

        year = min_year = max_year = string2int(years[0]);
        for(int i = 0; i < years.size(); ++i){
            year = string2int(years[i]);
            min_year = min(min_year, year);
            max_year = max(max_year, year);

            trim(types[i]);
            to_upper(types[i]);

            type_year_count[types[i]][year] += 1;
            type_count[types[i]] += 1;
        }
        /*
         * re-sort
         */
        vector<pair<string, int> > type_count_copy(
                type_count.begin(), type_count.end());
        sort(type_count_copy.begin(), type_count_copy.end(), 
                greater_second<string, int>());
        /*
         * print out
         */

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < type_count_copy.size(); ++i){
            fprintf(out, "%s\t", type_count_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", type_year_count[type_count_copy[i].first][year]);
            }
            fprintf(out, "%d\n", type_count_copy[i].second);
        }
        fprintf(out, "END\n");
    }

}



void year_field_count(
        const csvreader & csv,
        const char *year_field_name,
        const char *field_name,
        FILE * out
        ){
    /* 
     * check the csv data
     */
    int year_index = csv.getFieldIndex(year_field_name);
    int field_index = csv.getFieldIndex(field_name);

    if(year_index < 0 || field_index < 0 
            || strlen(year_field_name) < 1 || strlen(field_name) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector< string> years = csv.getCol(year_index);
        vector<string>  items = csv.getCol(field_index);
        map<string, map<int, int> > item_year_count;
        map<string, int> item_count;
        vector<string>tmp;

        int year, min_year, max_year;

        year = min_year = max_year = string2int(years[0]);
        for(int i = 0; i < years.size(); ++i){
            year = string2int(years[i]);
            min_year = min(min_year, year);
            max_year = max(max_year, year);

            trim(items[i]);
            to_upper(items[i]);

            item_year_count[items[i]][year] += 1;
            item_count[items[i]] += 1;
        }
        /*
         * re-sort
         */
        vector<pair<string, int> > item_count_copy(
                item_count.begin(), item_count.end());
        sort(item_count_copy.begin(), item_count_copy.end(), 
                greater_second<string, int>());
        /*
         * print out
         */

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < item_count_copy.size(); ++i){
            fprintf(out, "%s\t", item_count_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", item_year_count[item_count_copy[i].first][year]);
            }
            fprintf(out, "%d\n", item_count_copy[i].second);
        }
        fprintf(out, "END\n");
    }

}


void year_dual_keyword(
        const csvreader &csv,
        const char * year_field_name,
        const char * keyword1_field_name, 
        const char * keyword1_field_separator,
        const char * keyword2_field_name, 
        const char * keyword2_field_separator,
        FILE* out){
    
    /* 
     * check the csv data
     */
    int year_index = csv.getFieldIndex(year_field_name);
    int keyword1_index = csv.getFieldIndex(keyword1_field_name);
    int keyword2_index = csv.getFieldIndex(keyword2_field_name);

    if(year_index < 0 || keyword1_index < 0|| keyword2_index < 0 
            || strlen(year_field_name) < 1 
            || strlen(keyword1_field_name) < 1
            || strlen(keyword2_field_name) < 1
            ){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector< string> years = csv.getCol(year_index);
        vector<string>  keywords1 = csv.getCol(keyword1_index);
        vector<string>  keywords2 = csv.getCol(keyword2_index);
        map<string, map<int, int> > keyword_year_count;
        map<string, int> keyword_count;
        vector<string>tmp;
        int s = years.size();
        int year, min_year, max_year;

        year = min_year = max_year = string2int(years[0]);
        for(int i = 0; i < s; ++i){
            year = string2int(years[i]);
            min_year = min(min_year, year);
            max_year = max(max_year, year);

            if(keywords1[i].size() > 0){
                tmp = break_string(keywords1[i].c_str(),keyword1_field_separator,"");
                for(int j = 0; j < tmp.size(); ++j)
                {
                    trim(tmp[j]);
                    to_lower(tmp[j]);
                    keyword_year_count[tmp[j]][year] += 1;
                    keyword_count[tmp[j]] += 1;
                }
            }
            if(keywords2[i].size() > 0){
                tmp = break_string(keywords2[i].c_str(),keyword2_field_separator,"");
                for(int j = 0; j < tmp.size(); ++j)
                {
                    trim(tmp[j]);
                    to_lower(tmp[j]);
                    keyword_year_count[tmp[j]][year] += 1;
                    keyword_count[tmp[j]] += 1;
                }
            }
        }
        /*
         * re-sort
         */
        vector<pair<string, int> > keyword_count_copy(
                keyword_count.begin(), keyword_count.end());
        sort(keyword_count_copy.begin(), keyword_count_copy.end(), 
                greater_second<string, int>());
        /*
         * print out
         */

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < keyword_count_copy.size(); ++i){
            fprintf(out, "%s\t", keyword_count_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", keyword_year_count[keyword_count_copy[i].first][year]);
            }
            fprintf(out, "%d\n", keyword_count_copy[i].second);
        }
        fprintf(out, "END\n");
    }
}

void year_keyword(
        const csvreader &csv,
        const char * year_field_name,
        const char * keyword_field_name, 
        const char * keyword_field_separator,
        FILE* out){
    
    /* 
     * check the csv data
     */
    int year_index = csv.getFieldIndex(year_field_name);
    int keyword_index = csv.getFieldIndex(keyword_field_name);

    if(year_index < 0 || keyword_index < 0 
            || strlen(year_field_name) < 1 || strlen(keyword_field_name) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector< string> years = csv.getCol(year_index);
        vector<string>  keywords = csv.getCol(keyword_index);
        map<string, map<int, int> > keyword_year_count;
        map<string, int> keyword_count;
        vector<string>tmp;
        int s = years.size();
        int year, min_year, max_year;

        year = min_year = max_year = string2int(years[0]);
        for(int i = 0; i < s; ++i){
            year = string2int(years[i]);
            min_year = min(min_year, year);
            max_year = max(max_year, year);

            if(keywords[i].size() < 1)
                continue;
            tmp = break_string(keywords[i].c_str(),keyword_field_separator,"");
            for(int j = 0; j < tmp.size(); ++j)
            {
                trim(tmp[j]);
                to_lower(tmp[j]);
                keyword_year_count[tmp[j]][year] += 1;
                keyword_count[tmp[j]] += 1;
            }
        }
        /*
         * re-sort
         */
        vector<pair<string, int> > keyword_count_copy(
                keyword_count.begin(), keyword_count.end());
        sort(keyword_count_copy.begin(), keyword_count_copy.end(), 
                greater_second<string, int>());
        /*
         * print out
         */

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < keyword_count_copy.size(); ++i){
            fprintf(out, "%s\t", keyword_count_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", keyword_year_count[keyword_count_copy[i].first][year]);
            }
            fprintf(out, "%d\n", keyword_count_copy[i].second);
        }
        fprintf(out, "END\n");
    }
}

/**
 * @brief 
 * sum the papers for each subject in each year
 *
 * @param csv
 * @param year_field_name
 * @param subject_field_name
 * @param subject_field_separator
 * @param out
 * the result will be sent to out stream 
 */
void year_subject(
        const csvreader &csv,
        const char * year_field_name,
        const char * subject_field_name, 
        const char * subject_field_separator,
        FILE* out){
    year_keyword(csv, year_field_name, subject_field_name, subject_field_separator, out);
}

void year_pages(
        const csvreader & csv,
        const char *year_field_name,
        const char *page_field_name,
        FILE *out){
    
    int year_index = csv.getFieldIndex(year_field_name);
    int page_index = csv.getFieldIndex(page_field_name);
    if(year_index < 0 || strlen(year_field_name) < 1 
            || page_index < 0 || strlen(page_field_name) < 1 
            ){
        fprintf(out, "NULL\n");
    }
    else{
        int year, min_year, max_year;
        int page, min_page, max_page;

        map<int, int> year_count;
        map<int, int> year_sum;

        vector<string> years = csv.getCol(year_index);
        vector<string> pages = csv.getCol(page_index);

        year = min_year = max_year = string2int(years[0]);
        for(int i = 0; i < years.size(); ++i){
            year = string2int(years[i]);
            page = string2int(pages[i]);

            min_year = min(min_year, year);
            max_year = max(max_year, year);
            min_page = min(min_page, page);
            max_page = max(max_page, page);

            year_count[year] += 1;
            year_sum[year] += page;
        }
        /*
         * print result
         */
        fprintf(out, "year\tcount\tsum\tmean\n");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t%d\t%d\t%f\n", 
                    year, year_count[year], year_sum[year], 1.0 * year_sum[year] / year_count[year]);
        }
        fprintf(out, "END\n");
    }
}


void year_publication_count(
        const csvreader & csv,
        const char * year_field_name,
        const char * publication_name_field,
        FILE *out
        ){
    
    /* 
     * check the csv data
     */
    int year_index = csv.getFieldIndex(year_field_name);
    int publication_name_index = csv.getFieldIndex(publication_name_field);

    if(year_index < 0 || publication_name_index < 0 
            || strlen(year_field_name) < 1 || strlen(publication_name_field) < 1
             ){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector<string> years = csv.getCol(year_index);
        vector<string>  types = csv.getCol(publication_name_index);

        map<string, map<int, int> > type_year_count;
        map<string, int> type_count;
        vector<string>tmp;

        int year, min_year, max_year;

        year = min_year = max_year = string2int(years[0]);
        for(int i = 0; i < years.size(); ++i){
            year = string2int(years[i]);
            min_year = min(min_year, year);
            max_year = max(max_year, year);

            trim(types[i]);
            to_upper(types[i]);

            type_year_count[types[i]][year] += 1;
            type_count[types[i]] += 1;
        }
        /*
         * re-sort
         */
        vector<pair<string, int> > type_count_copy(
                type_count.begin(), type_count.end());
        sort(type_count_copy.begin(), type_count_copy.end(), 
                greater_second<string, int>());
        /*
         * print out
         */

        /* header */
        fprintf(out, "\t");
        for(year = min_year; year <= max_year; ++year){
            fprintf(out, "%d\t",year);
        }
        fprintf(out, "count\n");

        /* content */
        for(int i = 0; i < type_count_copy.size(); ++i){
            fprintf(out, "%s\t", type_count_copy[i].first.c_str());
            for(year = min_year; year <= max_year; ++year){
                fprintf(out, "%d\t", type_year_count[type_count_copy[i].first][year]);
            }
            fprintf(out, "%d\n", type_count_copy[i].second);
        }
        fprintf(out, "END\n");
    }
}

/**
 * @brief 
 * compute the relationship between institute
 *
 * if author A and B from institute X and Y(X != Y)
 * appeared in the same paper which finished by n authors,
 * the relationship index between X and Y will increase 1 / n
 *
 * @param csv
 * the csv data;
 *
 * @param address_field
 * @param address_separator
 * @param author_enclosor
 * @param 
 */
void institute_relationship(
        const csvreader & csv, 
        const char *address_field, 
        const char *address_separator, 
        const char *author_enclosor, 
        FILE *out
        ){
    /* 
     * check the csv data
     */
    int address_index = csv.getFieldIndex(address_field);

    if(address_index < 0 
             || strlen(address_field) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector<string>  addresses = csv.getCol(address_index);
        vector<string>institutes;
        map<pair<string, string>, double> rel;

        for(int i = 0; i < addresses.size(); ++i){

            if(addresses[i].size() < 1)
                continue;
            institutes = get_institutes_from_address(
                    addresses[i],address_separator,author_enclosor);

            /*
             * compute rel
             */
            for(int j = 0; j < institutes.size(); ++j)
            {
                for( int k = j + 1 ; k < institutes.size() ; ++k )
                {
                    if(institutes[j] > institutes[k])
                        rel[pair<string, string>(institutes[j], institutes[k])] += 1.00 / institutes.size();
                    else if(institutes[j] < institutes[k])
                        rel[pair<string, string>(institutes[k], institutes[j])] += 1.00 / institutes.size();
                }
            }
        }
        /*
         * print out
         */

        /* 
         * country count 
         * */
        vector<pair<pair<string,string> , double> > rel_copy(
                rel.begin(), rel.end());
        sort(rel_copy.begin(), rel_copy.end(), 
                greater_second<pair<string, string>, double>());

        /* header */

        /* content */
        for(int i = 0; i < rel_copy.size(); ++i){
            fprintf(out, "%s\t%s\t%f\n" 
                    ,rel_copy[i].first.first.c_str()
                    ,rel_copy[i].first.second.c_str()
                    ,rel_copy[i].second
                    );
        }
        fprintf(out, "OUT\n");
    }
}

void institute_cited(
        const csvreader & csv,
        const char * address_field,
        const char * address_separator,
        const char * author_enclosor,
        const char * total_cited_field,
        FILE*out){
    /* 
     * check the csv data
     */
    int address_index = csv.getFieldIndex(address_field);
    int total_cited_index = csv.getFieldIndex(total_cited_field);

    if(address_index < 0 || total_cited_index < 0 || strlen(address_field) < 1 || strlen(total_cited_field) < 1){
        fprintf(out, "NULL\n");
    }
    else{
        /*
         * get the field value
         */
        vector<string> addresses = csv.getCol(address_index);
        vector<string> cited = csv.getCol(total_cited_index);
        set<string> institute_names;

        map<string, int>institute_total_cited;//record the answers;
        map<string, int>first_institute_total_cited;//record the answers;
        map<string, double>institute_mean_cited;//record the answers;
        map<string, double>first_institute_mean_cited;//record the answers;

        /* ip and cp*/
        map<string, int>institute_ip_tc;//record the answers;
        map<string, int>institute_cp_tc;//record the answers;
        map<string, int>institute_ip_count;//record the answers;
        map<string, int>institute_cp_count;//record the answers;

        for( int i = 0 ; i < addresses.size() ; ++i )
            if(addresses[i].size() > 0)
            {
                vector<string>tmp = get_institutes_from_address(addresses[i], address_separator, "[]");
                for(int j = 0 ; j < tmp.size(); ++j){
                    trim(tmp[j]);
                }

                first_institute_total_cited[tmp[0]] += string2int(cited[i]);
                first_institute_mean_cited[tmp[0]] += 1;

                for(int j = 0 ; j < tmp.size(); ++j){
                    institute_total_cited[tmp[j]] += string2int(cited[i]);
                    institute_mean_cited[tmp[j]] += 1;
                    institute_names.insert(tmp[j]);
                }
                /*************/
                /* ip and cp */
                /*************/
                if(tmp.size() == 1){//ip
                    institute_ip_tc[tmp[0]] += string2int(cited[i]);
                    institute_ip_count[tmp[0]] += 1;
                }
                else{//cp
                    for(int j = 0 ; j < tmp.size(); ++j){
                        institute_cp_tc[tmp[j]] += string2int(cited[i]);
                        institute_cp_count[tmp[j]] += 1;
                    }
                }
            }
        /********************
         *  start output
         *
         *******************/
        fprintf(out, "Institute\tTOTAL CITED\tFIRST- TOTAL CITED\tMEAN CITED\tFIRST- MEAN CITED RANK\tIP Count\tIP total cited\tIP mean cited\tCP Count\tCP total cited\tCP mean cited\tCP / IP\n");

        for(set<string>::iterator it = institute_names.begin(); it != institute_names.end(); ++it){
            fprintf(out, "%s\t%d\t%d\t%f\t%f\t%d\t%d\t%f\t%d\t%d\t%f\t%f\n", it->c_str(), 
                    institute_total_cited[*it], first_institute_total_cited[*it],
                    (institute_mean_cited[*it]) ? 1.0 * institute_total_cited[*it] / institute_mean_cited[*it] : 0,
                    (first_institute_mean_cited[*it]) ? 1.0 * first_institute_total_cited[*it] / first_institute_mean_cited[*it] : 0,
                    institute_ip_count[*it],
                    institute_ip_tc[*it],
                    (institute_ip_count[*it]) ? 1.0 * institute_ip_tc[*it] / institute_ip_count[*it] : 0,
                    institute_cp_count[*it],
                    institute_cp_tc[*it],
                    (institute_cp_count[*it]) ? 1.0 * institute_cp_tc[*it] / institute_cp_count[*it] : 0,
                    (institute_ip_count[*it]) ? 1.0 * institute_cp_count[*it] / institute_ip_count[*it] : 0
                    );
        }                 
        fprintf(out, "END\n");
    }
    fclose(out);
}

