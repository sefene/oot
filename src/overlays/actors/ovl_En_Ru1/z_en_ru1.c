/*
 * File: z_en_ru1.c
 * Overlay: En_Ru1
 * Description: Ruto (child)
 */

#include "z_en_ru1.h"
#include "overlays/actors/ovl_Demo_Effect/z_demo_effect.h"

#include "libc64/math64.h"
#include "array_count.h"
#include "gfx.h"
#include "gfx_setupdl.h"
#include "printf.h"
#include "regs.h"
#include "segmented_address.h"
#include "seqcmd.h"
#include "sequence.h"
#include "sfx.h"
#include "sys_matrix.h"
#include "terminal.h"
#include "translation.h"
#include "versions.h"
#include "z_lib.h"
#include "effect.h"
#include "face_reaction.h"
#include "play_state.h"
#include "player.h"
#include "save.h"

#include "assets/objects/object_ru1/object_ru1.h"

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_CAN_PRESS_SWITCHES)

void EnRu1_Init(Actor* thisx, PlayState* play);
void EnRu1_Destroy(Actor* thisx, PlayState* play);
void EnRu1_Update(Actor* thisx, PlayState* play);
void EnRu1_Draw(Actor* thisx, PlayState* play);

void func_80AEC0B4(EnRu1* this, PlayState* play);
void func_80AEC100(EnRu1* this, PlayState* play);
void func_80AEC130(EnRu1* this, PlayState* play);
void func_80AEC17C(EnRu1* this, PlayState* play);
void func_80AEC1D4(EnRu1* this, PlayState* play);
void func_80AEC244(EnRu1* this, PlayState* play);
void func_80AEC2C0(EnRu1* this, PlayState* play);
void func_80AECA94(EnRu1* this, PlayState* play);
void func_80AECAB4(EnRu1* this, PlayState* play);
void func_80AECAD4(EnRu1* this, PlayState* play);
void func_80AECB18(EnRu1* this, PlayState* play);
void func_80AECB60(EnRu1* this, PlayState* play);
void func_80AECBB8(EnRu1* this, PlayState* play);
void func_80AECC1C(EnRu1* this, PlayState* play);
void func_80AECC84(EnRu1* this, PlayState* play);
void func_80AED304(EnRu1* this, PlayState* play);
void func_80AED324(EnRu1* this, PlayState* play);
void func_80AED344(EnRu1* this, PlayState* play);
void func_80AED374(EnRu1* this, PlayState* play);
void func_80AED3A4(EnRu1* this, PlayState* play);
void func_80AED3E0(EnRu1* this, PlayState* play);
void func_80AED414(EnRu1* this, PlayState* play);
void func_80AEF29C(EnRu1* this, PlayState* play);
void func_80AEF2AC(EnRu1* this, PlayState* play);
void func_80AEF2D0(EnRu1* this, PlayState* play);
void func_80AEF354(EnRu1* this, PlayState* play);
void func_80AEF3A8(EnRu1* this, PlayState* play);
void func_80AEEBD4(EnRu1* this, PlayState* play);
void func_80AEEC5C(EnRu1* this, PlayState* play);
void func_80AEECF0(EnRu1* this, PlayState* play);
void func_80AEED58(EnRu1* this, PlayState* play);
void func_80AEEDCC(EnRu1* this, PlayState* play);
void func_80AEEE34(EnRu1* this, PlayState* play);
void func_80AEEE9C(EnRu1* this, PlayState* play);
void func_80AEEF08(EnRu1* this, PlayState* play);
void func_80AEEF5C(EnRu1* this, PlayState* play);
void func_80AEF9D8(EnRu1* this, PlayState* play);
void func_80AEFA2C(EnRu1* this, PlayState* play);
void func_80AEFAAC(EnRu1* this, PlayState* play);
void func_80AEFB04(EnRu1* this, PlayState* play);
void func_80AEFB68(EnRu1* this, PlayState* play);
void func_80AEFCE8(EnRu1* this, PlayState* play);
void func_80AEFBC8(EnRu1* this, PlayState* play);
void func_80AEFC24(EnRu1* this, PlayState* play);
void func_80AEFECC(EnRu1* this, PlayState* play);
void func_80AEFF40(EnRu1* this, PlayState* play);

void EnRu1_PreLimbDraw(EnRu1* this, PlayState* play, s32 limbIndex, Vec3s* rot);

void EnRu1_DrawNothing(EnRu1* this, PlayState* play);
void EnRu1_DrawOpa(EnRu1* this, PlayState* play);
void EnRu1_DrawXlu(EnRu1* this, PlayState* play);

static ColliderCylinderInitType1 sStandingCylinderInit = {
    {
        COL_MATERIAL_HIT0,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_NONE,
        OCELEM_ON,
    },
    { 25, 80, 0, { 0, 0, 0 } },
};

static ColliderCylinderInitType1 sSittingCylinderInit = {
    {
        COL_MATERIAL_HIT0,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000101, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_NONE,
        OCELEM_ON,
    },
    { 20, 30, 0, { 0, 0, 0 } },
};

static void* sEyeTextures[] = {
    gRutoChildEyeOpenTex,     gRutoChildEyeHalfTex,  gRutoChildEyeClosedTex,
    gRutoChildEyeRollLeftTex, gRutoChildEyeHalf2Tex, gRutoChildEyeHalfWithBlushTex,
};

static void* sMouthTextures[] = {
    gRutoChildMouthClosedTex,
    gRutoChildMouthFrownTex,
    gRutoChildMouthOpenTex,
};

static s32 sUnused = 0;

#include "z_en_ru1_cutscene_data.inc.c"

static u32 D_80AF1938 = 0;

static EnRu1ActionFunc sActionFuncs[] = {
    func_80AEC0B4, // ENRU1_ACTION_00
    func_80AEC100, // ENRU1_ACTION_01
    func_80AEC130, // ENRU1_ACTION_02
    func_80AEC17C, // ENRU1_ACTION_03
    func_80AEC1D4, // ENRU1_ACTION_04
    func_80AEC244, // ENRU1_ACTION_05
    func_80AEC2C0, // ENRU1_ACTION_06
    func_80AECA94, // ENRU1_ACTION_07
    func_80AECAB4, // ENRU1_ACTION_08
    func_80AECAD4, // ENRU1_ACTION_09
    func_80AECB18, // ENRU1_ACTION_10
    func_80AECB60, // ENRU1_ACTION_11
    func_80AECBB8, // ENRU1_ACTION_12
    func_80AECC1C, // ENRU1_ACTION_13
    func_80AECC84, // ENRU1_ACTION_14
    func_80AED304, // ENRU1_ACTION_15
    func_80AED324, // ENRU1_ACTION_16
    func_80AED344, // ENRU1_ACTION_17
    func_80AED374, // ENRU1_ACTION_18
    func_80AED3A4, // ENRU1_ACTION_19
    func_80AED3E0, // ENRU1_ACTION_20
    func_80AED414, // ENRU1_ACTION_21
    func_80AEF29C, // ENRU1_ACTION_22
    func_80AEF2AC, // ENRU1_ACTION_23
    func_80AEF2D0, // ENRU1_ACTION_24
    func_80AEF354, // ENRU1_ACTION_25
    func_80AEF3A8, // ENRU1_ACTION_26
    func_80AEEBD4, // ENRU1_ACTION_27
    func_80AEEC5C, // ENRU1_ACTION_28
    func_80AEECF0, // ENRU1_ACTION_29
    func_80AEED58, // ENRU1_ACTION_30
    func_80AEEDCC, // ENRU1_ACTION_31
    func_80AEEE34, // ENRU1_ACTION_32
    func_80AEEE9C, // ENRU1_ACTION_33
    func_80AEEF08, // ENRU1_ACTION_34
    func_80AEEF5C, // ENRU1_ACTION_35
    func_80AEF9D8, // ENRU1_ACTION_36
    func_80AEFA2C, // ENRU1_ACTION_37
    func_80AEFAAC, // ENRU1_ACTION_38
    func_80AEFB04, // ENRU1_ACTION_39
    func_80AEFB68, // ENRU1_ACTION_40
    func_80AEFCE8, // ENRU1_ACTION_41
    func_80AEFBC8, // ENRU1_ACTION_42
    func_80AEFC24, // ENRU1_ACTION_43
    func_80AEFECC, // ENRU1_ACTION_44
    func_80AEFF40, // ENRU1_ACTION_45
};

static EnRu1PreLimbDrawFunc sPreLimbDrawFuncs[] = {
    EnRu1_PreLimbDraw,
};

static Vec3f sMultVec = { 0.0f, 10.0f, 0.0f };

static EnRu1DrawFunc sDrawFuncs[] = {
    EnRu1_DrawNothing,
    EnRu1_DrawOpa,
    EnRu1_DrawXlu,
};

ActorProfile En_Ru1_Profile = {
    /**/ ACTOR_EN_RU1,
    /**/ ACTORCAT_NPC,
    /**/ FLAGS,
    /**/ OBJECT_RU1,
    /**/ sizeof(EnRu1),
    /**/ EnRu1_Init,
    /**/ EnRu1_Destroy,
    /**/ EnRu1_Update,
    /**/ EnRu1_Draw,
};

void EnRu1_UpdateStandingOC(EnRu1* this, PlayState* play) {
    s32 pad[5];

    Collider_UpdateCylinder(&this->actor, &this->standingCollider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->standingCollider.base);
}

void EnRu1_UpdateSittingOC(EnRu1* this, PlayState* play) {
    s32 pad[5];

    Collider_UpdateCylinder(&this->actor, &this->sittingCollider);
    if (this->isSittingOCActive) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->sittingCollider.base);
    } else if (this->actor.xzDistToPlayer > 32.0f) {
        this->isSittingOCActive = true;
    }
}

void EnRu1_UpdateSittingAT(EnRu1* this, PlayState* play) {
    s32 pad[5];

    Collider_UpdateCylinder(&this->actor, &this->sittingCollider);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->sittingCollider.base);
}

void EnRu1_InitColliders(Actor* thisx, PlayState* play) {
    EnRu1* this = (EnRu1*)thisx;

    Collider_InitCylinder(play, &this->standingCollider);
    Collider_SetCylinderType1(play, &this->standingCollider, &this->actor, &sStandingCylinderInit);

    Collider_InitCylinder(play, &this->sittingCollider);
    Collider_SetCylinderType1(play, &this->sittingCollider, &this->actor, &sSittingCylinderInit);
}

void EnRu1_DestroyColliders(EnRu1* this, PlayState* play) {
    Collider_DestroyCylinder(play, &this->standingCollider);
    Collider_DestroyCylinder(play, &this->sittingCollider);
}

void EnRu1_DisableSittingOC(EnRu1* this) {
    this->isSittingOCActive = false;
}

u8 EnRu1_GetSwitchFlag(EnRu1* this) {
    u8 switchFlag = ENRU1_SWITCH_FLAG(&this->actor);

    return switchFlag;
}

u8 EnRu1_GetType(EnRu1* this) {
    u8 type = ENRU1_TYPE(&this->actor);

    return type;
}

void EnRu1_Destroy(Actor* thisx, PlayState* play) {
    EnRu1* this = (EnRu1*)thisx;

    EnRu1_DestroyColliders(this, play);
}

void EnRu1_UpdateEyes(EnRu1* this) {
    s32 pad[3];
    s16* blinkTimer = &this->blinkTimer;
    s16* eyes = &this->eyes;

    if (DECR(*blinkTimer) == 0) {
        *blinkTimer = Rand_S16Offset(60, 60);
    }

    *eyes = *blinkTimer;
    if (*eyes >= ENRU1_EYES_UP) {
        *eyes = ENRU1_EYES_OPEN;
    }
}

void EnRu1_SetEyes(EnRu1* this, s16 eyes) {
    this->eyes = eyes;
}

void EnRu1_SetMouth(EnRu1* this, s16 mouth) {
    this->mouth = mouth;
}

void func_80AEAECC(EnRu1* this, PlayState* play) {
    f32* velocityY = &this->actor.velocity.y;
    f32 velocityYHeld = *velocityY;

    *velocityY = -4.0f;
    Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 25.0f, 30.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2);
    *velocityY = velocityYHeld;
}

s32 EnRu1_IsCsStateIdle(PlayState* play) {
    if (play->csCtx.state == CS_STATE_IDLE) {
        return true;
    }
    return false;
}

CsCmdActorCue* EnRu1_GetCue(PlayState* play, s32 cueChannel) {
    s32 pad[2];
    CsCmdActorCue* cue = NULL;

    if (!EnRu1_IsCsStateIdle(play)) {
        cue = play->csCtx.actorCues[cueChannel];
    }
    return cue;
}

s32 func_80AEAFA0(PlayState* play, u16 cueId, s32 cueChannel) {
    CsCmdActorCue* cue = EnRu1_GetCue(play, cueChannel);

    if ((cue != NULL) && (cue->id == cueId)) {
        return true;
    }
    return false;
}

s32 func_80AEAFE0(PlayState* play, u16 cueId, s32 cueChannel) {
    CsCmdActorCue* cue = EnRu1_GetCue(play, cueChannel);

    if ((cue != NULL) && (cue->id != cueId)) {
        return true;
    }
    return false;
}

s32 func_80AEB020(EnRu1* this, PlayState* play) {
    Actor* actorIt = play->actorCtx.actorLists[ACTORCAT_NPC].head;
    EnRu1* someEnRu1;

    while (actorIt != NULL) {
        if (actorIt->id == ACTOR_EN_RU1) {
            someEnRu1 = (EnRu1*)actorIt;
            if (someEnRu1 != this) {
                if ((someEnRu1->action == ENRU1_ACTION_31) || (someEnRu1->action == ENRU1_ACTION_32) ||
                    (someEnRu1->action == ENRU1_ACTION_24)) {
                    return true;
                }
            }
        }
        actorIt = actorIt->next;
    }
    return false;
}

BgBdanObjects* EnRu1_FindBigOctoPlatform(PlayState* play) {
    Actor* actorIt = play->actorCtx.actorLists[ACTORCAT_BG].head;

    while (actorIt != NULL) {
        if (actorIt->id == ACTOR_BG_BDAN_OBJECTS && actorIt->params == 0) {
            return (BgBdanObjects*)actorIt;
        }
        actorIt = actorIt->next;
    }
    PRINTF(VT_FGCOL(RED) T("お立ち台が無い!!!!!!!!!!!!!!!!!!!!!!!!!\n", "There is no stand!!!!!!!!!!!!!!!!!!!!!!!!!\n")
               VT_RST);
    return NULL;
}

void EnRu1_SetPlatformCamSetting(EnRu1* this, s32 cameraSetting) {
    if (this->bigOctoPlatform != NULL) {
        this->bigOctoPlatform->cameraSetting = cameraSetting;
    }
}

s32 EnRu1_GetPlatformCamSetting(EnRu1* this) {
    if (this->bigOctoPlatform != NULL) {
        return this->bigOctoPlatform->cameraSetting;
    } else {
        return 0;
    }
}

Actor* func_80AEB124(PlayState* play) {
    Actor* actorIt = play->actorCtx.actorLists[ACTORCAT_BOSS].head;

    while (actorIt != NULL) {
        if ((actorIt->id == ACTOR_DEMO_EFFECT) && (PARAMS_GET_U(actorIt->params, 0, 8) == DEMO_EFFECT_JEWEL_ZORA)) {
            return actorIt;
        }
        actorIt = actorIt->next;
    }
    return NULL;
}

int func_80AEB174(PlayState* play) {
    return (Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play);
}

s32 func_80AEB1B4(PlayState* play) {
    return Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING;
}

#if DEBUG_FEATURES
void func_80AEB1D8(EnRu1* this) {
    this->action = ENRU1_ACTION_36;
    this->drawConfig = ENRU1_DRAW_NOTHING;
    this->actor.velocity.x = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.velocity.z = 0.0f;
    this->actor.speed = 0.0f;
    this->actor.gravity = 0.0f;
    this->actor.minVelocityY = 0.0f;
    EnRu1_SetPlatformCamSetting(this, 0);
}

void func_80AEB220(EnRu1* this, PlayState* play) {
    if ((EnRu1_IsCsStateIdle(play)) && (this->actor.params == 0xA)) {
        func_80AEB1D8(this);
    }
}
#endif

void EnRu1_AnimationChange(EnRu1* this, AnimationHeader* animation, u8 mode, f32 morphFrames, s32 playReversed) {
    s32 pad[2];
    AnimationHeader* animHeader = SEGMENTED_TO_VIRTUAL(animation);
    f32 frameCount = Animation_GetLastFrame(animHeader);
    f32 playbackSpeed;
    f32 startFrame;
    f32 endFrame;

    if (!playReversed) {
        startFrame = 0.0f;
        endFrame = frameCount;
        playbackSpeed = 1.0f;
    } else {
        startFrame = frameCount;
        endFrame = 0.0f;
        playbackSpeed = -1.0f;
    }

    Animation_Change(&this->skelAnime, animHeader, playbackSpeed, startFrame, endFrame, mode, morphFrames);
}

s32 EnRu1_UpdateSkelAnime(EnRu1* this) {
    // why?
    if (this->action != ENRU1_ACTION_32) {
        return SkelAnime_Update(&this->skelAnime);
    } else {
        return SkelAnime_Update(&this->skelAnime);
    }
}

void func_80AEB364(EnRu1* this, PlayState* play) {
    this->skelAnime.movementFlags |= ANIM_FLAG_UPDATE_XZ;
    AnimTaskQueue_AddActorMovement(play, &this->actor, &this->skelAnime, 1.0f);
}

void func_80AEB3A4(EnRu1* this, PlayState* play) {
    this->skelAnime.movementFlags |= ANIM_FLAG_UPDATE_XZ;
    func_80AEB364(this, play);
}

void func_80AEB3CC(EnRu1* this) {
    this->skelAnime.movementFlags &= ~ANIM_FLAG_UPDATE_XZ;
}

void EnRu1_InitOutsideJabuJabu(EnRu1* this, PlayState* play) {
    EnRu1_AnimationChange(this, &gRutoChildWaitHandsBehindBackAnim, ANIMMODE_LOOP, 0, false);
    this->action = ENRU1_ACTION_00;
    this->drawConfig = ENRU1_DRAW_OPA;
    EnRu1_SetEyes(this, ENRU1_EYES_GAZING);
    EnRu1_SetMouth(this, ENRU1_MOUTH_SMILING);
}

CsCmdActorCue* EnRu1_GetCueChannel3(PlayState* play) {
    return EnRu1_GetCue(play, 3);
}

s32 func_80AEB458(PlayState* play, u16 cueId) {
    return func_80AEAFA0(play, cueId, 3);
}

s32 func_80AEB480(PlayState* play, u16 cueId) {
    return func_80AEAFE0(play, cueId, 3);
}

void EnRu1_SpawnRipple(EnRu1* this, PlayState* play, s16 radiusMax, s16 life) {
    Vec3f pos;
    Actor* thisx = &this->actor;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + this->actor.depthInWater;
    pos.z = this->actor.world.pos.z;
    EffectSsGRipple_Spawn(play, &pos, 100, radiusMax, life);
}

void func_80AEB50C(EnRu1* this, PlayState* play) {
    this->unk_270 += 1.0f;
    if (this->unk_270 >= kREG(3) + 10.0f) {
        EnRu1_SpawnRipple(this, play, kREG(1) + 500, 0);
        this->unk_270 = 0.0f;
    }
}

void func_80AEB59C(EnRu1* this, PlayState* play) {
    EnRu1_SpawnRipple(this, play, kREG(2) + 500, 0);
    EnRu1_SpawnRipple(this, play, kREG(2) + 500, kREG(3) + 10.0f);
    EnRu1_SpawnRipple(this, play, kREG(2) + 500, (kREG(3) + 10.0f) * 2.0f);
}

void EnRu1_SpawnSplash(EnRu1* this, PlayState* play) {
    Vec3f pos;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + this->actor.depthInWater;
    pos.z = this->actor.world.pos.z;

    EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 1, 0);
}

void func_80AEB6E0(EnRu1* this, PlayState* play) {
    SkelAnime* skelAnime = &this->skelAnime;

    if (skelAnime->baseTransl.y < skelAnime->jointTable[0].y) {
        skelAnime->movementFlags |= ANIM_FLAG_UPDATE_XZ | ANIM_FLAG_UPDATE_Y;
        AnimTaskQueue_AddActorMovement(play, &this->actor, skelAnime, 1.0f);
    }
}

void func_80AEB738(EnRu1* this, PlayState* play) {
    SkelAnime* skelAnime = &this->skelAnime;

    skelAnime->baseTransl = skelAnime->jointTable[0];
    skelAnime->prevTransl = skelAnime->jointTable[0];
    if (skelAnime->baseTransl.y < skelAnime->jointTable[0].y) {
        skelAnime->movementFlags |= ANIM_FLAG_UPDATE_XZ | ANIM_FLAG_UPDATE_Y;
        AnimTaskQueue_AddActorMovement(play, &this->actor, skelAnime, 1.0f);
    }
}

void func_80AEB7D0(EnRu1* this) {
    this->skelAnime.movementFlags &= ~(ANIM_FLAG_UPDATE_XZ | ANIM_FLAG_UPDATE_Y);
}

f32 func_80AEB7E0(CsCmdActorCue* cue, PlayState* play) {
    s32 csCurFrame = play->csCtx.curFrame;

    if ((csCurFrame < cue->endFrame) && (cue->endFrame - cue->startFrame > 0)) {
        return (Math_CosS(((csCurFrame - cue->startFrame) / (f32)(cue->endFrame - cue->startFrame)) * 32768.0f) *
                -0.5f) +
               0.5f;
    }
    return 1.0f;
}

f32 func_80AEB87C(f32 arg0, s32 arg1, s32 arg2) {
    return (((f32)arg2 - arg1) * arg0) + arg1;
}

void func_80AEB89C(EnRu1* this, PlayState* play) {
    CsCmdActorCue* cue = EnRu1_GetCueChannel3(play);

    if (cue != NULL) {
        this->actor.world.rot.y = this->actor.shape.rot.y = cue->rot.y;

        this->actor.world.pos.x = cue->startPos.x;
        this->actor.world.pos.y = cue->startPos.y;
        this->actor.world.pos.z = cue->startPos.z;
    }
}

void func_80AEB914(EnRu1* this, PlayState* play) {
    func_80AEB89C(this, play);
}

void func_80AEB934(EnRu1* this, PlayState* play) {
    func_80AEB89C(this, play);
}

void func_80AEB954(EnRu1* this, PlayState* play) {
    func_80AEB6E0(this, play);
}

void func_80AEB974(EnRu1* this, PlayState* play) {
    Vec3f* thisPos;
    f32 sp30;
    CsCmdActorCue* cue = EnRu1_GetCueChannel3(play);
    s32 pad;

    if (cue != NULL) {
        sp30 = func_80AEB7E0(cue, play);
        thisPos = &this->actor.world.pos;
        thisPos->x = func_80AEB87C(sp30, cue->startPos.x, cue->endPos.x);
        thisPos->y = func_80AEB87C(sp30, cue->startPos.y, cue->endPos.y);
        thisPos->z = func_80AEB87C(sp30, cue->startPos.z, cue->endPos.z);
    }
}

void func_80AEBA0C(EnRu1* this, PlayState* play) {
    func_80AEB6E0(this, play);
}

void func_80AEBA2C(EnRu1* this, PlayState* play) {
    s32 pad;
    Vec3f* unk_364 = &this->unk_364;
    Vec3f* thisPos;
    f32 temp_ret_2;
    CsCmdActorCue* cue = EnRu1_GetCueChannel3(play);
    s32 pad2;

    if (cue != NULL) {
        temp_ret_2 = func_80AEB7E0(cue, play);
        thisPos = &this->actor.world.pos;
        thisPos->x = func_80AEB87C(temp_ret_2, unk_364->x, cue->endPos.x);
        thisPos->y = func_80AEB87C(temp_ret_2, unk_364->y, cue->endPos.y);
        thisPos->z = func_80AEB87C(temp_ret_2, unk_364->z, cue->endPos.z);
    }
}

void func_80AEBAFC(EnRu1* this) {
    if (this->unk_298 == 0) {
        Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_EV_DIVE_INTO_WATER);
        this->unk_298 = 1;
    }
}

void func_80AEBB3C(EnRu1* this) {
    if (Animation_OnFrame(&this->skelAnime, 5.0f)) {
        Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_PL_FACE_UP);
    }
}

void func_80AEBB78(EnRu1* this) {
    SkelAnime* skelAnime = &this->skelAnime;

    if (Animation_OnFrame(skelAnime, 4.0f) || Animation_OnFrame(skelAnime, 13.0f) ||
        Animation_OnFrame(skelAnime, 22.0f) || Animation_OnFrame(skelAnime, 31.0f)) {
        Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_PL_SWIM);
    }
}

void func_80AEBBF4(EnRu1* this) {
    if (Animation_OnFrame(&this->skelAnime, 8.0f)) {
        Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_PL_SUBMERGE);
    }
}

void func_80AEBC30(PlayState* play) {
    Player* player;

    if (play->csCtx.curFrame == 205) {
        player = GET_PLAYER(play);
        SFX_PLAY_AT_POS(&player->actor.projectedPos, NA_SE_EV_DIVE_INTO_WATER);
    }
}

void func_80AEBC84(EnRu1* this, PlayState* play) {
    if (play->csCtx.curFrame == 130) {
        Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_VO_RT_LAUGH_0);
    }
}

void func_80AEBCB8(EnRu1* this, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&this->skelAnime, &gRutoChildSwimOnBackAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildSwimOnBackAnim), ANIMMODE_LOOP, -8.0f);
    }
}

void func_80AEBD1C(EnRu1* this, PlayState* play) {
    if (func_80AEB480(play, 2)) {
        this->action = ENRU1_ACTION_01;
        this->drawConfig = ENRU1_DRAW_NOTHING;
        func_80AEB914(this, play);
        func_80AEAECC(this, play);
        EnRu1_SpawnSplash(this, play);
        func_80AEB59C(this, play);
    }
}

void func_80AEBD94(EnRu1* this, PlayState* play) {
    s32 pad[2];
    f32 frameCount;

    if (func_80AEB480(play, 3)) {
        frameCount = Animation_GetLastFrame(&gRutoChildResurfaceAnim);
        func_80AEB934(this, play);
        func_80AEB738(this, play);
        Animation_Change(&this->skelAnime, &gRutoChildResurfaceAnim, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, 0.0f);
        this->action = ENRU1_ACTION_02;
        this->drawConfig = ENRU1_DRAW_OPA;
    }
}

void func_80AEBE3C(EnRu1* this, PlayState* play, s32 arg2) {
    s32 pad[2];

    if (arg2 != 0) {
        f32 frameCount = Animation_GetLastFrame(&gRutoChildTreadWaterAnim);

        func_80AEB7D0(this);
        Animation_Change(&this->skelAnime, &gRutoChildTreadWaterAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
        this->action = ENRU1_ACTION_03;
    } else {
        func_80AEB954(this, play);
    }
}

void func_80AEBEC8(EnRu1* this, PlayState* play) {
    s32 pad[2];
    f32 frameCount;

    if (func_80AEB458(play, 6)) {
        frameCount = Animation_GetLastFrame(&gRutoChildTransitionToSwimOnBackAnim);
        func_80AEB738(this, play);
        Animation_Change(&this->skelAnime, &gRutoChildTransitionToSwimOnBackAnim, 1.0f, 0, frameCount, ANIMMODE_ONCE,
                         -8.0f);
        this->action = ENRU1_ACTION_04;
    }
}

void func_80AEBF60(EnRu1* this, PlayState* play) {
    if (func_80AEB480(play, 6)) {
        s32 pad;

        func_80AEB7D0(this);
        this->action = ENRU1_ACTION_05;
        this->unk_364 = this->actor.world.pos;
    } else {
        func_80AEBA0C(this, play);
    }
}

void func_80AEBFD8(EnRu1* this, PlayState* play) {
    CsCmdActorCue* cue = EnRu1_GetCueChannel3(play);
    f32 frameCount;
    u16 csCurFrame;
    u16 endFrame;

    if (cue != NULL) {
        csCurFrame = play->csCtx.curFrame;
        endFrame = cue->endFrame;

        if (csCurFrame >= endFrame - 2) {
            frameCount = Animation_GetLastFrame(&gRutoChildTransitionFromSwimOnBackAnim);
            Animation_Change(&this->skelAnime, &gRutoChildTransitionFromSwimOnBackAnim, 1.0, 0, frameCount,
                             ANIMMODE_ONCE, -8.0f);
            this->action = ENRU1_ACTION_06;
        }
    }
}

void func_80AEC070(EnRu1* this, PlayState* play, UNK_TYPE arg2) {
    if ((func_80AEB458(play, 8)) && (arg2 != 0)) {
        Actor_Kill(&this->actor);
    }
}

void func_80AEC0B4(EnRu1* this, PlayState* play) {
    func_80AEB89C(this, play);
    EnRu1_UpdateSkelAnime(this);
    func_80AEBC84(this, play);
    func_80AEBC30(play);
    func_80AEBD1C(this, play);
}

void func_80AEC100(EnRu1* this, PlayState* play) {
    func_80AEBAFC(this);
    func_80AEBD94(this, play);
}

void func_80AEC130(EnRu1* this, PlayState* play) {
    s32 something = EnRu1_UpdateSkelAnime(this);

    func_80AEAECC(this, play);
    func_80AEBB3C(this);
    func_80AEBE3C(this, play, something);
}

void func_80AEC17C(EnRu1* this, PlayState* play) {
    func_80AEB974(this, play);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
    func_80AEB50C(this, play);
    func_80AEBEC8(this, play);
}

void func_80AEC1D4(EnRu1* this, PlayState* play) {
    s32 something;

    something = EnRu1_UpdateSkelAnime(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateEyes(this);
    func_80AEB50C(this, play);
    func_80AEBCB8(this, something);
    func_80AEBBF4(this);
    func_80AEBF60(this, play);
}

void func_80AEC244(EnRu1* this, PlayState* play) {
    s32 something;

    something = EnRu1_UpdateSkelAnime(this);
    func_80AEBA2C(this, play);
    func_80AEAECC(this, play);
    EnRu1_UpdateEyes(this);
    func_80AEB50C(this, play);
    func_80AEBCB8(this, something);
    func_80AEBB78(this);
    func_80AEBFD8(this, play);
}

void func_80AEC2C0(EnRu1* this, PlayState* play) {
    s32 something;

    something = EnRu1_UpdateSkelAnime(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateEyes(this);
    func_80AEB50C(this, play);
    func_80AEC070(this, play, something);
}

void EnRu1_InitInJabuJabuHolesRoom(EnRu1* this, PlayState* play) {
    if (!GET_INFTABLE(INFTABLE_141)) {
        EnRu1_AnimationChange(this, &gRutoChildWait2Anim, ANIMMODE_LOOP, 0, false);
        this->action = ENRU1_ACTION_07;
        EnRu1_SetMouth(this, ENRU1_MOUTH_FROWNING);
    } else if (GET_INFTABLE(INFTABLE_147) && !GET_INFTABLE(INFTABLE_140) && !GET_INFTABLE(INFTABLE_145)) {
        if (!func_80AEB020(this, play)) {
            s8 actorRoom;

            EnRu1_AnimationChange(this, &gRutoChildWait2Anim, ANIMMODE_LOOP, 0, false);
            actorRoom = this->actor.room;
            this->action = ENRU1_ACTION_22;
            this->actor.room = -1;
            this->drawConfig = ENRU1_DRAW_NOTHING;
            this->roomNum1 = actorRoom;
            this->roomNum3 = actorRoom;
            this->roomNum2 = actorRoom;
        } else {
            Actor_Kill(&this->actor);
        }
    } else {
        Actor_Kill(&this->actor);
    }
}

void func_80AEC40C(EnRu1* this) {
    f32 unk_26C = this->unk_26C;

    if (unk_26C < 8.0f) {
        this->actor.speed = (((kREG(3) * 0.01f) + 2.7f) / 8.0f) * unk_26C;
    } else {
        this->actor.speed = (kREG(3) * 0.01f) + 2.7f;
    }
    this->actor.velocity.y = -1.0f;
    Actor_MoveXZGravity(&this->actor);
}

void func_80AEC4CC(EnRu1* this) {
    this->actor.velocity.y = -1.0f;
    Actor_MoveXZGravity(&this->actor);
}

void func_80AEC4F4(EnRu1* this) {
    f32* speedXZ = &this->actor.speed;
    f32* unk_26C = &this->unk_26C;

    if (this->unk_26C < 8.0f) {
        *unk_26C += 1.0f;
        *speedXZ *= (8.0f - *unk_26C) / 8.0f;
        this->actor.velocity.y = -*unk_26C * (((kREG(4) * 0.01f) + 13.0f) / 8.0f);
    } else {
        *speedXZ = 0.0f;
        this->actor.velocity.y = -((kREG(4) * 0.01f) + 13.0f);
    }
    Actor_MoveXZGravity(&this->actor);
}

s32 func_80AEC5FC(EnRu1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 thisPosZ = this->actor.world.pos.z;
    f32 playerPosZ = player->actor.world.pos.z;

    if ((playerPosZ - thisPosZ <= 265.0f) && (player->actor.world.pos.y >= this->actor.world.pos.y)) {
        return true;
    }
    return false;
}

void func_80AEC650(EnRu1* this) {
    s32 pad[2];

    if (this->unk_280 == 0) {
        if (Animation_OnFrame(&this->skelAnime, 2.0f) || Animation_OnFrame(&this->skelAnime, 7.0f)) {
            Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_PL_WALK_GROUND + SURFACE_SFX_OFFSET_JABU);
        }
    }
}

void func_80AEC6B0(EnRu1* this) {
    Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_EV_FALL_DOWN_DIRT);
    Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_VO_RT_FALL);
}

void func_80AEC6E4(EnRu1* this, PlayState* play) {
    if ((func_80AEAFA0(play, 4, 3)) && (this->unk_280 == 0)) {
        Animation_Change(&this->skelAnime, &gRutoChildBringArmsUpAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildBringArmsUpAnim), ANIMMODE_ONCE, -8.0f);
        this->unk_280 = 1;
        func_80AEC6B0(this);
    }
}

void func_80AEC780(EnRu1* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);

    if ((func_80AEC5FC(this, play)) && (!Play_InCsMode(play)) &&
        (!(player->stateFlags1 & (PLAYER_STATE1_13 | PLAYER_STATE1_14 | PLAYER_STATE1_21))) &&
        (player->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {

        play->csCtx.script = gRutoFirstMeetingCs;
        gSaveContext.cutsceneTrigger = 1;
        player->speedXZ = 0.0f;
        this->action = ENRU1_ACTION_08;
    }
}

void func_80AEC81C(EnRu1* this, PlayState* play) {
    CsCmdActorCue* cue;
    s16 newRotY;

    if (func_80AEAFE0(play, 1, 3)) {
        cue = play->csCtx.actorCues[3];
        this->actor.world.pos.x = cue->startPos.x;
        this->actor.world.pos.y = cue->startPos.y;
        this->actor.world.pos.z = cue->startPos.z;
        newRotY = cue->rot.y;
        this->actor.shape.rot.y = newRotY;
        this->actor.world.rot.y = newRotY;
        this->action = ENRU1_ACTION_09;
        this->drawConfig = ENRU1_DRAW_OPA;
    }
}

void func_80AEC8B8(EnRu1* this, PlayState* play) {
    if (func_80AEAFA0(play, 3, 3)) {
        Animation_Change(&this->skelAnime, &gRutoChildTurnAroundAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildTurnAroundAnim), ANIMMODE_ONCE, -8.0f);
        this->action = ENRU1_ACTION_10;
    }
}

void func_80AEC93C(EnRu1* this, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&this->skelAnime, &gRutoChildWalkAnim, 1.0f, 0, Animation_GetLastFrame(&gRutoChildWalkAnim),
                         ANIMMODE_LOOP, -8.0f);
        this->actor.world.rot.y += 0x8000;
        this->action = ENRU1_ACTION_11;
        this->unk_26C = 0.0f;
    }
}

void func_80AEC9C4(EnRu1* this) {
    this->unk_26C += 1.0f;
    if (this->unk_26C >= 8.0f) {
        this->action = ENRU1_ACTION_12;
        this->unk_26C = 0.0f;
        this->actor.velocity.y = -1.0f;
    }
}

void func_80AECA18(EnRu1* this) {
    if (!(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        s32 pad;

        this->action = ENRU1_ACTION_13;
        this->unk_26C = 0.0f;
        this->actor.velocity.y = 0.0f;
    }
}

void func_80AECA44(EnRu1* this, PlayState* play) {
    if (func_80AEAFA0(play, 5, 3)) {
        SET_INFTABLE(INFTABLE_141);
        this->action = ENRU1_ACTION_14;
    }
}

void func_80AECA94(EnRu1* this, PlayState* play) {
    func_80AEC780(this, play);
}

void func_80AECAB4(EnRu1* this, PlayState* play) {
    func_80AEC81C(this, play);
}

void func_80AECAD4(EnRu1* this, PlayState* play) {
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEC8B8(this, play);
}

void func_80AECB18(EnRu1* this, PlayState* play) {
    s32 something;

    something = EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEC93C(this, something);
}

void func_80AECB60(EnRu1* this, PlayState* play) {
    func_80AEC40C(this);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEC650(this);
    func_80AEC9C4(this);
}

void func_80AECBB8(EnRu1* this, PlayState* play) {
    func_80AEC4CC(this);
    func_80AEC6E4(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEC650(this);
    func_80AECA18(this);
}

void func_80AECC1C(EnRu1* this, PlayState* play) {
    func_80AEC4F4(this);
    func_80AEC6E4(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEC650(this);
    func_80AECA44(this, play);
}

void func_80AECC84(EnRu1* this, PlayState* play) {
    if (play->csCtx.state == CS_STATE_IDLE) {
        Actor_Kill(&this->actor);
    }
}

void func_80AECCB0(EnRu1* this, PlayState* play) {
    s32 pad;
    Vec3f* pos;
    s16 yawTowardsPlayer;
    f32 spawnX;
    f32 spawnY;
    f32 spawnZ;
    s32 pad2[2];

    yawTowardsPlayer = this->actor.yawTowardsPlayer;
    pos = &this->actor.world.pos;
    spawnX = ((kREG(1) + 12.0f) * Math_SinS(yawTowardsPlayer)) + pos->x;
    spawnY = pos->y;
    spawnZ = ((kREG(1) + 12.0f) * Math_CosS(yawTowardsPlayer)) + pos->z;
    this->blueWarp = (DoorWarp1*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_DOOR_WARP1, spawnX,
                                                    spawnY, spawnZ, 0, yawTowardsPlayer, 0, WARP_BLUE_RUTO);
}

void EnRu1_InitInBossRoom(EnRu1* this, PlayState* play) {
    EnRu1_AnimationChange(this, &gRutoChildWaitHandsOnHipsAnim, ANIMMODE_LOOP, 0, false);
    this->action = ENRU1_ACTION_15;
    this->actor.shape.yOffset = -10000.0f;
    EnRu1_SetEyes(this, ENRU1_EYES_BLUSH);
    EnRu1_SetMouth(this, ENRU1_MOUTH_OPEN);
}

void func_80AECE04(EnRu1* this, PlayState* play) {
    this->actor.shape.yOffset += (250.0f / 3.0f);
}

void func_80AECE20(EnRu1* this, PlayState* play) {
    s32 pad2;
    Player* player = GET_PLAYER(play);
    Vec3f* playerPos = &player->actor.world.pos;
    s16 shapeRotY = player->actor.shape.rot.y;
    s32 pad;
    f32 unk_27C = this->unk_27C;
    Vec3f* pos = &this->actor.world.pos;

    pos->x = (Math_SinS(shapeRotY) * unk_27C) + playerPos->x;
    pos->y = playerPos->y;
    pos->z = (Math_CosS(shapeRotY) * unk_27C) + playerPos->z;
}

void func_80AECEB4(EnRu1* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);
    Vec3f* player_unk_450 = &player->unk_450;
    Vec3f* pos = &this->actor.world.pos;
    s16 shapeRotY = this->actor.shape.rot.y;

    player_unk_450->x = ((kREG(2) + 30.0f) * Math_SinS(shapeRotY)) + pos->x;
    player_unk_450->z = ((kREG(2) + 30.0f) * Math_CosS(shapeRotY)) + pos->z;
}

s32 func_80AECF6C(EnRu1* this, PlayState* play) {
    s16* shapeRotY;
    Player* player = GET_PLAYER(play);
    Player* otherPlayer;
    s16 temp_f16;
    f32 temp1;
    f32 temp2;
    s32 pad2[5];

    this->unk_26C += 1.0f;
    if ((player->actor.speed == 0.0f) && (this->unk_26C >= 3.0f)) {
        otherPlayer = GET_PLAYER(play);
        player->actor.world.pos.x = otherPlayer->unk_450.x;
        player->actor.world.pos.y = otherPlayer->unk_450.y;
        player->actor.world.pos.z = otherPlayer->unk_450.z;
        shapeRotY = &player->actor.shape.rot.y;
        temp1 = this->actor.world.pos.x - player->actor.world.pos.x;
        temp2 = this->actor.world.pos.z - player->actor.world.pos.z;
        temp_f16 = RAD_TO_BINANG(Math_FAtan2F(temp1, temp2));
        if (*shapeRotY != temp_f16) {
            Math_SmoothStepToS(shapeRotY, temp_f16, 0x14, 0x1838, 0x64);
            player->actor.world.rot.y = *shapeRotY;
        } else {
            return true;
        }
    }
    return false;
}

s32 func_80AED084(EnRu1* this, s32 state) {
    if (this->blueWarp != NULL && this->blueWarp->rutoWarpState == state) {
        return true;
    }
    return false;
}

void func_80AED0B0(EnRu1* this, s32 state) {
    if (this->blueWarp != NULL) {
        this->blueWarp->rutoWarpState = state;
    }
}

void func_80AED0C8(EnRu1* this, PlayState* play) {
    this->action = ENRU1_ACTION_16;
}

void func_80AED0D8(EnRu1* this, PlayState* play) {
    this->action = ENRU1_ACTION_17;
    this->drawConfig = ENRU1_DRAW_OPA;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    func_80AECCB0(this, play);
}

void func_80AED110(EnRu1* this) {
    if (this->actor.shape.yOffset >= 0.0f) {
        this->action = ENRU1_ACTION_18;
        this->actor.shape.yOffset = 0.0f;
        func_80AED0B0(this, WARP_BLUE_RUTO_STATE_READY);
    }
}

void func_80AED154(EnRu1* this, PlayState* play) {
    if (func_80AED084(this, WARP_BLUE_RUTO_STATE_ENTERED)) {
        this->action = ENRU1_ACTION_19;
        this->unk_26C = 0.0f;
        func_80AECEB4(this, play);
    }
}

void func_80AED19C(EnRu1* this, s32 cond) {
    if (cond) {
        Animation_Change(&this->skelAnime, &gRutoChildTransitionHandsOnHipToCrossArmsAndLegsAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildTransitionHandsOnHipToCrossArmsAndLegsAnim), ANIMMODE_ONCE,
                         -8.0f);
        this->action = ENRU1_ACTION_20;
        func_80AED0B0(this, WARP_BLUE_RUTO_STATE_3);
    }
}

void func_80AED218(EnRu1* this, UNK_TYPE arg1) {
    if (func_80AED084(this, WARP_BLUE_RUTO_STATE_TALKING)) {
        if (arg1 != 0) {
            Animation_Change(&this->skelAnime, &gRutoChildWaitSittingAnim, 1.0f, 0,
                             Animation_GetLastFrame(&gRutoChildWaitSittingAnim), ANIMMODE_LOOP, -8.0f);
        }
    } else if (func_80AED084(this, WARP_BLUE_RUTO_STATE_WARPING)) {
        Animation_Change(&this->skelAnime, &gRutoChildWaitInBlueWarpAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildWaitInBlueWarpAnim), ANIMMODE_ONCE, -8.0f);
        this->action = ENRU1_ACTION_21;
        this->unk_27C = this->actor.xzDistToPlayer;
    }
}

void func_80AED304(EnRu1* this, PlayState* play) {
    func_80AED0C8(this, play);
}

void func_80AED324(EnRu1* this, PlayState* play) {
    func_80AED0D8(this, play);
}

void func_80AED344(EnRu1* this, PlayState* play) {
    func_80AECE04(this, play);
    EnRu1_UpdateSkelAnime(this);
    func_80AED110(this);
}

void func_80AED374(EnRu1* this, PlayState* play) {
    EnRu1_UpdateSkelAnime(this);
    func_80AED154(this, play);
}

void func_80AED3A4(EnRu1* this, PlayState* play) {
    EnRu1_UpdateSkelAnime(this);
    func_80AED19C(this, func_80AECF6C(this, play));
}

void func_80AED3E0(EnRu1* this, PlayState* play) {
    func_80AEAECC(this, play);
    func_80AED218(this, EnRu1_UpdateSkelAnime(this));
}

void func_80AED414(EnRu1* this, PlayState* play) {
    func_80AECE20(this, play);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
}

void EnRu1_InitInJabuJabuBasement(EnRu1* this, PlayState* play) {
    if (GET_INFTABLE(INFTABLE_141) && !GET_INFTABLE(INFTABLE_145) && !GET_INFTABLE(INFTABLE_140) &&
        !GET_INFTABLE(INFTABLE_147)) {
        if (!func_80AEB020(this, play)) {
            s8 actorRoom;

            EnRu1_AnimationChange(this, &gRutoChildWait2Anim, ANIMMODE_LOOP, 0, false);
            actorRoom = this->actor.room;
            this->action = ENRU1_ACTION_22;
            this->actor.room = -1;
            this->roomNum1 = actorRoom;
            this->roomNum3 = actorRoom;
            this->roomNum2 = actorRoom;
        } else {
            Actor_Kill(&this->actor);
        }
    } else {
        Actor_Kill(&this->actor);
    }
}

void func_80AED4FC(EnRu1* this) {
    Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_EV_LAND_DIRT);
}

void func_80AED520(EnRu1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    SFX_PLAY_AT_POS(&player->actor.projectedPos, NA_SE_PL_PULL_UP_RUTO);
    Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_VO_RT_LIFT);
}

void func_80AED57C(EnRu1* this) {
    if (this->actor.speed != 0.0f) {
        Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_VO_RT_THROW);
    }
}

void func_80AED5B8(EnRu1* this) {
    Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_VO_RT_CRASH);
}

void func_80AED5DC(EnRu1* this) {
    Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_VO_RT_UNBALLANCE);
}

void func_80AED600(EnRu1* this) {
    Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_VO_RT_DISCOVER);
}

s32 func_80AED624(EnRu1* this, PlayState* play) {
    s8 curRoomNum = play->roomCtx.curRoom.num;

    if (this->roomNum2 != curRoomNum) {
        Actor_Kill(&this->actor);
        return false;
    } else if (((this->roomNum1 != curRoomNum) || (this->roomNum2 != curRoomNum)) &&
               (this->actor.depthInWater > kREG(16) + 50.0f) && (this->action != ENRU1_ACTION_33)) {
        this->action = ENRU1_ACTION_33;
        this->drawConfig = ENRU1_DRAW_XLU;
        this->alpha = 0xFF;
        this->unk_2A4 = 0.0f;
    }
    return true;
}

void func_80AED6DC(EnRu1* this, PlayState* play) {
    s8 curRoomNum = play->roomCtx.curRoom.num;

    this->roomNum2 = curRoomNum;
    this->unk_288 = 0.0f;
}

void func_80AED6F8(PlayState* play) {
    s8 curRoomNum;

    if (!GET_INFTABLE(INFTABLE_147)) {
        curRoomNum = play->roomCtx.curRoom.num;
        if (curRoomNum == 2) {
            SET_INFTABLE(INFTABLE_147);
        }
    }
}

void func_80AED738(EnRu1* this, PlayState* play) {
    if (func_80AED624(this, play)) {
        s32 pad;

        this->unk_2A4 += 1.0f;
        if (this->unk_2A4 < 20.0f) {
            u32 temp_v0 = ((20.0f - this->unk_2A4) * 255.0f) / 20.0f;

            this->alpha = temp_v0;
            this->actor.shape.shadowAlpha = temp_v0;
        } else {
            Actor_Kill(&this->actor);
        }
    }
}

void func_80AED83C(EnRu1* this) {
    s32 pad[2];
    Vec3s* headRot;
    Vec3s* torsoRot;

    headRot = &this->interactInfo.headRot;
    Math_SmoothStepToS(&headRot->x, 0, 0x14, 0x1838, 0x64);
    Math_SmoothStepToS(&headRot->y, 0, 0x14, 0x1838, 0x64);
    torsoRot = &this->interactInfo.torsoRot;
    Math_SmoothStepToS(&torsoRot->x, 0, 0x14, 0x1838, 0x64);
    Math_SmoothStepToS(&torsoRot->y, 0, 0x14, 0x1838, 0x64);
}

void EnRu1_UpdateHeadRotation(EnRu1* this) {
    s32 headRotOffset;
    s16* headRotTimer = &this->headRotTimer;
    s16* headRotY = &this->interactInfo.headRot.y;
    s16* headTurnSpeed = &this->headTurnSpeed;
    s32 pad[2];

    if (DECR(*headRotTimer) == 0) {
        *headRotTimer = Rand_S16Offset(0xA, 0x19);
        headRotOffset = *headRotTimer % 5;
        if (headRotOffset == 0) {
            this->headRotDirection = 1;
        } else if (headRotOffset == 1) {
            this->headRotDirection = 2;
        } else {
            this->headRotDirection = 0;
        }
        *headTurnSpeed = 0;
    }

    if (this->headRotDirection == 0) {
        Math_SmoothStepToS(headTurnSpeed, 0 - *headRotY, 1, 0x190, 0x190);
        Math_SmoothStepToS(headRotY, 0, 3, ABS(*headTurnSpeed), 0x64);
    } else if (this->headRotDirection == 1) {
        Math_SmoothStepToS(headTurnSpeed, -0x2AAA - *headRotY, 1, 0x190, 0x190);
        Math_SmoothStepToS(headRotY, -0x2AAA, 3, ABS(*headTurnSpeed), 0x64);
    } else {
        Math_SmoothStepToS(headTurnSpeed, 0x2AAA - *headRotY, 1, 0x190, 0x190);
        Math_SmoothStepToS(headRotY, 0x2AAA, 3, ABS(*headTurnSpeed), 0x64);
    }
}

void func_80AEDAE0(EnRu1* this, PlayState* play) {
    DynaPolyActor* dynaPolyActor = DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId);

    if (dynaPolyActor == NULL || dynaPolyActor->actor.id == ACTOR_EN_BOX) {
        this->actor.bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL | BGCHECKFLAG_CEILING);
    }
}

void func_80AEDB30(EnRu1* this, PlayState* play) {
    f32* velocityY;
    f32* speedXZ;
    f32* gravity;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        DynaPolyActor* dynaPolyActor;

        velocityY = &this->actor.velocity.y;
        dynaPolyActor = DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId);
        if (*velocityY <= 0.0f) {
            speedXZ = &this->actor.speed;
            if (dynaPolyActor != NULL) {
                if (dynaPolyActor->actor.id != ACTOR_EN_BOX) {
                    *speedXZ = 0.0f;
                }
            } else {
                if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
                    *speedXZ *= (kREG(19) * 0.01f) + 0.8f;
                } else {
                    *speedXZ = 0.0f;
                }
            }
            gravity = &this->actor.gravity;
            if (dynaPolyActor != NULL) {
                if (dynaPolyActor->actor.id != ACTOR_EN_BOX) {
                    *velocityY = 0.0f;
                    this->actor.minVelocityY = 0.0f;
                    *gravity = 0.0f;
                } else {
                    *velocityY *= -1.0f;
                }
            } else {
                *velocityY *= -((kREG(20) * 0.01f) + 0.6f);
                if (*velocityY <= -*gravity * ((kREG(20) * 0.01f) + 0.6f)) {
                    *velocityY = 0.0f;
                    this->actor.minVelocityY = 0.0f;
                    *gravity = 0.0f;
                }
            }
            func_80AED4FC(this);
        }
    }
    if (this->actor.bgCheckFlags & BGCHECKFLAG_CEILING) {
        s32 pad;

        speedXZ = &this->actor.speed;
        velocityY = &this->actor.velocity.y;
        if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
            *speedXZ *= (kREG(19) * 0.01f) + 0.8f;
        } else {
            *speedXZ = 0.0f;
        }
        if (*velocityY >= 0.0f) {
            *velocityY *= -((kREG(20) * 0.01f) + 0.6f);
            func_80AED4FC(this);
        }
    }
    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        speedXZ = &this->actor.speed;
        if (*speedXZ != 0.0f) {
            s16 wallYaw;
            s16 rotY;
            s32 temp_a1_2;
            s32 temp_a0;
            s32 phi_v1;

            rotY = this->actor.world.rot.y;
            wallYaw = this->actor.wallYaw;
            temp_a0 = (wallYaw * 2) - rotY;
            temp_a1_2 = temp_a0 + 0x8000;
            if ((s16)((temp_a0 - wallYaw) + 0x8000) >= 0) {
                phi_v1 = (s16)(temp_a1_2 - wallYaw);
            } else {
                phi_v1 = -(s16)(temp_a1_2 - wallYaw);
            }
            if (phi_v1 < 0x4001) {
                if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
                    *speedXZ *= (kREG(21) * 0.01f) + 0.6f;
                } else {
                    *speedXZ = 0.0f;
                }
                this->actor.world.rot.y = temp_a1_2;
                func_80AED4FC(this);
                func_80AED5B8(this);
            }
        }
    }
}

void func_80AEDEF4(EnRu1* this, PlayState* play) {
    f32* speedXZ = &this->actor.speed;
    DynaPolyActor* dynaPolyActor = DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId);

    if (dynaPolyActor != NULL && dynaPolyActor->actor.id == ACTOR_EN_BOX) {
        if (*speedXZ != 0.0f) {
            *speedXZ *= 1.1f;
        } else {
            *speedXZ = 1.0f;
        }
    }
    if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
        *speedXZ *= (kREG(22) * 0.01f) + 0.98f;
    } else {
        *speedXZ = 0.0f;
    }
}

void func_80AEDFF4(EnRu1* this, PlayState* play) {
    func_80AEDB30(this, play);
    func_80AEDEF4(this, play);
    Actor_MoveXZGravity(&this->actor);
}

void func_80AEE02C(EnRu1* this) {
    this->actor.velocity.x = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.velocity.z = 0.0f;
    this->actor.speed = 0.0f;
    this->actor.gravity = 0.0f;
    this->actor.minVelocityY = 0.0f;
}

void EnRu1_UpdateWaterState(EnRu1* this) {
    s32 pad;
    f32 bobMagnitude;
    f32 startY;
    EnRu1* thisx = this; // necessary to match

    if (this->waterState == ENRU1_WATER_OUTSIDE) {
        if ((this->actor.minVelocityY == 0.0f) && (this->actor.speed == 0.0f)) {
            // When Ruto's velocity has been slowed enough by the water, stop her motion
            this->waterState = ENRU1_WATER_IMMERSED;
            func_80AEE02C(this);
            this->bobPhase = 0;
            this->bobDepth = (this->actor.depthInWater - 10.0f) * 0.5f;
            this->sinkingStartPosY = this->actor.world.pos.y + thisx->bobDepth;
        } else {
            // Ruto is touching the water but still in motion, e.g. from being thrown
            this->actor.gravity = 0.0f;
            this->actor.minVelocityY *= 0.2f;
            this->actor.velocity.y *= 0.2f;
            if (this->actor.minVelocityY >= -0.1f) {
                this->actor.minVelocityY = 0.0f;
                this->actor.velocity.y = 0.0f;
            }
            this->actor.speed *= 0.5f;
            if (this->actor.speed <= 0.1f) {
                this->actor.speed = 0.0f;
            }
            this->actor.velocity.x = Math_SinS(this->actor.world.rot.y) * this->actor.speed;
            this->actor.velocity.z = Math_CosS(this->actor.world.rot.y) * this->actor.speed;
            Actor_UpdatePos(&this->actor);
        }
    } else {
        if (this->waterState == ENRU1_WATER_IMMERSED) {
            if (this->bobDepth <= 1.0f) {
                func_80AEE02C(this);
                this->waterState = ENRU1_WATER_BOBBING;
                this->isSinking = 0.0f;
            } else {
                f32 deltaY;

                bobMagnitude = this->bobDepth;
                startY = this->sinkingStartPosY;
                deltaY = Math_CosS(this->bobPhase) * -bobMagnitude;
                this->actor.world.pos.y = deltaY + startY;
                this->bobPhase += 0x3E8;
                this->bobDepth *= 0.95f;
            }
        } else {
            this->isSinking += 1.0f;
            if (this->isSinking > 0.0f) {
                this->waterState = ENRU1_WATER_SINKING;
            }
        }
    }
}

s32 func_80AEE264(EnRu1* this, PlayState* play) {
    if (!Actor_TalkOfferAccepted(&this->actor, play)) {
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY;
        if (GET_INFTABLE(INFTABLE_143)) {
            this->actor.textId = 0x404E;
            Actor_OfferTalkNearColChkInfoCylinder(&this->actor, play);
        } else if (GET_INFTABLE(INFTABLE_142)) {
            this->actor.textId = 0x404D;
            Actor_OfferTalkNearColChkInfoCylinder(&this->actor, play);
        } else {
            this->actor.textId = 0x404C;
            Actor_OfferTalkNearColChkInfoCylinder(&this->actor, play);
        }
        return false;
    }
    return true;
}

void func_80AEE2F8(EnRu1* this, PlayState* play) {
    DynaPolyActor* dynaPolyActor;
    s32 floorBgId;

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->actor.floorBgId != BGCHECK_SCENE)) {
        floorBgId = this->actor.floorBgId;
        dynaPolyActor = DynaPoly_GetActor(&play->colCtx, floorBgId);
        if ((dynaPolyActor != NULL) && (dynaPolyActor->actor.id == ACTOR_BG_BDAN_SWITCH)) {
            if (PARAMS_GET_U(dynaPolyActor->actor.params, 8, 6) == 0x38) {
                SET_INFTABLE(INFTABLE_140);
                return;
            }
        }
    }
    CLEAR_INFTABLE(INFTABLE_140);
}

s32 func_80AEE394(EnRu1* this, PlayState* play) {
    s32 pad[2];
    CollisionContext* colCtx;
    DynaPolyActor* dynaPolyActor;
    s32 floorBgId;

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && this->actor.floorBgId != BGCHECK_SCENE) {
        colCtx = &play->colCtx;
        floorBgId = this->actor.floorBgId; // necessary match, can't move this out of this block unfortunately
        dynaPolyActor = DynaPoly_GetActor(colCtx, floorBgId);
        if (dynaPolyActor != NULL && dynaPolyActor->actor.id == ACTOR_BG_BDAN_OBJECTS &&
            dynaPolyActor->actor.params == 0 && !Player_InCsMode(play) && play->msgCtx.msgLength == 0) {
            func_80AEE02C(this);
            play->csCtx.script = gRutoObtainingSapphireCs;
            gSaveContext.cutsceneTrigger = 1;
            this->action = ENRU1_ACTION_36;
            this->drawConfig = ENRU1_DRAW_NOTHING;
            this->bigOctoPlatform = (BgBdanObjects*)dynaPolyActor;
            this->actor.shape.shadowAlpha = 0;
            return true;
        }
    }
    return false;
}

void func_80AEE488(EnRu1* this, PlayState* play) {
    s8 curRoomNum;

    if (Actor_HasParent(&this->actor, play)) {
        curRoomNum = play->roomCtx.curRoom.num;
        this->roomNum3 = curRoomNum;
        this->action = ENRU1_ACTION_31;
        func_80AED520(this, play);
    } else if (!func_80AEE394(this, play) && !(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        s32 pad;

        this->actor.minVelocityY = -((kREG(24) * 0.01f) + 6.8f);
        this->actor.gravity = -((kREG(23) * 0.01f) + 1.3f);
        this->action = ENRU1_ACTION_28;
    }
}

void func_80AEE568(EnRu1* this, PlayState* play) {
    if (!func_80AEE394(this, play)) {
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->actor.speed == 0.0f) &&
            (this->actor.minVelocityY == 0.0f)) {
            s32 pad;

            func_80AEE02C(this);
            Actor_OfferCarry(&this->actor, play);
            this->action = ENRU1_ACTION_27;
            EnRu1_DisableSittingOC(this);
            return;
        }

        if (this->actor.depthInWater > 0.0f) {
            this->action = ENRU1_ACTION_29;
            this->waterState = ENRU1_WATER_OUTSIDE;
        }
    }
}

void func_80AEE628(EnRu1* this, PlayState* play) {
    s32 pad[2];
    s8 curRoomNum = play->roomCtx.curRoom.num;

    if (EnRu1_IsCsStateIdle(play)) {
        Animation_Change(&this->skelAnime, &gRutoChildSittingAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildSittingAnim), ANIMMODE_LOOP, -8.0f);
        SET_INFTABLE(INFTABLE_144);
        this->action = ENRU1_ACTION_31;
    }
    this->roomNum3 = curRoomNum;
}

s32 func_80AEE6D0(EnRu1* this, PlayState* play) {
    s32 pad;
    s8 curRoomNum = play->roomCtx.curRoom.num;

    if (!GET_INFTABLE(INFTABLE_144) && (func_80AEB124(play) != NULL)) {
        if (!Player_InCsMode(play)) {
            Animation_Change(&this->skelAnime, &gRutoChildSeesSapphireAnim, 1.0f, 0,
                             Animation_GetLastFrame(&gRutoChildSquirmAnim), ANIMMODE_LOOP, -8.0f);
            func_80AED600(this);
            this->action = ENRU1_ACTION_34;
            this->unk_26C = 0.0f;
            play->csCtx.script = gRutoFoundSapphireCs;
            gSaveContext.cutsceneTrigger = 1;
        }
        this->roomNum3 = curRoomNum;
        return true;
    }
    this->roomNum3 = curRoomNum;
    return false;
}

void EnRu1_UpdateCarriedBehavior(EnRu1* this, PlayState* play) {
    s32 pad[9];
    Player* player;
    f32* carryIdleTimer = &this->carryIdleTimer;

    if (Actor_HasNoParent(&this->actor, play)) {
        f32 frameCount = Animation_GetLastFrame(&gRutoChildSittingAnim);

        Animation_Change(&this->skelAnime, &gRutoChildSittingAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
        func_80AED6DC(this, play);
        this->actor.speed *= (kREG(25) * 0.01f) + 1.0f;
        this->actor.velocity.y *= (kREG(26) * 0.01f) + 1.0f;
        this->actor.minVelocityY = -((kREG(24) * 0.01f) + 6.8f);
        this->actor.gravity = -((kREG(23) * 0.01f) + 1.3f);
        func_80AED57C(this);
        this->action = ENRU1_ACTION_28;
        *carryIdleTimer = 0.0f;
    } else if (func_80AEE6D0(this, play)) {
        s32 pad;

        *carryIdleTimer = 0.0f;
    } else {
        player = GET_PLAYER(play);
        if (player->stateFlags2 & PLAYER_STATE2_IDLE_FIDGET) {
            this->carryIdleTimer += 1.0f;
            if (this->action != ENRU1_ACTION_32) {
                if (*carryIdleTimer > 30.0f) {
                    if (Rand_S16Offset(0, 3) == 0) {
                        f32 frameCount = Animation_GetLastFrame(&gRutoChildSquirmAnim);

                        Animation_Change(&this->skelAnime, &gRutoChildSquirmAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP,
                                         -8.0f);
                        func_80AED5DC(this);
                        this->action = ENRU1_ACTION_32;
                    }
                    *carryIdleTimer = 0.0f;
                }
            } else {
                if (*carryIdleTimer > 50.0f) {
                    f32 frameCount = Animation_GetLastFrame(&gRutoChildSittingAnim);

                    Animation_Change(&this->skelAnime, &gRutoChildSittingAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP,
                                     -8.0f);
                    this->action = ENRU1_ACTION_31;
                    *carryIdleTimer = 0.0f;
                }
            }
        } else {
            f32 frameCount = Animation_GetLastFrame(&gRutoChildSittingAnim);

            Animation_Change(&this->skelAnime, &gRutoChildSittingAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
            *carryIdleTimer = 0.0f;
        }
    }
}

s32 EnRu1_CheckHitBottomUnderwater(EnRu1* this, PlayState* play) {
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        s32 pad;

        func_80AEE02C(this);
        Actor_OfferCarry(&this->actor, play);
        this->action = ENRU1_ACTION_27;
        EnRu1_DisableSittingOC(this);
        return true;
    }
    return false;
}

void EnRu1_CheckSinkingState(EnRu1* this, PlayState* play) {
    if ((!EnRu1_CheckHitBottomUnderwater(this, play)) && (this->waterState == ENRU1_WATER_SINKING)) {
        this->action = ENRU1_ACTION_30;
        func_80AEE02C(this);
        this->actor.gravity = -0.1f;
        this->actor.minVelocityY = -((kREG(18) * 0.1f) + 0.7f);
    }
}

void func_80AEEBB4(EnRu1* this, PlayState* play) {
    Actor_OfferCarry(&this->actor, play);
}

void func_80AEEBD4(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    EnRu1_UpdateSittingOC(this, play);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEEBB4(this, play);
    func_80AEE488(this, play);
    func_80AED624(this, play);
    func_80AEDAE0(this, play);
}

void func_80AEEC5C(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    EnRu1_UpdateSittingAT(this, play);
    func_80AEAECC(this, play);
    func_80AEE2F8(this, play);
    func_80AEDFF4(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEE568(this, play);
    func_80AED624(this, play);
    func_80AEDAE0(this, play);
}

void func_80AEECF0(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateWaterState(this);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    EnRu1_CheckSinkingState(this, play);
    func_80AED624(this, play);
}

void func_80AEED58(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    Actor_MoveXZGravity(&this->actor);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    EnRu1_CheckHitBottomUnderwater(this, play);
    func_80AED624(this, play);
    func_80AEDAE0(this, play);
}

void func_80AEEDCC(EnRu1* this, PlayState* play) {
    EnRu1_UpdateHeadRotation(this);
    EnRu1_UpdateSkelAnime(this);
    func_80AEAECC(this, play);
    func_80AEE2F8(this, play);
    EnRu1_UpdateEyes(this);
    func_80AED6F8(play);
    EnRu1_UpdateCarriedBehavior(this, play);
}

void func_80AEEE34(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    EnRu1_UpdateSkelAnime(this);
    func_80AEAECC(this, play);
    func_80AEE2F8(this, play);
    EnRu1_UpdateEyes(this);
    func_80AED6F8(play);
    EnRu1_UpdateCarriedBehavior(this, play);
}

void func_80AEEE9C(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    func_80AEDFF4(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AED738(this, play);
    func_80AED624(this, play);
}

void func_80AEEF08(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    EnRu1_UpdateSkelAnime(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateEyes(this);
    func_80AEE628(this, play);
}

void func_80AEEF5C(EnRu1* this, PlayState* play) {
}

void func_80AEEF68(EnRu1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 trackingPreset;

    this->interactInfo.trackPos = player->actor.world.pos;
    this->interactInfo.yOffset = kREG(16) - 3.0f;
    trackingPreset = kREG(17) + 0xC;
    Npc_TrackPoint(&this->actor, &this->interactInfo, trackingPreset, NPC_TRACKING_HEAD_AND_TORSO);
}

void func_80AEEFEC(EnRu1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 trackingPreset;

    this->interactInfo.trackPos = player->actor.world.pos;
    this->interactInfo.yOffset = kREG(16) - 3.0f;
    trackingPreset = kREG(17) + 0xC;
    Npc_TrackPoint(&this->actor, &this->interactInfo, trackingPreset, NPC_TRACKING_FULL_BODY);
    this->actor.world.rot.y = this->actor.shape.rot.y;
}

void func_80AEF080(EnRu1* this) {
    if (Animation_OnFrame(&this->skelAnime, 11.0f)) {
        Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_EV_LAND_DIRT);
    }
}

s32 func_80AEF0BC(EnRu1* this, PlayState* play) {
    if (GET_INFTABLE(INFTABLE_142)) {
        f32 frameCount = Animation_GetLastFrame(&gRutoChildSitAnim);

        Animation_Change(&this->skelAnime, &gRutoChildSitAnim, 1.0f, 0, frameCount, ANIMMODE_ONCE, -8.0f);
        play->msgCtx.msgMode = MSGMODE_PAUSED;
        this->action = ENRU1_ACTION_26;
        this->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY);
        return true;
    }
    return false;
}

void func_80AEF170(EnRu1* this, PlayState* play, s32 cond) {
    if (cond) {
        this->action = ENRU1_ACTION_25;
    }
}

void func_80AEF188(EnRu1* this, PlayState* play) {
    if (func_80AEB174(play) && !func_80AEF0BC(this, play)) {
        Message_CloseTextbox(play);
        SET_INFTABLE(INFTABLE_142);
        this->action = ENRU1_ACTION_24;
    }
}

void func_80AEF1F0(EnRu1* this, PlayState* play, UNK_TYPE arg2) {
    if (arg2 != 0) {
        Animation_Change(&this->skelAnime, &gRutoChildSittingAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gRutoChildSittingAnim), ANIMMODE_LOOP, 0.0f);
        Message_CloseTextbox(play);
        SET_INFTABLE(INFTABLE_143);
        func_80AED6DC(this, play);
        Actor_OfferCarry(&this->actor, play);
        this->action = ENRU1_ACTION_27;
        EnRu1_DisableSittingOC(this);
    }
}

void func_80AEF29C(EnRu1* this, PlayState* play) {
    this->action = ENRU1_ACTION_23;
}

void func_80AEF2AC(EnRu1* this, PlayState* play) {
    this->action = ENRU1_ACTION_24;
    this->drawConfig = ENRU1_DRAW_OPA;
    this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY;
}

void func_80AEF2D0(EnRu1* this, PlayState* play) {
    s32 cond;

    func_80AEEF68(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    EnRu1_UpdateStandingOC(this, play);
    func_80AEAECC(this, play);
    cond = func_80AEE264(this, play);
    func_80AED624(this, play);
    func_80AEF170(this, play, cond);
}

void func_80AEF354(EnRu1* this, PlayState* play) {
    func_80AEEFEC(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEF188(this, play);
}

void func_80AEF3A8(EnRu1* this, PlayState* play) {
    s32 something;

    func_80AED83C(this);
    something = EnRu1_UpdateSkelAnime(this);
    func_80AEF080(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEF1F0(this, play, something);
}

void func_80AEF40C(EnRu1* this) {
    SkelAnime* skelAnime = &this->skelAnime;

    if (Animation_OnFrame(skelAnime, 2.0f) || Animation_OnFrame(skelAnime, 7.0f) ||
        Animation_OnFrame(skelAnime, 12.0f) || Animation_OnFrame(skelAnime, 18.0f) ||
        Animation_OnFrame(skelAnime, 25.0f) || Animation_OnFrame(skelAnime, 33.0f)) {
        Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_PL_WALK_GROUND + SURFACE_SFX_OFFSET_JABU);
    }
}

void func_80AEF4A8(EnRu1* this, PlayState* play) {
    SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.projectedPos, 20, NA_SE_VO_RT_FALL);
}

void func_80AEF4E0(EnRu1* this) {
    if (Animation_OnFrame(&this->skelAnime, 5.0f)) {
        Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_VO_RT_LAUGH_0);
    }
}

void func_80AEF51C(EnRu1* this) {
    Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_VO_RT_THROW);
}

void func_80AEF540(EnRu1* this) {
    if (EnRu1_GetPlatformCamSetting(this) == 2) {
        EnRu1_SetEyes(this, ENRU1_EYES_UP);
        EnRu1_SetMouth(this, ENRU1_MOUTH_OPEN);
        if (this->skelAnime.mode != 2) {
            EnRu1_AnimationChange(this, &gRutoChildShutterAnim, ANIMMODE_ONCE, -8.0f, false);
            func_80AEF51C(this);
        }
    }
}

void func_80AEF5B8(EnRu1* this) {
    f32 curFrame;

    if (D_80AF1938 == 0) {
        curFrame = this->skelAnime.curFrame;
        if (curFrame >= 60.0f) {
            EnRu1_SetEyes(this, ENRU1_EYES_UP);
            EnRu1_SetMouth(this, ENRU1_MOUTH_SMILING);
            func_80AED57C(this);
            D_80AF1938 = 1;
        }
    }
}

void func_80AEF624(EnRu1* this, PlayState* play) {
    f32 frameCount;
    CsCmdActorCue* cue;
    CsCmdActorCue* cue2;
    s16 newRotTmp;

    if (func_80AEAFE0(play, 1, 3)) {
        frameCount = Animation_GetLastFrame(&gRutoChildWalkToAndHoldUpSapphireAnim);
        // this weird part with the redundant variable is necessary to match for some reason
        cue2 = play->csCtx.actorCues[3];
        cue = cue2;
        this->actor.world.pos.x = cue->startPos.x;
        this->actor.world.pos.y = cue->startPos.y;
        this->actor.world.pos.z = cue->startPos.z;
        newRotTmp = cue->rot.x;
        this->actor.shape.rot.x = newRotTmp;
        this->actor.world.rot.x = newRotTmp;
        newRotTmp = cue->rot.y;
        this->actor.shape.rot.y = newRotTmp;
        this->actor.world.rot.y = newRotTmp;
        newRotTmp = cue->rot.z;
        this->actor.shape.rot.z = newRotTmp;
        this->actor.world.rot.z = newRotTmp;
        Animation_Change(&this->skelAnime, &gRutoChildWalkToAndHoldUpSapphireAnim, 1.0f, 0.0f, frameCount,
                         ANIMMODE_ONCE, 0.0f);
        func_80AEB3A4(this, play);
        this->action = ENRU1_ACTION_37;
        this->drawConfig = ENRU1_DRAW_OPA;
        this->actor.shape.shadowAlpha = 0xFF;
    }
}

void func_80AEF728(EnRu1* this, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&this->skelAnime, &gRutoChildHoldArmsUpAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gRutoChildHoldArmsUpAnim), ANIMMODE_LOOP, 0.0f);
        func_80AEB3CC(this);
        this->action = ENRU1_ACTION_38;
    }
}

void func_80AEF79C(EnRu1* this, PlayState* play) {
    if (func_80AEAFE0(play, 2, 3)) {
        Animation_Change(&this->skelAnime, &gRutoChildBringHandsDownAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildBringHandsDownAnim), ANIMMODE_ONCE, -8.0f);
        this->action = ENRU1_ACTION_39;
    }
}

void func_80AEF820(EnRu1* this, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&this->skelAnime, &gRutoChildWait2Anim, 1.0f, 0, Animation_GetLastFrame(&gRutoChildWait2Anim),
                         ANIMMODE_LOOP, -8.0f);
        this->action = ENRU1_ACTION_40;
    }
}

void func_80AEF890(EnRu1* this, PlayState* play) {
    s32 pad[2];
    s8 curRoomNum;

    if (!(DEBUG_FEATURES && IS_CUTSCENE_LAYER) && EnRu1_IsCsStateIdle(play)) {
        curRoomNum = play->roomCtx.curRoom.num;
        SET_INFTABLE(INFTABLE_145);
        Flags_SetSwitch(play, EnRu1_GetSwitchFlag(this));
        EnRu1_SetPlatformCamSetting(this, 1);
        this->action = ENRU1_ACTION_42;
        this->actor.room = curRoomNum;
    }
}

void func_80AEF930(EnRu1* this, PlayState* play) {
    if (EnRu1_GetPlatformCamSetting(this) == 3) {
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY;
        this->actor.textId = 0x4048;
#if !OOT_PAL_N64
        Message_ContinueTextbox(play, this->actor.textId);
#else
        Message_StartTextbox(play, this->actor.textId, NULL);
#endif
        func_80AEF4A8(this, play);
        this->action = ENRU1_ACTION_43;
        this->drawConfig = ENRU1_DRAW_NOTHING;
    }
}

void func_80AEF99C(EnRu1* this, PlayState* play) {
    if (func_80AEB1B4(play) != 0) {
        EnRu1_SetPlatformCamSetting(this, 4);
        Actor_Kill(&this->actor);
    }
}

void func_80AEF9D8(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEF624(this, play);
#if DEBUG_FEATURES
    func_80AEB220(this, play);
#endif
}

void func_80AEFA2C(EnRu1* this, PlayState* play) {
    s32 something;

    func_80AED83C(this);
    func_80AEB364(this, play);
    func_80AEAECC(this, play);
    something = EnRu1_UpdateSkelAnime(this);
    func_80AEF4E0(this);
    func_80AEF5B8(this);
    func_80AEF40C(this);
    func_80AEF728(this, something);
#if DEBUG_FEATURES
    func_80AEB220(this, play);
#endif
}

void func_80AEFAAC(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
    func_80AEF79C(this, play);
#if DEBUG_FEATURES
    func_80AEB220(this, play);
#endif
}

void func_80AEFB04(EnRu1* this, PlayState* play) {
    s32 something;

    func_80AED83C(this);
    func_80AEAECC(this, play);
    something = EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEF820(this, something);
#if DEBUG_FEATURES
    func_80AEB220(this, play);
#endif
}

void func_80AEFB68(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEF890(this, play);
#if DEBUG_FEATURES
    func_80AEB220(this, play);
#endif
}

void func_80AEFBC8(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEF540(this);
    func_80AEF930(this, play);
}

void func_80AEFC24(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEF99C(this, play);
}

void EnRu1_InitInSapphireRoom(EnRu1* this, PlayState* play) {
    if (GET_INFTABLE(INFTABLE_145) && !GET_INFTABLE(INFTABLE_146)) {
        s32 pad;

        EnRu1_AnimationChange(this, &gRutoChildWait2Anim, ANIMMODE_LOOP, 0, false);
        this->action = ENRU1_ACTION_41;
        this->bigOctoPlatform = EnRu1_FindBigOctoPlatform(play);
        EnRu1_SetPlatformCamSetting(this, 1);
        this->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY);
    } else {
        Actor_Kill(&this->actor);
    }
}

void func_80AEFCE8(EnRu1* this, PlayState* play) {
    this->bigOctoPlatform = EnRu1_FindBigOctoPlatform(play);
    if (this->bigOctoPlatform != NULL) {
        this->action = ENRU1_ACTION_42;
        this->drawConfig = ENRU1_DRAW_OPA;
        EnRu1_SetPlatformCamSetting(this, 1);
    }
}

void EnRu1_InitBesideKingZora(EnRu1* this, PlayState* play) {
    if (GET_EVENTCHKINF(EVENTCHKINF_37) && LINK_IS_CHILD) {
        EnRu1_AnimationChange(this, &gRutoChildWait2Anim, ANIMMODE_LOOP, 0, false);
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        this->action = ENRU1_ACTION_44;
        this->drawConfig = ENRU1_DRAW_OPA;
    } else {
        Actor_Kill(&this->actor);
    }
}

s32 func_80AEFDC0(EnRu1* this, PlayState* play) {
    if (!Actor_TalkOfferAccepted(&this->actor, play)) {
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY;
        this->actor.textId = MaskReaction_GetTextId(play, MASK_REACTION_SET_RUTO);
        if (this->actor.textId == 0) {
            this->actor.textId = 0x402C;
        }
        Actor_OfferTalkNearColChkInfoCylinder(&this->actor, play);
        return false;
    }
    return true;
}

s32 func_80AEFE38(EnRu1* this, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        this->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY);
        return true;
    }
    return false;
}

void func_80AEFE84(EnRu1* this, PlayState* play, s32 cond) {
    if (cond) {
        this->action = ENRU1_ACTION_45;
    }
}

void func_80AEFE9C(EnRu1* this, PlayState* play) {
    if (func_80AEFE38(this, play)) {
        this->action = ENRU1_ACTION_44;
    }
}

void func_80AEFECC(EnRu1* this, PlayState* play) {
    func_80AEEF68(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    EnRu1_UpdateStandingOC(this, play);
    func_80AEAECC(this, play);
    func_80AEFE84(this, play, func_80AEFDC0(this, play));
}

void func_80AEFF40(EnRu1* this, PlayState* play) {
    func_80AEEFEC(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEFE9C(this, play);
}

/**
 * Places Ruto beside the door switch outside the room with the map.
 */
void EnRu1_InitBesideDoorSwitch(EnRu1* this, PlayState* play) {
    s8 actorRoom;

    if (GET_INFTABLE(INFTABLE_141) && GET_INFTABLE(INFTABLE_140) && !GET_INFTABLE(INFTABLE_145) &&
        (!(func_80AEB020(this, play)))) {
        EnRu1_AnimationChange(this, &gRutoChildWait2Anim, ANIMMODE_LOOP, 0, false);
        actorRoom = this->actor.room;
        this->action = ENRU1_ACTION_22;
        this->actor.room = -1;
        this->drawConfig = ENRU1_DRAW_NOTHING;
        this->roomNum1 = actorRoom;
        this->roomNum3 = actorRoom;
        this->roomNum2 = actorRoom;
        PRINTF(T("スイッチルトセット!!!!!!!!!!!!!!!!!!!!!!\n", "Ruto switch set!!!!!!!!!!!!!!!!!!!!!!\n"));
    } else {
        PRINTF(T("スイッチルトセットしない!!!!!!!!!!!!!!!!!!!!!!\n", "Ruto switch not set!!!!!!!!!!!!!!!!!!!!!!\n"));
        Actor_Kill(&this->actor);
    }
}

#if DEBUG_FEATURES
void func_80AF0050(EnRu1* this, PlayState* play) {
    EnRu1_AnimationChange(this, &gRutoChildWait2Anim, ANIMMODE_LOOP, 0, false);
    this->action = ENRU1_ACTION_36;
    this->roomNum1 = this->actor.room;
    this->bigOctoPlatform = EnRu1_FindBigOctoPlatform(play);
    this->actor.room = -1;
}
#endif

void EnRu1_Update(Actor* thisx, PlayState* play) {
    EnRu1* this = (EnRu1*)thisx;

    if (this->action < 0 || this->action >= ARRAY_COUNT(sActionFuncs) || sActionFuncs[this->action] == NULL) {
        PRINTF(VT_FGCOL(RED) T("メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n",
                               "The main mode is wrong!!!!!!!!!!!!!!!!!!!!!!!!!\n") VT_RST);
        return;
    }

    sActionFuncs[this->action](this, play);
}

void EnRu1_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnRu1* this = (EnRu1*)thisx;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gRutoChildSkel, NULL, this->jointTable, this->morphTable, 17);
    EnRu1_InitColliders(&this->actor, play);
    switch (EnRu1_GetType(this)) {
        case ENRU1_TYPE_BOSS_ROOM:
            EnRu1_InitInBossRoom(this, play);
            break;
        case ENRU1_TYPE_FOUNTAIN:
            EnRu1_InitOutsideJabuJabu(this, play);
            break;
        case ENRU1_TYPE_HOLES_ROOM:
            EnRu1_InitInJabuJabuHolesRoom(this, play);
            break;
        case ENRU1_TYPE_BASEMENT:
            EnRu1_InitInJabuJabuBasement(this, play);
            break;
        case ENRU1_TYPE_SAPPHIRE_ROOM:
            EnRu1_InitInSapphireRoom(this, play);
            break;
        case ENRU1_TYPE_BESIDE_KZ:
            EnRu1_InitBesideKingZora(this, play);
            break;
        case ENRU1_TYPE_BESIDE_DOOR_SWITCH:
            EnRu1_InitBesideDoorSwitch(this, play);
            break;
#if DEBUG_FEATURES
        case ENRU1_TYPE_DEBUG:
            func_80AF0050(this, play);
            break;
#endif
        default:
            Actor_Kill(&this->actor);
            PRINTF(T("該当 arge_data = %d 無し\n", "Relevant arge_data = %d unacceptable\n"), EnRu1_GetType(this));
            break;
    }
}

void EnRu1_PreLimbDraw(EnRu1* this, PlayState* play, s32 limbIndex, Vec3s* rot) {
    Vec3s* torsoRot = &this->interactInfo.torsoRot;
    Vec3s* headRot = &this->interactInfo.headRot;

    switch (limbIndex) {
        case RUTO_CHILD_CHEST:
            rot->x += torsoRot->y;
            rot->y -= torsoRot->x;
            break;
        case RUTO_CHILD_HEAD:
            rot->x += headRot->y;
            rot->z += headRot->x;
            break;
    }
}

s32 EnRu1_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                           Gfx** gfx) {
    EnRu1* this = (EnRu1*)thisx;

    if ((this->preLimbDrawIndex < 0) || (this->preLimbDrawIndex > 0) ||
        (*sPreLimbDrawFuncs[this->preLimbDrawIndex] == NULL)) {
        PRINTF(VT_FGCOL(RED) T("首回しモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n",
                               "Neck rotation mode is improper!!!!!!!!!!!!!!!!!!!!!!!!!\n") VT_RST);
    } else {
        sPreLimbDrawFuncs[this->preLimbDrawIndex](this, play, limbIndex, rot);
    }
    return false;
}

void EnRu1_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnRu1* this = (EnRu1*)thisx;

    if (limbIndex == RUTO_CHILD_HEAD) {
        Vec3f multVec;
        Vec3f focusPos;

        multVec = sMultVec;
        Matrix_MultVec3f(&multVec, &focusPos);
        this->actor.focus.pos.x = focusPos.x;
        this->actor.focus.pos.y = focusPos.y;
        this->actor.focus.pos.z = focusPos.z;
        this->actor.focus.rot.x = this->actor.world.rot.x;
        this->actor.focus.rot.y = this->actor.world.rot.y;
        this->actor.focus.rot.z = this->actor.world.rot.z;
    }
}

void EnRu1_DrawNothing(EnRu1* this, PlayState* play) {
}

void EnRu1_DrawOpa(EnRu1* this, PlayState* play) {
    s32 pad[2];
    s16 eyes = this->eyes;
    void* eyeTex = sEyeTextures[eyes];
    s16 mouth = this->mouth;
    SkelAnime* skelAnime = &this->skelAnime;
    void* mouthTex = sMouthTextures[mouth];
    s32 pad1;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_ru1.c", 1282);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(mouthTex));
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);

    POLY_OPA_DISP = SkelAnime_DrawFlex(play, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       EnRu1_OverrideLimbDraw, EnRu1_PostLimbDraw, this, POLY_OPA_DISP);

    CLOSE_DISPS(play->state.gfxCtx, "../z_en_ru1.c", 1309);
}

void EnRu1_DrawXlu(EnRu1* this, PlayState* play) {
    s32 pad[2];
    s16 eyes = this->eyes;
    void* eyeTex = sEyeTextures[eyes];
    s16 mouth = this->mouth;
    SkelAnime* skelAnime = &this->skelAnime;
    void* mouthTex = sMouthTextures[mouth];
    s32 pad1;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_ru1.c", 1324);

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(mouthTex));
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->alpha);
    gSPSegment(POLY_XLU_DISP++, 0x0C, &D_80116280[0]);

    POLY_XLU_DISP = SkelAnime_DrawFlex(play, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       EnRu1_OverrideLimbDraw, NULL, this, POLY_XLU_DISP);

    CLOSE_DISPS(play->state.gfxCtx, "../z_en_ru1.c", 1353);
}

void EnRu1_Draw(Actor* thisx, PlayState* play) {
    EnRu1* this = (EnRu1*)thisx;

    if (this->drawConfig < 0 || this->drawConfig >= ARRAY_COUNT(sDrawFuncs) || sDrawFuncs[this->drawConfig] == NULL) {
        PRINTF(VT_FGCOL(RED) T("描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n",
                               "The drawing mode is wrong!!!!!!!!!!!!!!!!!!!!!!!!!\n") VT_RST);
        return;
    }
    sDrawFuncs[this->drawConfig](this, play);
}
