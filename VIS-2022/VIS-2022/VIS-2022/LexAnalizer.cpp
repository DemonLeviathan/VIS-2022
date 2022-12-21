#include "pch.h"
#include <stack>
#include <string>

namespace LA
{
	FstTable fstTable[N_FST] =
	{
		{ LEX_SEPARATORS, FST::FST(FST_SEPARATORS) },
		{ LEX_LITERAL,	  FST::FST(FST_INT_LITERAL) },
		{ LEX_LITERAL,	  FST::FST(FST_STRING_LITERAL) },
		{ LEX_LITERAL,    FST::FST(FST_BOOL_TRUE_LITERAL)},
		{ LEX_LITERAL,	  FST::FST(FST_BOOL_FALSE_LITERAL)},
		{ LEX_NEW,		  FST::FST(FST_NEW) },
		{ LEX_MAIN,		  FST::FST(FST_MAIN) },
		{ LEX_ID_TYPE,	  FST::FST(FST_UINT) },
		{ LEX_ID_TYPE,    FST::FST(FST_STRING) },
		{ LEX_ID_TYPE,	  FST::FST(FST_BOOL) },
		{ LEX_FUNCTION,	  FST::FST(FST_FUNCTION) },
		{ LEX_PROCEDURE,  FST::FST(FST_PROCEDURE) },
		{ LEX_RETURN,     FST::FST(FST_RETURN) },
		{ LEX_PRINT,	  FST::FST(FST_PRINT) },
		{ LEX_NEWLINE,    FST::FST(FST_NEWLINE) },
		{ LEX_CONDITION,  FST::FST(FST_CONDITION) },
		{ LEX_CYCLE,	  FST::FST(FST_CYCLE) },
		{ LEX_ISFALSE,    FST::FST(FST_FALSE) },
		{ LEX_ISTRUE,	  FST::FST(FST_TRUE) },
		{ LEX_ID,		  FST::FST(FST_ID) }
	};

	char* getScopeName(IT::IdTable idtable, char* prevword) // имя текущей области видимости (если находимся внутри блока)
	{
		char *a = new char[5];
		a[0] = 'm';
		a[1] = 'a';
		a[2] = 'i';
		a[3] = 'n';
		a[4] = '\0';
		if (strcmp(prevword, MAIN) == 0)
			return a;
		for (int i = idtable.size - 1; i >= 0; i--)
			if (idtable.table[i].idtype == IT::IDTYPE::F)
				return idtable.table[i].id;
		return nullptr; // не найдено имя функции
	}

	int getLiteralIndex(IT::IdTable ittable, char* value, IT::IDDATATYPE type) // получаем индекс повторного литерала(по значению)
	{
		for (int i = 0; i < ittable.size; i++)
		{
			if (ittable.table[i].idtype == IT::IDTYPE::L && ittable.table[i].iddatatype == type)
			{
				switch (type)
				{
				case IT::IDDATATYPE::NUM:
					if (ittable.table[i].value.vint == atoi(value))
						return i;
						break;
				case IT::IDDATATYPE::STR:
					char buf[STR_MAXSIZE];
					for (unsigned j = 1; j < strlen(value) - 1; j++) // без кавычек
						buf[j - 1] = value[j];
					buf[strlen(value) - 2] = IN_CODE_NULL;
					if (strcmp(ittable.table[i].value.vstr.str, buf) == 0)
						return i;
					break;
				case IT::IDDATATYPE::BOOL:
					if (ittable.table[i].value.vbool == atoi(value))
						return i;
					break;
				}
			}
		}
		return NULLIDX_TI;
	}

	IT::IDDATATYPE getType(char* curword, char* idtype)
	{
		if (!strcmp(TYPE_PROCEDURE, idtype))
			return IT::IDDATATYPE::PROC; // процедуры
		if (!strcmp(TYPE_STRING, idtype)) 
			return IT::IDDATATYPE::STR;  // строковый ид
		if (!strcmp(TYPE_NUMBER, idtype))
			return IT::IDDATATYPE::NUM;	 // числовой  ид
		if (!strcmp(TYPE_BOOL, idtype))
			return IT::IDDATATYPE::BOOL; // логический ид
		if (isdigit(*curword)) 
			return IT::IDDATATYPE::NUM;				// числовой литерал
		else if (*curword == IN_CODE_QUOTE)  
			return IT::IDDATATYPE::STR;	// строковый литерал
		//else if (*curword == FST_BOOL_TRUE_LITERAL )

		return IT::IDDATATYPE::UNDEF;		// неопределенный тип, индикатор ошибки
	}

	int getIndexInLT(LT::LexTable &lextable, int itTableIndex)					// индекс первой встречи в таблице лексем
	{
		if (itTableIndex == NULLIDX_TI)		// если идентификатор встречается впервые
			return lextable.size;
		for (int i = 0; i < lextable.size; i++)
			if (itTableIndex == lextable.table[i].idxTI) 
				return i;
		return NULLIDX_TI;
	}

	bool isLiteral(char* id)
	{
		if (isdigit(*id) || *id == IN_CODE_QUOTE) 
			return true;
		return false;
	}

	IT::STDFNC getStandFunction(char* id)
	{
		if (!strcmp(CONCAT, id))
			return IT::STDFNC::F_CONCAT;
		if (!strcmp(LENGTH, id))
			return IT::STDFNC::F_LENGTH;
		if (!strcmp(STRTON, id))
			return IT::STDFNC::F_STRTON;
		return IT::STDFNC::F_NOT_STD;
	}

	char* getNextLiteralName()						// сгенерировать следующее имя литерала
	{
		static int literalNumber = 1;
		char* buf = new char[SCOPED_ID_MAXSIZE], num[3];
		strcpy_s(buf, MAXSIZE_ID, "LTRL");
		_itoa_s(literalNumber++, num, 10);
		strcat(buf, num);
		return buf;
	}
	
	IT::Entry* getEntry(		// формирует и возвращает строку ТИ
		LA::LEX& tables,						// ТЛ + ТИ
		char lex,								// лексема
		char* id,								// идентификатор
		char* idtype,							// предыдущая (тип)
		bool isParam,							// признак параметра функции
		bool isFunc,							// признак функции
		Log::LOG log,							// протокол
		int line,								// строка в исходном тексте
		bool& lex_ok)							// флаг ошибки(по ссылке)
	{
		// тип данных
		IT::IDDATATYPE type = getType(id, idtype);
		int index = IT::isId(tables.idtable, id);	// индекс существующего идентификатора или -1
		if (lex == LEX_LITERAL)
			index = getLiteralIndex(tables.idtable, id, type);
		if (index != NULLIDX_TI)
			return nullptr;	// уже существует

		IT::Entry *itentry = new IT::Entry;
		itentry->iddatatype = type; // сохраняем тип данных

		// индекс первой строки в таблице лексем 
		itentry->idxfirstLE = getIndexInLT(tables.lextable, index);

		if (lex == LEX_LITERAL) // литерал
		{
			bool int_ok = IT::SetValue(itentry, id);
			if (!int_ok)
			{
				// превышен максимальный размер числового литерала
				throw ERROR_THROW_IN(313, line, 0);
				lex_ok = false;
			}
			if (itentry->iddatatype == IT::IDDATATYPE::STR && itentry->value.vstr.len == 0)
			{
				// пустой строковый литерал
				throw ERROR_THROW_IN(310, line, 0);
				lex_ok = false;
			}
			strcpy_s(itentry->id, getNextLiteralName());
			itentry->idtype = IT::IDTYPE::L;
		}
		else // идентификатор
		{
			switch (type)
			{
			case IT::IDDATATYPE::STR:
				strcpy_s(itentry->value.vstr.str, "");
				itentry->value.vstr.len = STR_DEFAULT;
				break;
			case IT::IDDATATYPE::NUM:
				itentry->value.vint = NUM_DEFAULT;
				break;
			case IT::IDDATATYPE::BOOL:
				itentry->value.vbool = BOOL_DEFAULT;
				break;
			}

			if (isFunc)
			{
				switch (getStandFunction(id))
				{
				case IT::STDFNC::F_CONCAT:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = CONCAT_TYPE;
					itentry->value.params.count = CONCAT_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[CONCAT_PARAMS_CNT];
					for (int k = 0; k < CONCAT_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::CONCAT_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_LENGTH:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = LENGTH_TYPE;
					itentry->value.params.count = LENGTH_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[LENGTH_PARAMS_CNT];
					for (int k = 0; k < LENGTH_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::LENGTH_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_STRTON:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = STRTON_TYPE;
					itentry->value.params.count = STRTON_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[STRTON_PARAMS_CNT];
					for (int k = 0; k < STRTON_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::STRTON_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_NOT_STD:
					itentry->idtype = IT::IDTYPE::F;
					break;
				}
			}
			else if (isParam) 
				itentry->idtype = IT::IDTYPE::P;
			else 
				itentry->idtype = IT::IDTYPE::V;

			strncpy_s(itentry->id, id, SCOPED_ID_MAXSIZE);
		}
		// -------------------------------------------------------
		int i = tables.lextable.size; // индекс в ТЛ текущего ИД

		if (i > 1 && itentry->idtype == IT::IDTYPE::V && tables.lextable.table[i - 2].lexema != LEX_NEW)
		{
			// в объявлении отсутствует ключевое слово new
			throw ERROR_THROW_IN(304, line, 0);
			lex_ok = false;
		}
		if (i > 1 && itentry->idtype == IT::IDTYPE::F && tables.lextable.table[i - 1].lexema != LEX_FUNCTION)
		{
			// в объявлении не указан тип функции
			throw ERROR_THROW_IN(303, line, 0);
			lex_ok = false;
		}
		if (itentry->iddatatype == IT::IDDATATYPE::UNDEF)
		{
			// невозможно определелить тип
			throw ERROR_THROW_IN(300, line, 0);
			lex_ok = false;
		}
		// --------------------------------------------------------
		return itentry;
	}


	bool analyze(LEX& tables, In::IN &in, Log::LOG &log, Parm::PARM &parm)
	{
		static bool lex_ok = true;
		tables.lextable = LT::Create(MAXSIZE_LT);
		tables.idtable = IT::Create(MAXSIZE_TI);

		bool isParam = false, isFunc = false;
		int enterPoint = NULL;
		char curword[STR_MAXSIZE], nextword[STR_MAXSIZE];
		int curline;
		std::stack <char*> scopes;			// стек для хранения имени текущей области видимости

		for (int i = 0; i < In::InWord::size; i++)
		{
			strcpy_s(curword, in.words[i].word);
			if (i < In::InWord::size - 1)
				strcpy_s(nextword, in.words[i + 1].word);
			curline = in.words[i].line;
			isFunc = false;
			int idxTI = NULLIDX_TI;

			for (int j = 0; j < N_FST; j++)
			{
				FST::FST fst(curword, fstTable[j].graph);
				if (FST::execute(fst))
				{
					char lexema = fstTable[j].lexema;
					switch (lexema)
					{
					case LEX_MAIN: 
						enterPoint++; 
						break;
					case LEX_SEPARATORS:
					{
						switch (*curword)
						{
						case LEX_LEFTHESIS:		// открывающая скобка - параметры  функции
						{
							isParam = true;
							if (*nextword == LEX_RIGHTTHESIS || ISTYPE(nextword))
							{
								char* functionname = new char[MAXSIZE_ID];
								char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);
								if (scopename == nullptr) 
									break;
								strcpy_s(functionname, MAXSIZE_ID, scopename);
								scopes.push(functionname);
							}
							break;
						}
						case LEX_RIGHTTHESIS:	// закрывающая скобка
						{
							isParam = false;
							// конец области видимости
							if (*in.words[i - 1].word == LEX_LEFTHESIS ||(i > 2 && (tables.lextable.table[tables.lextable.size - 2].lexema == LEX_ID_TYPE)))
								scopes.pop();
							break;
						}
						case LEX_LEFTBRACE:		// начало области видимости
						{
							if (i > 0 && *in.words[i - 1].word == LEX_ISFALSE || *in.words[i - 1].word == LEX_ISTRUE || *in.words[i - 1].word == LEX_CYCLE)
								break;
							char* functionname = new char[MAXSIZE_ID];
							char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);
							if (scopename == nullptr)  break;
							strcpy_s(functionname, MAXSIZE_ID, scopename);
							scopes.push(functionname);
							break;
						}
						case LEX_BRACELET:		// конец области видимости
						{
							// только в этом случае закрываем область видимости
							if (*in.words[i + 1].word == LEX_ID_TYPE || *in.words[i + 1].word == LEX_PROCEDURE || *in.words[i + 1].word == LEX_MAIN)
							{
								if (!scopes.empty()) 
									scopes.pop();
							}
							break;
						}
						} 
						lexema = *curword;
						break;
					} 
					case LEX_ID:
					case LEX_LITERAL:
					{
						char id[STR_MAXSIZE] = "";
						idxTI = NULLDX_TI;  // индекс идентификатора в ТИ
						if (*nextword == LEX_LEFTHESIS)
							isFunc = true;						// идентификатор функции
						char* idtype = (isFunc && i > 1) ?	// тип идентификатора
							in.words[i - 2].word : in.words[i - 1].word;		// пропускаем ключевое слово function
						if (!isFunc && !scopes.empty())
							strncpy_s(id, scopes.top(), MAXSIZE_ID);
						strncat_s(id, curword, MAXSIZE_ID);
						if (isLiteral(curword))  // литерал заменяется своим значением
						{
							if (curword[0] == '0' && strlen(curword) > 1)
							{
								if (curword[1] != '0')// перевод из 8 в 10 ную
								{
									int result8to10 = 0;
									for (size_t i = 0; i < strlen(curword); i++)
									{
										if (curword[i] - '0' > 7)
										{
											throw ERROR_THROW_IN(126, curline , -1);
										}
										result8to10 += (curword[i] - '0') * pow(8, strlen(curword) - i - 1);
									}
									strcpy_s(id, std::to_string(result8to10).c_str());
								}
								else // перевод из 2 в 10 ную
								{
									int result2to10 = 0;
									for (size_t i = 0; i < strlen(curword); i++)
									{
										if (curword[i] - '0' > 1)
										{
											throw ERROR_THROW_IN(127, curline, -1);
										}
										result2to10 += (curword[i] - '0') * pow(2, strlen(curword) - i - 1);
									}
									strcpy_s(id, std::to_string(result2to10).c_str());
								}
							}
							else
							{
								strcpy_s(id, curword);
							}

						}
						IT::Entry *itentry = getEntry(tables, lexema, id, idtype, isParam, isFunc, log, curline, lex_ok);
						if (itentry != nullptr) // первая встреча - объявление
						{
							if (isFunc) // если функция - сохранить список параметров
							{
								if (getStandFunction(id) == IT::STDFNC::F_NOT_STD) // стандартная функция или нет
								{
									itentry->value.params.count = NULL;
									itentry->value.params.types = new IT::IDDATATYPE[MAX_PARAMS_COUNT];
									for (int k = i; in.words[k].word[0] != LEX_RIGHTTHESIS; k++)
									{
										if (k == In::InWord::size || in.words[k].word[0] == LEX_SEPARATOR) 
											break;
										if (ISTYPE(in.words[k].word))
										{
											if (itentry->value.params.count >= MAX_PARAMS_COUNT)
											{
												throw ERROR_THROW_IN(306, curline, 0);
												lex_ok = false;
												break;
											}
											itentry->value.params.types[itentry->value.params.count++] = getType(NULL, in.words[k].word);
										}
									} 
								}
							}
							IT::Add(tables.idtable, *itentry);
							idxTI = tables.idtable.size - 1;
						}
						else // повторный идентификатор (уже есть)
						{
							int i = tables.lextable.size - 1; // попытка переопределить идентификатор
							if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_NEW || tables.lextable.table[i].lexema == LEX_NEW
								|| tables.lextable.table[i - 1].lexema == LEX_FUNCTION || tables.lextable.table[i].lexema == LEX_FUNCTION
								|| tables.lextable.table[i - 1].lexema == LEX_ID_TYPE || tables.lextable.table[i].lexema == LEX_ID_TYPE
								|| tables.lextable.table[i - 1].lexema == LEX_PROCEDURE || tables.lextable.table[i].lexema == LEX_PROCEDURE)
								{
									throw ERROR_THROW_IN(305, curline, 0);
									lex_ok = false;
								}	
							idxTI = IT::isId(tables.idtable, id);	// индекс существующего идентификатора
							if (lexema == LEX_LITERAL)
								idxTI = getLiteralIndex(tables.idtable, curword, getType(id, in.words[i - 1].word)); // или литерала
						}
					}
					break;
					} 

					LT::Add(tables.lextable, LT::Entry(lexema, curline, idxTI));
					break;
				} 
				else if (j == N_FST - 1) // цепочка не распознана
				{
					throw ERROR_THROW_IN(201, curline, 0); // 
					lex_ok = false;
				}
			}  
		} 

		if (enterPoint == NULL) // не найдена точка входа
		{
			throw ERROR_THROW(301);
			lex_ok = false;
		}
		if (enterPoint > 1) //больше 1 точки входа
		{
			throw ERROR_THROW(302);
			lex_ok = false;
		}
		return lex_ok;
	}
};