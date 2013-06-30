#include <sstream>


class Pet{
    public:
    std::string name;
    int max_hp;
    int cur_hp;
    Person* master;

    bool is_dead;

    Pet (std::string name, int max_hp);
    Pet::Pet();
    // Pet::Init();
    void Pet::Attack(Pet* target, int dmg);
    void Pet::TakeDamage(int dmg);
    void printout();
    bool Pet::CheckDeath();
};