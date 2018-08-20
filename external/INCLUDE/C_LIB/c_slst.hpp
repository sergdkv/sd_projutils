/*-----------------------------------------------------------------------------
    c_slst.hpp
-----------------------------------------------------------------------------*/
#ifndef _C_SLST_HPP_
#define _C_SLST_HPP_


/*-----------------------------------------------------------------------------
    Простая реализация двунаправленного списка. Отсутствует поддержка
    одновременно нескольких курсоров.
    Элементы списка хранят указатели на объекты, которые ДОЛЖНЫ БЫТЬ созданы
    оператором new. Уничтожение этих объектов производится САМИМ СПИСКОМ, т.е.
    программа НЕ ДОЛЖНА вызывать для них delete.
-----------------------------------------------------------------------------*/
template <class T> class TSimpleList {
public:
    TSimpleList();

    ~TSimpleList();

/*
    Работа с курсором.
*/
    int setToFirst();
    int setToLast();
    int setToNext();
    int setToPrev();
/*
    Возвращаемое значение:
    0   курсор неопределен
    1   ok
*/
    int isValid() const;

/*
    Перевести курсор в неопределенное состояние.
*/
    void invalidate();

/*
    Возвращаемое значение:
    0       курсор не определен
    <>0     указатель, хранимый в элементе списка, на который указывает курсор
*/
    T * elementAt();

/*
    Добавляет элемент следом за курсором. Вызов функции не меняет положения курсора.
    Возвращаемое значение:
    0   курсор был неопределен - функция ничего не сделала
    1   ok
*/
    int addAt( T * );

/*
    Вызов функции не меняет положения курсора.
*/
    void addAsFirst( T * );

/*
    Вызов функции не меняет положения курсора.
*/
    void addAsLast( T * );

/*
    Удаляет элемент, на который указывает курсор. После вызова курсор указывает на элемент, следующий за удаленным.
    Возвращаемое значение:
    0   курсор был неопределен - функция ничего не сделала
    1   ok
*/
    int removeAt();

/*
    Возвращаемое значение:
    0   список не пуст
    1   список пустой
*/
    int isEmpty() const;

/*
    Текущее число элементов в списке.
*/
    uint32 numberOfElements() const;

/*
    Применить функцию ко всем элементам списка.
*/
    void applyToAll( void (* applyFunction)( T *, void * ), void * param = 0 );

private:

    // элемент списка
    class TElement {
    public:
        TElement( T * o, TElement * p, TElement * n ) : object( o ), prev( p ), next( n ) {}
        ~TElement() { delete object; }

        T * object;         // хранимый объект

        TElement * prev;
        TElement * next;
    };

    TElement * first;       // первый и последний элементы списка
    TElement * last;

    TElement * current;     // курсор

    uint32 nelements;        // размер списка

};




template <class T> TSimpleList<T>::TSimpleList() :
    first( 0 ),
    last( 0 ),
    current( 0 ),
    nelements( 0 )
{
}


template <class T> TSimpleList<T>::~TSimpleList()
{
    TElement * p = first;
    while( p ){
        TElement * next = p->next;
        delete p;
        p = next;
    }
}


template <class T> int TSimpleList<T>::addAt( T * o )
{
    if( current == 0 ) return 0;

    TElement * next = current->next;

    current->next = new TElement( o, current, next );

    if( next ) next->prev = current->next;

    nelements++;

    return 1;
}


template <class T> void TSimpleList<T>::addAsFirst( T * o )
{
    TElement * second = first;

    first = new TElement( o, 0, second );
    if( second ){       // на момент вызова список уже не был пустым
        second->prev = first;
    }
    else{               // на момент вызова список был пуст
        last = first;   // last был равен 0
    }

    nelements++;
}


template <class T> void TSimpleList<T>::addAsLast( T * o )
{
    TElement * beforelast = last;

    last = new TElement( o, beforelast, 0 );
    if( beforelast ){   // на момент вызова список уже не был пустым
        beforelast->next = last;
    }
    else{               // на момент вызова список был пуст
        first = last;   // first был равен 0
    }

    nelements++;
}


template <class T> int TSimpleList<T>::removeAt()
{
    if( current == 0 ) return 0;

    TElement * prev = current->prev;
    TElement * next = current->next;

    delete current;

    if( prev ) prev->next = next;
    if( next ) next->prev = prev;

    current = next;

    nelements--;

    return 1;
}


template <class T> void TSimpleList<T>::applyToAll( void (* applyFunction)( T *, void * ), void * param )
{
    TElement * p = first;
    while( p ){
        applyFunction( p->object, param );
        p = p->next;
    }
}


template <class T> inline int TSimpleList<T>::setToFirst()
{
    return (current = first) != 0;
}


template <class T> inline int TSimpleList<T>::setToLast()
{
    return (current = last) != 0;
}


template <class T> inline int TSimpleList<T>::setToNext()
{
    return current ? (current = current->next) != 0 : 0;
}


template <class T> inline int TSimpleList<T>::setToPrev()
{
    return current ? (current = current->prev) != 0 : 0;
}


template <class T> inline int TSimpleList<T>::isValid() const
{
    return current != 0;
}


template <class T> inline void TSimpleList<T>::invalidate()
{
    current = 0;
}


template <class T> inline T * TSimpleList<T>::elementAt()
{
    return current ? current->object : 0;
}


template <class T> inline int TSimpleList<T>::isEmpty() const
{
    return nelements == 0;
}


template <class T> inline uint32 TSimpleList<T>::numberOfElements() const
{
    return nelements;
}


#endif
