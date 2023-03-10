#pragma once
//bool PolishNotation(int i, Lexer::LEX& lex);
//bool startPolishNotation(Lexer::LEX& lex);

#include "LT.h"
#include "IT.h"
#include "LexAnalizer.h"
#include <vector>

typedef std::vector <LT::Entry> ltvec;
typedef std::vector <int> intvec;

namespace Polish
{

	bool PolishNotation(LA::LEX& lex, Log::LOG& log);
	// ? ?????? ????????? ??????? ?????? ???? ????????? ? ???????? ??????
	intvec getExprPositions(LA::LEX& lex);
	// ????????? ?????? ????????? ? ????????? ???????
	int fillVector(int lextable_pos, LT::LexTable& lextable, ltvec& v);
	// ?????? ??????? ???????? ??????? ??????????????? ? ???????? ???????
	bool setPolishNotation(IT::IdTable& idtable, Log::LOG& log, int lextable_pos, ltvec& v);
	// ????????? ?????? ? ????? ??????? ??????
	void addToTable(LT::LexTable& new_table, IT::IdTable& idtable, ltvec& v);
};