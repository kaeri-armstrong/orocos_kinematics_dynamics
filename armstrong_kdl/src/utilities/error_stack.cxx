/*****************************************************************************
 *  \author 
 *  	Erwin Aertbelien, Div. PMA, Dep. of Mech. Eng., K.U.Leuven
 *
 *  \version 
 *		ORO_Geometry V0.2
 *
 *	\par History
 *		- $log$
 *
 *	\par Release
 *		$Id: error_stack.cpp,v 1.1.1.1.2.1 2003/02/24 13:13:06 psoetens Exp $
 *		$Name:  $ 
 ****************************************************************************/


#include "error_stack.h"
#include <stack>
#include <vector>
#include <string>
#include <cstring>

namespace ARMstrongKDL {

// Trace of the call stack of the I/O routines to help user
// interpret error messages from I/O
typedef std::stack<std::string>  ErrorStack;

// should be in Thread Local Storage if this gets multithreaded one day...
static ErrorStack errorstack;


void IOTrace(const std::string& description) {
    errorstack.push(description);
}


void IOTracePop() {
    errorstack.pop();
}

void IOTraceOutput(std::ostream& os) {
    while (!errorstack.empty()) {
        os << errorstack.top().c_str() << std::endl;
        errorstack.pop();
    }
}


void IOTracePopStr(char* buffer,int size) {
    if (size <= 0)
    {
        // TODO: all sizes everywhere should be of size_t!
        return;
    }
    if (errorstack.empty()) {
        *buffer = 0;
        return;
    }
#if defined(_WIN32)
    strncpy_s(buffer,size,errorstack.top().c_str(),size);
#else
    strncpy(buffer,errorstack.top().c_str(),size);
#endif
    buffer[size - 1] = '\0';
    errorstack.pop();
}

}
