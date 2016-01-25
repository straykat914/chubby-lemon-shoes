#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#define _WIN32_WINNT 0x501

#include <ws2tcpip.h>
#include <windows.h>

#elif __linux__

#include <sys/socket.h>
// getaddrinfo
#include <netdb.h>
#endif

#include "bot.h"

/*
 * function to initlialise the
 * account struct
 */
pAccount new_account (void) {
	pAccount a = malloc (sizeof (*a));
	if (a == NULL) {
		fatal ("[!] Account struct malloc", POSIX);
	}

	memset (a->u_name, 0, MAX_NAME);
	memset (a->n_name, 0, MAX_NAME);
	a->pword = NULL;
	a->channel = NULL;
	return a;
}

/*
 * function to initialise the
 * message struct
 * default sizes are
 * DEFAULT_MALLOC_SIZE (2048)
 */
pMessage new_message (void) {
	pMessage m = malloc (sizeof (*m));
	if (m == NULL) {
		return m;
	}
	
	memset (m->n_name, 0, MAX_NAME);

	m->contact = malloc (DEFAULT_MALLOC_SIZE);
	if (m->contact == NULL) {
		return NULL;
	}
	m->contactSize = DEFAULT_MALLOC_SIZE;

	m->command = malloc (DEFAULT_MALLOC_SIZE);
	if (m->command == NULL) {
		return NULL;
	}
	m->commandSize = DEFAULT_MALLOC_SIZE;

	m->param = malloc (DEFAULT_MALLOC_SIZE);
	if (m->param == NULL) {
		return NULL;
	}
	m->paramSize = DEFAULT_MALLOC_SIZE;

	m->msg = malloc (DEFAULT_MALLOC_SIZE);
	if (m->msg == NULL) {
		return NULL;
	}
	m->msgSize = DEFAULT_MALLOC_SIZE;

	return m;
}

/*
 * function to create a 
 * connection to the IRC
 * server, returns socket
 */
SOCKET irc_connect (char *addr, char *port) {
    SOCKET s;
    int gai;
    struct addrinfo hints, *res;
    memset (&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    #ifdef _WIN32
    WSADATA wsa;
    
    if (WSAStartup (MAKEWORD (2, 2), &wsa)) {
        fatal ("WSAStartup");
    }
    #endif
    
    gai = getaddrinfo (addr, port, &hints, &res);
    if (gai) {
        fprintf(stderr, "[!] Get info error: %s\n", gai_strerror (gai));
        exit (EXIT_FAILURE);
    }
    
    s = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s != 0) {
        fatal ("Socket");
    }
    
    puts ("[*] Connecting...");
    if (connect (s, res->ai_addr, res->ai_addrlen) == -1) {
        fatal ("Connect");
    }
    
    return s;
}

/*
 * set up bot's nickname and 
 * username with the IRC server
 */
int setup_irc (SOCKET s, pAccount account) {
    // need to remove magic numbers
    char set_nick[strlen(account->n_name) + 9];
    char set_user[strlen(account->u_name) + 21];

    snprintf (set_nick, sizeof (set_nick), "NICK %s\r\n", account->n_name);
    snprintf (set_user, sizeof (set_user), "USER %s 0 * :~bot~\r\n", account->u_name);

    if (send (s, set_nick, strlen (set_nick), 0) == -1) {
        fatal ("Send nickname");
    }

    if (send (s, set_user, strlen (set_user), 0) == -1) {
        fatal ("Send username");
    }

    return EXIT_SUCCESS;
}

/*
 * convert all characters to 
 * lowercase for easier
 * string comparisons
 */
void str_to_lower (char *s) {
	if (s != NULL) {
		int i;
		
		for (i = 0; i < strlen (s); i++) {
			if (isalpha (s[i]) && isupper (s[i])) {
				s[i] = tolower (s[i]);
			}
		}
	}
}

/*
 * null terminates the
 * trailing \r\n
 */
static void trim_string (char *s) {
	if (s != NULL) {
		/*
		int i;

		for (i = 0; i < strlen (s); i++) {
			if (s[i] == '\r' || s[i] == '\n') {
				s[i] = '\0';
				break;
			}
		}
		*/
		str[strlen (str)-2] = '\0';
	}
}

/*
 * modify the strings of
 * the received messages
 * for easier string
 * comparisons
 */
void format_message (pMessage m) {
	str_to_lower (m->n_name);
	str_to_lower (m->contact);
	str_to_lower (m->command);
	str_to_lower (m->param);
	str_to_lower (m->msg);

	trim_string (m->n_name);
	trim_string (m->contact);
	trim_string (m->command);
	trim_string (m->param);
	trim_string (m->msg);
}