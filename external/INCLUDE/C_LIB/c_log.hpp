/*-----------------------------------------------------------------------------
    c_log.hpp
-----------------------------------------------------------------------------*/
#ifndef _C_LOG_HPP_
#define _C_LOG_HPP_


#include <stdlib.h>


/*
    Класс TLog реализует механизм ведения log-файлов.
    Каждый раз при печати нового сообщения файл открывается/закрывается заново.
    Библиотека НЕ предназначена для одновременного обновления одного log-файла несколькими программами !
    Нуждается в проверке также и возможность чтения log-файла другой программой в то время, как он обновляется этим классом.
*/
class _Export TLog {
public:

/*
    Уровни отображаемых сообщений: ничего, ошибки, информационные сообщения, отладочные сообщения, дампы буферов.
*/
    enum MessageLevel { nothing = 1, err, inf, dbg, dmp };

/*
    Способы отображения сообщений: только в файл, в файл и на экран.
*/
    enum MessageDisplay { file = 1, filescreen };

/*
    Помимо инициализации внутренних переменных в log-файл добавляется предваряющее сообщение, которое может содержать
    указанный заголовок.
    Предваряющее сообщение печатается независимо от выбранного уровня отображаемых сообщений.

    Параметры:
    file_name   Полный путь log-файла.
    title       Заголовок, будет помещен первой строкой в log-файл.
    level       Задает уровень фактически отображаемых сообщений: сообщения с меньшим уровнем не отображаются.
    display     Задает допустимый способ отображения сообщений.
    append      0 - создать файл заново, 1 - добавлять записи в конец файла

    Исключения:
    TAccessExc( 1, "TLog::TLog: ошибка открытия файла" )   Возбуждается если с указанным файлом невозможно работать.
*/
    TLog( const char * file_name, const char * title = 0, MessageLevel level = dmp, MessageDisplay display = filescreen, int append = 1 ) throw ( TAccessExc );

/*
    Помещает в конец log-файла завершающее сообщение.
    Завершающее сообщение печатается независимо от выбранного уровня отображаемых сообщений.
*/
    ~TLog();

    TLog & setLogLevel( MessageLevel );
    TLog & setLogDisplay( MessageDisplay );

/*
    Печать текста - общая функция.
    Символ '\n' в конце сообщения печатается автоматически.

    Параметры:
    priority    Уровень (приоритет) печатаемого сообщения. Если приоритет сообщения меньше указанного в конструкторе, оно
                печататься не будет. Значения nothing и dump не предназначены для использования в этой функции, они нужны
                для конструктора. Если тем не менее функция получит значение nothing, сообщение не будет напечатано вообще,
                если получит значение dump, приоритет сообщения будет действительно рассматриваться как dump, однако для
                печати дампов предназначены специальные функции dump().
    display     Запрашиваемый уровень отображения сообщения. Фактический уровень вывода определяется совместно с аналогичным
                параметром, указанном при вызове конструктора.
                file        Напечатать сообщение только в файл.
                filescreen  Напечатать сообщение и в файл, и на экран.

    Возвращаемое значение:
    0           ok
    1           ok, но сообщение не было напечатано, так как у него недостаточный приоритет
    2           ошибка доступа к log-файлу

*/
    int print( const char * text, MessageLevel priority = inf, MessageDisplay display = file );

/*
    Печать информационого сообщения - синоним print( test, TLog::inf, TLog::filescreen ).
*/
    int info( const char * text );

/*
    Печать ошибки - синоним print( text, TLog::err, TLog::filescreen ).
*/
    int error( const char * text );

/*
    Печать отладочного сообщения - синоним print( text, TLog::dbg, TLog::file ).
*/
    int debug( const char * text );

/*
    Печать строчного и шестнадцатиричного форматированного дампа буфера памяти.
    Приоритет печати - dmp.
*/
    int dump( const char * buf, uint32 buf_size, MessageDisplay display = file );

/*
    Печать дампа с предваряющей строкой заголовка (что удобно использовать для подписи дампа). Символ '\n' печатается
    после строки заголовка автоматически.
    Приоритет печати как самого дампа, так и заголовка - dmp.
*/
    int dump( const char * text, const char * buf, uint32 buf_size, MessageDisplay display = file );

private:
    MessageLevel fLevel;
    MessageDisplay fDisplay;

    char fFileName[_MAX_PATH];
};


inline TLog & TLog::setLogLevel( MessageLevel m )
{
    fLevel = m;
    return *this;
}


inline TLog & TLog::setLogDisplay( MessageDisplay m )
{
    fDisplay = m;
    return *this;
}


#endif