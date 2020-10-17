/*
 * @Author: your name
 * @Date: 2020-10-17 21:54:11
 * @LastEditTime: 2020-10-17 22:17:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /mycode/server.cc
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>

using namespace std;
using namespace libconfig;

/* g++ -o demo demo.cpp -I/home/leoox/local/libconfig-1.4.9/include/ /home/leoox/local/libconfig-1.4.9/lib/libconfig++.a */
int main( int argc,char** argv )
{
    Config cfg;

    /*
      解析配置文件。
    */
    try
    {
        cfg.readFile( "server.conf" );
    }
    catch ( const FileIOException& fioex )
    {
        std::cerr << "I/O error while reading file." << std::endl;
        return( EXIT_FAILURE );
    }
    catch ( const ParseException& pex )
    {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
            << " - " << pex.getError() << std::endl;
        return( EXIT_FAILURE );
    }

    /* 从配置文件中，得到version这个配置项的值。 */
    try
    {
        string version = cfg.lookup( "version" );
        cout << "version: " << version << endl << endl;
    }
    catch ( const SettingNotFoundException& nfex )
    {
        cerr << "No 'version' setting in configuration file." << endl;
    }

    /* 从配置文件中，得到日志相关配置值 */
    try
    {
        string log_path = cfg.lookup( "log.log_path" );
        cout << "log_path: " << log_path << endl;

        int64_t log_size = cfg.lookup( "log.log_size" );
        cout << "log_size: " << log_size << endl;

        int log_level = cfg.lookup( "log.log_level" );
        cout << "log_level: " << log_level << endl << endl;
    }
    catch ( const SettingNotFoundException& nfex )
    {
        cerr << "log setting mistake in configuration file." << endl;
    }

    try
    {
        const Setting& server = cfg.lookup( "server" );
        int count = server.getLength();
        cout << "server.count = " << count << endl;

        for ( int i = 0; i < count; i++ )
        {
            string addr = "";
            int port = 0;
            if ( !server[i].lookupValue( "addr",addr )
                || !server[i].lookupValue( "port",port ) )
            {
                cerr << "lookupValue error" << endl;
                continue;
            }

            cout << "server[" << i << "] = " << addr << ":" << port << endl;
        }

        {
            string addr = "";
            int port = 0;
            if ( !cfg.lookupValue( "server.[0].addr",addr )
                || !cfg.lookupValue( "server.[0].port",port ) ) {
                cerr << "lookupValue 'server.[0].addr' error" << endl;
            }
            else
                cout << "server[0] = " << addr << ":" << port << endl << endl;
        }
    }
    catch ( const SettingNotFoundException& nfex )
    {
        cerr << "server setting mistake in configuration file." << endl;
    }

    try
    {
        const Setting& root = cfg.getRoot();
        const Setting& uin = root["test"]["uin"];
        int count = uin.getLength();
        cout << "uin.count = " << count << endl;

        const Setting& test = cfg.lookup( "test" );
        const Setting& test2 = cfg.lookup( "test.uin" );
        for ( int i = 0; i < count; i++ )
        {
            int64_t u = test["uin"][i];
            int64_t uu = uin[i];
            int64_t uuu = test2[i];
            cout << "uin[" << i << "] = " << u << ", " << uu << ", " << uuu << endl;
        }

        const Setting& server = root["test"]["server"];
        string addr = "";
        int port = 0;
        if ( !server.lookupValue( "addr",addr )
            || !server.lookupValue( "port",port ) )
        {
            cerr << "test server lookupValue error" << endl;
        }
        else
            cout << "test server = " << addr << ":" << port << endl << endl;
    }
    catch ( const SettingNotFoundException& nfex )
    {
        cerr << "test setting mistake in configuration file." << endl;
    }

    return 0;
}