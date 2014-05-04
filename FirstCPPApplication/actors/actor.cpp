#include "stdafx.h"

// #include "libtcod_cpp_hpp\libtcod.hpp"

#include "actor.h"

#include "thinker.h"
#include "inventory.h"
#include "equipment.h"
#include "tile.h"
#include "attribute.h"
#include "attribute_container.h"
#include "Representation.h"
#include "item.h"
#include "item_effect.h"
#include "ui.h"
#include "messages.h"
#include "game.h"
#include "class.h"


int Actor::pack_size = 10;
int Actor::champ_chance = 10;
int Actor::preferred_pack_size = 10;

std::vector<IClass*>* Actor::actor_class_choices = new std::vector<IClass*>();

Actor::Actor()
{
    this->cls_name = "Actor";
    this->name = "Unset Actor name";

    this->xp = 0;
    this->xp_this_level = 0;
    this->xp_required = 100;
    this->xp_value = 10;
    this->level = 1;

    this->is_champion = false;
    this->timed_item_effects = new std::vector<TimedEffect*>;
    this->timed_spell_effects = new std::vector<TimedEffect*>;

    this->representation = new Representation;
    this->inventory = new Inventory;
    this->inventory->master = this;
    this->attrs = new AttributeContainer;
    this->attrs->owner = this;
    this->spells = new std::vector<Spell*>;

    this->actors_in_sight = new std::vector<Actor*>;

    this->thinker = new Thinker;
    this->thinker->master = this;

    this->is_active = true;

    this->is_moving_left = false;
    this->is_moving_right = false;
    this->is_moving_up = false;
    this->is_moving_down = false;

    this->l_path = NULL;
    this->has_attacked = false;
};

Actor::~Actor()
{
    delete this->timed_item_effects;
    delete this->timed_spell_effects;

    delete this->actors_in_sight;
    delete this->inventory;
    delete this->representation;
    delete this->attrs;
    delete this->spells;
    delete this->thinker;
};

void Actor::resetIsMoving()
{
    this->is_moving_up = false; 
    this->is_moving_right = false;
    this->is_moving_down = false;
    this->is_moving_left = false;
};

std::string Actor::GetName()
{
    return name;
};

const char* Actor::GetNameC()
{
    return name.c_str();
};

bool Actor::IsActorInSight(Actor * actor)
{

    std::vector<Actor*>::iterator aisItr;
    aisItr = std::find(this->actors_in_sight->begin(), this->actors_in_sight->end(),  actor);
    if (aisItr != this->actors_in_sight->end()) 
    {
        return true; 
    }
    else 
    {
        return false; 
    }
};

void Actor::ActorInSight(int x, int y, Actor * actor)
{
    std::vector<Actor*>* ais = this->actors_in_sight;
    //if the actor is in sight and isnt this actor:
    if (actor == this) 
    {
        return;
    }
    if (std::find(ais->begin(), ais->end(), actor) != ais->end())
    {
    }
    else 
    {
        this->actors_in_sight->push_back(actor);
    }

}

void Actor::putPerson( Tile * next_tile, int new_x, int new_y)
{	//puts a person on a tile, resets the old tile

    if (my_tile != NULL){
        my_tile->makeUnoccupied(this); 
    }

    if (next_tile != NULL){
        this->my_tile = next_tile;
        next_tile->makeOccupied(this); 
    }
    else if (next_tile == NULL)
    {
        std::cout << "next tile is null" << std::endl;
    }
    else
    {
        std::cout << "else, this shouldn't be a possiblity" << std::endl;
    }

    if (new_x != -1 && new_y != -1)
    {
        this->x = new_x;
        this->y = new_y;
    }

};

void Actor::pickUpItem(Item* item)
{

    //add to inventory
    this->inventory->add_item(item);

    // remove from Tiles
    this->my_tile->pick_up_item(item);

};

bool Actor::has_attributes()
{
    return this->attrs != NULL;
};

Item* Actor::CreateCorpse()
{
    Item* corpse = new Item;
    corpse->repr->repr = '%';
    corpse->repr->setFGColor(*this->representation->fg_color, true, false, true);

    corpse->name =  this->cls_name + " corpse";
    corpse->slot_type = slots_t::NoSlot;
    corpse->item_effect->set_all_vals_to(0);
    corpse->item_effect->health_regen_interval=1;
    corpse->item_effect->health_regen_rate=1;
    corpse->item_effect->hunger_current_val=-50;
    corpse->usable = true;
    corpse->equippable = false;

    return corpse;
};

int* Actor::get_direction_heading()
{
    int dir_array[2];

    //handle x coord
    if (this->is_moving_left)
    {
        dir_array[0] = -1;
    }
    else if (this->is_moving_right)
    {
        dir_array[0] =  1;
    }
    else 
    {
        dir_array[0] = 0;
    };

    //handle y coord
    if (this->is_moving_up)
    {
        dir_array[1] = -1;
    }
    else if (this->is_moving_down)
    {
        dir_array[1] =  1;
    }
    else 
    {
        dir_array[1] = 0;
    };

    return dir_array;

};

Item* spawnItemBase(std::string name, std::string description, char repr, slots_t slot, int weight)
{
        Item* dropped_item = new Item;
        dropped_item->equippable = true;
        dropped_item->repr->repr = repr;
        dropped_item->name = name;
        dropped_item->description = description;
        dropped_item->slot_type = slot;
        dropped_item->weight = weight;
        return dropped_item;
};

Item* spawnEquippable(std::string name, std::string description, char repr, slots_t slot, int weight)
{
    Item* dropped_item = spawnItemBase(name, description, repr, slot, weight);
    dropped_item->equippable = true;
    return dropped_item;
};

Item* spawnUsable(std::string name, std::string description, char repr, slots_t slot, int weight)
{
    Item* dropped_item = spawnItemBase(name, description, repr, slot, weight);
    dropped_item->usable = true;
    return dropped_item;
};

Item* spawnItem(int result)
{
    Item* dropped_item;
    TCODRandom *rng = TCODRandom::getInstance();
    result = rng->getInt(0, 100);
    

    if (result <= 25)
    {
        std::string description = "It looks like it's made up of smaller more delicate pieces.";
        dropped_item = spawnEquippable("Chainmail", description, '&', slots_t::Chest, 10);
        dropped_item->repr->setFGColor(TCODColor::grey, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0);

        //sword damage
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int armor = rng->getInt(1, 5, 2);
        dropped_item->item_effect->armor_current_val = armor;
        dropped_item->item_effect->armor_max_val = armor;
    }
    else if (result <= 35)
    {
        std::string description = "It looks like it can swipe left to right and up and down. Wow.";
        dropped_item = spawnEquippable("A sword", description, '/', slots_t::MainHand, 12);
        dropped_item->repr->setFGColor(TCODColor::grey, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0);

        //sword damage
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int damage = rng->getInt(5, 15, 8);
        dropped_item->item_effect->damage_current_val = damage;
        dropped_item->item_effect->damage_max_val = damage;
    }
    else if (result <= 45)
    {
        std::string description = "It looks like it can take a few hits.";
        dropped_item = spawnEquippable("A shield", description, ']', slots_t::OffHand, 6);
        dropped_item->repr->setFGColor(TCODColor::lightGrey, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0);

        //shield armor
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int armor = rng->getInt(1, 5, 3);
        dropped_item->item_effect->armor_current_val = armor;
        dropped_item->item_effect->armor_max_val = armor;
    }
    else if (result <= 55)
    {

        std::string description = "It looks like your mom.";
        dropped_item = spawnEquippable("A mace", description, '/', slots_t::MainHand, 12);
        dropped_item->repr->setFGColor(TCODColor::sepia, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0);

        //dagger damage
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int damage = rng->getInt(1, 5, 3);
        dropped_item->item_effect->damage_current_val = damage;
        dropped_item->item_effect->damage_max_val = damage;
    }
    else if (result <= 65)
    {
        std::string description = "It looks sturdy.";
        dropped_item = spawnEquippable("A helmet", description, '^', slots_t::Head, 5);
        dropped_item->repr->setFGColor(TCODColor::lightGrey, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0);


        //armor
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int armor = rng->getInt(1, 5, 3);
        dropped_item->item_effect->armor_current_val = armor;
        dropped_item->item_effect->armor_max_val = armor;
    }
    else if (result <= 70)
    {

        std::string description = "It glows.";
        dropped_item = spawnEquippable("A amulet", description, '|', slots_t::Neck, 2);
        dropped_item->repr->setFGColor(TCODColor::lightGrey, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0);

        //dagger damage
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int armor = rng->getInt(1, 5, 2);
        int damage = rng->getInt(1, 5, 2);
        int mana = rng->getInt(1, 5, 2);
        dropped_item->item_effect->armor_current_val = armor;
        dropped_item->item_effect->armor_max_val = armor;
        dropped_item->item_effect->damage_current_val = damage;
        dropped_item->item_effect->damage_max_val = damage;
        dropped_item->item_effect->mana_regen_rate = mana;
        dropped_item->item_effect->mana_regen_interval = mana;

    }
    else if (result <= 80)
    {

        std::string description = "It looks like it's safe to drink.";
        dropped_item = spawnUsable("A health potion", description, '!', slots_t::NoSlot, 1);
        dropped_item->repr->setFGColor(TCODColor::lightGreen, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0); 

        //health restore
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int health = rng->getInt(10, 100, 25);
        dropped_item->item_effect->health_current_val = health;
    }
    else if (result <= 82)
    {
        std::string description = "It looks like it's probably safe to drink.";
        dropped_item = spawnUsable("A glowing health potion", description, '!', slots_t::NoSlot, 1);
        dropped_item->repr->setFGColor(TCODColor::lighterGreen, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0); 

        //health restore
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int health = rng->getInt(5, 50, 15);
        dropped_item->item_effect->health_current_val = health;
        dropped_item->item_effect->health_max_val = health;
    }
    else if (result <= 83)
    {

        std::string description = "It looks like could be safe to drink.";
        dropped_item = spawnUsable("A pulsating health potion", description, '!', slots_t::NoSlot, 1);
        dropped_item->repr->setFGColor(TCODColor::lightestGreen, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0); 

        //health restore
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int health = rng->getInt(1, 5, 1);
        dropped_item->item_effect->health_regen_rate = health;
        dropped_item->item_effect->health_regen_interval = -floor((double)health/2);
    }
    else if (result <= 87)
    {
        std::string description = "It looks like a good time.";
        dropped_item = spawnUsable("A glowing blue potion", description, '!', slots_t::NoSlot, 1);
        dropped_item->repr->setFGColor(TCODColor::lightestBlue, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0); 


        //mana regen
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int mana = rng->getInt(1, 5, 1);
        dropped_item->item_effect->mana_regen_rate = mana;
        dropped_item->item_effect->mana_regen_interval = -floor((double)mana/2);
    }
    else if (result <= 90)
    {
        std::string description = "It looks like a small bit of essence is trapped inside. Better drink it.";
        dropped_item = spawnUsable("A blue potion", description, '!', slots_t::NoSlot, 1);
        dropped_item->repr->setFGColor(TCODColor::lightBlue, true, false, true);
        dropped_item->item_effect->set_all_vals_to(0); 

        //health restore
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int mana = rng->getInt(1, 50, 10);
        dropped_item->item_effect->mana_current_val = mana;
    }
    else
    {
        dropped_item = NULL;
    }

    return dropped_item;
};

void Actor::Die()
{
    //make the master's tile no longer occupied by him
    //drop corpse on floor or another item
    TCODRandom *rng = Game::item_spawn_rng;
    Item* dropped_item = NULL;
    int  result;
    // for (int xx =0; xx < 100; xx++){
    result = rng->getInt(0, 100);
    //     std::cout << result << std::endl;
    // }
    if (result <= 15)
    {
        dropped_item = this->CreateCorpse();
    }
    else if (result > 60)
    {
        dropped_item = spawnItem(result);
    }
    else 
    {
        //nothing happens between result of 15 and 60
    };
    if (dropped_item != NULL)
    {
        this->my_tile->place_item_down(dropped_item);
    }


    //remove master from ai update list
    this->is_active = false;
    this->putPerson(NULL, NULL, NULL);

    //destroy ai
    if (this->thinker != NULL)
    {
        delete this->thinker;
    };

    new Message(Ui::msg_handler_main, NOTYPE_MSG, "%s died!", this->name.c_str());

    int multiplier = 1;
    if (this == (Actor*)Game::player)
    {
        TCODSystem::setFps(0);
        for (int fade=255*multiplier; fade >= 0; fade --) {
            if (fade % multiplier == 0)
            {
                TCODConsole::setFade(fade/multiplier,TCODColor::red);
                TCODConsole::flush();
                printf("YOU'RE DEAD GIVE UP");
            };
        }
        exit(1);
        TCODSystem::setFps(Game::fps_limit);
    };

};
