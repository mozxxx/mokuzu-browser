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
#include "urlencode.h"
#include "../general/string.h"

static int _parse_url(const char *_url, struct urlinfo *response);

/**
 * URLパーサ
 * @param url パース対象のURL文字列
 * @param res パース結果が格納されているurlinfo構造体
 * @return 成功すれば0、失敗したら-1
 */
int parse_url(const char *source_url, struct urlinfo **res)
{
	
	char *url, *__url, *_url, *scheme_delimiter, *_scheme;
	struct splitedtext *splitedurl;
	int _return = -1;
	
	url = encode_url(source_url); //URLエンコード
	
	*res = malloc(sizeof(struct urlinfo));
	copy_string(&(*res)->url, url);
	
	/* urlの前処理 */
	scheme_delimiter = "://";
	
	if (split_string(url, scheme_delimiter, &splitedurl) == 1 ) {
		copy_string(&__url, splitedurl->string);
		_scheme = "";
	} else {
		copy_string(&__url, splitedurl->next->string);
		_scheme = splitedurl->string;
	}
	
	if (strstr(__url, "@") == NULL) {
		scheme_delimiter = "://:@";
	}
		
	_url = malloc(sizeof(char) * (strlen(_scheme) + strlen(scheme_delimiter) + strlen(__url) + 1));
	sprintf(_url, "%s%s%s", _scheme, scheme_delimiter, __url);
		
	freesplitedtext(splitedurl);
	free(__url);
		
	/* URLをパース */
	_return = _parse_url(_url, *res);
	
	/* 終了処理 */
	free(url);
	free(_url);
	
	return _return;

}

/**
 * 正規表現でURLをパースする
 * @param 
 * @return 成功=0, 不成功=-1
 */
static int _parse_url(const char *_url, struct urlinfo *response)
{

	regex_t preg;
	size_t nmatch = 9, len;
	regmatch_t pmatch[nmatch];
	char *result[nmatch];
	int err, i, _return = -1;
	
	err = regcomp(&preg, "^([^:/?#]*)://([^:/?#@]*):*([^:/?#@]*)@([^:/?#]+):*([0-9]*)([^?#]*)[?]*([^#]*)#*(.*)$", REG_EXTENDED);
		
	if (err == 0 && regexec(&preg, _url, nmatch, pmatch, 0) == 0) {
		
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
		
		/* パース結果を格納 */
		response->scheme = result[1];
		response->user = result[2];
		response->password = result[3];
		response->host = result[4];
		response->port = result[5];
		response->path = result[6];
		response->query_string = result[7];
		response->fragment = result[8];
		response->queries = NULL;

		/* クエリーの解析 */
		if (strcmp(response->query_string, "") != 0) {
			parse_query(response->query_string, &response->queries);
		}
		
		_return = 0;
		
	}
	
	regfree(&preg);
	return _return;
	
}

/**
 * URLクエリーパーサ
 * @param query_string パース対象のURLクエリー
 * @param res パース結果
 */
void parse_query(const char *query_string, struct queries **res)
{
	
	struct queries *response;
	struct splitedtext *query_splitedtext, *_query_splitedtext, *query_key_value;
	int i;
		
	/* 初期化 */
	*res = malloc(sizeof(struct queries));
	response = *res;
	
	/* クエリー数の設定 */
	response->length = split_string (query_string, "&", &query_splitedtext);
	response->parameters = malloc(sizeof(struct query) * response->length);
	
	/* 解析結果を格納 */
	for (i = 0, _query_splitedtext = query_splitedtext; _query_splitedtext != NULL; i++, _query_splitedtext = _query_splitedtext->next) {
	
		split_string(_query_splitedtext->string, "=", &query_key_value);
		
		/* key */
		copy_string(&response->parameters[i].key, query_key_value->string);
		
		/* value */
		if (query_key_value->next != NULL) {
			copy_string(&response->parameters[i].value, query_key_value->next->string);
		} else {
			response->parameters[i].value = malloc(sizeof(char));
			response->parameters[i].value[0] = '\0';
		}
					
		freesplitedtext(query_key_value);
		
	}
	
	freesplitedtext(query_splitedtext);
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
	free((*res)->user);
	free((*res)->password);
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
