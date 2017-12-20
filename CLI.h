#ifndef __CLI_H__
#define __CLI_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include <vector>
#include <map>

#include "Common.h"

#define MAX_COMMAND_LEN                         200
#define MAX_HISTORY_COMMANDS                    100

class CLI;

typedef void (*CliProcessingProc)(CLI*);

class CliCommand;

typedef std::vector<CliCommand*>				CliCommandVector;
typedef CliCommandVector::iterator				CliCommandVectorIterator;

typedef std::map<std::string, CliCommand*>		CliCommandMap;
typedef CliCommandMap::iterator					CliCommandMapIterator;

////////////////////////////////////////////////

class CliCommand {
	public:
		CliCommand (const char* prompt, CliProcessingProc processingProc, const char* helpString) {
			m_prompt = prompt;
			m_processingProc = processingProc;
			m_helpString = helpString;
		}

		const char*								m_prompt;
		CliProcessingProc						m_processingProc;
		const char*								m_helpString;
};

////////////////////////////////////////////////

class CLI {
	public:
		CLI ();

		void addCommand (const char* prompt, CliProcessingProc processingProc, const char* helpString=NULL);

		void processFile (const char* filename);
		void processInput (FILE* fp=stdin);

		char* getNextToken ();
		int getIntParameter (bool mandatory=true, int defaultValue=0);
		char* getStringParameter (bool mandatory=true, char* defaultValue=NULL);

	protected:
		void printPrompt ();
		void processLine (char* line);
		void processLineWithArgs (const char* format, ...);

		static void processExit (CLI*);

		virtual void processHelp ();
		static void processHelp (CLI* cli) { cli->processHelp(); }

		virtual void processHistory ();
		static void processHistory (CLI* cli) { cli->processHistory(); }

		virtual void processTrace ();
		static void processTrace (CLI* cli) { cli->processTrace(); }

		virtual void processFile ();
		static void processFile (CLI* cli) { cli->processFile(); }

	protected:
		char m_history[MAX_HISTORY_COMMANDS][MAX_COMMAND_LEN];
		int m_historyIndex;

		const char* m_prompt;

		CliCommandVector		m_commands; // in order of entry
		CliCommandMap			m_commandMap;
};

#endif
