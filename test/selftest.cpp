/*
 * tests/selftest.cpp
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "ConfLite.h"

std::string userconf_db_file("dbs/config.db");

void selftest_case1(UserConfLite &u)
{
    // -case--------------------------------------------------------
    LOG(u.get_double("Prepare.SwingAngle"))
}

void selftest_case2(UserConfLite &u)
{
    // -case--------------------------------------------------------

    MAP_SS ret;
    ret = u.get_map("View");

    for (MAP_SS::iterator it = ret.begin();
        it != ret.end(); it++)
    {
        LOG(it->first << " => " << decode_single<double>(it->second));
    }
}

void selftest_case3(UserConfLite &u)
{
    // -case--------------------------------------------------------
    u.set_value("View.QRotate", 270.0);
    LOG(u.get_double("View.QRotate"))

    LOG(u.get_string("Prepare.PreferedScan"))
    u.set_value("Prepare.PreferedScan", "3D");
    LOG(u.get_string("Prepare.PreferedScan"))
}

void selftest_case4(UserConfLite &u)
{
    // -case--------------------------------------------------------
    MAP_SS ret;
    ret = u.get_map("Prepare.");

    for (MAP_SS::iterator it = ret.begin();
        it != ret.end(); it++)
    {
        LOG(it->first << " => " << decode_single<double>(it->second));
        LOG(it->first << " => " << decode_single<std::string>(it->second));
    }

}

void selftest_case5(UserConfLite &u)
{
    // -case--------------------------------------------------------
    MAP_UC ret;

    ret = u.get_all("View");

    for (MAP_UC::iterator it = ret.begin();
        it != ret.end(); it++)
    {
        LOG(it->first << " => " << it->second);
    }
}

void selftest_case6(UserConfLite &u)
{
    // -case--------------------------------------------------------
    MAP_UC ret;
    ret = u.get_all("Prepare.");

    for (MAP_UC::iterator it = ret.begin();
        it != ret.end(); it++)
    {
        LOG(it->first << " => " << it->second);
    }
}

void selftest_case7(UserConfLite &u)
{
    // -case--------------------------------------------------------
    LOG("");
    UserConf uc1("View.DisplayFormat", "1", VT_INT);
    UserConf uc2("View.ReferenceImage", "C", VT_STRING);

//    u.add_item("Capture3D.Step", "10", VT_INT);
//    u.add_item(uc1);
//    u.add_item(uc2);

    MAP_UC ret;
    ret = u.get_all();

    for (MAP_UC::iterator it = ret.begin();
        it != ret.end(); it++)
    {
        LOG(it->first << " => " << it->second);
    }
}

void selftest_case8(UserConfLite &u)
{
    // -case--------------------------------------------------------
    u.reset_by_prefix("View.");
}

void (*selftests[])(UserConfLite&) = {

    selftest_case1,
    selftest_case2,
    selftest_case3,
    selftest_case4,
    selftest_case5,
    selftest_case6,
    selftest_case7,
    selftest_case8,
};

int main(int argc, char* argv[])
{
    try
    {
        UserConfLite u = UserConfLite(userconf_db_file);
        u.ConfTable("UserConf");

        size_t except_cases = 0;
        size_t total_cases = sizeof(selftests)/sizeof(void*);
        size_t success_cases = 0;

        for (size_t i = 0; i < total_cases; i++)
        {
            try
            {
                selftests[i](u);
                success_cases++;
            }
            catch (std::exception &e)
            {
                LOG_ERR(e.what())
                except_cases++;
            }
        }

        LOG("FINALLY: total=" << total_cases << " success=" << success_cases << " exception=" << except_cases)
    }
    catch (std::exception &e)
    {
        LOG_ERR(e.what())
    }

    return 0;
}

