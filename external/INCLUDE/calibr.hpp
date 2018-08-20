/**********************************************************\
 calibr
 калибровка исходных данных

 параметры:
    const TBlk0 * inb0
        нулевой блок исходных данных
    const short * indata
        исходные данные AVHRR
    const short * tlmdata
        данные телеметрии
    TBlk0 * outb0
        нулевой блок результата (память выделяется пользователем)
    short *outdata
        результат (память выделяется пользователем, размер должен равняться размеру исходных данных)
    const char * calibr_dat
        конфигурационная строка (должна соответствовать содержимому calibr.dat)

 В случае неудачи возбуждается исключение типа CalibrException;
 Возможные коды ошибок:
	1 --- ошибка разбора конфигурационного блока
	2 --- неверный тип нулевого блока
	3 --- нужен блок телеметрии
	4 --- ошибка поиска параметров конфигурационного блока
\***********************************************************/
void _Export calibr( const TBlk0 * inb0, const short * indata, const short * tlmdata, TBlk0 * outb0, short *outdata, const char * calibr_dat ) throw(CalibrException);


class _Export CalibrException {
	int id;
	char* msg;

	public:
	~CalibrException();
	CalibrException( int id, const char *fmt, ... );
	
	const char* getMsg() const;
	int getId() const;
};
