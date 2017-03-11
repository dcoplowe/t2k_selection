#ifndef ND__TTrueVertex_cxx
#define ND__TTrueVertex_cxx
TTrueVertex::TTrueVertex() {
}
TTrueVertex::TTrueVertex(const TTrueVertex & rhs)
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
TTrueVertex::~TTrueVertex() {
}
#endif // ND__TTrueVertex_cxx

#ifndef ND__TTrueParticle_cxx
#define ND__TTrueParticle_cxx
TTrueParticle::TTrueParticle() {
}
TTrueParticle::TTrueParticle(const TTrueParticle & rhs)
: TObject(const_cast<TTrueParticle &>( rhs ))
, ID(const_cast<TTrueParticle &>( rhs ).ID)
, Pur(const_cast<TTrueParticle &>( rhs ).Pur)
, Eff(const_cast<TTrueParticle &>( rhs ).Eff)
, Vertex(const_cast<TTrueParticle &>( rhs ).Vertex)
{
    // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
    // Use at your own risk!
    if (&rhs) {} // avoid warning about unused parameter
}
TTrueParticle::~TTrueParticle() {
}
#endif // ND__TTrueParticle_cxx
