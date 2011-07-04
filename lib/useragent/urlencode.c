/**
 * URL Encode
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

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "urlencode.h"

/**
 * 渡された文字列をURLエンコードに変換する（UTF-8限定）
 * @param string 変換対象文字列
 * @return 変換後文字列
 */
char * encode_url(char *string)
{
	size_t	i,
			src_len = strlen(string),
			encoded_string_size = sizeof(char) * (src_len + 1) * 3, /* URLエンコーディング後の予想サイズ */
			encoded_string_len;
			
	char	character,
			*encoded_string = malloc(encoded_string_size), /* URLエンコーディング後の予想サイズを確保 */
			encoded_char[4],
			hex[] = { '0', '1', '2', '3', '4', '5', '6', '7','8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	
	strcpy(encoded_string, "");
	encoded_string_len = strlen(encoded_string);
	
	/* 変換作業 */
	for (i = 0; i < src_len; i++) {
		
		character = string[i];
		
		/* URL予約文字か非予約文字でなかったらURLエンコードに変換 */
		if (is_url_unreserved_character(character) || is_url_reserved_character(character)) {
			
			encoded_char[0] = character;
			encoded_char[1] = '\0';
			
		} else {
			
			encoded_char[0] = '%';
			encoded_char[1] = hex[(unsigned char) character >> 4];
			encoded_char[2] = hex[(unsigned char) character & 0x0f];
			encoded_char[3] = '\0';

		}
		
		/* 確保済み領域が足りなくなった場合の処理 */
		/* TODO: これ必要ないかもしれないので、あとで調べる */
		encoded_string_len += strlen(encoded_char);
		if(encoded_string_size < sizeof(char) * (encoded_string_len + 1)) {
			
			encoded_string_size = sizeof(char) * (encoded_string_len + 1);
			encoded_string = realloc(encoded_string, encoded_string_size);
			
			if (encoded_string == NULL) {
				return NULL;
			}
			
		}
		
		sprintf(encoded_string, "%s%s", encoded_string, encoded_char);
		
	}
	
			
	/* 確保した領域サイズを最適化 */
	encoded_string = realloc(encoded_string, sizeof(char) * (strlen(encoded_string) + 1));
	
	return encoded_string;
	
}

/**
 * URL非予約文字かどうか調べる
 * @param character 調査対象文字
 * @return 調査対象文字がURL非予約文字であれば1を、そうでなければ0を返す
 */
int is_url_unreserved_character(char character)
{
	if (isalnum(character) || character == '-' || character == '.' || character == '~') {
		return 1;
	}
	return 0;
}

/**
 * URL予約文字かどうか調べる
 * @param character 調査対象文字
 * @return 調査対象文字がURL予約文字であれば1を、そうでなければ0を返す
 */
int is_url_reserved_character(char character)
{
	/* TODO: 正規表現が使えないか検討 */
        if (character == '!' || character == '*' || character == '\'' || character == '(' || character == ')' || character == ';' || character == ':' || character == '@' || character == '&' || character == '=' || character == '+' || character == '$' || character == ',' || character == '/' || character == '?' || character == '#' || character == '#' || character == '[' || character == ']') {
                return 1;
        }
        return 0;
}
