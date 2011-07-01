#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../third-party/minunit.h"
#include "../urlencode.c"


static char *all_tester();
static char *test_is_url_unreserved_character();

int tests_run = 0;

int main (void)
{
	char *result = all_tester();	
	if (result != NULL) {
			printf("%s\n", result);
	} else {
			printf("ALL TESTS PASSED\n");
	}
	
	printf("Tests run: %d\n", tests_run);
	
	exit(EXIT_SUCCESS);
	
}

static char * all_tester() {
	mu_run_test(test_is_url_unreserved_character);
	return NULL;
}

static char * test_is_url_unreserved_character()
{
	
	mu_assert("a is not URL Unreserved Character.", is_url_unreserved_character('a') == 1);
	mu_assert("- is not URL Unreserved Character.", is_url_unreserved_character('-') == 1);
	mu_assert("0 is not URL Unreserved Character.", is_url_unreserved_character('0') == 1);
	mu_assert(". is not URL Unreserved Character.", is_url_unreserved_character('.') == 1);
	mu_assert("~ is not URL Unreserved Character.", is_url_unreserved_character('~') == 1);
	mu_assert("? is URL Unreserved Character.", is_url_unreserved_character('?') == 0);
	
	return NULL;
}
