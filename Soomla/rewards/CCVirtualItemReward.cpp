//
// Created by Fedor on 04/06/14.
//

#include "CCVirtualItemReward.h"
#include "CCStoreConsts.h"
#include "CCStoreInventory.h"
#include "CCSoomlaUtils.h"

#define TAG "SOOMLA VirtualItemReward"

soomla::CCVirtualItemReward *soomla::CCVirtualItemReward::create(
        cocos2d::CCString *rewardId,
        cocos2d::CCString *name,
        cocos2d::CCInteger *amount,
        cocos2d::CCString *associatedItemId) {

    CCVirtualItemReward *ret = new CCVirtualItemReward();
    if (ret->init(rewardId, name, associatedItemId, amount)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool soomla::CCVirtualItemReward::init(
        cocos2d::CCString *rewardId,
        cocos2d::CCString *name,
        cocos2d::CCString *associatedItemId,
        cocos2d::CCInteger *amount
) {

    bool result = CCReward::init(rewardId, name);

    if (result) {
        setAmount(amount);
        setAssociatedItemId(associatedItemId);
        return true;
    } else {
        return false;
    }
}

bool soomla::CCVirtualItemReward::initWithDictionary(cocos2d::CCDictionary *dict) {
    CCReward::initWithDictionary(dict);

    fillAmountFromDict(dict);
    fillAssociatedItemIdFromDict(dict);

    return true;
}

cocos2d::CCDictionary *soomla::CCVirtualItemReward::toDictionary() {
    cocos2d::CCDictionary *dict = CCReward::toDictionary();

    putAmountToDict(dict);
    putAssociatedItemIdToDict(dict);

    return dict;
}

soomla::CCVirtualItemReward::~CCVirtualItemReward() {
    CC_SAFE_RELEASE(mAmount);
    CC_SAFE_RELEASE(mAssociatedItemId);
}

const char *soomla::CCVirtualItemReward::getType() const {
    return CCStoreConsts::JSON_JSON_TYPE_ITEM;
}

bool soomla::CCVirtualItemReward::takeInner() {
    CCError *error = NULL;
    char const *itemId = this->getAssociatedItemId()->getCString();
    CCStoreInventory::sharedStoreInventory()->takeItem(itemId, this->getAmount()->getValue(), &error);
    if (error) {
        CCSoomlaUtils::logError(TAG,
                cocos2d::CCString::createWithFormat("(take) Couldn't find associated itemId: %s", itemId)->getCString());
        CCSoomlaUtils::logError(TAG, error->getInfo());
        return false;
    }
    return true;
}

bool soomla::CCVirtualItemReward::giveInner() {
    CCError *error = NULL;
    char const *itemId = this->getAssociatedItemId()->getCString();
    CCStoreInventory::sharedStoreInventory()->giveItem(itemId, this->getAmount()->getValue(), &error);
    if (error) {
        CCSoomlaUtils::logError(TAG,
                cocos2d::CCString::createWithFormat("(give) Couldn't find associated itemId: %s", itemId)->getCString());
        CCSoomlaUtils::logError(TAG, error->getInfo());
        return false;
    }
    return true;
}
