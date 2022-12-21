#pragma once
#include <iostream>

#define MAXSIZE_ID	8					//���� ����� �������� ��������������
#define SCOPED_ID_MAXSIZE   MAXSIZE_ID*2 //���� ����� �������� ������������� + ������� ���������
#define MAXSIZE_TI		4096			//���� ����� ���������� ����� � ������� ���������������
#define NUM_DEFAULT	0x00000000		//�������� �� ��������� ��� int
#define STR_DEFAULT	0x00			//�������� �� ��������� ��� sting
#define BOOL_DEFAULT 1				//�������� �� ��������� ��� bool
#define NULLIDX_TI		0xffffffff		//��� �������� ������� ���������������
#define STR_MAXSIZE	255				//������������ ����� ���������� ��������
#define NUM_MAXSIZE   4294967295		//������������ �������� ��� ���� number
#define NUM_MINSIZE		0	//����������� �������� ��� ���� number
#define BOOL_MAXSIZE 1
#define BOOL_MINSIZE 0
#define MAX_PARAMS_COUNT 3				//������������ ���������� ���������� � �������
#define CONCAT_PARAMS_CNT 2			//���-�� ���������� � ������� concat
#define LENGTH_PARAMS_CNT 1				//���-�� ���������� � ������� lenght
#define STRTON_PARAMS_CNT 1				//���-�� ���������� � ������� strtonumber
#define CONCAT_TYPE IT::IDDATATYPE::STR // ������������ ��������
#define LENGTH_TYPE IT::IDDATATYPE::NUM
#define STRTON_TYPE IT::IDDATATYPE::NUM

namespace IT
{	
	enum IDDATATYPE { NUM = 1, STR = 2, BOOL = 3,  PROC = 4, UNDEF };//���� ������ ���������������: ��������, ���������, ����������, ��� ����(��� ��������), ��������������
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5 };	//���� ���������������: ����������, �������, ��������, �������, ����������� �������
	enum STDFNC { F_CONCAT, F_LENGTH, F_STRTON, F_NOT_STD };	//����������� �������
	static const IDDATATYPE CONCAT_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };//��������� �������  concatstr
	static const IDDATATYPE LENGTH_PARAMS[] = { IT::IDDATATYPE::STR };//��������� ������� strlen
	static const IDDATATYPE STRTON_PARAMS[] = { IT::IDDATATYPE::STR };//��������� ������� strton

	struct Entry // ������ ������� ���������������
	{
		union
		{
			int	vint;            			//�������� integer
			struct
			{
				int len;					//���������� ��������
				char str[STR_MAXSIZE - 1]; //�������
			} vstr;							//�������� ������
			bool vbool;
			struct
			{
				int count;					// ���������� ���������� �������
				IDDATATYPE *types;			//���� ���������� �������
			} params;
		} value;						//�������� ��������������
		int			idxfirstLE;				//������ � ������� ������		
		char		id[SCOPED_ID_MAXSIZE];	//�������������
		IDDATATYPE	iddatatype;				//��� ������
		IDTYPE		idtype;					//��� ��������������
		
		Entry()							//����������� ��� ����������
		{
			this->value.vint = NUM_DEFAULT;
			this->value.vstr.len = NULL;
			this->value.vbool = BOOL_DEFAULT;
			this->value.params.count = NULL;
		};
		Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype) //����������� � �����������
		{
			strncpy_s(this->id, id, SCOPED_ID_MAXSIZE - 1);
			this->idxfirstLE = idxLT;
			this->iddatatype = datatype;
			this->idtype = idtype;
		};
	};
	struct IdTable		//��������� ������� ���������������
	{
		int maxsize;	//������� ������� ��������������� < TI_MAXSIZE
		int size;		//������� ������ ������� ��������������� < maxsize
		Entry* table;	//������ ����� ������� ���������������
	};
	IdTable Create(int size = NULL);	//������� ������� ��������������� < TI_MAXSIZE
	void Add(					//�������� ������ � ������� ���������������
		IdTable &idtable,		//��������� ������� ���������������
		Entry entry);			//������ ������� ��������������� 
	int isId(					//�������: ����� ������(���� ����), TI_NULLIDX(���� ����)
		IdTable &idtable,		//��������� ������� ���������������
		char id[SCOPED_ID_MAXSIZE]);	//�������������
	bool SetValue(IT::Entry* entry, char* value);	//������ �������� ��������������
	bool SetValue(IT::IdTable& idtable, int index, char* value);
	void writeIdTable(std::ostream *stream, IT::IdTable &idtable); //������� ������� ���������������
};
