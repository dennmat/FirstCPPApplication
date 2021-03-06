#include "stdafx.h"

#include <iostream>

#include "equipment.h"
#include "item.h"
#include <actors/actor.h>
#include "item_effect.h"

Slot::Slot(slots_t type, Equipment* equipment)
{
    this->type = type;
    this->equipment = equipment;
    this->equipped_item = NULL;

    this->equipment->slots->push_back(this);

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
        std::cout << "added item to slot" << std::endl;
        this->equipped_item = item;
        // this->equipped_item->item_effect->ApplyAllEffects(this->equipment->master);
        this->apply_item_effect();
    }
    else
    {
        std::cout << "DID NOT HAVE ROOM to  item to slot" << std::endl;
    };

};

void Slot::RemoveFromSlot()
{
    this->remove_item_effect();
    this->equipped_item = NULL;

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

void Slot::remove_item_effect()
{
    //find equipment master
    Actor* master = this->get_master();
    //check if attributes
    if (master != NULL && master->has_attributes(), this->equipped_item != NULL)
    {
        //apply appropriate health mana damage armor changes

        this->equipped_item->item_effect->RemoveHealthEffects(master);
        this->equipped_item->item_effect->RemoveManaEffects(master);
        this->equipped_item->item_effect->RemoveArmorEffects(master);
        this->equipped_item->item_effect->RemoveDamageEffects(master);

    }

};

void Slot::apply_item_effect()
{

    //find equipment master
    Actor* master = this->get_master();
    //check if attributes
    if (master != NULL && master->has_attributes(), this->equipped_item != NULL)
    {
        //apply appropriate health mana damage armor changes
        this->equipped_item->item_effect->ApplyHealthEffects(master);
        this->equipped_item->item_effect->ApplyManaEffects(master);
        this->equipped_item->item_effect->ApplyArmorEffects(master);
        this->equipped_item->item_effect->ApplyDamageEffects(master);
    }

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
    this->slots = new std::vector<Slot*>;

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

void Equipment::equip_item(Item* item)
{
    //get item slot type
    slots_t slot_type = item->slot_type;

    //check if slot is empty
    for (std::vector<Slot*>::iterator it = this->slots->begin(); it != this->slots->end(); ++it)
    {
        if ( (*it)->type == slot_type )
        {
            if ( (*it)->HasRoomFor(item) )
            {
                //put in slot 
                (*it)->AddToSlot(item);
                break;
            }
        }
    }

    // NOTE DOESNT apply item effects

};

void Equipment::unequip_item(Item* item)
{
    for (std::vector<Slot*>::iterator it = this->slots->begin(); it != this->slots->end(); ++it)
    {
        if ((*it)->GetEquippedItem() == item)
        {
            (*it)->RemoveFromSlot();
            std::cout << "removed from slot" << std::endl;
        }

    };

};

bool Equipment::is_item_equipped(Item* item)
{
    if (
            this->head->GetEquippedItem() == item ||
            this->earrings->GetEquippedItem() == item ||
            this->necklace->GetEquippedItem() == item ||

            this->chest->GetEquippedItem() == item ||
            this->left_shoulder->GetEquippedItem() == item ||
            this->right_shoulder->GetEquippedItem() == item ||

            this->left_bracer->GetEquippedItem() == item ||
            this->right_bracer->GetEquippedItem() == item ||

            this->left_glove->GetEquippedItem() == item ||
            this->right_glove->GetEquippedItem() == item ||

            this->left_ring_ring->GetEquippedItem() == item ||
            this->right_ring_ring->GetEquippedItem() == item ||
            this->left_ring_middle->GetEquippedItem() == item ||
            this->right_ring_middle->GetEquippedItem() == item ||
            this->left_ring_index->GetEquippedItem() == item ||
            this->right_ring_index->GetEquippedItem() == item ||

            this->main_weapon->GetEquippedItem() == item ||
            this->off_weapon->GetEquippedItem() == item
            ) 
    {
        return true;
    }
    else 
    {
        return false;
    };

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
    // //apply buffs/debuffs and durability for all equipment
    // this->head->apply_item_effect();
    // this->earrings->apply_item_effect();
    // this->necklace->apply_item_effect();

    // this->chest->apply_item_effect();
    // this->left_shoulder->apply_item_effect();
    // this->right_shoulder->apply_item_effect();

    // this->left_bracer->apply_item_effect();
    // this->right_bracer->apply_item_effect();

    // this->left_glove->apply_item_effect();
    // this->right_glove->apply_item_effect();

    // this->left_ring_ring->apply_item_effect();
    // this->right_ring_ring->apply_item_effect();
    // this->left_ring_middle->apply_item_effect();
    // this->right_ring_middle->apply_item_effect();
    // this->left_ring_index->apply_item_effect();
    // this->right_ring_index->apply_item_effect();

    // this->main_weapon->apply_item_effect();
    // this->off_weapon->apply_item_effect();
};
