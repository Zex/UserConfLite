/*
 * userconflite.cpp
 *
 * A sqlite solution to user configure
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "userconflite.h"

UserConfLite::UserConfLite(std::string fname)
: userconf_db_file_(fname)
{
    EXEC_SQLITE_LOG(sqlite3_open(userconf_db_file_.c_str(), &conn_), "sqlite3_open, database connected", "sqlite3_open failed")
}

UserConfLite::~UserConfLite()
{
    EXEC_SQLITE_LOG(sqlite3_close(conn_), "sqlite3_close, database disconnected", "sqlite3_close failed")
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
    //ex: select Key, Value from UserConf where Key like "Prepare%";
    std::string sql = "select Key, Value from " + userconf_table_ + " where Key like \"%" + key + "%\";";
    LOG("::[" << sql << "]")

    MAP_SS ret;

    EXEC_SQLITE_LOG(sqlite3_exec(conn_, sql.c_str(), sql_query_map_cb, &ret, 0), "sqlite3_exec, query done", "sqlite3_exec failed")

    return ret;
}

VEC_UC UserConfLite::get_full(std::string key)
{
    //ex: select Key, Value from UserConf where Key like "Prepare%";
    std::string sql = "select Key, Value, ValueType from " + userconf_table_ + " where Key like \"%" + key + "%\";";
    LOG("::[" << sql << "]")

    VEC_UC ret;

    EXEC_SQLITE_LOG(sqlite3_exec(conn_, sql.c_str(), sql_query_full_cb, &ret, 0), "sqlite3_exec, query done", "sqlite3_exec failed")

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
// ------------------------------------selftest begin----------------------------------    
//
//std::string userconf_db_file("UserConf.db");
//
//int main(int argc, char* argv[])
//{
//    try
//    {
//        UserConfLite u = UserConfLite(userconf_db_file);
//        u.UserConfTable("UserConf");
//
//        LOG(u.get_double("Prepare.SwingAngle"))
//
//        MAP_SS ret = u.get_map("View");
//
//        for (MAP_SS::iterator it = ret.begin();
//            it != ret.end(); it++)
//        {
//            LOG(it->first << " => " << decode_single<double>(it->second));
//        }
//
//        u.set_value("View.QRotate", 90.0);
//        LOG(u.get_double("View.QRotate"))
//    }
//    catch (std::exception &e)
//    {
//        LOG_ERR(e.what())
//    }
//
//    return 0;
//}
//
// ------------------------------------selftest end----------------------------------    

