#pragma once
#include "LT.h"
#include "In.h"
#include "Log.h"
#include "FST.h"

#define TYPE_NUMBER		"uint"
#define TYPE_STRING		"str"
#define TYPE_BOOL		"bool"
#define MAIN			"main"
#define TYPE_PROCEDURE	"procedure"
#define CONCAT			"concat"
#define LENGTH			"length"
#define STRTON			"strton"

#define ISTYPE(str) ( !strcmp(str, TYPE_NUMBER) || !strcmp(str, TYPE_STRING) || !strcmp(str, TYPE_BOOL) )

namespace LA
{
	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable	idtable;
		LEX() {}
	};
	struct FstTable
	{
		char lexema;
		FST::FST graph;
	};

	

	IT::Entry* getEntry		// ��������� � ���������� ������ ��
	(
		LA::LEX& tables,						// �� + ��
		char lex,								// �������
		char* id,								// �������������
		char* idtype,							// ���������� (���)
		bool isParam,							// ������� ��������� �������
		bool isFunc,							// ������� �������
		Log::LOG log,							// ��������
		int line,								// ������ � �������� ������
		bool& rc_err							// ���� ������(�� ������)
	);

	struct ERROR_S									// ��� ���������� ��� throw ERROR_THROW | ERROR_THROW_IN
	{
		int id;
		char message[ERROR_MAXSIZE_MESSAGE];			// ���������			
		struct
		{
			short line = -1;						//����� ������ (0, 1, 2, ...)
			short col = -1;						//����� ������� � ������(0, 1, 2, ...)
		} position;
	};
	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm);
	int getIndexInLT(LT::LexTable &lextable, int itTableIndex);					// ������ ������ ������� � ������� ������
};

