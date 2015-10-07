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
 *  \file   beagle/GP/src/BitwiseNot.cpp
 *  \brief  Source code of class GP::BitwiseNot.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.2 $
 *  $Date: 2007/08/08 19:26:36 $
 */

#include "Beagle/GP.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

using namespace Beagle;


/*!
 *  \brief Construct a new boolean NOT primitive.
 *  \param inName Name of the addition primitive.
 */
GP::BitwiseNot::BitwiseNot(std::string inName) :
		Beagle::GP::Primitive(1, inName)
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive BitwiseNot.
 *  \param  inN Index of the argument to get the type tag.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type needed.
 *  \throw  AssertException If the index inN given is greater than 1.
 */
const std::type_info* GP::BitwiseNot::getArgType(unsigned int inN, GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	Beagle_AssertM(inN<2);
	return &typeid(Int);
	Beagle_StackTraceEndM();
}


/*!
 *  \brief  Return the tag of the type of data return by primitive BitwiseNot.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
const std::type_info* GP::BitwiseNot::getReturnType(GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(Int);
	Beagle_StackTraceEndM();
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Execute the characteristic operation of logical NOT between of a bool.
 *  \param outResult Result of the logical NOT.
 *  \param ioContext Evolutionary context.
 */
void GP::BitwiseNot::execute(GP::Datum& outResult, GP::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	Int& lResult = castObjectT<Int&>(outResult);
	get1stArgument(lResult, ioContext);
	lResult = ~(lResult.getWrappedValue());
	Beagle_StackTraceEndM();
}
