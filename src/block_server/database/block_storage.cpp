
//
// Created by lhx on 2/11/22.
//

#include "blockserver/database/block_storage.h"
#include "blockserver/database/impl/block_storage_to_file.h"


BlockStorage *BlockStorage::getInstance() {
    static auto* self = new BlockStorageToFile();
    return self;
}