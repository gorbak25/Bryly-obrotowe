//Zadanie 5 - C++ - uzyto kompilatora MS Visual Studio 2012

/*
										INSTRUKCJA OBSLUGI PROGRAMU
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Program prosi o podanie funkcji zmniennej x, nastepnie o podanie kolejno lewego i prawego kranca przedzialu
	oraz parametr N.
	Zwraca on oszacowane pola powierzchni bocznej bryly opisanej w zadaniu.

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Program obsluguje liczenie funkcji:          

	FUNCKCJA						NAZWA W WYRAZENIU	PRZYKLAD UZYCIA
	sinus							"sin"			//Przyklad: f(x) = sin(x)
	arcsin							"arcsin"		//Przyklad: f(x) = arcsin(sin(x))
	cosinus							"cos"			//Przyklad: f(x) = cos(2*x)
	arccos							"arccos"		//Przyklad: f(x) = arccos(sin(x))
	wartosc bezwzgledna				"abs"			//Przyklad: f(x) = abs(sin(x))
	pierwiastek kwadratowy			"sqrt"			//Przyklad: f(x) = sqrt(abs(2x))
	zamiana stopni na radiany		"DegToRad"		//Przyklad: f(x) = sin(DegToRad(90))
	zamiara radianow na stopnie		"RadToDeg"		//Przyklad: f(x) = cos(RadToDeg(3.14))

	Aby uzyc danej funkcji nalezy napisac jej nazwe(2 kolumna) a nastepnie otworzyc nawias, napisac argument, zamknac nawias

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Program obsluguje operatory:

	NAZWA W WYRAZENIU		FUNKCJA					PRZYKLAD UZYCIA
		+					dodawanie				f(x) = 20+2*x
		-					odejmowanie				f(x) = 30-10*x
		*					mnozenie				f(x) = 1*1*1*1*1*1*x
		/					dzielenie				f(x) = 1/2
		%					reszta z dzielenia		f(x) = x%30
		^					potegowanie				f(x) = x^x
		.					separator dziesietny	f(x) = 3.14*x

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Program dokonuje mozliwych optymalizacji przed przystapieniem do kalkulacji:

	zostaja obliczone wszystkie czynniki ktore moga zostac obliczone bez znajomosci wartosci zmiennej x

	Przyklad:

	f(x) = 2+3*4				zostanie potraktowane jak	f(x) = 14
	f(x) = 1*1*1*1*1*20*x^(1+1) zostanie potraktowane jak	f(x) = 20*x^2

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	W przypadku nie podania operatora miedzy kolejnymi czynnikami automatycznie zostanie tam dodane mnozenie
	
	Przyklad:

	f(x) = (10+x)(2x-5)			zostanie potraktowane jak	f(x) = (10+x)*(2*x-5)
	f(x) = 20x + (1)[1]{x}		zostanie potraktowane jak	f(x) = 20*x + (1)*[1]*{x}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mozna stosowac trzy typy nawiasow - sa one rownowazne:
	( wyrazenie )
	[ wyrazenie ]
	{ wyrazenie }

	Przyklad:

	f(x) = (20+1)[20+1]{30+1}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Program wykrywa gdy zostana wprowadzone nieprawidlowe funkcje, rzucany jest wtedy wyjatek i program jest przerywany

	Przyklady nieprawidlowych funkcji np:
	Nieprawidlowe nawiasy ---------->		f(x) = sin(2+x            //Poprawnie f(x) = sin(2+x)
	Nieprawidlowa skladnia --------->		f(x) = cos^40(x)          //Poprawnie f(x) = (cos(x))^2
	Nieobslugiwane funkcje --------->		f(x) = x*tg(20)           //tangens nie jest obslugiwany
	Niekonsekwentnosc nawiasow ----->       f(x) = (20*[10)*10]		  //Poprawnie f(x) = [(20*10)*10]

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Po poprawnym wczytaniu funkcji istnieje mozliwosc ze podczas obliczen zostana zlamane przyjete tutaj zasady matematyki
	Jest to wykrywane, rzucany jest wtedy wyjatek i program jest przerywany

	Przyklady mozliwych komplikacji:
	Liczby zespolone ---------------->		f(x) = 20 + 10*sqrt(-1)   //Nieobslugiwane
	Wyjscie poza dziedzine ---------->		f(x) = arcsin(-2)         //Dziedzina arcsin to -1<=x<=1
	Dzielenie przez zero ------------>		f(x) = 1/x				  //Przy przyjeciu przedzialu np. [0,1] wykonujemy dzielenie przez zero
	Nieprawidlowe uzycie operatorow ->		f(x) = x*(0.5.0)          //Niedozwolone

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

#include <string>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <iomanip>
#include <utility>

using namespace std;
class entry
{
public:
	bool isOperand;
	bool isRightSided;
	bool isFunction;
	bool isNumber;
	bool isUnknown;
	bool isOpeningBracket;
	bool isClosingBracket;

	double(*operand)(double, double);
	int priority;

	double num;

	double(*func)(double);

	static entry makeOpeningBracket() { entry n; n.isOpeningBracket = true; return n; }
	static entry makeClosingBracket() { entry n; n.isClosingBracket = true; return n; }
	static entry makeUnknown() { entry n; n.isUnknown = true; return n; }

	entry()
	{
		isOperand = isFunction = isNumber = isUnknown = isOpeningBracket = isClosingBracket = isRightSided = false;
		operand = NULL; func = NULL;
		priority = -1;
		num = numeric_limits<double>::infinity();
	}

	entry(double number)
	{
		isOperand = isFunction = isUnknown = isOpeningBracket = isClosingBracket = isRightSided = false;
		operand = NULL; func = NULL;
		priority = -1;

		isNumber = true;
		num = number;
	}

	entry(int priority, bool isRightSided, double(*operation)(double, double))
	{
		isFunction = isNumber = isUnknown = isOpeningBracket = isClosingBracket = false;
		func = NULL;
		num = numeric_limits<double>::infinity();

		isOperand = true;
		this->priority = priority;
		this->isRightSided = isRightSided;
		operand = operation;
	}

	entry(double(*function)(double))
	{
		isOperand = isNumber = isUnknown = isOpeningBracket = isClosingBracket = isRightSided = false;
		operand = NULL;
		priority = -1;
		num = numeric_limits<double>::infinity();

		isFunction = true;
		func = function;
	}
};

const double PI = 4 * atan(1);

//dozwolone znaki wzraz z ich znaczeniem
set <char> opening_brackets;
map <char, char> closing_brackets;
map <char, entry> operands;

map <string, entry> functions;

const char unknown = 'x';
const char default_operand = '*';

entry OpBracket = entry::makeOpeningBracket();
entry EnBracket = entry::makeClosingBracket();
entry UnknownEnt = entry::makeUnknown();

void Init()
{
	opening_brackets.insert('(');
	opening_brackets.insert('[');
	opening_brackets.insert('{');

	closing_brackets.insert(make_pair(')', '('));
	closing_brackets.insert(make_pair(']', '['));
	closing_brackets.insert(make_pair('}', '{'));

	operands.insert(make_pair( '+', entry(0, false, [](double a, double b)->double{ return a + b; }) ));
	operands.insert(make_pair( '-', entry(0, false, [](double a, double b)->double{ return a - b; }) ));
	operands.insert(make_pair( '*', entry(1, false, [](double a, double b)->double{ return a*b;   }) ));
	operands.insert(make_pair( '/', entry(1, false, [](double a, double b)->double{ if (b == 0) throw exception("Division By Zero"); return a / b;   }) ));
	operands.insert(make_pair( '%', entry(1, false, [](double a, double b)->double{ return fmod(a, b); }) ));
	operands.insert(make_pair( '^', entry(2, true, [](double a, double b)->double{ if (b < 0) throw exception("Complex Numbers Not Suported"); return pow(a, b); }) ));
	operands.insert(make_pair( '.', entry(999, false, 
		[](double a, double b)->double
		{
			double tmp;
			if (modf(a,&tmp) != 0 || modf(b, &tmp) != 0) throw exception("Invalid decimal");
			return a + (b / pow(10,(int(log10(b))+1)));
		}) ));

	functions.insert(make_pair( "sin", entry([](double a)->double		{ return sin(a); }) ));
	functions.insert(make_pair( "arcsin", entry([](double a)->double	{ if (a < -1 || a>1) throw exception("Outside Domain"); return asin(a); }) ));
	functions.insert(make_pair( "cos", entry([](double a)->double		{ return cos(a); }) ));
	functions.insert(make_pair( "arccos", entry([](double a)->double	{ if (a < -1 || a>1) throw exception("Outside Domain"); return acos(a); }) ));
	functions.insert(make_pair( "abs", entry([](double a)->double		{ return abs(a); }) ));
	functions.insert(make_pair( "sqrt", entry([](double a)->double		{ if (a < 0) throw exception("Complex Numbers Not Suported"); return sqrt(a); }) ));
	functions.insert(make_pair( "DegToRad", entry([](double a)->double	{ return a*PI / 180; }) ));
	functions.insert(make_pair( "RadToDeg", entry([](double a)->double	{ return a * 180 / PI; }) ));
}

double Evaluate(double unknown_val, const vector<entry>& ONP)
{
	stack<double> num_stack;

	for (entry cur : ONP)
	{
		if (cur.isUnknown) num_stack.push(unknown_val);
		else if (cur.isNumber) num_stack.push(cur.num);
		else if (cur.isFunction)
		{
			double arg = num_stack.top();
			num_stack.pop();
			num_stack.push(cur.func(arg));
		}
		else if (cur.isOperand)
		{
			double a = num_stack.top();
			num_stack.pop();
			double b = num_stack.top();
			num_stack.pop();

			num_stack.push(cur.operand(b, a));
		}
	}

	return num_stack.top();
}

//sprawdz poprawnosc nawiasowania
void ValidateBrackets(const string& a)
{
	stack<char> stos;

	for (char cur : a)
	{
		if (opening_brackets.find(cur) != opening_brackets.end())
		{
			stos.push(cur);
		}
		else
		{
			map<char, char>::iterator pos = closing_brackets.find(cur);
			if (pos != closing_brackets.end())
			{
				//jezeli stos jest pusty badz zamykamy to co nie trzeba
				if (stos.empty() || (stos.top() != ((*pos).second))) throw exception("Syntax Error - Brackets");

				stos.pop();
			}
		}
	}

	if (!stos.empty()) throw exception("Syntax Error - Brackets");
}

//WIP - zejdz ze zlozonoscia
//Oblicz wstepnie to co sie da obliczyc
void OptimiseONP(vector<entry>& ONP)
{
	if (ONP.size() == 0) return;
	bool changed = true;
	while (changed)
	{
		changed = false;
		//optymalizuj operatory
		if(ONP.size()>2)
			for (auto it = ONP.begin() + 2; it != ONP.end(); it++)
			{
				if ((*it).isOperand)
				{
					if ((*(it - 1)).isNumber && (*(it - 2)).isNumber)
					{
						double a = (*(it - 2)).num;
						double b = (*(it - 1)).num;

						double res = (*it).operand(a, b);

						it = ONP.erase(it - 2, it+1);
						ONP.insert(it, entry(res));
						changed = true;
						break;
					}
				}
			}
		//optymalizuj funkcje
		for (auto it = ONP.begin() + 1; it != ONP.end(); it++)
		{
			if ((*it).isFunction)
			{
				if ((*(it - 1)).isNumber)
				{
					double a = (*(it - 1)).num;

					double res = (*it).func(a);

					it = ONP.erase(it - 1, it+1);
					ONP.insert(it, entry(res));
					changed = true;
					break;
				}
			}
		}
	}
	
}

//zamien wyrazenie na postac ONP
vector<entry> Process(const string& str)
{
	//sprawdz czy nawiasy sa sensowne
	ValidateBrackets(str);

	vector<entry> pre_computed;

	bool terminateNum = false;
	bool terminateFunc = false;

	bool add_curent = false;
	entry add_to_curent;

	string cur_func;
	int cur_number = -1;

	for (char cur : str)
	{
		if (cur >= '0' && cur <= '9')
		{
			//mamy kolejny znak do liczby
			terminateFunc = true;
			if (cur_number == -1) cur_number = 0;
			cur_number *= 10;
			cur_number += (cur - '0');
		}
		else
		{
			auto op_it = operands.find(cur);

			if (op_it != operands.end())
			{
				//znalezlismy operator wiec go wrzucamy do vectora
				add_to_curent = (*op_it).second;
				add_curent = true;
				terminateFunc = true;
				terminateNum = true;
			}
			else
			{
				if (opening_brackets.find(cur) != opening_brackets.end())
				{
					//mamy nawias otwierajace wrzucamy do vektora
					add_to_curent = OpBracket;
					add_curent = true;
					terminateFunc = true;
					terminateNum = true;
				}
				else
				{
					auto bracket_it = closing_brackets.find(cur);
					if (bracket_it != closing_brackets.end())
					{
						//mamy nawias zamykajacy wrzucamy do vektora
						add_to_curent = EnBracket;
						add_curent = true;
						terminateFunc = true;
						terminateNum = true;
					}
					else
					{
						//ignorujemy znaki biale
						if ((cur >= 'a'&&cur <= 'z') || (cur >= 'A'&&cur <= 'Z'))
						{
							//mamy kolejny znak do funkcji
							terminateNum = true;
							cur_func.push_back(cur);
						}
					}
				}
			}
		}

		if (terminateNum)
		{
			if (cur_number != -1)
			{
				//wrzucamy liczbe do vektora
				pre_computed.push_back(entry(static_cast<double>(cur_number)));
			}

			terminateNum = false;
			cur_number = -1;
		}
		if (terminateFunc)
		{
			if (cur_func.size() != 0)
			{
				if (cur_func.size() == 1 && cur_func[0] == unknown)
				{
					//mamy niewiadoma wiec wrzucamy ja do wektora
					pre_computed.push_back(UnknownEnt);
				}
				else
				{
					//sprawdzamy czy mamy taka funkcje
					//...
					auto func_it = functions.find(cur_func);
					if (func_it != functions.end())
					{
						pre_computed.push_back((*func_it).second);
					}
					else throw exception(("Function: " + cur_func + " Not Supported").c_str());
				}
				//wywalamy funkcje
				cur_func.erase(cur_func.begin(), cur_func.end());
			}
			terminateFunc = false;
		}
		if (add_curent)
		{
			//dodajemy wpis
			pre_computed.push_back(add_to_curent);
			add_curent = false;
		}
	}
	//wrzucamy to co bylo na koncu jezeli cos jest
	if (cur_number != -1) //jezeli jest liczba
	{
		//wrzucamy liczbe do vektora
		pre_computed.push_back(entry(static_cast<double>(cur_number)));
	}
	if (cur_func.size() != 0) //jezeli mamy albo niewiadoma badz funkcje jednoargumentowa
	{
		if (cur_func.size() == 1 && cur_func[0] == unknown)
		{
			//mamy niewiadoma wiec wrzucamy ja do wektora
			pre_computed.push_back(UnknownEnt);
		}
		else
		{
			//sprawdzamy czy mamy taka funkcje
			//...
			auto func_it = functions.find(cur_func);
			if (func_it != functions.end())
			{
				pre_computed.push_back((*func_it).second);
			}
			else throw exception(("Function: " + cur_func + " Not Supported").c_str());
		}
	}

	//mamy zapisane w pre_computed wyrazenie w notacji infiksowej

	//ogarniamy mnozenia

	entry default_OP = operands[default_operand];

	for (vector<entry>::iterator it = pre_computed.begin(); it != pre_computed.end(); it++)
	{
		entry cur = *it;
		if (it + 1 == pre_computed.end()) continue;
		entry next = *(it+1);

		//)(, )x, )func, )number
		if (cur.isClosingBracket && (!(next.isOperand || next.isClosingBracket)))
		{
			it = pre_computed.insert(it+1, default_OP);
		}
		//NumberFunc, Number(, NumberX
		else if(cur.isNumber && (next.isFunction || next.isOpeningBracket || next.isUnknown))
		{
			it = pre_computed.insert(it+1, default_OP);
		}
		//UnknownFunc, Unknown(, UnknownNumber
		else if (cur.isUnknown && (next.isFunction || next.isOpeningBracket || next.isNumber))
		{
			it = pre_computed.insert(it+1, default_OP);
		}
		else if(cur.isFunction && !next.isOpeningBracket)
		{
			throw exception("Syntax Error - After a function there must be an opening bracket!");
		}
	}

	vector<entry> result;
	stack<entry> operands_stack;

	for (entry cur : pre_computed)
	{
		if (cur.isNumber || cur.isUnknown) result.push_back(cur);
		else if (cur.isFunction) operands_stack.push(cur);
		else if (cur.isOperand)
		{
			if (cur.isRightSided)
				while (!operands_stack.empty() && operands_stack.top().isOperand && (cur.priority < operands_stack.top().priority))
				{
					result.push_back(operands_stack.top());
					operands_stack.pop();
				}
			else
				while (!operands_stack.empty() && operands_stack.top().isOperand && (cur.priority <= operands_stack.top().priority))
				{
					result.push_back(operands_stack.top());
					operands_stack.pop();
				}

			operands_stack.push(cur);

		}
		else if (cur.isOpeningBracket) operands_stack.push(cur);
		else if (cur.isClosingBracket)
		{
			while (!operands_stack.top().isOpeningBracket)
			{
				result.push_back(operands_stack.top());
				operands_stack.pop();
			}

			operands_stack.pop();

			if (!operands_stack.empty() && operands_stack.top().isFunction)
			{
				result.push_back(operands_stack.top());
				operands_stack.pop();
			}
		}
	}

	while (!operands_stack.empty())
	{
		result.push_back(operands_stack.top());
		operands_stack.pop();
	}

	OptimiseONP(result);

	return result;
}

int main()
{
	try
	{
		Init();

		string userInput;
		double A, B;
		int N;

		cout << "f(x) = ";
		getline(cin, userInput);

		vector<entry> Function = Process(userInput);
	
		cout << "From: x = ";
		cin >> A;
		cout << "To: x = ";
		cin >> B;

		if (A > B) swap(A, B);

		cout << "Probing Param: n = ";
		cin >> N;

		if (N < 1) throw exception("Invalid Probing Param");

		double step = (B - A) / N;
		double step2 = step*step;

		double result = 0;

		double prev_H = Evaluate(A, Function);

		for (int i = 1; i <= N; i++)
		{
			double cur_H = Evaluate(A + step*i, Function);
			double H_diff = prev_H - cur_H;

			result += PI*sqrt(step2 + H_diff*H_diff)*(prev_H + cur_H);

			prev_H = cur_H;
		}

		cout << "Estimated Surface Area:\n" << fixed << setprecision(5) << result << endl;

	}
	catch (exception &ex)
	{
		cout << "An unexcepted exception ocured: \n" << ex.what() << endl;
	}
	return 0;
}
