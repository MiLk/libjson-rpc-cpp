/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    test_integration.cpp
 * @date    28.09.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE

#include <boost/test/unit_test.hpp>
#include <jsonrpc/server.h>
#include <jsonrpc/client.h>
#include "server.h"

using namespace jsonrpc;
using namespace std;

BOOST_AUTO_TEST_CASE(test_helloworld)
{
    HttpServer sconn(8383);
    HttpClient cconn("http://localhost:8383");
    TestServer server(sconn);
    Client client(cconn);

    server.StartListening();

    Json::Value v;
    v["name"] = "Peter";
    Json::Value result = client.CallMethod("sayHello", v);
    BOOST_CHECK_EQUAL(result.asString(),  "Hello: Peter!");

    v["name"] = "Peter Spiess-Knafl";
    result = client.CallMethod("sayHello", v);
    BOOST_CHECK_EQUAL(result.asString(), "Hello: Peter Spiess-Knafl!");

    server.StopListening();
}

BOOST_AUTO_TEST_CASE(test_connector)
{
    HttpServer sconn(8383);
    HttpServer sconn2(8383);
    TestServer server(sconn);
    TestServer server2(sconn2);

    BOOST_CHECK_EQUAL(server.StartListening(), true);
    BOOST_CHECK_EQUAL(server.StartListening(), true);

    BOOST_CHECK_EQUAL(server.StopListening(), true);
    BOOST_CHECK_EQUAL(server.StopListening(), true);

    //Start two servers on same port should not be possible.
    BOOST_CHECK_EQUAL(server.StartListening(), true);
    BOOST_CHECK_EQUAL(server2.StartListening(), false);
    server.StopListening();
}

bool is_critical( JsonRpcException const& ex ) { return ex.GetCode() != -32303; }
BOOST_AUTO_TEST_CASE(test_clientexception)
{
    HttpClient cconn("http://localhost:8383");
    Client client(cconn);
    BOOST_CHECK_EXCEPTION(client.CallNotification("initCounter", Json::nullValue), JsonRpcException, is_critical);
}

BOOST_AUTO_TEST_CASE(test_remotecalculator)
{
    HttpServer sconn(8383);
    HttpClient cconn("http://localhost:8383");
    TestServer server(sconn);
    Client client(cconn);
    Json::Value v;

    server.StartListening();

    v["value1"] = 1432;
    v["value2"] = 556;
    BOOST_CHECK_EQUAL(client.CallMethod("add",v).asInt(), 1988);

    v["value1"] = -1432;
    v["value2"] = 556;
    BOOST_CHECK_EQUAL(client.CallMethod("add",v).asInt(), -876);

    v["value1"] = -1432;
    v["value2"] = 556;
    BOOST_CHECK_EQUAL(client.CallMethod("sub",v).asInt(), -1988);


    v["value1"] = 1432;
    v["value2"] = 556;
    BOOST_CHECK_EQUAL(client.CallMethod("sub",v).asInt(), 876);
    server.StopListening();
}

BOOST_AUTO_TEST_CASE(test_remotecounter)
{
    HttpServer sconn(8383);
    HttpClient cconn("http://localhost:8383");
    TestServer server(sconn);
    Client client(cconn);
    Json::Value v;

    server.StartListening();

    v["value"] = 3;
    client.CallNotification("initCounter", v);

    BOOST_CHECK_EQUAL(client.CallMethod("getCounterValue", Json::nullValue), 3);

    client.CallNotification("incrementCounter", v);
    BOOST_CHECK_EQUAL(client.CallMethod("getCounterValue", Json::nullValue), 4);
    server.StopListening();
}

BOOST_AUTO_TEST_CASE(test_jsonrpcprotocol)
{
    HttpServer sconn(8383);
    HttpClient cconn("http://localhost:8383");
    TestServer server(sconn);
    Client client(cconn);
    Json::Value v;
    server.StartListening();

    v["name"] = "Peter";
    client.CallMethod("sayHello", v);

    BOOST_CHECK_THROW(client.CallMethod("nonexistingmethod", v), JsonRpcException);

    v.removeMember("name");
    v["aaawrwer"] = "asökfjölwer";
    BOOST_CHECK_THROW(client.CallMethod("sayHello", v), JsonRpcException);
    BOOST_CHECK_THROW(client.CallMethod("incrementCounter", v), JsonRpcException);
}

BOOST_AUTO_TEST_CASE(test_specification)
{

}

BOOST_AUTO_TEST_CASE(test_parametervalidation)
{
    Procedure proc1("someprocedure", PARAMS_BY_NAME, JSON_BOOLEAN, "name", JSON_STRING, "ssnr", JSON_INTEGER, NULL);

    //Expected to pass validation
    Json::Value param1;
    param1["name"] = "Peter";
    param1["ssnr"] = 4711;
    BOOST_CHECK_EQUAL(proc1.ValidateNamedParameters(param1), true);

    //Expected to fail validation
    Json::Value param2;
    param2.append("Peter");
    param2.append(4711);
    BOOST_CHECK_EQUAL(proc1.ValidateNamedParameters(param2), false);

    //Expected to fail validation
    Json::Value param3;
    param3.append(4711);
    param3.append("Peter");
    BOOST_CHECK_EQUAL(proc1.ValidateNamedParameters(param3), false);

}
