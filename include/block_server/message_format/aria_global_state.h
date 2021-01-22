//
// Created by peng on 2021/1/16.
//

#ifndef NEUBLOCKCHAIN_ARIA_GLOBAL_STATE_H
#define NEUBLOCKCHAIN_ARIA_GLOBAL_STATE_H

enum class AriaGlobalState {
    START,  // the init state of aria coordinator
    Aria_READ,      // start the read state of the protocol
    Aria_COMMIT,    // start the commit state of the protocol
    EXIT,            // the coordinator is about to exit
    CLEANUP,
    C_PHASE,
    S_PHASE,
    Analysis,
    Execute,
    AriaFB_READ,
    AriaFB_COMMIT,
    AriaFB_Fallback_Prepare,
    AriaFB_Fallback,
    Bohm_Analysis,
    Bohm_Insert,
    Bohm_Execute,
    Bohm_GC,
    Pwv_Analysis,
    Pwv_Execute,
    RESET
};


#endif //NEUBLOCKCHAIN_ARIA_GLOBAL_STATE_H
