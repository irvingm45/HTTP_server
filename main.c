#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

// ai = address info
struct addrinfo {
	int 			ai_flags;				// AI_PASSIVE, AI_CANONNAME, etc.
	int 			ai_family;				// AF_INET, AF_INET6, AF_UNSPEC
	int 			ai_socktype;			// SOCK_STREAM, SOCK_DGRAM
	int 			ai_protocol;			// use 0 for "any"
	size_t 			ai_addrlen;				// size of ai_addr in bytes
	struct sockaddr *ai_addr;				// struct sockaddr_in or _in6
	char 			*ai_canonname;			// full canonical hostname

	struct addrinfo *ai_next;				// linked list, next node
};

// Both of these structures can be casted to each of them like a sockaddr
// to a sockaddr_in and viceversa
// sa = socket address
struct sockaddr {
	unsigned short sa_family; // address family, AF_xxx
	char sa_data[14];		  // 14 bytes of protocol address
};

// IPv4
struct in_addr {
	uint32_t s_addr; // 32 bit int (4 bytes)
};

// sin = socket internet
struct sockaddr_in {
	short int			sin_family;		// Address family, AF_INET
	unsigned short int	sin_port;		// Port number
	struct in_addr		sin_addr;		// Internet address
	unsigned char		sin_zero[8];	// Same size as struct sockaddr
};

// IPv6
struct in6_addr {
	unsigned char	s6_addr[16];	// IPv6 address
};

struct sockaddr_in6 {
// sin6 = socket internet 6
	uint16_t		sin6_family;	// address family, AF_INET6
	uint16_t		sin6_port;		// port, Network Byte Order
	uint32_t		sin6_flowinfo;	// IPV6 flow information
	struct in6_addr sin6_addr;		// IPv6 address
	uint32_t		sin6_scope_id;	// Scope ID
};

// Struct for both types (ipv4 and 6)
// ss = socket storage
struct sockaddr_storage {
	sa_family_t ss_family; // family of the address

	char		__ss_pad1[_SS_PAD1SIZE];
	int64_t		__ss_align;
	char		__ss_pad2[_SS_PAD2SIZE];
};


// Converting IP addresses from text to binary and viceversa
// ------------------------------------------------------------
struct sockaddr_in sa;	 // IPV4
struct sockaddr_in6 sa6; // IPv6

// inet : internet
// pton : presentation to network
// AF_INET: Address Family: Internet
inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr));
inet_pton(AF_INET6, "2001::db8::63b3:1::3490", &(sa6.sin6_addr));

//IPv4:
// INET_ADDRSTRLEN is defined as 16 bytes (XXX.XXX.XXX.XXX\0)
char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string
struct sockaddr_in sa;      // pretend this is loaded with something

// ntop : network to presentation
inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

printf("The IPv4 address is: %s\n", ip4);


// IPv6:

char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
struct sockaddr_in6 sa6;    // pretend this is loaded with something

inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

printf("The address is: %s\n", ip6);

// NOTE: functionas above only work with numeric IP addresses


// Binding to any address example
// ---------------------------------
struct sockaddr_in sa;
struct sockaddr_in6 sa6;

// We are going to bind to any address
sa.sin_addr.s_addr = INADDR_ANY; // IPv4 equivalent
sa6.sin6_addr = in6addr_any; // IPv6 equivalent

// SYSTEM CALLS or BUST -------
// ----------------------------
// ************************************
// ********getaddrinfo() **************
// ************************************
// returns a pointer to a linked-list

int getaddrinfo(const char *node, 				// e.g. "www.example.com" or IP
				const char *service,			// e.g. "http" or port number
				const struct addrinfo *hints,
				struct addrinfo **res);

// Example
int status;
struct addrinfo hints;
struct addrinfo *servinfo; // points to the result

memset(&hints, 0, sizeof hints); // to ensure that the struct is empty
hints.ai_family = AF_UNSPEC;		// any type
hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets
hints.ai_flags = AI_PASSIVE;		

if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0){
	fprintf(stderr, "gai error: %s\n", gai_strerro(status));
	exit(1);
}

int socket(int domain,		// PF_INET or PF_INET6
			int type,		// SOCK_STREAM or SOCK_DGRAM
			int protocol);	// 0 if you want to choose the proper protocol (tcp, udp)

// ---- Example of socket() using getaddrinfo() ----
int s;
struct addrinfo hints, *res;

getaddrinfo("example.com", "http", &hints, &res);

s = socket(res->ai_family, ai->ai_socktype, ai->ai_protocol)
