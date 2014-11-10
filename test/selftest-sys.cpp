/*
 * tests/selftest-sys.cpp
 *
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "ConfLite.h"

std::string conf_db_file("dbs/config.db");

void selftest_case1(SysConfLite &u)
{
    // -case--------------------------------------------------------
//    LOG(u.get("Prepare.SwingAngle"));
}

void selftest_case2(SysConfLite &u)
{
    // -case--------------------------------------------------------
//    LOG(u.get("View.RotationZ"));
    u.get("*");
}

void selftest_case3(SysConfLite &u)
{
    // -case--------------------------------------------------------
    MAP_SC ret = u.get_all();

    for(MAP_SC::iterator it = ret.begin(); it != ret.end(); it++)
        LOG(it->first << " => " << it->second);
}

void selftest_case4(SysConfLite &u)
{
    // -case--------------------------------------------------------
}

void selftest_case5(SysConfLite &u)
{
    // -case--------------------------------------------------------
}

void selftest_case6(SysConfLite &u)
{
    // -case--------------------------------------------------------
}

void selftest_case7(SysConfLite &u)
{
    // -case--------------------------------------------------------
}

void (*selftests[])(SysConfLite&) = {

    selftest_case1,
    selftest_case2,
    selftest_case3,
    selftest_case4,
    selftest_case5,
    selftest_case6,
    selftest_case7,
};

int main(int argc, char* argv[])
{
    try
    {
        SysConfLite u = SysConfLite(conf_db_file);
        u.ConfTable("SysConf");

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





