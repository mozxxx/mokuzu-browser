/**
 * HTTP Client
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
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define  BUF_LEN  1024

int main(int argc, char *argv[])
{

	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int error, sfd;
	char *host, *err_mess, buf[BUF_LEN];
	size_t len;
	ssize_t n;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s hostname\n", argv[0]);
		return 1;
	}

	//host情報を代入
	host = argv[1];
	
	//hintsを初期化
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; //IPv4とIPv6どちらでもOK
	hints.ai_socktype = SOCK_STREAM; //TCP
	hints.ai_flags = 0;
	hints.ai_protocol = 0; //Any protocol

	//名前解決
	error = getaddrinfo(host, "http", &hints, &result);

	//エラー処理
	if (error != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
		return 1;
	}

	//ホストに接続
	for (rp = result; rp != NULL; rp = rp->ai_next) {

		//ソケットを生成
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		//ソケットエラー処理
		if (sfd == -1) {
			//ソケットが生成できなかった場合,次のアドレスでリトライ
			err_mess = "socket";
			continue;
		}

		//ホストに接続
		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
			//接続できた場合はループを抜ける
			break;		
		}

		err_mess = "connect";
		//接続できかなった場合はソケットを閉じて次のアドレスでリトライ
		close(sfd);

	}

	//すべてのアドレスで接続に成功しなかった場合は終了
	if (rp == NULL) {
		fprintf(stderr, "error : %s\n", err_mess);
		return 1;
	}
	
	//resultのメモリ領域を開放
	freeaddrinfo(result);

	//送信内容を作成
	snprintf(buf, sizeof(buf), "GET / HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n", host);
	
	//リクエストを送信
	len = strlen(buf);
	error = write(sfd, buf, len);
	if(error < len) {
		fprintf(stderr, "error : HTTP");
		close(sfd);
		return 1;
	}
	
	//バッファを空に
	memset(buf, 0, sizeof(buf));
	
	//レスポンスを受信して出力
	do {
		//バッファ分読み込む
		n = read(sfd, buf, sizeof(buf));		
		
		//標準出力に表示
		if(write(STDOUT_FILENO, buf, n) < 0) {
			perror("write STDOUT");
			return 1;
		}
		
	} while(n > 0);
	
	//エラー処理
	if(n == -1) {
		perror("");
		return 1;
	}
	
	close(sfd);

	return 0;

}

