/*
 Copyright (C) 2012-2014 Soomla Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

// Created by Fedor Shubin on 5/19/13.

#include "CCUpgradeVG.h"

USING_NS_CC;

namespace soomla {
    CCUpgradeVG *CCUpgradeVG::create(__String *goodItemId, __String *prevItemId, __String *nextItemId, __String *name, __String *description, __String *itemId, CCPurchaseType *purchaseType) {
        CCUpgradeVG *ret = new CCUpgradeVG();
        if (ret->init(goodItemId, prevItemId, nextItemId, name, description, itemId, purchaseType)) {
            ret->autorelease();
        }
        else {
            CC_SAFE_DELETE(ret);
        }

        return ret;
    }

    bool CCUpgradeVG::init(__String *goodItemId, __String *prevItemId, __String *nextItemId, __String *name, __String *description, __String *itemId, CCPurchaseType *purchaseType) {
        bool res = CCVirtualGood::init(name, description, itemId, purchaseType);
        if (res) {
            setGoodItemId(goodItemId);
            setPrevItemId(prevItemId);
            setNextItemId(nextItemId);
            return true;
        } else {
            return false;
        }
    }

    bool CCUpgradeVG::initWithDictionary(__Dictionary *dict) {
        bool res = CCVirtualGood::initWithDictionary(dict);
        if (res) {
            fillGoodItemIdFromDict(dict);
            fillPrevItemIdFromDict(dict);
            fillNextItemIdFromDict(dict);
            return true;
        } else {
            return false;
        }
    }

    __Dictionary *CCUpgradeVG::toDictionary() {
        __Dictionary *dict = CCVirtualGood::toDictionary();

        putGoodItemIdToDict(dict);
        if (mPrevItemId != NULL) {
            putPrevItemIdToDict(dict);
        } else {
            dict->setObject(__String::create(""), CCStoreConsts::JSON_VGU_PREV_ITEM_ID);
        }
        if (mNextItemId != NULL) {
            putNextItemIdToDict(dict);
        } else {
            dict->setObject(__String::create(""), CCStoreConsts::JSON_VGU_NEXT_ITEM_ID);
        }

        return dict;
    }


    const char *CCUpgradeVG::getType() const {
        return CCStoreConsts::JSON_JSON_TYPE_UPGRADE_VG;
    }

    CCUpgradeVG::~CCUpgradeVG() {
        CC_SAFE_RELEASE(mGoodItemId);
        CC_SAFE_RELEASE(mPrevItemId);
        CC_SAFE_RELEASE(mNextItemId);
    }
}
