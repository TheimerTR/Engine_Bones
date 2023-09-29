#pragma once
#include "Globals.h"

#include "Module.h"
#include "Application.h"
#include "ModuleEditor.h"

#include <stdio.h>
#include <vector>
#include <string>

void AddLog(std::string logText)
{
	if (&app->editor->l_Logs != NULL)
	{
		//app->editor->l_Logs.push_back(logText);
		VectorStringPushBack(app->editor->l_Logs, MAX_LOG_FPS, logText);
	}
}

void ClearLogs(std::vector<std::string>& l_Logs) //This method will remove the last element 
{            
	for (int i = 0; i < l_Logs.size() - 1; i++)
	{
		l_Logs[i].clear();
	}

	l_Logs.clear();
}

std::string RetLogs(std::string text)
{
	return text;
}

void log(const char file[], int line, LogTypeCase _type, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);

	std::string log_text = std::string(tmp_string2);

	if (app != nullptr)
	{
		if (app->editor != nullptr)
		{
			AddLog(log_text);
			//RetLogs(log_text);
		}
	}
}

void VectorStringPushBack(std::vector<std::string>& vector, int MaxValue, std::string Log)
{
	if (vector.size() == vector.capacity())
	{
		//If the Vectir is full capacity, deletes the most old element of the list, and move all the rest elementes one pos back.

		for (size_t i = 0; i < vector.size(); i++)
		{
			if (i + 1 < vector.size())
			{
				vector[i] = vector[i + 1];
			}
		}

		//Put the last element at the back
		vector[vector.capacity() - 1] = Log;
	}
	else
	{
		//If vec is not full so put new elements
		vector.push_back(Log);
	}
}