/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _OBSERVER_H_RPCGEN
#define _OBSERVER_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PROCESSES 5

struct stamp {
	u_int proccess_id;
	u_int action_number;
};
typedef struct stamp stamp;


struct wakeup_response {
	u_int process_id;
	int errno;
};
typedef struct wakeup_response wakeup_response;

typedef wakeup_response wakeup_reponse;

struct sndmsg_response {
	stamp process[MAX_PROCESSES];
	u_int stamp_number;
};
typedef struct sndmsg_response sndmsg_response;


struct action_report {
	stamp process_stamp;
	int action_type;
};
typedef struct action_report action_report;


#define OBSERVER 0x20000001
#define FIRST_VERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define wakeup_request 0
extern  wakeup_response * wakeup_request_1(void *, CLIENT *);
extern  wakeup_response * wakeup_request_1_svc(void *, struct svc_req *);
#define sndmsg_request 1
extern  sndmsg_response * sndmsg_request_1(void *, CLIENT *);
extern  sndmsg_response * sndmsg_request_1_svc(void *, struct svc_req *);
#define report_action 2
extern  sndmsg_response * report_action_1(action_report *, CLIENT *);
extern  sndmsg_response * report_action_1_svc(action_report *, struct svc_req *);
extern int observer_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define wakeup_request 0
extern  wakeup_response * wakeup_request_1();
extern  wakeup_response * wakeup_request_1_svc();
#define sndmsg_request 1
extern  sndmsg_response * sndmsg_request_1();
extern  sndmsg_response * sndmsg_request_1_svc();
#define report_action 2
extern  sndmsg_response * report_action_1();
extern  sndmsg_response * report_action_1_svc();
extern int observer_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_stamp (XDR *, stamp*);
extern  bool_t xdr_stamp (XDR *, stamp*);
extern  bool_t xdr_wakeup_response (XDR *, wakeup_response*);
extern  bool_t xdr_wakeup_reponse (XDR *, wakeup_reponse*);
extern  bool_t xdr_sndmsg_response (XDR *, sndmsg_response*);
extern  bool_t xdr_sndmsg_response (XDR *, sndmsg_response*);
extern  bool_t xdr_action_report (XDR *, action_report*);
extern  bool_t xdr_action_report (XDR *, action_report*);

#else /* K&R C */
extern bool_t xdr_stamp ();
extern bool_t xdr_stamp ();
extern bool_t xdr_wakeup_response ();
extern bool_t xdr_wakeup_reponse ();
extern bool_t xdr_sndmsg_response ();
extern bool_t xdr_sndmsg_response ();
extern bool_t xdr_action_report ();
extern bool_t xdr_action_report ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_OBSERVER_H_RPCGEN */
