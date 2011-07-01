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
	char unreserved_char[] = "01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.~";
	char not_unreserved_char[] = "?&^=/#$%*@+";
	int i, len;
	
	for (i = 0, len = strlen(unreserved_char); i < len ; i++) {
		printf("%c\n", unreserved_char[i]);
		mu_assert("err: unreserved_char", is_url_unreserved_character(unreserved_char[i]) == 1);
	}
	
	for (i = 0, len = strlen(not_unreserved_char); i < len ; i++) {
		printf("%c\n", not_unreserved_char[i]);
		mu_assert("err not_unreserved_char", is_url_unreserved_character(not_unreserved_char[i]) == 0);
	}
		
	return NULL;
}
