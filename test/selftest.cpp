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

        // -case--------------------------------------------------------
        LOG(u.get_double("Prepare.SwingAngle"))
        // -case--------------------------------------------------------

        MAP_SS ret1;
        // -case--------------------------------------------------------

        ret1 = u.get_map("View");

        for (MAP_SS::iterator it = ret1.begin();
            it != ret1.end(); it++)
        {
            LOG(it->first << " => " << decode_single<double>(it->second));
        }

        // -case--------------------------------------------------------
        u.set_value("View.QRotate", 90.0);
        LOG(u.get_double("View.QRotate"))

        LOG(u.get_string("Prepare.PreferedScan"))
        u.set_value("Prepare.PreferedScan", "4D");
        LOG(u.get_string("Prepare.PreferedScan"))

        // -case--------------------------------------------------------

        ret1 = u.get_map("Prepare.");

        for (MAP_SS::iterator it = ret1.begin();
            it != ret1.end(); it++)
        {
            LOG(it->first << " => " << decode_single<double>(it->second));
            LOG(it->first << " => " << decode_single<std::string>(it->second));
        }
        // -case--------------------------------------------------------

        VEC_UC ret2;

        ret2 = u.get_full("View.");

        for (VEC_UC::iterator it = ret2.begin();
            it != ret2.end(); it++)
        {
            LOG(*it);
        }
        // -case--------------------------------------------------------

        ret2 = u.get_full("Prepare.");

        for (VEC_UC::iterator it = ret2.begin();
            it != ret2.end(); it++)
        {
            LOG(*it);
        }
    }
    catch (std::exception &e)
    {
        LOG_ERR(e.what())
    }

    return 0;
}

