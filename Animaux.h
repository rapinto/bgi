#pragma once
#include <fstream>
#include "Equipes.h"
#include "Animal.h"
class Animaux
{
public:
	Animaux();
	void animalDuPersonnage(int indicePersonnage, int& indiceAnimal, int& rareteAnimal)const;
	int indiceAnimal(int indicePersonnage) const;
	int rareteAnimal(int indicePersonnage, int indiceAnimal) const;
	bool animalDebloquer(int indicePersonnage, int indiceAnimal, int rareteAnimal)const;
	void deblocageAnimal(int indicePersonnage, int indiceAnimal, int rareteAnimal, std::string perso);
	void sauvegarder();
	Animal animalNumero(int indice)const;
private:
	std::vector<std::vector<int >> _animauxPosseder;
	std::vector<int> _animauxUtiliser;
	std::vector<Animal> _listeAnimaux;
};

