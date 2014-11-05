/*
 * tests/selftest.cpp
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "userconflite.h"

std::string userconf_db_file("UserConf.db3x");

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

        u.set_double("View.QRotate", 90.0);
        LOG(u.get_double("View.QRotate"))
    }
    catch (std::exception &e)
    {
        LOG_ERR(e.what())
    }

    return 0;
}

