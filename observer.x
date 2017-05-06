
const MAX_PROCESSES  = 5;

const REPORT_SNDMSG = 1;
const REPORT_SNDREQ = 2;
const REPORT_SNDREP = 3;
const REPORT_SNDREL = 4;
const REPORT_RCVREQ = 5;
const REPORT_RCVREP = 6;
const REPORT_RCVREL = 7;
const REPORT_RCVMSG = 8;
const REPORT_LCLACT = 9;

struct stamp {
    unsigned int proccess_id;
    unsigned int action_number;
};

typedef struct stamp stamp;

struct wakeup_response{
    unsigned int process_id;
             int errno;
};

typedef struct wakeup_response wakeup_reponse;

struct sndmsg_response {
    stamp process[MAX_PROCESSES];
    unsigned int stamp_number;
};

typedef struct sndmsg_response sndmsg_response;

struct action_report {
    stamp process_stamp;
    int action_type;
};

typedef struct action_report action_report;


program OBSERVER{
    version FIRST_VERSION{
        wakeup_response wakeup_request() = 0;
        
        sndmsg_response sndmsg_request() = 1;

        sndmsg_response report_action(action_report) = 2;
    }
    = 1;
}
= 0x20000001;