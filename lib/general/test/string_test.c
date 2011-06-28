/**
 * $Id$
 * splite_text()のテスト用コード
 * 
 * Copyright (c) 2011 mozxxx <mozxxx81@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
 
#include <stdio.h>
#include <string.h>
#include "../../../lib/minunit.h"
#include "../string.c"

int tests_run = 0;

static char * test_split_string();
static char * test_copy_string();
static char * all_tests();

static char * test_split_string()
{
    
    char *string, *delimiter, *results[2];
    struct splitedtext *res, *rp;
    int split_count, i;
    
    string = "hoge";
    delimiter = ":";
    split_count = split_string(string, delimiter, &res);
    mu_assert("error split_count != 0", split_count == 1);
    
    string = "http://yahoo.co.jp";
    delimiter = "://";
    
    split_count = split_string(string, delimiter, &res);
    mu_assert("error split_count != 2 ", split_count == 2);
  
    i = 0;
    for (rp = res; rp != NULL ; rp=rp->next) {
		
        results[i] = rp->string;
		i++;
        
    }
		
    mu_assert("error results[0] != 'http' ", strcmp(results[0], "http") == 0); 
    mu_assert("error results[1] != 'yahoo.co.jp' ", strcmp(results[1], "yahoo.co.jp") == 0); 
    
    freesplitedtext(res);
    
    return NULL;

}

static char * test_copy_string()
{
	char *dest, *src;
	
	src = "source";
	copy_string(&dest, src);
	mu_assert("error dest != src", strcmp(dest, src) == 0);
		
	src = "hoge";
	copy_string(&dest, src);
	mu_assert("error dest2 != src", strcmp(dest, src) == 0);
	
	free(dest);
	
	return NULL;
}

static char * all_tests()
{
    mu_run_test(test_split_string);
    mu_run_test(test_copy_string);
	return NULL;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
