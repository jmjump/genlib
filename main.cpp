#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Common.h"

extern void applicationSpecific();

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
printf("g_debugLevel=%d\n", g_debugLevel);
		} else if (opt == 'D') {
			g_debugLevel = atoi(optarg);
printf("g_debugLevel=%d\n", g_debugLevel);
		}
	}

	for (; optind<argc; optind++) {
		printf("optind %d: \"%s\"\n", optind, argv[optind]);
	}

	TRACE(0, "Hello, world\n");
	TRACE(1, "Goodbye cruel world\n");
	TRACE(2, "Parting is such sweet sorrow\n");

	applicationSpecific();
}
