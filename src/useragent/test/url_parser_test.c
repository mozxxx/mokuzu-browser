#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../lib/minunit.h"
#include "../url_parser.c"

static char *all_tester();
static char *test_parse_url();
static char *test_free_urlinfo();
static char *test_parse_url_query();

int tests_run = 0;

static char *
all_tester() {
	mu_run_test(test_parse_url);
	mu_run_test(test_free_urlinfo);
	mu_run_test(test_parse_url_query);
	return NULL;
}

static char *
test_parse_url() {
	
	struct urlinfo *res1, *res2, *res3, *res4, *res5, *res6, *res7;
	char *url, *url2, *url3, *url4, *url5, *url6, *url7;
		
	url = "http://www.ics.uci.edu:80/pub/ietf/uri/?query1=value1&query1=value2#Related";
	int result1 = parse_url(url, &res1);
	
	mu_assert("parse error", result1 == 0);
	mu_assert("url error", strcmp(res1->url, url) == 0);
	mu_assert("scheme error", strcmp(res1->scheme,	 "http") == 0);
	mu_assert("host error", strcmp(res1->host,	 "www.ics.uci.edu") == 0);
	mu_assert("port error", strcmp(res1->port,	 "80") == 0);
	mu_assert("path error", strcmp(res1->path,	 "/pub/ietf/uri/") == 0);
	mu_assert("query_string error", strcmp(res1->query_string,"query1=value1&query1=value2") == 0);
	mu_assert("fragment error", strcmp(res1->fragment, "Related") == 0);
	
	url2 = "yahoo.co.jp/index.html";
	int result2 = parse_url(url2, &res2);
	
	mu_assert("parse2 error", result2 == 0);
	mu_assert("url2 error", strcmp(res2->url, url2) == 0);
	mu_assert("scheme2 error", strcmp(res2->scheme,	"") == 0);
	mu_assert("host2 error", strcmp(res2->host,	 "yahoo.co.jp") == 0);
	mu_assert("port2 error", strcmp(res2->port,	 "") == 0);
	mu_assert("path2 error", strcmp(res2->path,	 "/index.html") == 0);
	mu_assert("query2_str error", strcmp(res2->query_string,"") == 0);
	mu_assert("fragment2 error", strcmp(res2->fragment, "") == 0);
	
	url3 = "https://yahoo.co.jp:/?query=val1";
	int result3 = parse_url(url3, &res3);
	
	mu_assert("parse3 error", result3 == 0);
	mu_assert("url3 error", strcmp(res3->url, url3) == 0);
	mu_assert("scheme3 error", strcmp(res3->scheme,	"https") == 0);
	mu_assert("host3 error", strcmp(res3->host,	 "yahoo.co.jp") == 0);
	mu_assert("port3 error", strcmp(res3->port,	 "") == 0);
	mu_assert("path3 error", strcmp(res3->path,	 "/") == 0);
	mu_assert("query_string3 error", strcmp(res3->query_string,"query=val1") == 0);
	mu_assert("fragmen3 error", strcmp(res3->fragment, "") == 0);

	url4 = "yahoo.co.jp?hoge=fuge";
	int result4 = parse_url(url4, &res4);
	
	mu_assert("parse4 error", result4 == 0);
	mu_assert("url4 error", strcmp(res4->url, url4) == 0);
	mu_assert("scheme4 error", strcmp(res4->scheme,	"") == 0);
	mu_assert("host4 error", strcmp(res4->host,	 "yahoo.co.jp") == 0);
	mu_assert("port4 error", strcmp(res4->port,	 "") == 0);
	mu_assert("path4 error", strcmp(res4->path,	 "") == 0);
	mu_assert("query_string4 error", strcmp(res4->query_string,"hoge=fuge") == 0);
	mu_assert("fragment4 error", strcmp(res4->fragment, "") == 0);

	url5 = "http://www.google.co.jp/search?aq=f&sourceid=chrome&client=ubuntu&channel=cs&ie=UTF-8&q=C+Unittest/search?aq=f&sourceid=chrome&client=ubuntu&channel=cs&ie=UTF-8&q=C+Unittest";
	int result5 = parse_url(url5, &res5);
	
	mu_assert("parse5 error", result5 == 0);
	mu_assert("url5 error", strcmp(res5->url, url5) == 0);
	mu_assert("scheme5 error", strcmp(res5->scheme,	"http") == 0);
	mu_assert("host5 error", strcmp(res5->host,	 "www.google.co.jp") == 0);
	mu_assert("port5 error", strcmp(res5->port,	 "") == 0);
	mu_assert("path5 error", strcmp(res5->path,	 "/search") == 0);
	mu_assert("query_string5 error", strcmp(res5->query_string,"aq=f&sourceid=chrome&client=ubuntu&channel=cs&ie=UTF-8&q=C+Unittest/search?aq=f&sourceid=chrome&client=ubuntu&channel=cs&ie=UTF-8&q=C+Unittest") == 0);
	mu_assert("fragment5 error", strcmp(res5->fragment, "") == 0);
	
	url6 = "http://twitter.com/#!/toptweets_ja";
	int result6 = parse_url(url6, &res6);
	
	mu_assert("parse6 error", result6 == 0);
	mu_assert("url6 error", strcmp(res6->url, url6) == 0);
	mu_assert("scheme6 error", strcmp(res6->scheme,	"http") == 0);
	mu_assert("host6 error", strcmp(res6->host,	 "twitter.com") == 0);
	mu_assert("port6 error", strcmp(res6->port,	 "") == 0);
	mu_assert("path6 error", strcmp(res6->path,	 "/") == 0);
	mu_assert("query_string6 error", strcmp(res6->query_string,"") == 0);
	mu_assert("fragment6 error", strcmp(res6->fragment, "!/toptweets_ja") == 0);
	
	url7 = ":80?hoge=fuge";
	int result7 = parse_url(url7, &res7);
	
	mu_assert("parse6 error", result7 == -1);

	return NULL;
}

static char *
test_free_urlinfo() {
	
	struct urlinfo *_urlinfo;
	parse_url("http://www.google.com", &_urlinfo);
	free_urlinfo(&_urlinfo);

	return NULL;

}

static char *
test_parse_url_query()
{
	
	struct urlinfo *res1, *res2, *res3;
	char *url = "http://www.ics.uci.edu:80/pub/ietf/uri/?query1=value1&query2=value2#Related";
	parse_url(url, &res1);
	mu_assert("error : queries != NULL", res1->queries != NULL);
	mu_assert("error : queries->length == 2", res1->queries->length == 2);
		
	mu_assert("error : parameters[0].key == 'query1'", strcmp(res1->queries->parameters[0].key, "query1") == 0);
	mu_assert("error : parameters[0].value == 'value1'", strcmp(res1->queries->parameters[0].value, "value1") == 0);
	mu_assert("error : parameters[1].key == 'query2'", strcmp(res1->queries->parameters[1].key, "query2") == 0);
	mu_assert("error : parameters[1].value == 'value2'", strcmp(res1->queries->parameters[1].value, "value2") == 0);
	
	char *url2 = "http://www.ics.uci.edu:80/pub/ietf/uri#Related";
	parse_url(url2, &res2);
	mu_assert("error : queries2 == NULL", res2->queries == NULL);

	char *url3 = "http://www.ics.uci.edu:80/pub/ietf/uri?q=1&r=&s#Related";
	parse_url(url3, &res3);
	mu_assert("error : queries->length == 3", res3->queries->length == 3);
	mu_assert("error : parameters[0].key == 'q'", strcmp(res3->queries->parameters[0].key, "q") == 0);
	mu_assert("error : parameters[0].value == '1'", strcmp(res3->queries->parameters[0].value, "1") == 0);
	mu_assert("error : parameters[1].key == 'r'", strcmp(res3->queries->parameters[1].key, "r") == 0);
	mu_assert("error : parameters[1].value == '2'", strcmp(res3->queries->parameters[1].value, "") == 0);
	mu_assert("error : parameters[2].key == 's'", strcmp(res3->queries->parameters[2].key, "s") == 0);
	mu_assert("error : parameters[2].value == '3'", strcmp(res3->queries->parameters[2].value, "") == 0);
	
	
	return NULL;
	
}

int
main (void)
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
