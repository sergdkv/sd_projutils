#include <vector>
// Для TBlk0_Proj
#include <c_types.hpp>  
#include <c_b0.hpp>
#include <c_b0proj.hpp>

#include <string>
#include <string>

typedef std::vector<short> TProjShortLine; 
typedef std::vector<TProjShortLine> TProjShortData;

typedef std::vector<float> TProjFloatLine; 
typedef std::vector<TProjFloatLine> TProjFloatData;

/// данные и нулевой блок проекции нового формата lab34
struct TLab34NProj {
public :
    TProjFloatData geo; ///< матрица физических величин проекции
    TProjShortData raw; ///< матрица сырых данных проекции
    TBlk0_Proj blk0;    ///< нулевой блок нового формата

    TLab34NProj(); ///< конструктор пустой (проекция инициализируется из файла вызовом функции read)
    /**
    загрузка данных в объект-проекцию
    @param file_name имя файла-проекции нового формата
    @param proj данный объект-проекция будет модифицирован
    @return функция возвращает ссылку на модифицированный объект-проекцию

    В случае ошибки функция прерывает выполнение программы
    */
    static TLab34NProj& read( std::string file_name,  TLab34NProj& proj );
    /**
    копирование данных из одной проекции в другую
    @param from_proj источник данных
    @param to_proj   данный объект-проекция будет модифицирован
    @return функция возвращает ссылку на модифицированный объект-проекцию

    В случае ошибки функция прерывает выполнение программы
    */
    static TLab34NProj& copyFromTo( TLab34NProj& from_proj, TLab34NProj& to_proj );

    /// количество строк в проекции
    int lines() const {  
       return  blk0.scanNum; 
    };

    /// количество пикселей в строке проекции
    int cols() const {  
       return  blk0.pixNum;
    };


    /**
    сохранение данных объект-проекции в файл
    @param file_name имя файла-проекции нового формата
    @return функция возвращает ссылку на объект-проекцию

    В случае ошибки функция прерывает выполнение программы
    */
    TLab34NProj& write( std::string file_name);
    /// Копирующий конструктор
    TLab34NProj( const TLab34NProj& old );
    /**
    @brief Очистка проекции

    Все пиксели проекции маркируются указанным значением
    @param cloud_value заносится во все пиксели массива raw 
           (рекомендумое значение -1)
    */
    TLab34NProj& clear( int cloud_value );
    /**
     @brief Долгота центра пикселя в градусах
     @param  column 0 <= column  < pixNum
     @ingroup coords
     @ingroup lon
    */
    double centerLon_for_Column( double column ) const;
    /**
     Номер колонки в которую попадают пиксели с указанной долготой
        (долгота в градусах восточной долготы)
     @ingroup coords
     @ingroup lon
    */
    int getColumn_for_Lon( double lon ) const;
    /**
     @brief широта центра пикселя в градусах
     @param  line 0 <= line  < blk0.scanNum
     @ingroup coords
     @ingroup lat
    */
    double centerLat_for_Line( double line ) const;
    /**
     @brief строка, в которую попадают пиксели с указанной широтой
     @ingroup coords
     @ingroup lat
    */
    int getLine_for_Lat( double lat ) const ;
private :
    double mercatorLat( double lat ) const;
    double unmercatorLat( double mlat ) const;
};


