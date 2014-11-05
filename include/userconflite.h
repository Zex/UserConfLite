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

#define EXEC_SQLITE_LOG(cmd, ok_log, err_log) \
    if (SQLITE_OK != cmd)\
    {\
        LOG(err_log)\
        throw std::runtime_error(err_log);\
    }\
    else\
    {\
        LOG(ok_log)\
    }

enum ValueType {
 VT_DOUBLE,
 VT_INT,
 VT_STRING,
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

    /* get value by key */
    double get_double(std::string key);
    int get_int(std::string key);
    std::string get_string(std::string key);

    /* get value in map by key */
    std::map<std::string, std::string> get_map(std::string key);

    /* set value referenced by key */
    void set_value(std::string key, double value);
    void set_value(std::string key, int value);
    void set_value(std::string key, std::string value);
 
    void UserConfTable(std::string table)
    {
        userconf_table_ = table;
    }

    std::string UserConfTable()
    {
        return userconf_table_;
    }

    template <typename T>
    void __set_value(std::string key, T value)
    {
        // ex: update UserConf set Value="5.0" where Key="View.Zoom";
        std::string sql = "update " + userconf_table_ + " set Value=\"" + encode_single<T>(value) + "\" where Key=\"" + key + "\";";
        LOG("::[" << sql << "]")
    
        EXEC_SQLITE_LOG(sqlite3_exec(conn_, sql.c_str(), 0, 0, 0), "sqlite3_exec, query done", "sqlite3_exec failed");
    }

    template <typename T>
    T __get_value(std::string key)
    {
        // ex: key = "Prepare.SwingAngle";
        std::string sql = "select Value from " + userconf_table_ + " where Key=\"" + key + "\";";
        LOG("::[" << sql << "]")
    
        std::string buf;
    
        EXEC_SQLITE_LOG(sqlite3_exec(conn_, sql.c_str(), sql_query_cb, &buf, 0), "sqlite3_exec, query done", "sqlite3_exec failed");
    
        return decode_single<T>(buf);
    }
};

