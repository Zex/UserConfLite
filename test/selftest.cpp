/*
 * tests/selftest.cpp
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "userconflite.h"

std::string userconf_db_file("UserConf.db");

int main(int argc, char* argv[])
{
    try
    {
        UserConfLite u = UserConfLite(userconf_db_file);
        u.UserConfTable("UserConf");

        LOG(u.get_double("Prepare.SwingAngle"))

        std::map<std::string, std::string> ret = u.get_map("View");

        for (std::map<std::string, std::string>::iterator it = ret.begin();
            it != ret.end(); it++)
        {
            LOG(it->first << " => " << decode_single<double>(it->second));
        }

        u.set_value("View.QRotate", 90.0);
        LOG(u.get_double("View.QRotate"))

        LOG(u.get_string("Prepare.PreferedScan"))
        u.set_value("Prepare.PreferedScan", "4D");
        LOG(u.get_string("Prepare.PreferedScan"))
    }
    catch (std::exception &e)
    {
        LOG_ERR(e.what())
    }

    return 0;
}

