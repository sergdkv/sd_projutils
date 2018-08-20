/*-------------------------------------------------------------------------
    c_except.hpp
-------------------------------------------------------------------------*/
#ifndef _C_EXCEPT_HPP_
#define _C_EXCEPT_HPP_


class _Export TException {
public:
    TException( uint32 n, const char * s );
    TException( const TException & );
    ~TException();

    TException & operator = ( const TException & );

    uint32 id() const { return Id; }
    const char * text() const { return Text; }

private:

    uint32 Id;
    char * Text;
};


class _Export TAccessExc : public TException {
public:
    TAccessExc( uint32 n, const char * s );

};


class _Export TParamExc : public TException {
public:
    TParamExc( uint32 n, const char * s );
};


class _Export TRequestExc : public TException {
public:
    TRequestExc( uint32 n, const char * s );
};


#endif
