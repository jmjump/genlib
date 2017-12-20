#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "Common.h"

#include "CLI.h"

CLI::CLI () {
	m_historyIndex = 0;

	m_prompt = ">";

	addCommand("?", processHelp, "this command");
	addCommand("help", processHelp, "this command");
	addCommand("quit", processExit, "quit the program");
	addCommand("exit", processExit, "quit the program");
	addCommand("file", processFile, "read commands from the specified file");
	addCommand("history", processHistory, "print the previously executed commands");
	addCommand("trace", processTrace, "set the debug level");
}

void CLI::addCommand (const char* prompt, CliProcessingProc processingProc, const char* helpString) {
	CliCommand* cliCommand = new CliCommand(prompt, processingProc, helpString);

	m_commands.push_back(cliCommand);
	m_commandMap[prompt] = cliCommand;
}

void CLI::processFile (const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("ERROR: unable to open \"%s\"\n", filename);
		return;
	}

	processInput(fp);

	fclose(fp);
}

void CLI::processFile () {
	const char* filename = getNextToken();
	if (!filename) {
		printf("missing filename\n");
		return;
	}

	processFile(filename);
}

#define ESC                                     27

void CLI::printPrompt () {
	printf("%c[2K", ESC); // erase to the end of the line
	printf("\r%s ", m_prompt);
}

static const char* WHITESPACE = " \t\n";

void CLI::processLineWithArgs (const char* format, ...) {
	char lineBuffer[512];
	va_list aptr;
	va_start(aptr, format);
	vsprintf(lineBuffer, format, aptr);
	va_end(aptr);

	processLine(lineBuffer);
}

void CLI::processLine (char* line) {
	const char* firstToken = strtok(line, WHITESPACE);

	CliCommandMapIterator it = m_commandMap.find(firstToken);
	if (it == m_commandMap.end()) {
		printf("unsupported command: %s\n", firstToken);
	} else {
		CliCommand* cliCommand = it->second;

		try {
			(cliCommand->m_processingProc)(this);
		} catch (std::string exception) {
			printf("%s\n", exception.c_str());
		}
	}
}

void CLI::processInput (FILE* fp) {
	while (1) {
		printPrompt();

		static char line[MAX_COMMAND_LEN];
		char* pLine = line;
		size_t n = sizeof(line);
		size_t read = getline(&pLine, &n, fp);

		// If we get to the end of the file, break
		if (read == -1) {
			break;
		}

		// ignore blank lines
		if (read == 1) {
			continue;
		}

		// ignore comments (#)
		if (pLine[0] == '#') {
			continue;
		}

		// strip the newline
		pLine[read-1] = 0;

		// Did the line start off with a bang!
		// TBD: anywhere in the line (e.g., doit !$)
		if (pLine[0] == '!') {
			int index = -1;
			if (pLine[1] == '!') {
				index = m_historyIndex - 1;
			} else {
				int offset;
				if (sscanf(&pLine[1], "%d", &offset) != 1) {
					continue;
				}
				if (offset < 0) {
					index = m_historyIndex + offset;
				} else {
					index = offset - 1;
				}
			}
			if ((index < 0) || (index >= m_historyIndex)) {
				continue;
			}
			strcpy(pLine, m_history[index]);
			printf("%s\n", pLine);
		}

		// copy the line to the history
		strcpy(m_history[m_historyIndex], pLine);

		// break the "line" into semicolon separated commands
		char* savePtr;
		char* separator = ";";
		char* cmd;
		while (cmd = strtok_r(pLine, separator, &savePtr)) {
			processLine(cmd);
			pLine = NULL;
		}

		// Update the history index
		m_historyIndex = (m_historyIndex + 1) % ArraySize(m_history);
	}
}

void CLI::processHistory () {
	for (int i=0; i<m_historyIndex; i++) {
		printf("%3d: %s\n", i+1, m_history[i]);
	}
}

char* CLI::getNextToken () {
	return strtok(NULL, WHITESPACE);
}

int CLI::getIntParameter (bool mandatory, int defaultValue) {
	int value = defaultValue;

	char* token = getNextToken();
	if (!token) {
		if (mandatory) {
			THROW("missing mandatory parameter");
		}
	} else {
		if (sscanf(token, "%d", &value) != 1) {
			THROW("unable to convert \"%s\" to an integer", token);
		}
	}

	return value;
}

char* CLI::getStringParameter (bool mandatory, char* defaultValue) {
	char* value = getNextToken();
	if (!value) {
		if (mandatory) {
			printf("missing mandatory parameter");
			throw;
		}

		value = defaultValue;
	}

	return value;
}

void CLI::processTrace () {
	int newTraceLevel = getIntParameter(false, -1);

	if (newTraceLevel == -1) {
		printf("trace level = %d\n", g_debugLevel);
	} else {
		printf("setting the trace level to %d\n", newTraceLevel);

		g_debugLevel = newTraceLevel;
	}
}


void CLI::processHelp () {
	printf("help:\n");

	CliCommandVectorIterator it;
	for (it=m_commands.begin(); it!=m_commands.end(); it++) {
		CliCommand* cliCommand = *it;

		const char* helpString = cliCommand->m_helpString;
		if (!helpString) {
			continue;
		}

		printf("%15s: %s\n", cliCommand->m_prompt, helpString);
	}
}

void CLI::processExit (CLI*) {
	exit(0);
}

