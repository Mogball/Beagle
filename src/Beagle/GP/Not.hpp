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
 *  \file   beagle/GP/Not.hpp
 *  \brief  Definition of the templated class GP::Not.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:35 $
 */

#ifndef Beagle_GP_Not_hpp
#define Beagle_GP_Not_hpp

#include <string>

#include "Beagle/config.hpp"
#include "Beagle/macros.hpp"
#include "Beagle/Core/Object.hpp"
#include "Beagle/Core/AllocatorT.hpp"
#include "Beagle/Core/PointerT.hpp"
#include "Beagle/Core/ContainerT.hpp"
#include "Beagle/Core/WrapperT.hpp"
#include "Beagle/Core/Bool.hpp"
#include "Beagle/GP/Primitive.hpp"
#include "Beagle/GP/PrimitiveInline.hpp"
#include "Beagle/GP/Context.hpp"
#include "Beagle/GP/Datum.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


namespace Beagle
{
namespace GP
{

/*!
 *  \class Not beagle/GP/Not.hpp "beagle/GP/Not.hpp"
 *  \brief GP boolean NOT primitive class.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup BoolPrimit
 */
class Not : public Primitive
{

public:

	//! GP::Not allocator type.
	typedef AllocatorT<Not,Primitive::Alloc>
	Alloc;
	//! GP::Not handle type.
	typedef PointerT<Not,Primitive::Handle>
	Handle;
	//! GP::Not bag type.
	typedef ContainerT<Not,Primitive::Bag>
	Bag;

	explicit Not(std::string inName="NOT");
	virtual ~Not()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void execute(GP::Datum& outResult, GP::Context& ioContext);

};

}
}


#endif // Beagle_GP_Not_hpp
