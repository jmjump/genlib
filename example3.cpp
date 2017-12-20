#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Common.h"

#include "tinyxml2.h"

using namespace tinyxml2;

/////////////////////////////////////////////////////////

void processFile (const char* filename) {
	TRACE(0, "%s(%s)\n", __func__, filename);

	XMLDocument* doc = new XMLDocument();

	XMLError status = doc->LoadFile(filename);
	if (status != XML_SUCCESS) {
		FATAL("%s() unable to LoadFile(%s) status=%s\n", __func__, filename, XMLDocument::ErrorIDToName(status));
	}

	XMLElement* league = doc->RootElement();

	static const char* CONFERENCE = "conference";
	XMLElement* conference = league->FirstChildElement(CONFERENCE);
	while (conference) {
		const char* abbrev = conference->Attribute("abbrev");
		const char* name = conference->Attribute("name");

		if (!abbrev && !name) {
			FATAL("missing attribute(s) for conference\n");
		}

		printf("conference(abbrev=%s, name=%s)\n", abbrev, name);

		static const char* DIVISION = "division";
		XMLElement* division = conference->FirstChildElement(DIVISION);
		while (division) {
			const char* name = division->Attribute("name");
			if (!name) {
				FATAL("missing attribute \"name\" for division\n");
			}

			printf("    division(%s)\n", name);

			static const char* TEAM = "team";
			XMLElement* team = division->FirstChildElement(TEAM);
			while (team) {
				const char* abbrev = team->Attribute("abbrev");
				const char* name = team->Attribute("name");

				if (!abbrev || !name) {
					FATAL("missing attriubte for team\n");
				}

				TRACE(0, "        %s(abbrev=%s, name=%s)\n", TEAM, abbrev, name);

				team = team->NextSiblingElement(TEAM);
			}

			division = division->NextSiblingElement(DIVISION);
		}

		conference = conference->NextSiblingElement(CONFERENCE);
	}

	static const char* RESULTS = "results";
	XMLElement* results = league->FirstChildElement(RESULTS);
	if (results) {
		printf("results:\n");
		static const char* WEEK = "week";
		XMLElement* week = results->FirstChildElement(WEEK);
		while (week) {
			int id;
			XMLError status = week->QueryIntAttribute("id", &id);
			if (status != XML_SUCCESS) {
				FATAL("missing attribute \"id\" for week\n");
			}

			printf("  week %d\n", id);

			static const char* GAME = "game";
			XMLElement* game = week->FirstChildElement(GAME);
			while (game) {
				const char* text = game->GetText();
				if (!text) {
					FATAL("missing info for game\n");
				}

				printf("    %s\n", text);

				game = game->NextSiblingElement(GAME);
			}

			week = week->NextSiblingElement(WEEK);
		}
	}
}

/////////////////////////////////////////////////////////

static void printHelp (const char* argv0) {
	printf("%s [OPTIONS]\n", argv0);

	exit(0);
}

static void printVersion (const char* argv0) {
	static const char* version = "0.1";

	printf("%s version %s (%s %s)\n", argv0, version, __DATE__, __TIME__);

	exit(0);
}

int main (int argc, char* argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "hvdD:")) != EOF) {
		if (opt == 'h') {
			printHelp(argv[0]);
		} else if (opt == 'v') {
			printVersion(argv[0]);
		} else if (opt == 'd') {
			g_debugLevel++;
		} else if (opt == 'D') {
			g_debugLevel = atoi(optarg);
		}
	}

	if (argc > optind) {
		for (; optind<argc; optind++) {
			const char* filename = argv[optind];
			processFile(filename);
		}
	} else {
		const char* filename = "nfl.xml";
		processFile(filename);
	}

}
