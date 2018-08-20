/**********************************************************\
 calibr
 �����஢�� ��室��� ������

 ��ࠬ����:
    const TBlk0 * inb0
        �㫥��� ���� ��室��� ������
    const short * indata
        ��室�� ����� AVHRR
    const short * tlmdata
        ����� ⥫����ਨ
    TBlk0 * outb0
        �㫥��� ���� १���� (������ �뤥����� ���짮��⥫��)
    short *outdata
        १���� (������ �뤥����� ���짮��⥫��, ࠧ��� ������ ࠢ������ ࠧ���� ��室��� ������)
    const char * calibr_dat
        ���䨣��樮���� ��ப� (������ ᮮ⢥��⢮���� ᮤ�ন���� calibr.dat)

 � ��砥 ��㤠� ����㦤����� �᪫�祭�� ⨯� CalibrException;
 �������� ���� �訡��:
	1 --- �訡�� ࠧ��� ���䨣��樮����� �����
	2 --- ������ ⨯ �㫥���� �����
	3 --- �㦥� ���� ⥫����ਨ
	4 --- �訡�� ���᪠ ��ࠬ��஢ ���䨣��樮����� �����
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
