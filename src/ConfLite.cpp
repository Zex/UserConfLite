/*
 * ConfLite.cpp
 *
 * A sqlite solution to user configure
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "ConfLite.h"

int sql_query_cb (void* ret, int col_nr, char** rows, char** colnames)
{
    if (rows == 0) return 0;
    if (0 > col_nr) return 0;

    std::string *val = reinterpret_cast<std::string*>(ret);
    *val = rows[0];

    return 0;
}

int sql_query_map_cb (void* ret, int col_nr, char** rows, char** colnames)
{
    if (2 > col_nr)
        return 0;

    MAP_SS *ret_map = reinterpret_cast<MAP_SS*>(ret);
    (*ret_map)[rows[0]] = rows[col_nr-1];

    return 0;
}

int sql_query_user_cb (void* ret, int col_nr, char** rows, char** colnames)
{
    if (3 > col_nr)
        return 0;

    VEC_UC *ret_vec = reinterpret_cast<VEC_UC*>(ret);
    ret_vec->resize(ret_vec->size()+1);

    ret_vec->at(ret_vec->size()-1).Key = rows[0];
    ret_vec->at(ret_vec->size()-1).Value = rows[1];
    ret_vec->at(ret_vec->size()-1).ValueType = (VT_TABLE)decode_single<int>(rows[2]);

    return 0;
}

int sql_query_sys_cb (void* ret, int col_nr, char** rows, char** colnames)
{
    if (5 > col_nr)
        return 0;

    MAP_SC *ret_map = reinterpret_cast<MAP_SC*>(ret);
    SysConf s; 

    s.Key = rows[0];
    s.DefaultValue = decode_single<double>(rows[1]);
    s.Step = decode_single<double>(rows[2]);
    s.Upper = decode_single<double>(rows[3]);
    s.Lower = decode_single<double>(rows[4]);
    s.Unit = rows[5];

    ret_map->insert(std::make_pair(s.Key, s));

    return 0;
}

UserConfLite::UserConfLite(std::string fname)
:ConfLite(fname)
{
}

UserConfLite::~UserConfLite()
{
}

double UserConfLite::get_double(std::string key)
{
    return __get_value<double>(key, "Value");
}

int UserConfLite::get_int(std::string key)
{
    return __get_value<int>(key, "Value");
}

std::string UserConfLite::get_string(std::string key)
{
    return __get_value<std::string>(key, "Value");
}

MAP_SS UserConfLite::get_map(std::string key)
{
    if (key.empty())
        throw std::runtime_error("Iteration not allow");

    std::string sql = "select Key, Value from " + conf_table_ + " where Key like \"%" + key + "%\";";
    LOG("::[" << sql << "]")

    MAP_SS ret;

    EXEC_SQLITE_LOG(conn_, sqlite3_exec(conn_, sql.c_str(), sql_query_map_cb, &ret, 0), "sqlite3_exec, query done", "sqlite3_exec failed")

    return ret;
}

VEC_UC UserConfLite::get_full(std::string key)
{
    if (key.empty())
        throw std::runtime_error("Iteration not allow");

    std::string sql = "select Key, Value, ValueType from " + conf_table_ + " where Key like \"%" + key + "%\";";
    LOG("::[" << sql << "]")

    VEC_UC ret;

    EXEC_SQLITE_LOG(conn_, sqlite3_exec(conn_, sql.c_str(), sql_query_user_cb, &ret, 0), "sqlite3_exec, query done", "sqlite3_exec failed")

    return ret;
}

void UserConfLite::set_value(std::string key, double value)
{
    __set_value<double>(key, value, "Value");
}

void UserConfLite::set_value(std::string key, int value)
{
    __set_value<int>(key, value, "Value");
}

void UserConfLite::set_value(std::string key, std::string value)
{
    __set_value<std::string>(key, value, "Value");
}

void UserConfLite::add_item(UserConf uc)
{
    add_item(uc.Key, uc.Value, uc.ValueType);
}

void UserConfLite::add_item(std::string k, std::string v, VT_TABLE vt)
{
    std::string sql = "insert into " + conf_table_ + " values (\"" + k + "\", \"" + v + "\", " + encode_single<VT_TABLE>(vt) + ");";
    LOG("::[" << sql << "]")

    VEC_UC ret;

    EXEC_SQLITE_LOG(conn_, sqlite3_exec(conn_, sql.c_str(), 0, 0, 0), "sqlite3_exec, query done", "sqlite3_exec failed")
}

SysConf SysConfLite::get(std::string key)
{
    SysConf sys;

    sys.Key = key;
    sys.DefaultValue = __get_value<double>(key, "DefaultValue");
    sys.Step = __get_value<double>(key, "Step");
    sys.Upper = __get_value<double>(key, "Upper");
    sys.Lower = __get_value<double>(key, "Lower");
    sys.Unit = __get_value<std::string>(key, "Unit");

    return sys;
}

std::map<std::string, SysConf> SysConfLite::get_all()
{
    std::string entry = "Key, DefaultValue, Step, Upper, Lower, Unit";
    std::string sql = "select " + entry + " from " + conf_table_ + ";";
    LOG("::[" << sql << "]")

    MAP_SC ret;

    EXEC_SQLITE_LOG(conn_, sqlite3_exec(conn_, sql.c_str(), sql_query_sys_cb, &ret, 0), "sqlite3_exec, query done", "sqlite3_exec failed");

    return ret;
}

