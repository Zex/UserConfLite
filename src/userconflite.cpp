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
//  userconf_table_("UserConf")
{
    if (SQLITE_OK != sqlite3_open(userconf_db_file_.c_str(), &conn_))
    {
        LOG("sqlite3_open failed")
        throw std::runtime_error("sqlite3_open failed");
    }
    else
    {
        LOG("sqlite3_open, database connected")
    }
}

UserConfLite::~UserConfLite()
{
    if (SQLITE_OK != sqlite3_close(conn_))
    {
        LOG("sqlite3_close failed")
        throw std::runtime_error("sqlite3_close failed");
    }
    else
    {
        LOG("sqlite3_close, database disconnected")
    }
}

double UserConfLite::get_double(std::string key)
{
    // ex: key = "Prepare.SwingAngle";
    std::string sql = "select Value from " + userconf_table_ + " where Key=\"" + key + "\";";
    LOG("::[" << sql << "]")

    std::string buf;

    if (SQLITE_OK != sqlite3_exec(conn_, sql.c_str(), sql_query_cb, &buf, 0))
    {
        LOG("sqlite3_exec failed")
        throw std::runtime_error("sqlite3_exec failed");
    }
    else
    {
        LOG("sqlite3_exec, query done")
    }

    return decode_single<double>(buf);
}

std::map<std::string, std::string> UserConfLite::get_map(std::string key)
{
    //ex: select Key, Value from UserConf where Key like "Prepare%";
    std::string sql = "select Key, Value from " + userconf_table_ + " where Key like \"" + key + "%\";";
    LOG("::[" << sql << "]")

    std::map<std::string, std::string> ret;

    if (SQLITE_OK != sqlite3_exec(conn_, sql.c_str(), sql_query_map_cb, &ret, 0))
    {
        LOG("sqlite3_exec failed")
        throw std::runtime_error("sqlite3_exec failed");
    }
    else
    {
        LOG("sqlite3_exec, query done")
    }

    return ret;
}

void UserConfLite::set_double(std::string key, double value)
{
    // ex: update UserConf set Value="5.0" where Key="View.Zoom";
    std::string sql = "update " + userconf_table_ + " set Value=\"" + encode_single<double>(value) + "\" where Key=\"" + key + "\";";
    LOG("::[" << sql << "]")

    std::string buf;

    if (SQLITE_OK != sqlite3_exec(conn_, sql.c_str(), 0, 0, 0))
    {
        LOG("sqlite3_exec failed")
        throw std::runtime_error("sqlite3_exec failed");
    }
    else
    {
        LOG("sqlite3_exec, query done")
    }
}
// ------------------------------------selftest begin----------------------------------    
//
//std::string userconf_db_file("UserConf.db3x");
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
//        std::map<std::string, std::string> ret = u.get_map("View");
//
//        for (std::map<std::string, std::string>::iterator it = ret.begin();
//            it != ret.end(); it++)
//        {
//            LOG(it->first << " => " << decode_single<double>(it->second));
//        }
//
//        u.set_double("View.QRotate", 90.0);
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

