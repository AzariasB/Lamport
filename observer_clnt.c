/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "observer.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

wakeup_response *
wakeup_request_1(void *argp, CLIENT *clnt)
{
	static wakeup_response clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, wakeup_request,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_wakeup_response, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

sndmsg_response *
sndmsg_request_1(void *argp, CLIENT *clnt)
{
	static sndmsg_response clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, sndmsg_request,
		(xdrproc_t) xdr_void, (caddr_t) argp,
		(xdrproc_t) xdr_sndmsg_response, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

void *
report_action_1(action_report *argp, CLIENT *clnt)
{
	static char clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, report_action,
		(xdrproc_t) xdr_action_report, (caddr_t) argp,
		(xdrproc_t) xdr_void, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return ((void *)&clnt_res);
}