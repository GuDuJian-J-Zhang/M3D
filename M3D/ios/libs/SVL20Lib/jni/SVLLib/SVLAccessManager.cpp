#include "SVLAccessManager.h"

// This is to work around the Turkish locale issue, i.e.,
// toupper('i') != toupper('I') if locale is "tr_TR"
char uppercase(char c) {
	if ('a' <= c && c <= 'z') {
		return c + ('A' - 'a');
	}
	return c;
}

/**
* Match a name with a pattern. The pattern may include wildcard. A single
* wildcard "*" can match up to one component in the domain name.
*
* @param  host    Host name, typically the name of the remote host
* @param  pattern Name retrieved from certificate
* @param  size    Size of "pattern"
* @return True, if "host" matches "pattern". False otherwise.
*/
bool matchName(const char* host, const char* pattern, int size) {
	bool match = false;
	int i = 0, j = 0;
	while (i < size && host[j] != '\0') {
		if (uppercase(pattern[i]) == uppercase(host[j])) {
			i++;
			j++;
			continue;
		}
		if (pattern[i] == '*') {
			while (host[j] != '.' && host[j] != '\0') {
				j++;
			}
			i++;
			continue;
		}
		break;
	}
	if (i == size && host[j] == '\0') {
		match = true;
	}
	return match;
}


SVLAccessManager::~SVLAccessManager()
{
}

/**
* Default implementation of AccessManager
*/
Decision SVLAccessManager::verify(const sockaddr_storage& sa) throw() {
	(void)sa;
	//return SKIP;
	return ALLOW;
}

Decision SVLAccessManager::verify(const string& host,
	const char* name,
	int size) throw() 
{
	return ALLOW;
#if 0
	if (host.empty() || name == NULL || size <= 0) {
		return SKIP;
	}
	return (matchName(host.c_str(), name, size) ? ALLOW : SKIP);
#endif
}

Decision SVLAccessManager::verify(const sockaddr_storage& sa,
	const char* data,
	int size) throw() {
	return ALLOW;

#if 0
	bool match = false;
	if (sa.ss_family == AF_INET && size == sizeof(in_addr)) {
		match = (memcmp(&((sockaddr_in*)&sa)->sin_addr, data, size) == 0);
	}
	else if (sa.ss_family == AF_INET6 && size == sizeof(in6_addr)) {
		match = (memcmp(&((sockaddr_in6*)&sa)->sin6_addr, data, size) == 0);
	}
	return (match ? ALLOW : SKIP);
#endif
}

