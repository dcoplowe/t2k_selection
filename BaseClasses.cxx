#include "BaseClasses.h"

#ifndef ND__TTrueVertex_cxx
#define ND__TTrueVertex_cxx
ND::TTrueVertex::TTrueVertex() {
}
ND::TTrueVertex::TTrueVertex(const TTrueVertex & rhs)
: TObject(const_cast<TTrueVertex &>( rhs ))
, Position(const_cast<TTrueVertex &>( rhs ).Position)
, ID(const_cast<TTrueVertex &>( rhs ).ID)
, Pur(const_cast<TTrueVertex &>( rhs ).Pur)
, Eff(const_cast<TTrueVertex &>( rhs ).Eff)
{
    // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
    // Use at your own risk!
    if (&rhs) {} // avoid warning about unused parameter
}
ND::TTrueVertex::~TTrueVertex() {
}
#endif // ND__TTrueVertex_cxx