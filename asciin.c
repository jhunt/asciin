#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

static int EACH[129] = {0};
static int COUNT = 0;
static const char *NAMES[128] = {
	"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
	"BS",  "TAB", "LF",  "VT",  "FF",  "CR",  "SO",  "SI",
	"DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
	"CAN", "EM",  "SUB", "ESC", "FS",  "GS",  "RS",  "US",

	"sp", 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, "DEL",
};

int main(int argc, char **argv)
{
	char buf[8192], c;
	ssize_t i, n;
	char mode = '*';

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && argv[i][2] == '\0') {
			mode = argv[i][1];
			if (mode == 'h' || mode == '?') {
				fprintf(stderr, "asciin - ASCII character counter\n");
				fprintf(stderr, "USAGE: asciin [-a|-n|-p|-u] <file\n\n");
				fprintf(stderr, "    -a  Only display alphabetic characters\n");
				fprintf(stderr, "    -n  Only display numeric characters\n");
				fprintf(stderr, "    -s  Only display symbol characters\n");
				fprintf(stderr, "    -p  Only display printable characters\n");
				fprintf(stderr, "    -u  Only display unprintable characters\n");
				return 0;
			}
		}
	}

	while ( (n = read(0, buf, 8192)) > 0 ) {
		for (i = 0; i < n; i++) {
			c = buf[i] & 0x7f;
			if (mode == 'a' &&  !isalpha(c)) { continue; }
			if (mode == 'n' &&  !isdigit(c)) { continue; }
			if (mode == 's' &&  !ispunct(c)) { continue; }
			if (mode == 'p' &&  !isprint(c)) { continue; }
			if (mode == 'u' &&   isprint(c)) { continue; }
			COUNT++;
			if (buf[i] > 0x7f) {
				EACH[128]++;
			} else {
				EACH[c & 0x7f]++;
			}
		}
	}
	if (n < 0) {
		fprintf(stderr, "error: %s (%d)\n", strerror(errno), errno);
		return 1;
	}

	for (i = 0; i < 128; i++) {
		if (EACH[i] > 0) {
			if (NAMES[i]) {
				printf("%02x %3s % 9d (%6.2lf%%)\n", (unsigned int)i, NAMES[i], EACH[i], EACH[i] * 100. / COUNT);
			} else {
				printf("%02x   %c % 9d (%6.2lf%%)\n", (unsigned int)i, (char)i, EACH[i], EACH[i] * 100. / COUNT);
			}
		}
	}
	fprintf(stderr, "%u total\n", COUNT);
	if (EACH[128] > 0) {
		fprintf(stderr, "%u bogons\n", EACH[128]);
	}
	return 0;
}
