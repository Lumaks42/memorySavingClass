// strimem2.cpp
// для использования небезопасной функции wcscpy
#define _CRT_SECURE_NO_WARNINGS

#include <io.h>       // для функции _setmode
#include <fcntl.h>    // для константы _O_U16TEXT
#include <iostream>   // для функций ввода/вывода
using namespace std;

class strCount 
{              
private:
	int count;           // счетчик объектов
	wchar_t* str;        // указатель на строку
	friend class String; // делаю члены текущего класса strCount доступными для String	


	strCount(const wchar_t* s)         // конструктор с одним аргументом
	{
		int length = wcslen(s);        // длина строки-аргумента
		str = new wchar_t[length + 1]; // получу память под строку и
		wcscpy(str, s);                // скопирую в нее аргумент
		count = 1;                     // начну счет с единицы
	}

	~strCount()                        // деструктор
	{
		delete[] str;
	}									// освободим память, занятую строкой
};

class String							// класс, представляющий строку
{
private:
	strCount* psc;							// указатель на объект класса strCount
public:
	String()								// конструктор без аргументов
	{
		psc = new strCount(L"");
	}

	String(const wchar_t* s)				// конструктор с одним аргументом
	{
		psc = new strCount(s);
	}

	String(String& S)					    // копирующий конструктор
	{
		wcout << L"\nКопирующий конструктор";
		psc = S.psc;
		(psc->count)++;
	}

	~String()								// деструктор
	{
		if (psc->count == 1)				// если текущий объект — последний пользователь объекта strCount,
			delete psc;						// удалить этот объект strCount
		else								// в противоположном случае —
			(psc->count)--;					// уменьшить счетчик в объекте strCount
	}

	void display()							// вывожу строку на экран
	{
		wcout << psc->str;                  // вывод строки
		wcout << L" (адрес = " << psc << L")"; // вывожу адрес объекта strCount
	}

	String& operator= (String& S)			// операция присваивания одного объекта String другому
	{
		wcout << L"\nОперация присваивания";

		if (this == &S) return *this;

		if (psc->count == 1)   
			delete psc;						// удалю этот объект strCount
		else                
			(psc->count)--;					// уменьшу счетчик в объекте strCount

		psc = S.psc;						// использую объект strCount параметра
		(psc->count)++;						// увеличиваю счетчик в объекте strCount параметра

		return *this;						// верну текущий объект
	}
};

int main()
{
	// переключение стандартного потока вывода в формат Юникода
	_setmode(_fileno(stdout), _O_U16TEXT);

	String s3 = L"Людям плевать на твои попытки, им важен момент славы";
	wcout << L"s3 = "; s3.display();		// вывожу на экран строку s3

	String s1, s2;							// определю еще две строки
	s1 = s2 = s3;							// чтобы задать им значения, дважды применим присваивание
	wcout << L"\ns1 = "; s1.display();		// и выведу эти строки на экран
	wcout << L"\ns2 = "; s2.display();

	wcout << endl;

	return 0;
}