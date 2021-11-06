// SPDX-License-Identifier: GPL-2.0
/* Copyright 2020 NXP */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "isochron.h"

enum isochron_func {
	ISOCHRON_SEND,
	ISOCHRON_RCV,
	ISOCHRON_STATS,
	ISOCHRON_REPORT,
};

static void isochron_usage(void)
{
	fprintf(stderr, "isochron usage:\n");
	fprintf(stderr, "isochron send ...\n");
	fprintf(stderr, "isochron rcv ...\n");
	fprintf(stderr, "isochron stats ...\n");
	fprintf(stderr, "isochron report ...\n");
	fprintf(stderr, "Run \"isochron send --help\" or ");
	fprintf(stderr, "\"isochron rcv --help\" or ");
	fprintf(stderr, "\"isochron stats --help\" or ");
	fprintf(stderr, "\"isochron report --help\" for more details.\n");
}

static int isochron_parse_args(int *argc, char ***argv,
			       enum isochron_func *func)
{
	char *prog_name;
	char *prog_func;

	if (*argc < 2) {
		isochron_usage();
		return -EINVAL;
	}

	prog_name = *argv[0];
	(*argv)++;
	(*argc)--;
	if (strcmp(prog_name, "isochron-send") == 0) {
		*func = ISOCHRON_SEND;
		return 0;
	} else if (strcmp(prog_name, "isochron-rcv") == 0) {
		*func = ISOCHRON_RCV;
		return 0;
	} else if (strcmp(prog_name, "isochron-stats") == 0) {
		*func = ISOCHRON_STATS;
		return 0;
	} else if (strcmp(prog_name, "isochron-report") == 0) {
		*func = ISOCHRON_REPORT;
		return 0;
	}

	prog_func = (*argv)[0];
	(*argv)++;
	(*argc)--;

	if (!strcmp(prog_func, "-V") || !strcmp(prog_func, "--version")) {
		fprintf(stderr, "%s version %s\n", prog_name, VERSION);
		return -EINVAL;
	}

	if (!strcmp(prog_func, "-h") || !strcmp(prog_func, "--help")) {
		isochron_usage();
		return -EINVAL;
	}

	if (strcmp(prog_func, "send") == 0) {
		*func = ISOCHRON_SEND;
		return 0;
	} else if (strcmp(prog_func, "rcv") == 0) {
		*func = ISOCHRON_RCV;
		return 0;
	} else if (strcmp(prog_func, "stats") == 0) {
		*func = ISOCHRON_STATS;
		return 0;
	} else if (strcmp(prog_func, "report") == 0) {
		*func = ISOCHRON_REPORT;
		return 0;
	}

	fprintf(stderr,
		"%s: unknown function %s, expected \"send\", \"rcv\", \"stats\" or \"report\"\n",
		prog_name, prog_func);

	return -EINVAL;
}

int main(int argc, char *argv[])
{
	enum isochron_func func;
	int rc;

	rc = isochron_parse_args(&argc, &argv, &func);
	if (rc)
		return -rc;

	switch (func) {
	case ISOCHRON_SEND:
		rc = isochron_send_main(argc, argv);
		break;
	case ISOCHRON_RCV:
		rc = isochron_rcv_main(argc, argv);
		break;
	case ISOCHRON_STATS:
		rc = isochron_stats_main(argc, argv);
		break;
	case ISOCHRON_REPORT:
		rc = isochron_report_main(argc, argv);
		break;
	default:
		isochron_usage();
		rc = -EINVAL;
		break;
	}

	return -rc;
}
