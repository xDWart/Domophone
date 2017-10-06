#include "config/include.h"

volatile int32_t Events[CNT_EVENTS];

Ticker timer1(RunEvents, MEASURE_PERIOD);

void eventsSetup(){
  ClrEvents();
  timer1.start();
}

void eventsloop(){
    timer1.update();

    if (TestEvent_Done(EVNT_OPEN_DOOR)) {
        OPEN();
    }

    if (TestEvent_Done(EVNT_CLOSE_DOOR)) {
        CLOSE();
    }

    if (TestEvent_Done(EVNT_PUTDOWN_PHONE)) {
        PICKUP_PHONE(false);
        CLOSE();
        DOMOPHONE(Var[SOUND].st); //подключить трубку, если была включена
        SetEvent(EVNT_DOMOPHONE_WAIT,1000);
    }

    if (TestEvent_Done(EVNT_DOMOPHONE_WAIT)) {
        Serial.println("Calling status false");
        setStatusCall(ST_WAITING);
        pubStatus(Var[CALLING].sTopic, Var[CALLING].stat);
        DomophoneStatus = WAIT;
    }
}

void RunEvents(){
  for (uint8_t i=0; i<CNT_EVENTS;i++){
    if (Events[i]>EVENT_DONE) Events[i]--;
  }
}

void ClrEvents(){
  for (uint8_t i=0; i<CNT_EVENTS;i++){
    Events[i] = EVENT_CLR;
  }
}

void SetEvent(uint8_t event_id){
  SetEvent(event_id,0);
}

void SetEvent(uint8_t event_id, uint32_t msec){
  Events[event_id] = msec/MEASURE_PERIOD;
}

bool TestEvent_Done(uint8_t event_id){
  if (Events[event_id] == EVENT_DONE){
    Events[event_id] = EVENT_CLR;
    return true;
  }else{
    return false;
  }
}

bool TestEvent(uint8_t event_id){
  if (Events[event_id] == EVENT_DONE){
    return true;
  }else{
    return false;
  }
}

bool TestEvent_Clr(uint8_t event_id){
  if (Events[event_id] <= EVENT_CLR){
    return true;
  }else{
    return false;
  }
}

void ClrEvent(uint8_t event_id){
  Events[event_id] = EVENT_CLR;
}

int32_t GetEventTime(uint8_t event_id)
{
  return Events[event_id]*MEASURE_PERIOD;
}
