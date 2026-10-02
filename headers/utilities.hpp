#pragma once

#define array_size(arr) int(sizeof((arr))/sizeof((arr)[0]))

#define MAXHOUSES 40
#define MAXPLAYERS 5
#define QNTCARDS 16

typedef enum{
    NORMAL,
    START,
    PARK,
    QUESTION_MARK,
    PRISION,
    TELEPORT,
    RAILROAD,
    COMPANY,
    TAXES,
    CHEST
} HSETYPE;

typedef enum{
    COLLECT_MONEY, 
    PAY_MONEY, 
    ESPECIAL_PAY, 
    MOVE_TO, 
    MOVE_BACK, 
    MOVE_AND_RECEIVE, 
    MOVE_NEAREST_RAILROAD, 
    MOVE_NEAREST_COMPANY, 
    GO_TO_JAIL, 
    GET_OUT_OF_JAIL,
    PAY_EACH_PLAYER,
    COLLECT_FROM_EACH_PLAYER, 
    MOVE_RECEIVE_IF 
} CARD_ACTION;

typedef enum{
    BANKRUPT,
    NEED_LIQUIDATION,
    CAN_PAY
} PAYMENT_STATUS;

typedef enum{
    NONE,
    BUY,
    AUCTION
} EVENT_ACTION;