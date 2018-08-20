/*-------------------------------------------------------------------------
    c_except.h
-------------------------------------------------------------------------*/
#ifndef _C_EXCEPT_H_
#define _C_EXCEPT_H_

#include <c_types.hpp>

class TException {
public:
    TException( ulong n, const char * s );
    TException( const TException & );
    ~TException();

    TException & operator = ( const TException & );

    ulong id() const { return Id; }
    const char * text() const { return Text; }

private:

    ulong Id;
    char * Text;
};


class TAccessExc : public TException {
public:
    TAccessExc( ulong n, const char * s );

};


class TParamExc : public TException {
public:
    TParamExc( ulong n, const char * s );
};


class TRequestExc : public TException {
public:
    TRequestExc( ulong n, const char * s );
};


#endif
