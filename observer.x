
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
    stamp process[5];
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