/*
 * ConfLite.h
 *
 * A sqlite solution to user configure
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#pragma once

#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <map>
#include <vector>

#define _OUT_ std::cout
#define _ERR_ std::cerr

#define LOG(s) \
    _OUT_ << __PRETTY_FUNCTION__ << '[' << __FILE__ << ':' << __LINE__ << "]: " <<  s << '\n';

#define LOG_ERR(s) \
    _ERR_ << __PRETTY_FUNCTION__ << '[' << __FILE__ << ':' << __LINE__ << "]: " <<  s << '\n';

#define TH(s) \
    _OUT_ << s 

#define EXEC_SQLITE_LOG(conn, cmd, ok_log, err_log) \
    if (SQLITE_OK != cmd)\
    {\
        LOG(err_log << ": " << sqlite3_errmsg(conn))\
        throw std::runtime_error(err_log);\
    }\
    else\
    {\
        LOG(ok_log)\
    }

enum VT_TABLE
{
    VT_DOUBLE, VT_INT, VT_STRING,
    VT_LAST
};

static std::string VALUE_TYPE[] = {
    "VT_DOUBLE", "VT_INT", "VT_STRING",
};


/*
 * Key => State(M) + '.' + Item(M) + '.' + Specification(O)
 *
 * Ex:  View.Zoom
 *      Prepare.SwingAngle.Volumn ( for volumn probe)
 *      Prepare.SwingAngle.Linear ( for linear probe)
 *
 */
struct SysConf
{
    std::string Key;
    double DefaultValue;
    double Step;
    double Upper;
    double Lower;
    std::string Unit;

    SysConf (std::string k, double v, double s, double p, double l, std::string u = "")
    : Key(k), DefaultValue(v), Step(s), Upper(p), Lower(l), Unit(u)
    {
    }

    SysConf ()
    {
    }

    ~SysConf ()
    {
    }

    friend std::ostream& operator<< (std::ostream &o, SysConf a)
    {
        return o << a.Key << " (" << a.DefaultValue << " " << a.Unit << ") [" << a.Lower << ", " << a.Upper << "](" << a.Step << ")";
    }
};

struct UserConf
{
    std::string Key;
    std::string Value;
    VT_TABLE ValueType;

    UserConf (std::string k, std::string v, VT_TABLE vt)
    : Key(k), Value(v), ValueType(vt)
    {
    }

    UserConf ()
    {
    }

    ~UserConf ()
    {
    }

    friend std::ostream& operator<< (std::ostream &o, UserConf a)
    {
        return o << a.Key << " => " << a.Value << "(" << VALUE_TYPE[a.ValueType] << ")";
    }
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
    o.precision(1);
    o << std::fixed << val;

   return o.str();
}

typedef std::map<std::string, std::string> MAP_SS;
typedef std::vector<UserConf> VEC_UC;
typedef std::map<std::string, SysConf> MAP_SC;
typedef std::map<std::string, UserConf> MAP_UC;

extern int sql_query_cb (void* ret, int col_nr, char** rows, char** colnames);
extern int sql_query_map_cb (void* ret, int col_nr, char** rows, char** colnames);
extern int sql_query_user_cb (void* ret, int col_nr, char** rows, char** colnames);

class ConfLite
{
protected:
    std::string conf_db_file_;
    std::string conf_table_;
    sqlite3 *conn_;

public:

    ConfLite(std::string fname) : conf_db_file_(fname)
    {
        EXEC_SQLITE_LOG(conn_, sqlite3_open(conf_db_file_.c_str(), &conn_), "sqlite3_open, database connected", "sqlite3_open failed")
    }

    ~ConfLite()
    {
        EXEC_SQLITE_LOG(conn_, sqlite3_close(conn_), "sqlite3_close, database disconnected", "sqlite3_close failed")
    }

    std::string ConfTable()
    {
        return conf_table_;
    }

    std::string ConfDB()
    {
        return conf_db_file_;
    }

    void ConfTable(std::string table)
    {
        conf_table_ = table;
    }

    template <typename T>
    void __set_value(std::string key, T value, std::string entry)
    {
        std::string sql = "update " + conf_table_ + " set " + entry + "=\"" + encode_single<T>(value) + "\" where Key=\"" + key + "\";";
        LOG("::[" << sql << "]")
    
        EXEC_SQLITE_LOG(conn_, sqlite3_exec(conn_, sql.c_str(), 0, 0, 0), "sqlite3_exec, query done", "sqlite3_exec failed");
    }

    template <typename T>
    T __get_value(std::string key, std::string entry)
    {
        std::string sql = "select " + entry + " from " + conf_table_ + " where Key=\"" + key + "\";";
        LOG("::[" << sql << "]")
    
        std::string buf;
    
        EXEC_SQLITE_LOG(conn_, sqlite3_exec(conn_, sql.c_str(), sql_query_cb, &buf, 0), "sqlite3_exec, query done", "sqlite3_exec failed");
    
        return decode_single<T>(buf);
    }
};

class UserConfLite : public ConfLite
{
public:
    UserConfLite(std::string fname);

    ~UserConfLite();

    /* get value in map by key */
    MAP_SS get_map(std::string key);

    /* get all column values by key */
    MAP_UC get_all(std::string key = "");

    /* set value referenced by key */
    void set_value(std::string key, double value);
    void set_value(std::string key, int value);
    void set_value(std::string key, std::string value);

    /* get value by key */
    double get_double(std::string key);
    int get_int(std::string key);
    std::string get_string(std::string key);
 
    /* add new user configure item */
    void add_item(UserConf uc);
    void add_item(std::string k, std::string v, VT_TABLE vt);

    void reset_by_prefix(std::string key);
};

class SysConfLite : public ConfLite
{
public:
    SysConfLite(std::string fname) : ConfLite(fname) {}

    ~SysConfLite(){}

//    /* get value in map by key */
//    MAP_SS get_map(std::string key);
//
//    /* get all column values by key */
//    VEC_UC get_full(std::string key);
//
//    /* set value referenced by key */
//    void set_value(std::string key, double value);

    /* get value by key */
    SysConf get(std::string key = "");
 
    std::map<std::string, SysConf> get_all(std::string key = "");
//    /* add new user configure item */
//    void add_item(SysConf uc);
//    void add_item(std::string k, std::string v, VT_TABLE vt);
};
