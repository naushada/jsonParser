#include "json.h"

int main(int argc, char **argv) {
	FILE *fp = stdin;
	JSONValue *json;
	JSONValue *tmpJson;

	if (argc >= 2) {
		if (!(fp = fopen(argv[1], "r"))) {
			fprintf(stderr, "File [%s] is not found!\n", argv[1]);
			return 1;
		}
	}

	if ((json = json_parser(fp))) {
		json_print(stdout, json);
		double d_val = json_value_at_index(json_value_at_key(json, "num" ), 9 )->d_value;
		fprintf(stderr, "Value num[9] = %f\n", d_val);
	} else {
		printf("Json error!\n");
	}

	return 0;
}
