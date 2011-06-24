/**
 * $Id$
 * URL parser
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

/* TODO:
 * URLエンコードに対応
 * user:password に対応
 * http://yahoo.co.jp//// の時の動作
 * 国際化ドメインへの対応
 */
 
/**
 * 参考
 * RFC 2396
 */

#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>
#include "url_parser.h"
#include "../lib/string.h"

/**
 * URLパーサ
 * @param url パース対象のURL文字列
 * @param res パース結果が格納されているurlinfo構造体
 * @return 成功すれば0、失敗したら-1
 */
int parse_url(const char *url, struct urlinfo **res)
{
	
	struct urlinfo *response;
	regex_t preg;
	size_t nmatch = 7, len;
	regmatch_t pmatch[nmatch];
	char *_url, *scheme_delimiter,*result[nmatch];
	int err,i,_return = 0;
	struct splitedtext *query_splitedtext, *_query_splitedtext, *query_key_value;
	
	/* urlを初期化 */
	scheme_delimiter = "://";
	copy_string(&_url, url);
	if(strstr(_url, scheme_delimiter) == NULL) { /* :// がURLに含まれなかった場合の処理 */
		_url = malloc(sizeof(char) * (strlen(url) + strlen(scheme_delimiter) + 1));
		sprintf(_url, "%s%s", scheme_delimiter, url);
	}
	
	/* 正規表現でパース */
	err = regcomp(&preg, "^([^:/?#]*)://([^:/?#]+):*([0-9]*)([^?#]*)[?]*([^#]*)#*(.*)$", REG_EXTENDED); //^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?
	if (err != 0) {
		_return = -1;
		goto EXIT;
	}
	
	if (regexec(&preg, _url, nmatch, pmatch, 0) != 0) {
		
		_return = -1;
		goto EXIT;
		
	} else {
		
		for (i = 1; i < nmatch; i++) {
						
			if(pmatch[0].rm_so >= 0 && pmatch[i].rm_eo >= 0) {
				
				len = pmatch[i].rm_eo-pmatch[i].rm_so;
				result[i] = malloc(sizeof(char) * (len+1));
				strncpy(result[i], &_url[pmatch[i].rm_so], len);
				result[i][len] = '\0';
			
			} else {
			
				result[i] =  malloc(sizeof(char));
				result[i] = '\0';
				
			}
			
		}
		
	}
	
	/* パース結果を格納 */
	*res = malloc(sizeof(struct urlinfo));
	response = *res;
	copy_string(&response->url, url);
	response->scheme = result[1];
	response->host = result[2];
	response->port = result[3];
	response->path = result[4];
	response->query_string = result[5];
	response->fragment = result[6];
	response->queries = NULL;

	/* クエリーの解析 */
	if (strcmp(response->query_string, "") != 0) {
		
		/* 初期化 */
		response->queries = malloc(sizeof(struct queries));
		
		/* クエリー数の設定 */
		response->queries->length = split_string (response->query_string, "&", &query_splitedtext);
		response->queries->parameters = malloc(sizeof(struct query) * response->queries->length);
		
		/* 解析結果を格納 */
		for (i = 0, _query_splitedtext = query_splitedtext; _query_splitedtext != NULL; i++, _query_splitedtext = _query_splitedtext->next) {
		
			split_string(_query_splitedtext->string, "=", &query_key_value);
			
			/* key */
			copy_string(&response->queries->parameters[i].key, query_key_value->string);
			
			/* value */
			if (query_key_value->next != NULL) {
				copy_string(&response->queries->parameters[i].value, query_key_value->next->string);
			} else {
				response->queries->parameters[i].value = malloc(sizeof(char));
				response->queries->parameters[i].value[0] = '\0';
			}
						
			freesplitedtext(query_key_value);
			
		}
		
		freesplitedtext(query_splitedtext);

	}
	
	/* 終了処理 */
	EXIT:
	regfree(&preg);
	free(_url);
	return _return;

}

/**
 * urlinfoのメモリ領域を開放する
 * @param res 開放対象のurlinfo構造体
 */
void free_urlinfo(struct urlinfo **res)
{
	int i;
	
	free((*res)->url);
	free((*res)->scheme);
	free((*res)->host);
	free((*res)->port);
	free((*res)->path);
	free((*res)->query_string);
	free((*res)->fragment);
	
	/* query */
	if ((*res)->queries != NULL) {
		for (i = 0; i < (*res)->queries->length; i++) {
			free((*res)->queries->parameters[i].key);
			free((*res)->queries->parameters[i].value);
			free((*res)->queries);
		}
		free((*res)->queries);
	}
	
	free(*res);
	
}
