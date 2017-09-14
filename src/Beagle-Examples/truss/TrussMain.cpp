#include "Beagle/EC.hpp"
#include "Beagle/BitStr.hpp"

#include <numeric>

using namespace std;
using namespace Beagle;

int main(int argc, char** argv) {
	try {
		System::Handle system = new System;

	} catch (Exception &inException) {
		inException.terminate(cerr);
	} catch (exception &inException) {
		cerr << "Standard exception caught" << endl << flush;
		cerr << inException.what() << endl << flush;
	}
}
