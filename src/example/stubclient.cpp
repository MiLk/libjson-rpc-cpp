/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    stubclient.cpp
 * @date    01.05.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <iostream>

#include "gen/stubclient.h"

using namespace jsonrpc;
using namespace std;

int main()
{
    HttpClient httpclient("http://localhost:8383");
    StubClient c(httpclient);

    try
    {
        cout << c.sayHello("Peter Knafl") << endl;
        c.notifyServer();

        cout << " 3 + 5 = " << c.addNumbers(3,5) << endl;
        cout << " 3.2 + 5.3 = " << c.addNumbers2(3.2,5.3) << endl;
        cout << "Compare: " << c.isEqual("Peter", "peter") << endl;
        cout << "Build object: " << c.buildObject("Peter", 1990) << endl;

    }
    catch (JsonRpcException e)
    {
        cerr << e.what() << endl;
    }


}

