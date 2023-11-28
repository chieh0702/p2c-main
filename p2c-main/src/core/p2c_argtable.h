#ifndef _P2C_ARGTABLE_H_
#define _P2C_ARGTABLE_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <map>

void parseValue(const std::string &jsonStr, size_t &pos, std::string &value)
{
	// 解析 JSON 值
	size_t start = pos;
	while (pos < jsonStr.length() && jsonStr[pos] != ',' && jsonStr[pos] != '}' && jsonStr[pos] != ']')
	{
		if (jsonStr[pos] == '"')
		{
			// 處理字串
			++pos; // 跳過開頭的雙引號
			while (pos < jsonStr.length() && jsonStr[pos] != '"')
			{
				value += jsonStr[pos++];
			}
			++pos; // 跳過結尾的雙引號
		}
		else if (jsonStr[pos] == '{' || jsonStr[pos] == '[')
		{
			// 遞迴處理對象或陣列
			int countBraces = 1; // 計數大括號或中括號的數量
			value += jsonStr[pos++];
			while (pos < jsonStr.length() && countBraces > 0)
			{
				value += jsonStr[pos++];
				if (jsonStr[pos - 1] == '{' || jsonStr[pos - 1] == '[')
				{
					++countBraces;
				}
				else if (jsonStr[pos - 1] == '}' || jsonStr[pos - 1] == ']')
				{
					--countBraces;
				}
			}
		}
		else
		{
			// 其他情況，例如數字、布爾值等
			value += jsonStr[pos++];
		}
	}

	// 去除首尾空格
	while (start < value.length() && isspace(value[start]))
	{
		++start;
	}
	while (!value.empty() && isspace(value.back()))
	{
		value.pop_back();
	}
}

class p2c_argtable
{
private:
	std::map<std::string, std::queue<std::string> *> *_args;

public:
	p2c_argtable()
	{
		_args = new std::map<std::string, std::queue<std::string> *>;
		p2c_alerter::alerting(DEBUG, "'p2c_argtable':71: success new '_args'");
	};
	~p2c_argtable()
	{
		_args->clear();
		delete _args;
	};
	void initArgs(int argc, char const *argv[]);
	void addArg(std::string, std::vector<std::string>);
	void addArg(std::string, std::string);
	bool countArg(std::string name)
	{
		return _args->count(name) == 1 ? true : false;
	};
	std::queue<std::string> *getArg(std::string);
	std::string getJSON();
	// TODO:json to argtable
	void parserJSON(std::string);
};

// store args to 'init_args'
void p2c_argtable::initArgs(int argc, char const *argv[])
{
	int state = 0;
	std::string tempstr;
	for (int i = 1; i < argc; i++)
		switch (state)
		{
		case 0:
			state = 1;
			tempstr.clear();
			tempstr = argv[i];
			break;
		case 1:
			if (argv[i][0] == '-')
			{
				this->addArg("init_args", tempstr);
				i--;
				state = 0;
				break;
			}
			tempstr += " ";
			tempstr += argv[i];
			break;
		default:
			break;
		}
	if (!tempstr.empty())
		this->addArg("init_args", tempstr);
	p2c_alerter::alerting(DEBUG, "'p2c_argtable':116: success init_args");
}

void p2c_argtable::addArg(std::string name, std::vector<std::string> contexts)
{
	for (std::string context : contexts)
		this->addArg(name, context);
}

void p2c_argtable::addArg(std::string name, std::string context)
{
	if (!_args)
	{
		p2c_alerter::alerting(WARN, "'p2c_argtable':129: _args is null, when add", name);
		return;
	}
	if (!_args->count(name))
	{
		_args->insert(std::pair<std::string, std::queue<std::string> *>(name, new std::queue<std::string>()));
		p2c_alerter::alerting(DEBUG, "'p2c_argtable':135: success new queue for", name);
	}
	(*_args)[name]->push(context);
}

// return pointer of queue, if not found, return empty queue<string>
std::queue<std::string> *p2c_argtable::getArg(std::string name)
{
	if (_args->count(name))
		return (*_args)[name];
	else
	{
		p2c_alerter::alerting(INFO, "map '_args' hasn't key =", name);
		return new std::queue<std::string>;
	}
}

std::string p2c_argtable::getJSON()
{
	if (_args->empty())
		return "{}";
	std::string JSON = "{";
	bool first_key = true;
	for (auto &n : *_args)
	{
		if (n.second->empty())
			continue;
		if (first_key)
		{
			first_key = false;
			JSON += "\"" + n.first + "\":[";
		}
		else
			JSON += ",\"" + n.first + "\":[";
		bool first_val = true;
		std::queue<std::string> duplicate(*n.second);
		do
		{
			if (first_val)
			{
				first_val = false;
				JSON += "\"" + duplicate.front() + "\"";
			}
			else
				JSON += ",\"" + duplicate.front() + "\"";
			duplicate.pop();
		} while (!duplicate.empty());
		JSON += "]";
	}
	JSON += "}";
	return JSON;
}

void p2c_argtable::parserJSON(std::string jsonStr)
{
	// 清空 _args，以便反向解析 JSON 時重新填充
	for (auto &n : *_args)
	{
		delete n.second;
	}
	_args->clear();

	size_t pos = 0;
	while (pos < jsonStr.length())
	{
		// 找到 JSON 鍵
		if (jsonStr[pos] == '"')
		{
			size_t keyStart = ++pos;
			while (pos < jsonStr.length() && jsonStr[pos] != '"')
			{
				++pos;
			}
			std::string key = jsonStr.substr(keyStart, pos - keyStart);

			// 跳過冒號和空格
			while (pos < jsonStr.length() && (jsonStr[pos] == ':' || isspace(jsonStr[pos])))
			{
				++pos;
			}

			// 解析值
			std::string value;
			parseValue(jsonStr, pos, value);

			// 將解析得到的資料存入 _args 中
			addArg(key, value);

			// 跳過逗號和空格
			while (pos < jsonStr.length() && (jsonStr[pos] == ',' || isspace(jsonStr[pos])))
			{
				++pos;
			}
		}
		else
		{
			// 跳過其他字符
			++pos;
		}
	}
}

#endif
