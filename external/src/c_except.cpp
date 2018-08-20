/*-------------------------------------------------------------------------
    c_except.cpp
-------------------------------------------------------------------------*/
#include <string.h>
#include "c_types.hpp"
#include "c_except.h"

TException::TException( ulong n, const char * s ) :
    Id( n )
{
    if( s ){
        Text = new char [strlen(s)+1];
        strcpy( Text, s );
    }
    else
        Text = 0;
}


TException::TException( const TException & e ) :
    Id( e.Id )
{
    if( e.Text ){
        Text = new char [strlen(e.Text) + 1];
        strcpy( Text, e.Text );
    }
    else
        Text = 0;
}


TException::~TException()
{
    if( Text ) delete [] Text;
}


TException & TException::operator = ( const TException & e )
{
    if( Text ) delete [] Text;

    if( e.Text ){
        Text = new char [strlen(e.Text) + 1];
        strcpy( Text, e.Text );
    }
    else
        Text = 0;

    Id = e.Id;

    return *this;
}


TAccessExc::TAccessExc( ulong n, const char * s ) :
    TException( n, s )
{
}


TParamExc::TParamExc( ulong n, const char * s ) :
    TException( n, s )
{
}


TRequestExc::TRequestExc( ulong n, const char * s ) :
    TException( n, s )
{
}
