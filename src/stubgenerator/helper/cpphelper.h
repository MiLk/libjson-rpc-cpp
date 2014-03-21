/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    cpphelper.h
 * @date    29.09.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef JSONRPC_CPP_CPPHELPER_H
#define JSONRPC_CPP_CPPHELPER_H

#include <string>
#include <jsonrpc/common/procedure.h>

namespace jsonrpc
{
    class CPPHelper
    {
        public:
            static std::string normalizeString  (const std::string &text);
            static std::string toCppType        (jsontype_t type, bool isConst = false, bool isReference = false);
            static std::string toCppConversion  (jsontype_t);
            static std::string isCppConversion(jsontype_t);
            static std::string toString         (jsontype_t type);
            static std::string generateParameterDeclarationList(Procedure& proc);
    };
}

#endif // JSONRPC_CPP_CPPSTUBGENERATOR_H
