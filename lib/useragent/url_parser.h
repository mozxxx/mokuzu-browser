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

/**
 * パースしたURL情報格納用構造体
 */
struct urlinfo {
    char *url;
    char *scheme;
    char *user;
    char *password;
    char *host;
    char *port;
    char *path;
    char *query_string;
    char *fragment;
    struct queries *queries;
};

/**
 * クエリーパラメーター格納用構造体
 */
struct queries {
	int length; /* クエリー数 */
	struct query *parameters; /* クエリー配列 */
};

/**
 * クエリー key value 構造体
 */
struct query {
    char *key;
    char *value;
};

/**
 * URLパーサ
 * @param url パース対象のURL文字列
 * @param res パース結果が格納されているurlinfo構造体
 * @return 成功すれば0、失敗したら-1
 */
int parse_url(const char * url, struct urlinfo **res);

/**
 * urlinfoのメモリ領域を開放する
 * @param res 開放対象のurlinfo構造体
 */
 void free_urlinfo(struct urlinfo **res);

/**
 * URLクエリーパーサ
 * @param query_string パース対象のURLクエリー
 * @param res パース結果
 */
void parse_query(const char *query_string, struct queries **res);
