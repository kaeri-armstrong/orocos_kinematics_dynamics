#include "jacobiantests.hpp"
#include "jacobiandoubletests.hpp"
#include "jacobianframetests.hpp"


int main(int argc,char** argv) {
    ARMstrongKDL::checkDoubleOps();
    ARMstrongKDL::checkFrameOps();
	ARMstrongKDL::checkFrameVelOps();
}




