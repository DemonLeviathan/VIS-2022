#include "pch.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#define W(x, y)\
		<< std::setw(x) << (y) <<
#define STR(n, line, type, id)\
		"|" W(4,n) " |  " W(5,line) "    |" W(17,type) " |  " W(SCOPED_ID_MAXSIZE, id) " |"


namespace IT
{
	IdTable Create(int size)
	{
		if (size > MAXSIZE_TI)
			throw ERROR_THROW(203);
		IdTable idtable;
		idtable.table = new Entry[idtable.maxsize = size];
		idtable.size = NULL;
		return idtable;
	}

	void Add(IdTable &idtable, Entry entry)
	{
		if (idtable.size >= idtable.maxsize)	
			throw ERROR_THROW(203);
		idtable.table[idtable.size++] = entry;
	}

	// возврат: номер строки(если есть), TI_NULLIDX(если нет)
	int isId(IdTable& idtable, char id[SCOPED_ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0)
				return i;
		}
		return NULLIDX_TI;
	}

	bool SetValue(IT::IdTable& idtable, int index, char* value)
	{
		return SetValue(&(idtable.table[index]), value);
	}

	bool SetValue(IT::Entry* entry, char* value) // установка значения переменной
	{
		bool rc = true;
		if (entry->iddatatype == NUM)
		{
			std::stringstream strValue;
			strValue << value;
			unsigned int temp;
			strValue >> temp;
			if (temp >= NUM_MAXSIZE || temp < NUM_MINSIZE)
			{
				if (temp > NUM_MAXSIZE)
					temp = NUM_MAXSIZE;
				if (temp < NUM_MINSIZE) 
					temp = NUM_MINSIZE;
				rc = false;
			}
			entry->value.vint = temp;
		}
		else if (entry->iddatatype == BOOL)
		{
			std::stringstream strValue;
			strValue << value;
			bool temp;
			strValue >> temp;
			if (temp != BOOL_MINSIZE)
				temp = FST_BOOL_TRUE_LITERAL;
			else if (temp == BOOL_MINSIZE)
				temp = FST_BOOL_FALSE_LITERAL;
			rc = false;
			entry->value.vbool = temp;
		}
		else
		{
			for (unsigned i = 1; i < strlen(value) - 1; i++)	// без кавычек
				entry->value.vstr.str[i - 1] = value[i];
			entry->value.vstr.str[strlen(value) - 2] = '\0';
			entry->value.vstr.len = strlen(value) - 2;
		}
		return rc;
	}
	void writeIdTable(std::ostream* stream, IT::IdTable& idtable)
	{
		*stream << "---------------------------- TABLE OF INDIFICATORS ------------------------\n" << std::endl;
		*stream << "|  N  |СТРОКА В ТЛ| ТИП ИДЕНТИФИКАТОРА |        ИМЯ        | ЗНАЧЕНИЕ (ПАРАМЕТРЫ)" << std::endl;
		for (int i = 0; i < idtable.size; i++)
		{
			IT::Entry *e = &idtable.table[i];
			char type[50] = "";

			switch (e->iddatatype) 
			{
				case IT::IDDATATYPE::NUM: 
					strcat_s(type, "  number "); 
					break;
				case IT::IDDATATYPE::STR:
					strcat_s(type, " string  "); 
					break;
				case IT::IDDATATYPE::PROC: 
					strcat_s(type, "   proc  ");
					break;
				case IT::IDDATATYPE::BOOL:
					strcat_s(type, " bool");
					break;
				case IT::IDDATATYPE::UNDEF: 
					strcat_s(type, "UNDEFINED");
					break;
			}
			switch (e->idtype)
			{
				case IT::IDTYPE::V:
					strcat_s(type, "  variable");
					break;
				case IT::IDTYPE::F:
					strcat_s(type, "  function");
					break;
				case IT::IDTYPE::P:
					strcat_s(type, " parameter");
					break;
				case IT::IDTYPE::L:
					strcat_s(type, "   literal");
					break;
			    case IT::IDTYPE::S: strcat_s(type, "  LIB FUNC"); break;
				default:
					strcat_s(type, "UNDEFINED ");
					break;
			}

			*stream << STR(i, e->idxfirstLE, type, e->id);
			if (e->idtype == IT::IDTYPE::L || e->idtype == IT::IDTYPE::V && e->iddatatype != IT::IDDATATYPE::UNDEF)
			{
				if (e->iddatatype == IT::IDDATATYPE::NUM)
					*stream << e->value.vint;
				else if (e->iddatatype == IT::IDDATATYPE::BOOL)
					*stream << e->value.vbool;
				else 
					*stream << "[" << (int)e->value.vstr.len << "]" << e->value.vstr.str;
			}
			if (e->idtype == IT::IDTYPE::F || e->idtype == IT::IDTYPE::S)
			{
				for (int i = 0; i < e->value.params.count; i++)
				{
					*stream << " P" << i << ":";
					switch (e->value.params.types[i]) 
					{
						case IT::IDDATATYPE::NUM:
							*stream << "NUMBER |"; 
							break;
						case IT::IDDATATYPE::STR:
							*stream << "STRING |";
							break;
						case IT::IDDATATYPE::BOOL:
							*stream << "BOOL |";
							break;
						case IT::IDDATATYPE::PROC:
						case IT::IDDATATYPE::UNDEF:
							*stream << "UNDEFINED";
							break;
					}
				}
			}
			*stream << std::endl;
		}
		*stream << "\n-------------------------------------------------------------------------\n\n";
	}
};