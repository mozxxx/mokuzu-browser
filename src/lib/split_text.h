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

/**
 * @struct 分割された文字列を格納するための連結リスト用構造体
 * 分割された数分ノードが連結されます
 */
struct splitedtext {
    char *string;				/* 分割された文字列 */
    struct splitedtext *next;	/* 次のノード */
};

/**
 * テキストを分割して連結リストに格納する
 * @param string 分割対象テキスト
 * @param delimiter 区切り文字（デリミタ）
 * @param res 分割結果を格納するための連結リスト
 * @return 分割した回数を返す（分割されなかった場合は０を返す）
 */
int split_text (const char *string, const char *delimiter, struct splitedtext **res);

/**
 * リンクリスト res に対して動的に割り当てられたメモリを解放する
 * @param res 開放対象のsplitedtext構造体
 */
int freesplitedtext (struct splitedtext *res);

/**
 * 区切り文字の出現ポイントを返す
 * @param string 対象テキスト
 * @param delimiter 区切り文字
 * @return 区切り文字の出現ポイント（区切り文字が見つからなかった場合は -1 を返す）
 */
int search_delimiter (const char *string, const char *delimiter);
