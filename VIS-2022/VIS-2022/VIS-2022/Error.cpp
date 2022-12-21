#include "pch.h"
#include "Error.h"

namespace Error 
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Ошибка системы: Недопустимый код ошибки"),
		ERROR_ENTRY(1, "Ошибка системы: Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "Ошибка системы: Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "Ошибка системы: Превышена длина входного параметра"),
		ERROR_ENTRY(102, "Ошибка системы: Ошибка при открытии файла с исходным кодом(-in)"),
		ERROR_ENTRY(103, "Ошибка системы: Ошибка при создании файла протокола(-log)"),
		ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY_NODEF10(110), ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140),
		ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY(200, "Ошибка лексики: Недопустимый символ в исходном файле(-in)"),
		ERROR_ENTRY(201, "Ошибка лексики: Неизвестная последовательность символов"),
		ERROR_ENTRY(202, "Ошибка лексики: Превышен размер таблицы лексем"),
		ERROR_ENTRY(203, "Ошибка лексики: Превышен размер таблицы идентификаторов"),
		ERROR_ENTRY_NODEF(204), ERROR_ENTRY_NODEF(205), ERROR_ENTRY_NODEF(206), ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240),
		ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY(300, "Ошибка семантики: Необъявленный идентификатор"),
		ERROR_ENTRY(301, "Ошибка семантики: Отсутствует точка входа main"),
		ERROR_ENTRY(302, "Ошибка семантики: Обнаружено несколько точек входа main"),
		ERROR_ENTRY(303, "Ошибка семантики: В объявлении не указан тип идентификатора"),
		ERROR_ENTRY(304, "Ошибка семантики: В объявлении отсутствует ключевое слово"),
		ERROR_ENTRY(305, "Ошибка семантики: Попытка переопределения идентификатора"),
		ERROR_ENTRY(306, "Ошибка семантики: Превышено максимальное количество параметров функции"),
		ERROR_ENTRY(307, "Ошибка семантики: Слишком много параметров в вызове"),
		ERROR_ENTRY(308, "Ошибка семантики: Кол-во ожидаемыех функцией и передаваемых параметров не совпадают"),
		ERROR_ENTRY(309, "Ошибка семантики: Несовпадение типов передаваемых параметров"),
		ERROR_ENTRY(310, "Ошибка семантики: Использование пустого строкового литерала недопустимо"),
		ERROR_ENTRY(311, "Ошибка семантики: Обнаружен символ \'\"\'. Возможно, не закрыт строковый литерал"),
		ERROR_ENTRY(312, "Ошибка семантики: Превышен размер строкового литерала"),
		ERROR_ENTRY(313, "Ошибка семантики: Недопустимый целочисленный литерал"),
		ERROR_ENTRY(314, "Ошибка семантики: Типы данных в выражении не совпадают"),
		ERROR_ENTRY(315, "Ошибка семантики: Тип функции и возвращаемого значения не совпадают"),
		ERROR_ENTRY(316, "Ошибка семантики: Недопустимое строковое выражение справа от знака \'=\'"),
		ERROR_ENTRY(317, "Ошибка семантики: Неверное условное выражение"),
		ERROR_ENTRY(318, "Ошибка семантики: Деление на ноль"),
		ERROR_ENTRY(319, "Ошибка семантики: В операции вычитания результатом является отрицательное число"),
		ERROR_ENTRY_NODEF10(320),ERROR_ENTRY_NODEF10(330),ERROR_ENTRY_NODEF10(340),ERROR_ENTRY_NODEF10(350),
		ERROR_ENTRY_NODEF10(360),ERROR_ENTRY_NODEF10(370),ERROR_ENTRY_NODEF10(380),ERROR_ENTRY_NODEF10(390),
		ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "Ошибка синтаксиса: Неверная структура программы"),
		ERROR_ENTRY(601, "Ошибка синтаксиса: Не найден список параметров функции"),
		ERROR_ENTRY(602, "Ошибка синтаксиса: Ошибка в теле функции"),
		ERROR_ENTRY(603, "Ошибка синтаксиса: Ошибка в теле процедуры"),
		ERROR_ENTRY(604, "Ошибка синтаксиса: Ошибка в списке параметров функции"),
		ERROR_ENTRY(605, "Ошибка синтаксиса: Ошибка в вызове функции/выражении"),
		ERROR_ENTRY(606, "Ошибка синтаксиса: Ошибка в списке фактических параметров функции"),
		ERROR_ENTRY(607, "Ошибка синтаксиса: Ошибка при констуировании цикла/условного выражения"),
		ERROR_ENTRY(608, "Ошибка синтаксиса: Ошибка в теле цикла/условного выражения"),
		ERROR_ENTRY(609, "Ошибка синтаксиса: Ошибка в условии цикла/условного выражения"),
		ERROR_ENTRY(610, "Ошибка синтаксиса: Неверный условный оператор"),
		ERROR_ENTRY(611, "Ошибка синтаксиса: Неверный арифметический оператор"),
		ERROR_ENTRY(612, "Ошибка синтаксиса: Неверное выражение. Ожидаются только идентификаторы/литералы"),
		ERROR_ENTRY(613, "Ошибка синтаксиса: Ошибка в арифметическом выражении"),
		ERROR_ENTRY(614, "Ошибка синтаксиса: Недопустимая синтаксическая конструкция"),
		ERROR_ENTRY(615, "Ошибка синтаксиса: Недопустимая синтаксическая конструкция в теле цикла/условного выражения"),
		ERROR_ENTRY_NODEF(616), ERROR_ENTRY_NODEF(617),ERROR_ENTRY_NODEF(618), ERROR_ENTRY_NODEF(619),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR getError(int id)
	{
		if (0 > id || id > ERROR_MAX_ENTRY)
		{
			id = 0;
		}

		return errors[id];
	}

	ERROR getErrorIn(int id, int line, int col)
	{
		ERROR er = getError(id);

		er.inext.col = col;
		er.inext.line = line;

		return er;
	}
}