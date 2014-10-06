//
// Created by Fedor on 04/06/14.
//

#ifndef __CCVirtualItemReward_H_
#define __CCVirtualItemReward_H_

#include "CCReward.h"
#include "CCCoreConsts.h"
#include "CCStoreConsts.h"

namespace soomla {
    class CCVirtualItemReward : public CCReward {
        SL_SYNTHESIZE_RETAIN_WITH_DICT(cocos2d::CCInteger *, mAmount, Amount, CCStoreConsts::JSON_AMOUNT);
        SL_SYNTHESIZE_RETAIN_WITH_DICT(cocos2d::CCString *, mAssociatedItemId, AssociatedItemId, CCStoreConsts::JSON_ASSOCIATED_ITEM_ID);
    public:
        static CCVirtualItemReward *create(
                cocos2d::CCString *rewardId,
                cocos2d::CCString *name,
                cocos2d::CCInteger * amount,
                cocos2d::CCString * associatedItemId
        );

        SL_CREATE_WITH_DICTIONARY(CCVirtualItemReward);

        virtual bool init(
                cocos2d::CCString *rewardId,
                cocos2d::CCString *name,
                cocos2d::CCString *associatedItemId,
                cocos2d::CCInteger *amount
        );

        virtual bool initWithDictionary(cocos2d::CCDictionary *dict);

        virtual cocos2d::CCDictionary *toDictionary();

        virtual ~CCVirtualItemReward();

        virtual const char *getType() const;


    protected:
        virtual bool takeInner();

        virtual bool giveInner();
    };
};

#endif //__CCVirtualItemReward_H_
