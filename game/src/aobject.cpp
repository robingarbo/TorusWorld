#ifndef _DEF_AOBJECT
#define _DEF_AOBJECT

#include <iostream>

#include "AObject.hh"

AObject::AObject()
{

}

AObject::~AObject()
{

}

AObject::getType(void) const
{
	return this->type;
}


#endif // !_DEF_AOBJECT
