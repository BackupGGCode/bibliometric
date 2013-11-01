#ifndef BIBLIOMETRICS_H
#  define BIBLIOMETRICS_H

#include    <cstring>
#include    <cstdio>
#include    <map>
#include    <vector>
#include    <string>
#include    <cstring>
#include    <iostream>
#include    <set>
#include    <algorithm>
//#include <QDebug>
#include    "csvreader.h"

#define for_i(i, size) for(int i = 0;i < size; ++i)

using namespace std;
//using namespace boost;

void trim(string & str);
void to_lower(string & str);
void to_upper(string & str);

/**
 * @brief
 * less operator
 */
template <typename T1, typename T2>
struct less_second {
    typedef pair<T1, T2> type;
    bool operator ()(type const& a, type const& b) const {
        return a.second < b.second;
    }
};

/**
 * @brief
 * greater operator
 */
template <typename T1, typename T2>
struct greater_second {
    typedef pair<T1, T2> type;
    bool operator ()(type const& a, type const& b) const {
        return a.second > b.second;
    }
};

/**
 * @brief
 *
 * remove the duplicates in a vector<T>
 *
 * @tparam T
 *
 * the element type in the vector
 *
 * @param vec
 */
template<typename T>
void removeDuplicates(std::vector<T>& vec)
{
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
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
int string2int(const string &s);


void author_cited(
        const csvreader & csv,
        const char *author_field,
        const char * author_separator,
        const char * total_cited_field,
        FILE*out);


string country_special_judge(const string & _cntry);
/**
 * @brief 
 * fliter the characters within the delimiters 
 *
 * @param str
 * @param delimiters
 */
void fliter_string_by_delimiters(string & str, const string & delimiters);

vector<string> get_institutes_from_address(
        const string & address,
        const char * address_separator, 
        const char * author_enclosor
        );

vector<string> get_countries_from_address(
        const string & address,
        const char * address_separator, 
        const char * address_enclosor
        );

string get_rp_name(string RP);
void showstrings(vector<string> str);


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
        );

void year_author_count(
        const csvreader & csv,
        const char *year_field, 
        const char *author_field,
        const char * author_separator,
        const char *rp_field,
        FILE *out
        );

void author_relationship(
        const csvreader & csv, 
        const char *author_field, 
        const char *author_separator, 
        FILE *out
        );


void publication_tc_count(
        const csvreader & csv,
        const char* pub_name_field,
        const char* total_cited_field,
        FILE* out);


void country_cited(
        const csvreader & csv, 
        const char *address_field, 
        const char *address_separator, 
        const char *address_enclosor, 
        const char * total_cited_field,
        FILE *out
        );

void year_cooperation_count(
        const csvreader & csv, 
        const char *year_field, 
        const char *address_field, 
        const char *address_separator, 
        const char *address_enclosor, 
        FILE *out
        );

void year_count(
        const csvreader &csv,
        const char * year_field_name,
        FILE *out);


void year_institute_count(
        const csvreader & csv, 
        const char *year_field, 
        const char *address_field, 
        const char *address_separator, 
        const char *author_enclosor, 
        FILE *out
        );

void year_country_count(
        const csvreader & csv, 
        const char *year_field, 
        const char *address_field, 
        const char *address_separator, 
        const char *address_enclosor, 
        FILE *out
        );


void year_document_type(
        const csvreader & csv,
        const char *year_field_name,
        const char *document_type_name,
        FILE * out
        );


void year_field_count(
        const csvreader & csv,
        const char *year_field_name,
        const char *field_name,
        FILE * out
        );


void year_dual_keyword(
        const csvreader &csv,
        const char * year_field_name,
        const char * keyword1_field_name, 
        const char * keyword1_field_separator,
        const char * keyword2_field_name, 
        const char * keyword2_field_separator,
        FILE* out);

void year_keyword(
        const csvreader &csv,
        const char * year_field_name,
        const char * keyword_field_name, 
        const char * keyword_field_separator,
        FILE* out);

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
        FILE* out);

void year_pages(
        const csvreader & csv,
        const char *year_field_name,
        const char *page_field_name,
        FILE *out);

void year_publication_count(
        const csvreader & csv,
        const char * year_field_name,
        const char * publication_name_field,
        FILE *out
        );

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
        );

void institute_cited(
        const csvreader & csv,
        const char * address_field,
        const char * address_separator,
        const char * author_enclosor,
        const char * total_cited_field,
        FILE*out);

#endif /* ifndef BIBLIOMETRICS_H */

