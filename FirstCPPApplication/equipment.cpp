#include "stdafx.h"

#include "equipment.h"
#include "item.h"
#include "actor.h"

Slot::Slot(slots_t type, Equipment* equipment)
{
    this->type = type;
    this->equipment = equipment;

};

bool Slot::HasRoomFor(Item* item)
{
    return equipped_item == NULL;

};

void Slot::AddToSlot(Item* item)
{
    //if item slot isn't full
    if (this->HasRoomFor(item))
    {
        //add item to slot
        this->equipped_item = item;

    };

};

Actor* Slot::get_master()
{
    if (this->equipment->master != NULL)
    {
        return this->equipment->master;
    }
    else
    {
        return NULL;
    };
};

void Slot::apply_item_effect()
{

    //find equipment master
    Actor* master = this->get_master();
    //check if attributes
    if (master != NULL && master->has_attributes())
    {

    }

    //apply appropriate health mana damage armor changes
    //
    //TODO TODO TODO
    //add equipment update to main loop
    
};

Item* Slot::GetEquippedItem()
{
    return this->equipped_item;
};


bool Slot::CanFitInSlot(Item* item)
{
    return this->type==item->slot_type;

};

Equipment::Equipment()
{
    this->master = NULL;

    this->head = new Slot(slots_t::Head, this);
    this->earrings = new Slot(slots_t::Ear, this);
    this->necklace = new Slot(slots_t::Neck, this);

    this->chest = new Slot(slots_t::Chest, this);
    this->left_shoulder = new Slot(slots_t::Shoulders, this);
    this->right_shoulder = new Slot(slots_t::Shoulders, this);

    this->left_bracer = new Slot(slots_t::Forearms, this);
    this->right_bracer = new Slot(slots_t::Forearms, this);

    this->left_glove = new Slot(slots_t::Hands, this);
    this->right_glove = new Slot(slots_t::Hands, this);

    this->left_ring_ring = new Slot(slots_t::Fingers, this);
    this->right_ring_ring = new Slot(slots_t::Fingers, this);
    this->left_ring_middle = new Slot(slots_t::Fingers, this);
    this->right_ring_middle = new Slot(slots_t::Fingers, this);
    this->left_ring_index = new Slot(slots_t::Fingers, this);
    this->right_ring_index = new Slot(slots_t::Fingers, this);

    this->main_weapon = new Slot(slots_t::MainHand, this);
    this->off_weapon = new Slot(slots_t::OffHand, this);

};
Equipment::~Equipment()
{

    delete this->head;
    delete this->earrings;
    delete this->necklace;

    delete this->chest;
    delete this->left_shoulder;
    delete this->right_shoulder;

    delete this->left_bracer;
    delete this->right_bracer;

    delete this->left_glove;
    delete this->right_glove;

    delete this->left_ring_ring;
    delete this->right_ring_ring;
    delete this->left_ring_middle;
    delete this->right_ring_middle;
    delete this->left_ring_index;
    delete this->right_ring_index;

    delete this->main_weapon;
    delete this->off_weapon;

};

void Equipment::Update()
{
    //apply buffs/debuffs and durability for all equipment
    this->head->apply_item_effect();
    this->earrings->apply_item_effect();
    this->necklace->apply_item_effect();

    this->chest->apply_item_effect();
    this->left_shoulder->apply_item_effect();
    this->right_shoulder->apply_item_effect();

    this->left_bracer->apply_item_effect();
    this->right_bracer->apply_item_effect();

    this->left_glove->apply_item_effect();
    this->right_glove->apply_item_effect();

    this->left_ring_ring->apply_item_effect();
    this->right_ring_ring->apply_item_effect();
    this->left_ring_middle->apply_item_effect();
    this->right_ring_middle->apply_item_effect();
    this->left_ring_index->apply_item_effect();
    this->right_ring_index->apply_item_effect();

    this->main_weapon->apply_item_effect();
    this->off_weapon->apply_item_effect();
};
