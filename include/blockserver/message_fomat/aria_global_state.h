//
// Created by peng on 2021/1/16.
//

#ifndef NEUBLOCKCHAIN_ARIA_GLOBAL_STATE_H
#define NEUBLOCKCHAIN_ARIA_GLOBAL_STATE_H

enum class AriaGlobalState {
    START,  // AriaCoordinator 的最初始状态
    Aria_READ,      // 开启 AriaREAD协议 阶段
    Aria_COMMIT,    // 开启 AriaCommit协议 阶段
    EXIT,            // 退出状态
};


#endif //NEUBLOCKCHAIN_ARIA_GLOBAL_STATE_H
