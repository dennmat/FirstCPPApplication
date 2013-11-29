#include "stdafx.h"

#include "equipment.h"
#include "item.h"

Slot::Slot(slots_t type)
{
    this->type = type;

};


bool Slot::CanFitInSlot(Item* item)
{
    return this->type==item->slot_type;

};

Equipment::Equipment()
{
        this->head = new Slot(slots_t::Head);
        this->earrings = new Slot(slots_t::Ear);
        this->necklace = new Slot(slots_t::Neck);

        this->chest = new Slot(slots_t::Chest);
        this->left_shoulder = new Slot(slots_t::Shoulders);
        this->right_shoulder = new Slot(slots_t::Shoulders);

        this->left_bracer = new Slot(slots_t::Forearms);
        this->right_bracer = new Slot(slots_t::Forearms);

        this->left_glove = new Slot(slots_t::Hands);
        this->right_glove = new Slot(slots_t::Hands);

        this->left_ring_ring = new Slot(slots_t::Fingers);
        this->right_ring_ring = new Slot(slots_t::Fingers);
        this->left_ring_middle = new Slot(slots_t::Fingers);
        this->right_ring_middle = new Slot(slots_t::Fingers);
        this->left_ring_index = new Slot(slots_t::Fingers);
        this->right_ring_index = new Slot(slots_t::Fingers);

        this->main_weapon = new Slot(slots_t::MainHand);
        this->off_weapon = new Slot(slots_t::OffHand);

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

};
