// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include "shared.h"

//
// Flag constant used by the Shared class. Derived classes
// such as GCObject define more flag constants.
//
const unsigned char ICT::Shared::NoDelete = 1;

ICT::SimpleShared::SimpleShared() :
  _ref(0),
  _noDelete(false)
{
}

ICT::SimpleShared::SimpleShared(const SimpleShared&) :
  _ref(0),
  _noDelete(false)
{
}

ICT::Shared::Shared() :
  _ref(0),
  _flags(0)
{
}

ICT::Shared::Shared(const Shared&) :
  _ref(0),
  _flags(0)
{
}

void
ICT::Shared::__incRef()
{
  _ASSERT(_ref >= 0);
  ++_ref;
}

void
ICT::Shared::__decRef()
{
  _ASSERT(_ref > 0);
  if (--_ref == 0 && !(_flags & NoDelete))
  {
    delete this;
  }
}

int
ICT::Shared::__getRef() const
{
  return _ref;
}

void
ICT::Shared::__setNoDelete(bool b)
{
  _flags = b ? (_flags | NoDelete) : (_flags & ~NoDelete);
}
