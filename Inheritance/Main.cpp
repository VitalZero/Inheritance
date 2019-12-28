#include <iostream>
#include <string>
#include <conio.h>
#include <random>
#include <algorithm>
#include <assert.h>

class Dice
{
public:
	int Roll( int nDice )
	{
		int total = 0;
		for( int n = 0; n < nDice; n++ )
		{
			total += d6( rng );
		}
		return total;
	}
private:
	std::uniform_int_distribution<int> d6 = std::uniform_int_distribution<int>( 1,6 );
	std::mt19937 rng = std::mt19937( std::random_device{}() );
};

struct Attributes
{
	int hp;
	int speed;
	int power;
};

class Weapon
{
public:
	Weapon(const std::string& name, int rank)
		:
		name(name),
		rank(rank)
	{}
	const std::string& GetName() const
	{
		return name;
	}
	int GetRank() const
	{
		return rank;
	}
	virtual int CalculateDamage(const Attributes& attr, Dice& d) const = 0;
	virtual ~Weapon() = default;
private:
	std::string name;
	int rank;
};

class MemeFighter
{
public:
	std::string GetName() const
	{
		return name;
	}
	bool IsAlive() const
	{
		return attr.hp > 0;
	}
	int GetInitiative()
	{
		return attr.speed + Roll(2);
	}
	void Attack(MemeFighter& target)
	{
		if (IsAlive() && target.IsAlive())
		{
			std::cout << name << " attacks " << target.name << " with his " << pWeapon->GetName() << "!.\n";
			assert(pWeapon != nullptr);
			ApplyDamageTo(target, pWeapon->CalculateDamage(attr, dice) );
		}
	}
	virtual void Tick()
	{
		if (IsAlive())
		{
			int recover = Roll(1);
			std::cout << name << " gains " << recover << "hp.\n";
			attr.hp += recover;
		}
	}
	virtual void SpecialMove(MemeFighter& target) = 0;
	virtual ~MemeFighter()
	{
		delete pWeapon;
		pWeapon = nullptr;
	}
	void GiveWeapon(Weapon* pNewWeapon)
	{
		delete pWeapon;
		pWeapon = pNewWeapon;
	}
	Weapon* PilferWeapon()
	{
		auto pWep = pWeapon;
		pWeapon = nullptr;
		return pWep;
	}
	bool HasWeapon() const
	{
		return pWeapon != nullptr;
	}
	const Weapon& GetWeapon() const
	{
		return *pWeapon;
	}
protected:
	MemeFighter(const std::string& name, int hp, int speed, int power, Weapon* pWeapon = nullptr)
		:
		name(name),
		attr({ hp, speed, power } ),
		pWeapon(pWeapon)
	{
		std::cout << GetName() << " enters the ring.\n";
	}
	void ApplyDamageTo(MemeFighter& target, int damage)
	{
		target.attr.hp -= damage;
		std::cout << target.GetName() << " takes " << damage << " damage." << std::endl;
		if (!target.IsAlive())
		{
			std::cout << "As the life leaves " << target.GetName() << "'s body, so does the poop.\n";
		}
	}
	int Roll(int nDice = 1) const
	{
		return dice.Roll(nDice);
	}
protected:
	Attributes attr;
	std::string name;
private:
	Weapon* pWeapon = nullptr;
	mutable Dice dice;
};

class Fists : public Weapon
{
public:
	Fists()
		:
		Weapon("Fists", 0)
	{}
	int CalculateDamage(const Attributes& attr, Dice& d) const override
	{
		return attr.power + d.Roll(2);
	}
};

class Knife : public Weapon
{
public:
	Knife()
		:
		Weapon("Knife", 2)
	{}
	int CalculateDamage(const Attributes& attr, Dice& d) const override
	{
		return attr.speed * 3 + d.Roll(3);
	}
};

class Bat : public Weapon
{
public:
	Bat()
		:
		Weapon("Bat", 1)
	{}
	int CalculateDamage(const Attributes& attr, Dice& d) const override
	{
		return attr.power * 2 + d.Roll(1);
	}
};

class MemeFrog : public MemeFighter
{
public:
	MemeFrog(const std::string& name, Weapon* pWeapon = nullptr)
		:
		MemeFighter(name, 69, 7, 14, pWeapon)
	{}
	void SpecialMove(MemeFighter& target) override
	{
		if (IsAlive() && target.IsAlive())
		{
			if (Roll() > 4)
			{
				std::cout << name << " attacks " << target.GetName() << " with a rainbow beam!.\n";
				ApplyDamageTo(target, 20 + Roll(3));
			}
			else
			{
				std::cout << name << " falls off his unicycle.\n"; 
			}
		}
	}
	void Tick() override
	{
		if (IsAlive())
		{
			std::cout << name << " is AIDS hurt.\n";
			ApplyDamageTo(*this, Roll());
			MemeFighter::Tick();
		}
	}
	~MemeFrog() override
	{
		std::cout << "Destroying MemeFrog " << name << "!" << std::endl;
	}
private:
};

class MemeCat : public MemeFighter
{
public:
	MemeCat(const std::string& name, Weapon* pWeapon = nullptr)
		:
		MemeFighter(name, 65, 9, 14, pWeapon)
	{}
	void SpecialMove(MemeFighter& other) override
	{
		if (IsAlive())
		{
			if (Roll() > 2)
			{
				std::cout << GetName() << " eats a cheeseburger and gains 20 hp.\n";
				attr.hp += 20;
			}
			else
			{
				std::cout << GetName() << " meows demurely.\n";
			}
		}
	}
	~MemeCat() override
	{
		std::cout << "Destroying MemeCat " << name << "!" << std::endl;
	}
};

class MemeStoner : public MemeFighter
{
public:
	MemeStoner(const std::string& name, Weapon* pWeapon = nullptr)
		:
		MemeFighter(name, 80, 4, 10, pWeapon)
	{}
	void SpecialMove(MemeFighter& target) override
	{
		if (IsAlive() )
		{
			if (Roll() > 3)
			{
				std::cout << name << " smokes the danky sticky icky, becoming Super " << name << "!.\n";
				name = "Super " + name;
				attr.speed += 3;
				attr.power = (attr.power * 69) / 42;
				attr.hp += 10;
			}
			else
			{
				std::cout << name << " spaces out.\n";
			}
		}
	}
	~MemeStoner() override 
	{
		std::cout << "Destroying MemeStoner " << name << "!" << std::endl;
	}
};

void TakeWeaponIfDead(MemeFighter& taker, MemeFighter& giver)
{
	if (taker.IsAlive() && !giver.IsAlive() && giver.HasWeapon())
	{
		if (giver.GetWeapon().GetRank() > taker.GetWeapon().GetRank())
		{
			std::cout << taker.GetName() << " takes the " << giver.GetWeapon().GetName()
				<< " from " << giver.GetName() << "'s still-cooling corpse\n";
			taker.GiveWeapon(giver.PilferWeapon());
		}
	}
}

void Engage( MemeFighter& f1,MemeFighter& f2 )
{
	// pointers for sorting purposes
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if( p1->GetInitiative() < p2->GetInitiative() )
	{
		std::swap( p1,p2 );
	}
	// execute attacks
	p1->Attack( *p2 );
	TakeWeaponIfDead(*p1, *p2);
	p2->Attack( *p1 );
	TakeWeaponIfDead(*p2, *p1);
}

void DoSpecials(MemeFighter& f1, MemeFighter& f2)
{
	// pointers for sorting purposes
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if (p1->GetInitiative() < p2->GetInitiative())
	{
		std::swap(p1, p2);
	}
	// execute attacks
	p1->SpecialMove(*p2);
	TakeWeaponIfDead(*p1, *p2);
	p2->SpecialMove(*p1);
	TakeWeaponIfDead(*p2, *p1);
}

int main()
{
	std::vector<MemeFighter*> t1 = { 
		new MemeFrog("Dat Boi", new Fists), 
		new MemeStoner("Good Guy Greg", new Bat), 
		new MemeCat("Haz cheeseburger", new Knife)
	};

	std::vector<MemeFighter*> t2 = {
		new MemeCat("NEDM", new Fists),
		new MemeStoner("Scumbag Steve", new Bat),
		new MemeFrog("Pepe", new Knife)
	};

	const auto alive_pred = [](MemeFighter* pf) { return pf->IsAlive(); };

	while ( std::any_of(t1.begin(), t1.end(), alive_pred) &&
		std::any_of(t2.begin(), t2.end(), alive_pred) )
	{
		// random shuffle
		std::random_shuffle(t1.begin(), t1.end());
		std::partition(t1.begin(), t1.end(), alive_pred);
		std::random_shuffle(t2.begin(), t2.end());
		std::partition(t2.begin(), t2.end(), alive_pred);

		for (size_t i = 0; i < t1.size(); ++i)
		{
			// trade blows
			Engage(*t1[i], *t2[i]);
			// special moves
			DoSpecials(*t1[i], *t2[i]);
			std::cout << "---------------------------------------" << std::endl;
		}
		std::cout << "=======================================" << std::endl;
		
		// end of turn maintainence
		for (size_t i = 0; i < t1.size(); ++i)
		{
			t1[i]->Tick();
			t2[i]->Tick();
		}
		std::cout << "=======================================" << std::endl;

		std::cout << "Press any key to continue...";
		while( !_kbhit() );
		_getch();
		std::cout << std::endl << std::endl;
	}

	if( std::any_of(t1.begin(), t1.end(), alive_pred ) )
	{
		std::cout << "Team ONE is victorious!" << std::endl;
	}
	else
	{
		std::cout << "Team TWO is victorious!" << std::endl;
	}
	for (size_t i = 0; i < t1.size(); ++i)
	{
		delete t1[i];
		delete t2[i];
	}
	while( !_kbhit() );


	return 0;
}