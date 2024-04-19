#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Value
{
	string value_str;
	int value_int;
	bool isNumber;
	bool isExist;
};

map<string, Value> variable_dictionary;

bool isSpace(const char character)
{
	bool case1 = (character == ' '),
		 case2 = (character == '\n'),
		 case3 = (character == '\t'),
		 case4 = (character == '\r'),
		 case5 = (character == 11),
		 case6 = (character == 12);

	return case1 || case2 || case3 || case4 || case5 || case6;
}
bool isLetter(const char character)
{
	bool case1 = (character > 64 && character < 91),
		 case2 = (character > 96 && character < 123);

	return case1 || case2;
}
bool isIdentifier(string word)
{
	if (!(isLetter(word[0]) || word[0] == 95))
	{
		return false;
	}
	if (word == "var")
	{
		return false;
	}
	for (unsigned i = 1; i < word.size(); i++)
	{
		if (!(isdigit(word[i]) || isLetter(word[i]) || word[i] == 95))
		{
			return false;
		}
	}
	return true;
}
bool isNumber(string word)
{
	stringstream str2int(word);
	int number;
	unsigned i = 0;
	bool negative = word[0] == '-';
	if (negative)
	{
		i++;
	}
	for (i; i < word.size(); i++)
	{
		if (word[i] != '0')
		{
			word = word.substr(i, word.size() - i);
			if (negative && i != 0)
			{
				word = "-" + word;
			}
			break;
		}
	}
	if (i == word.size())
	{
		return true;
	}

	if (!(str2int >> number) || to_string(number) != word)
		return false;
	else
		return true;
}
bool isString(string word)
{
	return (word[0] == '"' && word[word.length() - 1] == '"' && word.length() > 1);
}
bool isInfixExpression(vector<string> maybe_infix)
{
	bool sign_num = 0;
	bool isNum;
	int bracket = 0;
	for (unsigned i = 0; i < maybe_infix.size(); i++)
	{
		if (maybe_infix[i] != "(")
		{
			if (isNumber(maybe_infix[i]) || (variable_dictionary[maybe_infix[i]].isExist && variable_dictionary[maybe_infix[i]].isNumber))
				isNum = true;
			else if (isString(maybe_infix[i]) || (variable_dictionary[maybe_infix[i]].isExist && (!variable_dictionary[maybe_infix[i]].isNumber)))
				isNum = false;
			else
				return false;
			break;
		}
	}

	for (unsigned i = 0; i < maybe_infix.size(); i++)
	{
		if (maybe_infix[i] == "+" || maybe_infix[i] == "-" || maybe_infix[i] == "*")
		{
			if ((!isNum) && (maybe_infix[i] != "+"))
			{
				return false;
			}
			if (!sign_num)
			{
				return false;
			}
			sign_num = false;
		}
		else if (maybe_infix[i] == "(")
		{
			if (sign_num)
			{
				return false;
			}
			sign_num = false;
			bracket++;
		}
		else if (maybe_infix[i] == ")")
		{
			if (!sign_num)
			{
				return false;
			}
			sign_num = true;
			bracket--;
		}
		else
		{
			if (sign_num)
			{
				return false;
			}
			if (isNum)
			{
				if (!(isNumber(maybe_infix[i]) || (variable_dictionary[maybe_infix[i]].isExist && variable_dictionary[maybe_infix[i]].isNumber)))
				{
					return false;
				}
			}
			else
			{
				if (!(isString(maybe_infix[i]) || (variable_dictionary[maybe_infix[i]].isExist && (!variable_dictionary[maybe_infix[i]].isNumber))))
				{
					return false;
				}
			}
			sign_num = true;
		}
	}

	if (bracket == 0 && sign_num)
	{
		return true;
	}
	else
	{
		return false;
	}
}

vector<string> split(string line)
{
	vector<string> words;
	for (unsigned i = 0; i < line.size(); i++)
	{
		if (!isSpace(line[i]))
		{
			for (unsigned j = i; j < line.size(); j++)
			{
				if (isSpace(line[j]))
				{
					words.push_back(line.substr(i, j - i));
					i = j;
					break;
				}
			}
		}
	}
	return words;
}

vector<string> infix2postfix(vector<string> infix_expression)
{
	vector<string> stack;
	vector<string> postfix_expression;
	for (unsigned i = 0; i < infix_expression.size(); i++)
	{
		string element = infix_expression[i];

		if (element == "+" || element == "-")
		{
			while ((!stack.empty()) && stack.back() != "(")
			{
				postfix_expression.push_back(stack.back());
				stack.pop_back();
			}
			stack.push_back(element);
		}
		else if (element == "*")
		{
			while ((!stack.empty()) && stack.back() == "*")
			{
				postfix_expression.push_back(stack.back());
				stack.pop_back();
			}
			stack.push_back(element);
		}
		else if (element == "(")
		{
			stack.push_back(element);
		}
		else if (element == ")")
		{
			while (stack.back() != "(")
			{
				postfix_expression.push_back(stack.back());
				stack.pop_back();
			}
			stack.pop_back();
		}
		else
		{
			postfix_expression.push_back(element);
		}
	}

	while (!stack.empty())
	{
		postfix_expression.push_back(stack.back());
		stack.pop_back();
	}

	return postfix_expression;
}

int calculate_int(vector<string> postfix_expression)
{
	vector<int> stack;
	for (unsigned i = 0; i < postfix_expression.size(); i++)
	{
		string element = postfix_expression[i];

		if (element == "+")
		{
			int x = stack.back();
			stack.pop_back();
			int y = stack.back();
			stack.pop_back();

			stack.push_back(y + x);
		}
		else if (element == "-")
		{
			int x = stack.back();
			stack.pop_back();
			int y = stack.back();
			stack.pop_back();

			stack.push_back(y - x);
		}
		else if (element == "*")
		{
			int x = stack.back();
			stack.pop_back();
			int y = stack.back();
			stack.pop_back();

			stack.push_back(y * x);
		}
		else
		{
			if (!isNumber(element))
			{
				stack.push_back(variable_dictionary[element].value_int);
			}
			else
			{
				stringstream str2int(element);
				int number;
				str2int >> number;
				stack.push_back(number);
			}
		}
	}
	return stack.back();
}
string calculate_str(vector<string> postfix_expression)
{
	vector<string> stack;
	for (unsigned i = 0; i < postfix_expression.size(); i++)
	{
		string element = postfix_expression[i];

		if (element == "+")
		{
			string y = stack.back();
			stack.pop_back();
			string x = stack.back();
			stack.pop_back();

			stack.push_back(x + y);
		}
		else
		{
			if (isString(element))
			{
				stack.push_back(element.substr(1, element.length() - 2));
			}
			else
			{
				stack.push_back(variable_dictionary[element].value_str);
			}
		}
	}
	return stack.back();
}

int main()
{
	string line;
	while (getline(cin, line))
	{
		vector<string> command = split(line + " ");

		if (command.size() < 1)
		{
			continue;
		}

		if (command[0] == "var")
		{
			if (command.size() % 2 != 1)
			{
				cout << "Error: illegal identifier" << endl;
				continue;
			}
			for (unsigned i = 1; i < command.size() - 1; i += 1)
			{
				if (i % 2 == 1)
				{
					if (!isIdentifier(command[i]))
					{
						cout << "Error: illegal identifier" << endl;
						goto Error;
					}
					if (variable_dictionary[command[i]].isExist)
					{
						cout << "Error: Redeclare" << endl;
						goto Error;
					}
				}
				else
				{
					if (command[i] != ",")
					{
						cout << "Error: illegal identifier" << endl;
						goto Error;
					}
				}
			}
			if (command[command.size() - 1] == "int")
			{
				for (unsigned i = 1; i < command.size(); i += 2)
				{
					variable_dictionary[command[i]].isExist = true;
					variable_dictionary[command[i]].isNumber = true;
				}
			}
			else if (command[command.size() - 1] == "string")
			{
				for (unsigned i = 1; i < command.size(); i += 2)
				{
					variable_dictionary[command[i]].isExist = true;
					variable_dictionary[command[i]].isNumber = false;
				}
			}
			else
			{
				cout << "Error: Unknown type " << command[command.size() - 1] << endl;
				continue;
			}
		}
		else
		{
			if (command.size() > 1 && command[1] == ":=")
			{
				if (!isIdentifier(command[0]))
				{
					cout << "Error: illegal identifier." << endl;
					continue;
				}
				if (variable_dictionary[command[0]].isExist)
				{
					cout << "Error:  Redeclare" << endl;
					continue;
				}
				if (command.size() < 3)
				{
					cout << "Error:  No expression" << endl;
					continue;
				}

				vector<string> expression;
				expression.assign(command.begin() + 2, command.end());
				if (!isInfixExpression(expression))
				{
					cout << "Error: illegal Expression." << endl;
					continue;
				}

				unsigned i;
				for (i = 0; i < expression.size(); i++)
				{
					if (expression[i] != "(")
					{
						break;
					}
				}

				if (isNumber(expression[i]) || (variable_dictionary[expression[i]].isExist && variable_dictionary[expression[i]].isNumber))
				{
					variable_dictionary[command[0]].isExist = true;
					variable_dictionary[command[0]].isNumber = true;
					variable_dictionary[command[0]].value_int = calculate_int(infix2postfix(expression));
				}
				else
				{
					variable_dictionary[command[0]].isExist = true;
					variable_dictionary[command[0]].isNumber = false;
					variable_dictionary[command[0]].value_str = calculate_str(infix2postfix(expression));
				}
			}
			else if (command.size() > 1 && command[1] == "=")
			{
				if (!variable_dictionary[command[0]].isExist)
				{
					cout << "Error: Identifier a is undeclared." << endl;
					continue;
				}
				if (command.size() < 3)
				{
					cout << "Error:  No expression" << endl;
					continue;
				}

				vector<string> expression;
				expression.assign(command.begin() + 2, command.end());
				if (!isInfixExpression(expression))
				{
					cout << "Error: illegal Expression." << endl;
					continue;
				}

				unsigned i;
				for (i = 0; i < expression.size(); i++)
				{
					if (expression[i] != "(")
					{
						break;
					}
				}

				if (variable_dictionary[command[0]].isNumber)
				{
					if (isNumber(expression[i]) || (variable_dictionary[expression[i]].isExist && variable_dictionary[expression[i]].isNumber))
						variable_dictionary[command[0]].value_int = calculate_int(infix2postfix(expression));
					else
						cout << "Error: Type error." << endl;
					continue;
				}
				else
				{
					if (isString(expression[0]) || (variable_dictionary[expression[0]].isExist && (!variable_dictionary[expression[0]].isNumber)))
						variable_dictionary[command[0]].value_str = calculate_str(infix2postfix(expression));
					else
						cout << "Error: Type error." << endl;
					continue;
				}
			}
			else
			{
				if (!isInfixExpression(command))
				{
					cout << "Error: I don't know what you want to do." << endl;
					continue;
				}
				else
				{
					unsigned i;
					for (i = 0; i < command.size(); i++)
					{
						if (command[i] != "(")
						{
							break;
						}
					}

					if (isNumber(command[i]) || (variable_dictionary[command[i]].isExist && variable_dictionary[command[i]].isNumber) || command[i] == "(")
						cout << calculate_int(infix2postfix(command)) << endl;
					else
					{
						string output = calculate_str(infix2postfix(command));
						cout << "\"";
						for (unsigned i = 0; i < output.size(); i++)
						{
							if (output[i] == '"')
							{
								cout << '\\' << '"';
							}
							else
							{
								cout << output[i];
							}
						}
						cout << "\"" << endl;
					}
				}
			}
		}
	Error:
		line;
	}

	return 0;
}