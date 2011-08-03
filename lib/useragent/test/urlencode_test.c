#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../third-party/minunit.h"
#include "../urlencode.c"


static char *all_tester();
static char *test_encode_url();
static char *test_is_url_unreserved_character();
static char *test_is_url_reserved_character();

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
	mu_run_test(test_is_url_reserved_character);
	mu_run_test(test_encode_url);
	return NULL;
}

static char * test_is_url_unreserved_character()
{
	char unreserved_char[] = "01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.~";
	char not_unreserved_char[] = "!*'();:@&=+$,/?#[]";
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

static char * test_is_url_reserved_character()
{
	char reserved_char[] = "!*'();:@&=+$,/?#[]";
	char not_reserved_char[] = "01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.~";

	int i, len;

	for (i = 0, len = strlen(reserved_char); i < len ; i++) {
		printf("%c\n", reserved_char[i]);
		mu_assert("err: reserved_char", is_url_reserved_character(reserved_char[i]) == 1);
	}

	for (i = 0, len = strlen(not_reserved_char); i < len ; i++) {
		printf("%c\n", not_reserved_char[i]);
		mu_assert("err not_reserved_char", is_url_reserved_character(not_reserved_char[i]) == 0);
	}

        return NULL;
}


static char *test_encode_url()
{
	
	char *url, *encoded_url;
	
	url = "wikipediaウィキペディア";
	encoded_url = encode_url(url);
	mu_assert("err1 エンコーディング失敗", strcmp(encoded_url, "wikipedia%E3%82%A6%E3%82%A3%E3%82%AD%E3%83%9A%E3%83%87%E3%82%A3%E3%82%A2") == 0);
	free(encoded_url);
	
	url = "01234abcXYZ-.~あいうえお";
	encoded_url = encode_url(url);
	mu_assert("err2 エンコーディング失敗", strcmp(encoded_url, "01234abcXYZ-.~%E3%81%82%E3%81%84%E3%81%86%E3%81%88%E3%81%8A") == 0);
	
	url = "-";
	encoded_url = encode_url(url);
	mu_assert("err3 エンコーディング失敗", strcmp(encoded_url, "%8A") != 0);
	free(encoded_url);

	url = "http://ja.wikipedia.org/wiki/URLエンコード";
	encoded_url = encode_url(url);
	mu_assert("err4 エンコーディング失敗", strcmp(encoded_url, "http://ja.wikipedia.org/wiki/URL%E3%82%A8%E3%83%B3%E3%82%B3%E3%83%BC%E3%83%89") == 0);
	free(encoded_url);
	
	return NULL;
	
}
