#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Herbivore {
protected:
    int weight;
    bool life;

public:
    Herbivore(int weight) : weight(weight), life(true) {}

    virtual void EatGrass() {
        if (life) {
            weight += 10;
            cout << "Травоядное кушает траву. Вес теперь: " << weight << endl;
        }
        else {
            cout << "Это животное мертво и не может есть траву." << endl;
        }
    }

    int GetWeight() const { return weight; }
    bool IsAlive() const { return life; }
    void Kill() { life = false; }

    virtual string GetName() const = 0;
    virtual ~Herbivore() {}
};

class Wildebeest : public Herbivore {
public:
    Wildebeest(int weight) : Herbivore(weight) {}

    string GetName() const override { return "Антилопа Гну"; }
};

class Bison : public Herbivore {
public:
    Bison(int weight) : Herbivore(weight) {}

    string GetName() const override { return "Бизон"; }
};

class Carnivore {
protected:
    int power;

public:
    Carnivore(int power) : power(power) {}

    virtual void Eat(Herbivore* herbivore) {
        if (!herbivore->IsAlive()) {
            cout << herbivore->GetName() << " уже мертва. Хищник не может её съесть." << endl;
            return;
        }

        if (power > herbivore->GetWeight()) {
            power += 10;
            herbivore->Kill();
            cout << GetName() << " съел " << herbivore->GetName()
                << ". Сила теперь: " << power << endl;
        }
        else {
            power -= 10;
            cout << GetName() << " не смог победить " << herbivore->GetName()
                << ". Сила уменьшилась: " << power << endl;
        }
    }

    int GetPower() const { return power; }

    virtual string GetName() const = 0;
    virtual ~Carnivore() {}
};

class Lion : public Carnivore {
public:
    Lion(int power) : Carnivore(power) {}

    string GetName() const override { return "Лев"; }
};

class Wolf : public Carnivore {
public:
    Wolf(int power) : Carnivore(power) {}

    string GetName() const override { return "Волк"; }
};

class Continent {
public:
    virtual vector<Herbivore*> CreateHerbivores() = 0;
    virtual vector<Carnivore*> CreateCarnivores() = 0;
    virtual ~Continent() {}
};

class Africa : public Continent {
public:
    vector<Herbivore*> CreateHerbivores() override {
        return { new Wildebeest(50), new Wildebeest(60) };
    }

    vector<Carnivore*> CreateCarnivores() override {
        return { new Lion(100), new Lion(120) };
    }
};

class NorthAmerica : public Continent {
public:
    vector<Herbivore*> CreateHerbivores() override {
        return { new Bison(80), new Bison(90) };
    }

    vector<Carnivore*> CreateCarnivores() override {
        return { new Wolf(70), new Wolf(85) };
    }
};

class AnimalWorld {
private:
    vector<Herbivore*> herbivores;
    vector<Carnivore*> carnivores;

public:
    AnimalWorld(Continent& continent) {
        herbivores = continent.CreateHerbivores();
        carnivores = continent.CreateCarnivores();
    }

    ~AnimalWorld() {
        for (auto herbivore : herbivores) {
            delete herbivore;
        }
        for (auto carnivore : carnivores) {
            delete carnivore;
        }
    }

    void MealsHerbivores() {
        cout << "Травоядные начинают есть траву:\n";
        for (auto herbivore : herbivores) {
            herbivore->EatGrass();
        }
    }

    void NutritionCarnivores() {
        cout << "Хищники начинают охотиться:\n";
        for (auto carnivore : carnivores) {
            for (auto herbivore : herbivores) {
                if (herbivore->IsAlive()) {
                    carnivore->Eat(herbivore);
                }
            }
        }
    }
};

int main() {
    cout << "Мир животных: Африка\n";
    Africa africa;
    AnimalWorld africanWorld(africa);

    africanWorld.MealsHerbivores();
    africanWorld.NutritionCarnivores();

    cout << "\nМир животных: Северная Америка\n";
    NorthAmerica northAmerica;
    AnimalWorld americanWorld(northAmerica);

    americanWorld.MealsHerbivores();
    americanWorld.NutritionCarnivores();

}
