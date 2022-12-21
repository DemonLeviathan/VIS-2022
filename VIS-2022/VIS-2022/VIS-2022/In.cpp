#include "pch.h"
#include <iostream>
#include <fstream>


int In::InWord::size = NULL;

namespace In
{
	IN getin(wchar_t infile[], std::ostream *stream)			// ввести и проверить входной поток
	{
		unsigned char* text = new unsigned char[IN_MAX_LEN_TEXT];
		std::ifstream instream(infile);
		if (!instream.is_open()) 
			throw ERROR_THROW(102);
		IN in;
		int pos = 0;
		bool isLiteral = false;
		while (true)
		{
			unsigned char uch = instream.get();
			if (instream.eof())
				break;
			if (in.code[uch] == IN::Q)   // кавычка 
				isLiteral = !isLiteral;
			switch (in.code[uch])
			{
				case IN::N: // новая строка 
					text[in.size++] = uch;
					in.lines++;
					pos = -1;
					break;
				case IN::T: 
				case IN::P: 
				case IN::S: 
				case IN::Q: 
					text[in.size++] = uch;  
					break;
				case IN::F: 
					throw ERROR_THROW_IN(200, in.lines, pos);
					break;
				case IN::I:
					in.ignore++; 
					break;
				default:
					text[in.size++] = in.code[uch];
					break;
			}
			pos++;
		}
		text[in.size] = IN_CODE_NULL;
		in.text = text;
		instream.close(); 
		return in;
	}
	void addWord(InWord* words, char* word, int line)					
	{
		if (*word == IN_CODE_NULL)
			return;
		words[InWord::size].line = line;
		strcpy_s(words[InWord::size].word, strlen(word) + 1, word);
		InWord::size++;
	}
	InWord* getWordsTable(std::ostream* stream, unsigned char* text, int* code, int textSize)				// формируем таблицу слов
	{
		InWord *words = new InWord[MAXSIZE_LT];
		int bufpos = 0;				// позиция в буфере
		int line = 1;				// номер строки исходного кода
		char buffer[MAX_LEN_BUFFER] = "";		// буфер
		for (int i = 0; i < textSize; i++)			// заполнение
		{
			switch (code[text[i]])
			{
			case IN::S:
			{
				char letter[] = { (char)text[i], IN_CODE_NULL };
				addWord(words, buffer, line);	// буфер перед односимвольной лексемой
				addWord(words, letter, line);	// сама односимвольная лексема
				*buffer = IN_CODE_NULL; 
				bufpos = 0;
				break;
			}
			case IN::N:											//новая строка
			case IN::P:											// разделители и пробелы
				addWord(words, buffer, line); 
				*buffer = IN_CODE_NULL; 
				bufpos = 0;
				if (code[text[i]] == IN::N) 
					line++;
				break;
			case IN::Q:		// строковый литерал
			{
				addWord(words, buffer, line);  
				*buffer = IN_CODE_NULL;
				bufpos = 0;
				bool isltrlgood = true;
				// если литерал не закрыт - перевод строки будет раньше кавычки
				for (int j = i + 1; text[j] != IN_CODE_QUOTE; j++)
				{
					if (code[text[j]] == IN::N)
					{
						throw ERROR_THROW_IN(311, line, 0);
						isltrlgood = false; // литерал не закрыт!
						break;
					}
				}
				if (isltrlgood)
				{
					buffer[bufpos++] = IN_CODE_QUOTE;
					for (int j = 1; ; j++)
					{
						if (j == 256 || i + j == textSize)
						{
							throw ERROR_THROW_IN(312, line, 0);
							break; // превышен размер литерала(учтена открытая кавычка)
						}
						buffer[bufpos++] = text[i + j];
						if (text[i + j] == IN_CODE_QUOTE) // закрывающая кавычка
						{
							buffer[bufpos] = IN_CODE_NULL;
							addWord(words, buffer, line);
							i = i + j;
							break;
						}
					}
				} // literal good
				*buffer = IN_CODE_NULL;  bufpos = 0;
				break;
			}
			default:
				buffer[bufpos++] = text[i];
				buffer[bufpos] = IN_CODE_NULL;
			}
		}
		return words;
	}

	void printTable(InWord* table)								// вывод таблицы на экран
	{
		std::cout << " ------------------ СЛОВА ВХОДНОГО ТЕКСТА ------------------" << std::endl;
		std::cout << std::setw(10) << "№ СЛОВА " << std::setw(10) << "№ СТРОКИ" << std::setw(12) << "СЛОВО" << std::endl;
		for (int i = 0; i < table->size; i++)
		{
			std::cout << std::setw(7) << i << std::setw(9) << table[i].line << std::setw(15) << table[i].word << std::endl;
		}
	}
};