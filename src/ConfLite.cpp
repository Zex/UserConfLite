/*
 * ConfLite.cpp
 *
 * A sqlite solution to user configure
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "ConfLite.h"

UserConfLite::UserConfLite(std::string fname)
: userconf_db_file_(fname)
{
    EXEC_SQLITE_LOG(conn_, sqlite3_open(userconf_db_file_.c_str(), &conn_), "sqlite3_open, database connected", "sqlite3_open failed")
}

UserConfLite::~UserConfLite()
{
    EXEC_SQLITE_LOG(conn_, sqlite3_close(conn_), "sqlite3_close, database disconnected", "sqlite3_close failed")
}

double UserConfLite::get_double(std::string key)
{
    return __get_value<double>(key);
}

int UserConfLite::get_int(std::string key)
{
    return __get_value<int>(key);
}

std::string UserConfLite::get_string(std::string key)
{
    return __get_value<std::string>(key);
}

MAP_SS UserConfLite::get_map(std::string key)
{
    if (key.empty())
        throw std::runtime_error("Iteration not allow");

    std::string sql = "select Key, Value from " + userconf_table_ + " where Key like \"%" + key + "%\";";
    LOG("::[" << sql << "]")

    MAP_SS ret;

    EXEC_SQLITE_LOG(conn_, sqlite3_exec(conn_, sql.c_str(), sql_query_map_cb, &ret, 0), "sqlite3_exec, query done", "sqlite3_exec failed")

    return ret;
}

VEC_UC UserConfLite::get_full(std::string key)
{
    if (key.empty())
        throw std::runtime_error("Iteration not allow");

    std::string sql = "select Key, Value, ValueType from " + userconf_table_ + " where Key like \"%" + key + "%\";";
    LOG("::[" << sql << "]")

    VEC_UC ret;

    EXEC_SQLITE_LOG(conn_, sqlite3_exec(conn_, sql.c_str(), sql_query_full_cb, &ret, 0), "sqlite3_exec, query done", "sqlite3_exec failed")

    return ret;
}

void UserConfLite::set_value(std::string key, double value)
{
    __set_value<double>(key, value);
}

void UserConfLite::set_value(std::string key, int value)
{
    __set_value<int>(key, value);
}

void UserConfLite::set_value(std::string key, std::string value)
{
    __set_value<std::string>(key, value);
}

void UserConfLite::add_item(UserConf uc)
{
    add_item(uc.Key, uc.Value, uc.ValueType);
}

void UserConfLite::add_item(std::string k, std::string v, VT_TABLE vt)
{
    std::string sql = "insert into " + userconf_table_ + " values (\"" + k + "\", \"" + v + "\", " + encode_single<VT_TABLE>(vt) + ");";
    LOG("::[" << sql << "]")

    VEC_UC ret;

    EXEC_SQLITE_LOG(conn_, sqlite3_exec(conn_, sql.c_str(), 0, 0, 0), "sqlite3_exec, query done", "sqlite3_exec failed")
}

