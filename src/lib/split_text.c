/**
 * $Id$
 * 文字列分割用ライブラリ
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
#include <stdlib.h>
#include "split_text.h"

/**
 * テキストを分割して連結リストに格納する
 * @param string 分割対象テキスト
 * @param delimiter 区切り文字（デリミタ）
 * @param res 分割結果が格納された連結リストの最初のノード
 * @return 分割した回数を返す（分割されなかった場合は０を返す）
 */
int split_text (char const *string, char const *delimiter, struct splitedtext **res)
{
	
	char *chunk;
	int	counter,
		current_index,
		search_index,
		len;
	struct	splitedtext	*current_splitedtext,
							*prev;
	
	/* chunkを空文字で初期化 */
	chunk = malloc(sizeof(char));
	chunk[0] = '\0';
	
	/* resを初期化 */
	*res = malloc(sizeof(struct splitedtext));
	(*res)->string = chunk;
	(*res)->next = NULL;
	
	current_splitedtext = *res; /* current_splitedtextをresで初期化 */
	prev = NULL;
	
	/* 各変数を初期化 */
	counter = 0;
	current_index = 0;
	search_index = 0;
	
	while (search_index != -1) { /* 探索点以降にデリミタが見つからなかった場合は分割を終了 */
		
		/* 探索点を起点としてデリミタの位置を検索 */
		search_index = search_delimiter(&string[current_index], delimiter);
		
		/* 探索点にデリミタがあった場合は分割せずスルー */
		if (search_index != 0) {
				
			/* 探索点からデリミタが見つかった点までの長さを算出：デリミタが探索点以降見つからなかった場合は残りの文字列の長さ */	
			len = (search_index == -1)? strlen(string) - current_index : search_index;
			
			/* 探索点からデリミタが見つかった地点までの文字列を抜き出して、
			 * 構造体に代入
			 */
			chunk = malloc(sizeof(char) * (len + 1));
			strncpy(chunk, &string[current_index], len);
			chunk[len] = '\0'; /* 抜き出した文字列の最後がNULL文字であるとは限らないため、NULL文字を追加。 */
			current_splitedtext->string = chunk;
			current_splitedtext->next = NULL;
			
			if(prev != NULL) {
				prev->next = current_splitedtext; /* 一つ前のノードに連結 */
			}
			
			/* 新しいノードを作成 */
			prev = current_splitedtext;
			current_splitedtext = malloc(sizeof(struct splitedtext));
			
			/* 分割した数を記録 */
			counter++;
		
		}
		
		/* デリミタが見つかった位置の後に探索点を移動 */
		current_index += search_index + strlen(delimiter);
		
		/* 探索点が文字長を超えた場合は分割を終了 */
		if(current_index >= strlen(string)) {
			break;
		}
		
	}
	
	return counter; /* 分割数を返す */
	
}

/**
 * リンクリスト res に対して動的に割り当てられたメモリを解放する
 * @param res 開放対象のsplitedtext構造体
 */
int freesplitedtext (struct splitedtext *res) {
	
	struct splitedtext *pr, *deleted;
	pr = res;
	
	do {
		free(pr->string);
		
		deleted = pr;
		pr=deleted->next;
		
		free(deleted);
	} while (pr != NULL);
	
	return 0;
	
}

/**
 * 区切り文字の出現ポイントを返す
 * @param string 対象テキスト
 * @param delimiter 区切り文字
 * @return 区切り文字の出現ポイント
 */
int search_delimiter (const char *string, const char *delimiter)
{
	
	char *pointer;
	int index;
	
	pointer = strstr(string, delimiter);
	index = (pointer == NULL)? -1 : pointer-string;
	
	return index;
	
}
