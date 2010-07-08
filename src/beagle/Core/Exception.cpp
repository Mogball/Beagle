/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/Core/Exception.cpp
 *  \brief  Implementation of class Exception.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/09/12 14:23:47 $
 */

#include "beagle/Core.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a Beagle::Exception with the message given.
 *  \param inMessage Message describing the exception.
 */
Exception::Exception(std::string inMessage) :
		mMessage(inMessage)
{ }


/*!
 *  \brief Explain the exception throwed in the stream given.
 *  \param ioES Output C++ stream where the explanation about the exception is inserted.
 */
void Exception::explain(std::ostream& ioES) throw()
{
	std::string lMessage = std::string("'")+mMessage+std::string("'");

#ifndef BEAGLE_NDEBUG
	if(!mStackTrace.empty()) {
		lMessage += "\nIn:";
		for (unsigned int i=0; i<mStackTrace.size(); i++) {
			lMessage += std::string("\n")+uint2str(i)+std::string(": ")+mStackTrace[i].mName+
			            std::string("\t(")+mStackTrace[i].mFilename+std::string(", line ")+
			            int2str(mStackTrace[i].mLineNumber)+std::string(")");
		}
	}
#endif // BEAGLE_NDEBUG

	wrapString(lMessage);
	ioES << lMessage << std::endl << std::flush;
}


/*!
 *  \brief Terminate execution of actual program by displaying exception message et aborting.
 *  \param ioES C++ output stream to display the error.
 */
void Exception::terminate(std::ostream& ioES) throw()
{
	ioES << getExceptionName() << ":" << std::endl << std::flush;
	explain(ioES);
	ioES << std::endl << std::flush;
	abort();
}

/*!
 *  \brief  Return a copy of the error message.
 *  \return Error message.
 */
const char* Exception::what() const throw()
{
	return mMessage.c_str();
}

