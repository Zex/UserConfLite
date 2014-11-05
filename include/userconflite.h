/*
 * userconflite.h
 *
 * A sqlite solution to user configure
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#pragma once

#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>

#define LOG(s) \
    std::cout << __PRETTY_FUNCTION__ << '[' << __FILE__ << ':' << __LINE__ << "]: " <<  s << '\n';

#define LOG_ERR(s) \
    std::cerr << __PRETTY_FUNCTION__ << '[' << __FILE__ << ':' << __LINE__ << "]: " <<  s << '\n';

#define TH(s) \
    std::cout << s 

enum ValueType {
 vt_double,
 vt_int,
 vt_string,
};

template <typename T>
T decode_single(std::string buf)
{
   T ret;
   std::istringstream s(buf);
   s >> ret;

   return ret;
}

template <typename T>
std::string encode_single(T val)
{
   std::ostringstream o;
   o << val;

   return o.str();
}

int sql_query_cb (void* ret, int col_nr, char** rows, char** colnames)
{
//    LOG("Total column: " << col_nr);

//    for (int i = 0; i < col_nr; i++)
//    {
//        TH("Title: " << colnames[i] << ' ');
//        TH(rows[i] << ' ');
//    }
    std::string *val = reinterpret_cast<std::string*>(ret);
    *val = rows[0];

    return 0;
}

int sql_query_map_cb (void* ret, int col_nr, char** rows, char** colnames)
{
//    LOG("Total column: " << col_nr);

//    for (int i = 0; i < col_nr; i++)
//    {
//        TH(colnames[i] << ": ");
//        TH(rows[i] << " ");
//    }
//    TH('\n');

    std::map<std::string, std::string> *ret_map = reinterpret_cast<std::map<std::string, std::string>*>(ret);
    (*ret_map)[rows[0]] = rows[col_nr-1];

    return 0;
}

class UserConfLite
{
    
    std::string userconf_db_file_;
    std::string userconf_table_;
    sqlite3 *conn_;

public:

    UserConfLite(std::string fname);

    ~UserConfLite();

    /* get value in double by key */
    double get_double(std::string key);

    /* get value in map by key */
    std::map<std::string, std::string> get_map(std::string key);

    /* set value referenced by key */
    void set_double(std::string key, double value);
 
    void UserConfTable(std::string table)
    {
        userconf_table_ = table;
    }

    std::string UserConfTable()
    {
        return userconf_table_;
    }
};

