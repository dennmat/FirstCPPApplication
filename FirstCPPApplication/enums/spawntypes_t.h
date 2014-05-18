#pragma once
#ifndef SPAWNTYPES_T_H
#define SPAWNTYPES_T_H

enum MonsterSpawnTypes {
    TrollSpawn = 1,
    JackalSpawn,
    SkeletonSpawn,
    OgreSpawn,
    BadMotherSpawn,

    HulkingMantisSpawn,
    IdolSpawn,
    ImpSpawn,
    MutantFishSpawn,
    SpinyLizardSpawn,
    CrazedCookSpawn,
    WildlingSpawn,
    SludgeSpawn,
    JumperSpawn,

    ErrorSpawn
};

enum ItemSpawnTypes {
    CorpseSpawn,
    GenericSpawn, 

    ArmorSpawn,
    WeaponSpawn,
    PotionSpawn,
    ScrollSpawn,
    WandSpawn,
    HeroSpawn,
    SpecialSpawn,

    NothingItemSpawn
};

enum WeaponSpawnTypes {
    SwordSpawn,
    MaceSpawn,
    DaggerSpawn,
    TridentSpawn,
    WhipSpawn,
    KatanaSpawn,
    NothingWeaponSpawn
};

enum ArmorSpawnTypes {
    ChainmailSpawn,
    ShieldSpawn,
    HelmetSpawn,
    NothingArmorSpawn
};

enum PotionSpawnTypes {
    HealthPotionSpawn,
    GlowingHealthPotionSpawn,
    PulsatingHealthSpawn,
    GlowingManaPotionSpawn,
    ManaPotionSpawn,
    NothingPotionSpawn
};

enum SpecialSpawnTypes {
    GlowingAmuletSpawn,
    NothingAmuletSpawn
};
#endif
