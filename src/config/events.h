#ifndef EVENTS_H_
#define EVENTS_H_

enum {
    EVNT_OPEN_DOOR,
    EVNT_CLOSE_DOOR,
    EVNT_PUTDOWN_PHONE,
    EVNT_DOMOPHONE_WAIT,
    CNT_EVENTS  //count of registered events
};

#define   MEASURE_PERIOD                  20   //ms
#define   EVENT_DONE                      0
#define   EVENT_CLR                       -1

#endif //EVENTS_H_
